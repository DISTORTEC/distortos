#!/usr/bin/env python

#
# file: dtsThroughJinja.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from __future__ import print_function

import argparse
import ast
import datetime
import fnmatch
import jinja2
import os
import pydts
import re

#
# Tests whether a devicetree node contains any of given properties with given values.
#
# param [in] node is the node which will be tested
# param [in] properties is a list of tuples, each with property name and property value, empty list accepts all nodes
#
# return True if node contains any of given properties with given values (or if properties is an empty list), False
# otherwise
#

def testProperties(node, properties):
	if len(properties) == 0:
		return True
	for propertyName, propertyValue in properties:
		if propertyName in node['properties'] and propertyValue in node['properties'][propertyName]:
			return True
	return False

#
# Generator which can be used to iterate devicetree nodes, with optional filtering by presence of properties with given
# values
#
# param [in] dictionary is the dictionary which will be iterated over
# param [in] properties is a list of tuples, each with property name and property value, empty list accepts all nodes,
# default - empty list
#
# return node name and node dictionary
#

def iterateNodes(dictionary, properties = []):
	for name, node in dictionary.items():
		if 'nodes' in node and 'properties' in node:
			if testProperties(node, properties) == True:
				yield name, node
			for subname, subnode in iterateNodes(node['nodes'], properties):
				yield subname, subnode

#
# Generator which can be used to iterate devicetree properties, with optional filtering by property names
#
# param [in] dictionary is the dictionary which will be iterated over
# param [in] propertyNames is a list of property names, empty list accepts all properties, default - empty list
#
# return property name and list of property values
#

def iterateProperties(dictionary, propertyNames = []):
	for name, node in dictionary.items():
		if 'nodes' in node and 'properties' in node:
			for propertyName, propertyValues in node['properties'].items():
				if len(propertyNames) == 0 or propertyName in propertyNames:
					yield propertyName, propertyValues
			for propertyName, propertyValues in iterateProperties(node['nodes'], propertyNames):
				yield propertyName, propertyValues

#
# Generator which can be used to iterate devicetree properties, with optional filtering by property names - unpacked
# variant
#
# param [in] dictionary is the dictionary which will be iterated over
# param [in] propertyNames is a list of property names, empty list accepts all properties, default - empty list
#
# return property name and property value
#

def iteratePropertiesUnpacked(dictionary, propertyNames = []):
	for propertyName, propertyValues in iterateProperties(dictionary, propertyNames):
		for propertyValue in propertyValues:
			yield propertyName, propertyValue

#
# Filter which sanitizes provided path
#
# param [in] path is the path that will be sanitized
# param [in] pattern is the pattern which will be replaced, default - '[^0-9A-Za-z-]'
# param [in] replacement is the replacement string, default - '_'
#
# return sanitized path
#

def sanitizePath(path, pattern = '[^0-9A-Za-z-]', replacement = '_'):
	regex = re.compile(pattern)
	return regex.sub(replacement, path)

########################################################################################################################
# main
########################################################################################################################

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('-I', '--in-format', choices = ['dts', 'pickle'], default = 'dts',
			help = 'input format, default - dts')
	parser.add_argument('inputFile', type = argparse.FileType('r'), help = 'input file')
	parser.add_argument('outputPath', help = 'output path')
	arguments = parser.parse_args()

	print('Reading dts...')

	dictionary = pydts.loadDictionary(arguments.inputFile, arguments.in_format == 'dts')

	print()
	print('Searching for templates...')

	jinjaTemplates = []
	metadataRegex = re.compile('{#\s*(.*)\s*#}')
	for path, directories, filenames in os.walk('.'):
		for filename in fnmatch.filter(filenames, '*.jinja'):
			templateFilename = os.path.join(path, filename)
			with open(templateFilename) as jinjaTemplate:
				print('Trying {}... '.format(templateFilename), end = '')
				metadataMatch = metadataRegex.match(jinjaTemplate.readline())
			if metadataMatch == None:
				print('no metadata found')
				continue
			try:
				propertyNames, valuePattern, outputFilenameTemplate = ast.literal_eval(metadataMatch.group(1))
			except:
				print('invalid metadata format')
				continue
			valueRegex = re.compile(valuePattern)
			for key, value in iteratePropertiesUnpacked(dictionary, propertyNames):
				if valueRegex.match(str(value)) != None:
					print('matches "{}" in "{}"'.format(value, key))
					jinjaTemplates.append((templateFilename, outputFilenameTemplate))
					break
			else:
				print('no match')

	print()
	print('Rendering files...')

	jinjaEnvironment = jinja2.Environment(trim_blocks = True, lstrip_blocks = True, keep_trailing_newline = True,
			loader = jinja2.FileSystemLoader('.'))
	jinjaEnvironment.globals['year'] = datetime.date.today().year
	jinjaEnvironment.globals['iterateNodes'] = iterateNodes
	jinjaEnvironment.filters['sanitizePath'] = sanitizePath

	filenameRegex = re.compile('[^a-zA-Z0-9_.-]')
	for templateFilename, outputFilenameTemplate in jinjaTemplates:
		outputFilename = os.path.join(arguments.outputPath,
				jinjaEnvironment.from_string(outputFilenameTemplate).render(dictionary = dictionary))
		output = jinjaEnvironment.get_template(templateFilename).render(dictionary = dictionary)
		with open(outputFilename, 'w') as outputFile:
			print('Rendering {} from {}'.format(outputFilename, templateFilename))
			outputFile.write(output)

	print()
	print('Done')
