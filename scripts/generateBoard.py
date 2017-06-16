#!/usr/bin/env python

#
# file: generateBoard.py
#
# author: Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import json
import jinja2
from jinja2 import meta
import argparse
from string import digits
import os
import re
import datetime

outputTemplates = {}

def generateJinja2File(filename, templateFile, templateVars):
	templateLoader = jinja2.FileSystemLoader([".", os.path.dirname(templateFile)])
	templateEnv = jinja2.Environment(trim_blocks = True, lstrip_blocks = True, keep_trailing_newline = True,
			loader = templateLoader)
	template = templateEnv.get_template(templateFile)

	outputText = template.render(templateVars)

	outFile=open(filename, 'w')
	outFile.write(outputText)
	outFile.close()

def searchForJinja2FilesInPath(paths):
	searchPaths = []

	for path in paths:
		for root, dirs, files in os.walk(path):
			for file in files:
				if file.endswith(".jinja"):
					searchPaths.append(os.path.join(root, file))

	return searchPaths

def collectMetaDataFromJinja2File(templateFile):
	global outputTemplates

	templateLoader = jinja2.FileSystemLoader([".", os.path.dirname(templateFile)])
	templateEnv = jinja2.Environment(trim_blocks = True, lstrip_blocks = True, keep_trailing_newline = True,
			loader = templateLoader)

	source = templateEnv.loader.get_source(templateEnv, templateFile)[0]
	variables = meta.find_undeclared_variables(templateEnv.parse(source))
	stringFromVarNames = str(variables).split('\'')

	metaData = {}

	for value in stringFromVarNames:
		matchObj = re.match(r'.+?(?=__)', value, re.M|re.I)
		if matchObj:
			metaData['id'] = str(matchObj.group())
		matchObj = re.search(r'(?<=__)[^}]*(?=__)', value, re.M|re.I)
		if matchObj:
			metaData['type'] = str(matchObj.group())
		matchObj = re.search(r'(?<=__)v[0-9]', value, re.M|re.I)
		if matchObj:
			metaData['version'] = str(matchObj.group())

	if metaData:
		outputTemplates[templateFile] = metaData

def getTemplateFileFromTypeAndVersion(id, version):
	for fileName, parameters in outputTemplates.items():
		if parameters['id'] == id:
			if (version):
				if (parameters['version'] == version):
					return fileName
			else:
				return fileName
	return Null

def getOutputFileName(template_file):
	splitedVal = str(template_file).split("/")
	name = splitedVal[-1].partition(".jinja")
	return name[0]

def isFileTypeHpp(file_name):
	return '.hpp' in file_name

def replaceBoardStringInFileName(fileName, boardName):
	newStr,found,endPart = fileName.partition('BOARD_NAME')

	if found:
		newStr+=boardName+endPart

	return newStr

def collectLedsTemplateParams(template_vars, data):
	ledsIds = []
	ledsAlternativeIds = []
	ledsIdsToOutPinsIds = []
	ledsPins = []
	ledsPinsGroups = []

	for x in data["leds"]:
		ledsIds.append(x["id"])
		ledsAlternativeIds.append(x["alternativeId"])
		ledsIdsToOutPinsIds.append(x["outputPins"])

	for x in ledsIdsToOutPinsIds:
		for y in data["outputPins"]:
			if y["id"] == x:
				ledsPins.append(y)
				matchObj = re.search(r'(?<=p)[^}]?(?=\d{1,2})', str(y["pin"]), re.M|re.I)
				if matchObj:
					ledsPinsGroups.append(matchObj.group())

	template_vars["ledsIds"] = list(zip(ledsIds, ledsAlternativeIds, ledsPinsGroups))
	template_vars["ledsPins"] = ledsPins

def collectButtonsTemplateParams(variablesForTemplates, data):
	buttonsIds = []
	buttonsIdToInPinIds = []
	buttonsPins = []
	buttonsPinsGroup = []

	for x in data["buttons"]:
		buttonsIds.append(x["id"])

	for x in data["buttons"]:
		buttonsIdToInPinIds.append(x["inputPins"])

	for x in buttonsIdToInPinIds:
		for y in data["inputPins"]:
			if y["id"] == x:
				buttonsPins.append(y)
				matchObj = re.search(r'(?<=p)[^}]?(?=\d{1,2})', str(y["pin"]), re.M|re.I)
				if matchObj:
					buttonsPinsGroup.append(matchObj.group())

	variablesForTemplates["buttonsIds"] = list(zip(buttonsIds, buttonsPinsGroup))
	variablesForTemplates["buttonsPins"] = buttonsPins

def collectPinGroupsTemplateParams(data):
	pinsType = set()
	for x in data:
		pinWithoutNumber = str(x["pin"])
		pinWithoutNumber = pinWithoutNumber.strip(digits)
		pinsType.add(pinWithoutNumber[1:])

	return sorted(pinsType)

def removeFromOutputTemplatesIfNotConfiguredParam(input_data):
	for template_path, parameters in outputTemplates.copy().items():
		if 'type' in parameters:
			if not parameters['type'] in input_data:
				del outputTemplates[template_path]

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument('config', help = 'path to json config file')
	parser.add_argument('-o', '--output', help = 'path to directory where files will be generated,'
			'if empty the files will be generated in the same folder as the one with config file')
	parser.add_argument('-s', '--search', nargs = '*', default = '.', help = 'search paths for templates')
	inputParameters = parser.parse_args()

	jinjaFiles = searchForJinja2FilesInPath(inputParameters.search)

	for file in jinjaFiles:
		collectMetaDataFromJinja2File(file)

	with open(inputParameters.config) as configDataFile:
		data = json.load(configDataFile)

	if inputParameters.output:
		outputBoardPath = inputParameters.output
		if not outputBoardPath.endswith('/'):
			outputBoardPath += '/'
		outputBoardPath += data["board"]
	else:
		outputBoardPath = os.path.dirname(inputParameters.config)

	outputBoardPath += "/"
	includeBoard = outputBoardPath + "include"
	includeDirectory = outputBoardPath + "include/distortos/board"

	if not os.path.exists(includeDirectory):
		os.makedirs(includeDirectory)

	templateVars = {}
	templateVars["boardIncludes"] = includeBoard
	templateVars["actualYear"] = datetime.datetime.now().year

	for key in data:
		if (key == "leds"):
			collectLedsTemplateParams(templateVars, data)
			templateVars["usedPinsGroupsLeds"] = collectPinGroupsTemplateParams(templateVars["ledsPins"])
		elif (key == "buttons"):
			collectButtonsTemplateParams(templateVars, data)
			templateVars["usedPinsGroupsButtons"] = collectPinGroupsTemplateParams(templateVars["buttonsPins"])
		elif (key == "gpioDriverVersion"):
			templateVars["gpioInputTemplate"] = getTemplateFileFromTypeAndVersion('inputPinTemplate',
											data["gpioDriverVersion"])
			templateVars["gpioOutputTemplate"] = getTemplateFileFromTypeAndVersion('outputPinTemplate',
												data["gpioDriverVersion"])
			templateVars["gpioDriverVersion"] = data["gpioDriverVersion"]
		else:
			templateVars[key] = data[key]

	removeFromOutputTemplatesIfNotConfiguredParam(data)

	for pathToTemplate, parameters in outputTemplates.items():
		if parameters['id'] == 'outputTemplate':
			filename = replaceBoardStringInFileName(getOutputFileName(pathToTemplate), templateVars["board"])
			if(isFileTypeHpp(filename)):
				filename = includeDirectory + "/" + filename
			else:
				filename = outputBoardPath + filename

			generateJinja2File(filename, pathToTemplate, templateVars)

if __name__ == '__main__': main()
