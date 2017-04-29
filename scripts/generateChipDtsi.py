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
import os

#
# Handles "flash" element
#
# param [in] label is the label of element
# param [in] data is a dictionary with element's data
#
# return string constructed from label and data
#

def handleFlash(label, data):
	if 'virtualAddress' in data:
		virtualReg = '\t\tvirtual-reg = <{:#x}>;\n'.format(int(data['virtualAddress'], 0))
	else:
		virtualReg = ''
	return ('\t{label}: flash@{address:x} {{\n'
	'\t\tcompatible = "on-chip-flash";\n'
	'\t\treg = <{address:#x} {size:d}>;\n'
	'{virtualReg}'
	'\t\tlabel = "{label}";\n'
	'\t}};\n'.format(label = label, address = int(data['address'], 0), size = int(data['size'], 0),
			virtualReg = virtualReg))

#
# Handles "memory" element
#
# param [in] label is the label of element
# param [in] data is a dictionary with element's data
#
# return string constructed from label and data
#

def handleMemory(label, data):
	return ('\t{label}: memory@{address:x} {{\n'
	'\t\tdevice_type = "memory";\n'
	'\t\treg = <{address:#x} {size:d}>;\n'
	'\t\tlabel = "{label}";\n'
	'\t}};\n'.format(label = label, address = int(data['address'], 0), size = int(data['size'], 0)))

# dictionary with handlers of groups
handlers = {
	'flash': handleFlash,
	'memory': handleMemory,
}

#
# Handles single row read from CSV file
#
# param [in] outputPath is the output path for generated file
# param [in] header is the header of CSV row
# param [in] row is the row read from CSV file
#

def handleRow(outputPath, header, row):
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
			groups[element[0]][element[1]][element[2]] = row[index]

	with open(os.path.join(outputPath, singles['name'] + '.dtsi'), 'w') as dtsiFile:

		dtsiFile.write('/**\n'
		 ' * \\file\n'
		 ' * \\brief Devicetree fragment for {name}\n'
		 ' *\n'
		 ' * \\author Copyright (C) 2014-{year:04d} Kamil Szczygiel'
		 ' http://www.distortec.com http://www.freddiechopin.info\n'
		 ' *\n'
		 ' * \par License\n'
		 ' * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.'
		 ' If a copy of the MPL was not\n'
		 ' * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.\n'
		 ' *\n'
		 ' * \warning\n'
		 ' * Automatically generated file - do not edit!\n'
		 ' */\n'
		 '\n'
		 '#include "{architecture}.dtsi"\n'
		 '\n'
		 '/ {{\n'
		 ''.format(name = singles['name'], year = datetime.date.today().year, architecture = singles['architecture']))

		first = True
		for groupType, group in groups.items():
			for label, data in group.items():
				if first == False:
					dtsiFile.write('\n')
				dtsiFile.write(handlers[groupType](label, data))
				first = False

		dtsiFile.write('};\n')

########################################################################################################################
# main
########################################################################################################################

parser = argparse.ArgumentParser()
parser.add_argument('csvFile', help = 'input CSV file')
parser.add_argument('outputPath', help = 'output path')
arguments = parser.parse_args()

with open(arguments.csvFile, newline = '') as csvFile:
	csvReader = csv.reader(csvFile)
	header = list(zip(next(csvReader), next(csvReader), next(csvReader)))
	for row in csvReader:
		handleRow(arguments.outputPath, header, row)
