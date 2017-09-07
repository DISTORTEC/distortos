#!/usr/bin/env python

#
# file: generateChipDtsi.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import argparse
import ast
import collections
import csv
import datetime
import jinja2
import os

#
# Tries to parse string into proper type
#
# param [in] string is the string which will be parsed
#
# return string parsed into proper type
#

def parseString(string):
	try:
		return ast.literal_eval(string)
	except ValueError:
		return string

#
# Handles single row read from CSV file
#
# param [in] jinjaEnvironment is the jinja environment
# param [in] outputPath is the output path for generated file
# param [in] header is the header of CSV row
# param [in] row is the row read from CSV file
#

def handleRow(jinjaEnvironment, outputPath, header, row):
	singles = collections.OrderedDict()
	nodes = collections.OrderedDict()
	for index, element in enumerate(header):
		if element[0] == '' or row[index] == '':
			continue
		if element[1] == '':	# single element
			singles[element[0]] = parseString(row[index])
		else:	# node
			if element[0] not in nodes:
				nodes[element[0]] = collections.OrderedDict()
			if element[1] != 'dtsiTemplate':
				nodes[element[0]][element[1]] = parseString(row[index])
			else:
				nodes[element[0]][element[1]] = jinjaEnvironment.get_template(row[index])

	with open(os.path.join(outputPath, singles['name'] + '.dtsi'), 'w') as dtsiFile:
		jinjaTemplate = jinjaEnvironment.get_template(singles['dtsiTemplate'])
		dtsiFile.write(jinjaTemplate.render(nodes = nodes, **singles))

########################################################################################################################
# main
########################################################################################################################

parser = argparse.ArgumentParser()
parser.add_argument('csvFile', help = 'input CSV file')
parser.add_argument('outputPath', help = 'output path')
arguments = parser.parse_args()

with open(arguments.csvFile, newline = '') as csvFile:

	jinjaEnvironment = jinja2.Environment(trim_blocks = True, lstrip_blocks = True, keep_trailing_newline = True,
			loader = jinja2.FileSystemLoader('.'))
	jinjaEnvironment.globals['year'] = datetime.date.today().year

	csvReader = csv.reader(csvFile)
	header = list(zip(next(csvReader), next(csvReader)))
	for row in csvReader:
		handleRow(jinjaEnvironment, arguments.outputPath, header, row)
