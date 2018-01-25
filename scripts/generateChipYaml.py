#!/usr/bin/env python

#
# file: generateChipYaml.py
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import argparse
import ast
import common
import csv
import os
import ruamel.yaml

#
# Adds '$labels' keys to appropriate nodes in dictionary.
#
# param [in] dictionary is a dictionary representing an assembled row of CSV file
# param [in] labels is a list of labels which should be added to the dictionary
#
# return dictionary with added labels
#

def addLabels(dictionary, labels):
	for keys, label in labels:
		try:
			node = dictionary
			for key in keys:
				node = node[key]
			node['$labels'] = [label]
		except:
			pass

	return dictionary

#
# Parses first row of the CSV file into list of paths and list of labels.
#
# List of paths has one entry for each column in the CSV file. Each entry is a list of keys for the element in that
# column.
#
# List of labels has one entry for any path which should have the label added. Each entry is a tuple of path (list of
# keys) and a label for the node on that path.
#
# param [in] firstRow is a list of strings representing the first row of the CSV file
#
# return tuple with a list of paths and a list of labels
#

def parseFirstRow(firstRow):
	previousKeys = firstRow[0].split('/')
	paths = []
	labels = []
	for column, header in enumerate(firstRow):
		keys = header.split('/')

		i = 0
		while keys[i] == '..':
			i += 1
		keys[0:i] = previousKeys[0:-i]

		for i, key in enumerate(keys):
			if isinstance(key, str):
				key, separator, label = key.partition('$')
				key = parseString(key)
				keys[i] = key
				if separator == '$':
					labels.append((keys[:i + 1], label or key))

		paths.append(keys)
		previousKeys = keys

	return paths, labels

#
# Assembles single row from CSV file into a dictionary
#
# param [in] row is a list of strings representing a single row of CSV file
# param [in] paths is a list of paths parsed from the first row of CSV file
#
# return dictionary representing data from row assembled according to paths
#

def parseRow(row, paths):
	dictionary = {}
	for column, element in enumerate(row):
		if element != '':
			node = dictionary
			for key in paths[column][:-1]:
				node = node.setdefault(key, {})
			node[paths[column][-1]] = parseString(element)

	return dictionary

#
# Tries to parse string into proper type
#
# param [in] string is the string which will be parsed
#
# return string possibly parsed into another type
#

def parseString(string):
	try:
		return ast.literal_eval(string)
	except ValueError:
		return string

########################################################################################################################
# main
########################################################################################################################

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('csvFile', help = 'input CSV file')
	parser.add_argument('outputPath', help = 'output path')
	arguments = parser.parse_args()

	with open(arguments.csvFile) as csvFile:
		csvReader = csv.reader(csvFile)
		firstRow = next(csvReader)
		paths, labels = parseFirstRow(firstRow)
		yaml = ruamel.yaml.YAML()
		for row in csvReader:
			yamlFilename = os.path.join(arguments.outputPath, common.sanitize(parseString(row[0])[0]) + '.yaml')
			print('Generating {}...'.format(yamlFilename))
			with open(yamlFilename, 'w') as yamlFile:
				dictionary = parseRow(row, paths)
				dictionary = addLabels(dictionary, labels)
				yaml.dump(dictionary, yamlFile)
