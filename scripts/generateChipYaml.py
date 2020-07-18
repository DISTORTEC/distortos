#!/usr/bin/env python

#
# file: generateChipYaml.py
#
# author: Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import argparse
import ast
import common
from common import Reference
import csv
import datetime
import os
import re
import ruamel.yaml

def addLabels(dictionary, labels):
	"""Add `'$labels'` keys to appropriate nodes in dictionary and return this dictionary.

	* `dictionary` is a dictionary representing an assembled row of CSV file
	* `labels` is a list of labels which should be added to the dictionary
	"""
	for keys, label in labels:
		try:
			node = dictionary
			for key in keys:
				node = node[key]
			node['$labels'] = [label]
		except KeyError:
			pass

	return dictionary

def parseFirstRow(firstRow):
	"""Parse first row of the CSV file into list of paths and list of labels and return tuple with the list of paths.

	List of paths has one entry for each column in the CSV file. Each entry is a list of keys for the element in that
	column.

	List of labels has one entry for any path which should have the label added. Each entry is a tuple of path (list of
	keys) and a label for the node on that path.

	* `firstRow` is a list of strings representing the first row of the CSV file
	"""
	previousKeys = firstRow[0].split('/')
	paths = []
	labels = []
	for header in firstRow:
		keys = parseString(header)

		i = 0
		while keys[i] == '..':
			i += 1
		prefix = previousKeys[0:-i]
		keys[0:i] = prefix
		prefixLength = len(prefix)

		for i, key in enumerate(keys[prefixLength:], start = prefixLength):
			keys[i] = key
			if isinstance(key, str) == True:
				key, separator, label = key.partition('$')
				keys[i] = key
				if separator == '$':
					labels.append((keys[:i + 1], label or key))

		paths.append(keys)
		previousKeys = keys

	return paths, labels

def parseRow(row, paths):
	"""Assemble single row from CSV file into a dictionary and return this dictionary.

	* `row` is a list of strings representing a single row of CSV file
	* `paths` is a list of paths parsed from the first row of CSV file
	"""
	dictionary = {}
	for column, element in enumerate(row):
		if element != '':
			node = dictionary
			for key in paths[column][:-1]:
				node = node.setdefault(key, {})
			node[paths[column][-1]] = parseString(element)

	return dictionary

def parseString(string):
	"""Try to parse `string` into proper type and return it.

	Only selected Python constructs are whitelisted, everything else will be rejected. The code is based on
	https://stackoverflow.com/a/4236328/157344

	* `string` is the string which will be parsed
	"""
	tree = ast.parse(string)
	try:
		# Python 3 has `ast.NameConstant`...
		whitelist = (ast.Dict, ast.Expr, ast.keyword, ast.List, ast.Load, ast.Module, ast.Name, ast.NameConstant,
				ast.Num, ast.Str)
	except AttributeError:
		# ... while Python 2 does not
		whitelist = (ast.Dict, ast.Expr, ast.keyword, ast.List, ast.Load, ast.Module, ast.Name, ast.Num, ast.Str)
	for node in ast.walk(tree):
		if isinstance(node, whitelist):
			continue
		if isinstance(node, ast.Call) and isinstance(node.func, ast.Name) and node.func.id == 'Reference':
			continue

		raise SyntaxError(string)

	return eval(string)

########################################################################################################################
# main
########################################################################################################################

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('csvFile', help = 'input CSV file')
	parser.add_argument('-o','--outputPath', help = 'output path')
	arguments = parser.parse_args()

	if not arguments.outputPath:
		arguments.outputPath = os.path.dirname(arguments.csvFile) + '/chipYaml'

	year = datetime.date.today().year

	with open(arguments.csvFile) as csvFile:
		csvReader = csv.reader(csvFile)
		firstRow = next(csvReader)
		paths, labels = parseFirstRow(firstRow)
		yaml = ruamel.yaml.YAML()
		yaml.register_class(Reference)
		for row in csvReader:
			yamlFilename = os.path.join(arguments.outputPath, common.sanitize(parseString(row[0])[0]) + '.yaml')
			print('Generating {}...'.format(yamlFilename))
			with open(yamlFilename, 'w') as yamlFile:
				dictionary = parseRow(row, paths)
				dictionary = addLabels(dictionary, labels)
				yamlFile.write("#\n"
						"# file: {}\n"
						"#\n"
						"# author: Copyright (C) {} Kamil Szczygiel https://distortec.com "
						"https://freddiechopin.info\n"
						"#\n"
						"# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a "
						"copy of the MPL was not\n"
						"# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.\n"
						"#\n"
						"# Automatically generated file - do not edit!\n"
						"#\n"
						"\n".format(os.path.basename(yamlFilename), year))
				yaml.dump(dictionary, yamlFile)
