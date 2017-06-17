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
import csv
import datetime
import jinja2
import os

#
# Handles single row read from CSV file
#
# param [in] jinjaEnvironment is the jinja environment
# param [in] outputPath is the output path for generated file
# param [in] header is the header of CSV row
# param [in] row is the row read from CSV file
#

def handleRow(jinjaEnvironment, outputPath, header, row):
	singles = {}
	groups = {}
	for index, element in enumerate(header):
		if element[0] == '' or row[index] == '':
			continue
		if element[1] == '' and element[2] == '':	# single element
			singles[element[0]] = row[index]
		elif element[1] != '' and element[2] != '':	# group
			if not element[0] in groups:	# start new group
				groups[element[0]] = {}
			if not element[1] in groups[element[0]]:	# start new group element
				groups[element[0]][element[1]] = {}

			if element[2] != 'dtsiTemplate':
				groups[element[0]][element[1]][element[2]] = row[index]
			else:
				groups[element[0]][element[1]][element[2]] = jinjaEnvironment.get_template(row[index])

	with open(os.path.join(outputPath, singles['name'] + '.dtsi'), 'w') as dtsiFile:
		jinjaTemplate = jinjaEnvironment.get_template(singles['dtsiTemplate'])
		dtsiFile.write(jinjaTemplate.render(**singles, **groups))

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
	header = list(zip(next(csvReader), next(csvReader), next(csvReader)))
	for row in csvReader:
		handleRow(jinjaEnvironment, arguments.outputPath, header, row)
