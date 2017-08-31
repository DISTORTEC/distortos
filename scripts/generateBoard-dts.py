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
# Returns devicetree node from specified path.
#
# param [in] dictionary is the dictionary with devicetree
# param [in] path is the path of selected node in devicetree
#
# return devicetree node from specified path
#

def getNode(dictionary, path):
	pathComponents = path.split('/')
	if pathComponents[0] == '':
		pathComponents[0] = '/'
	node = dictionary
	for pathComponent in pathComponents:
		node = node.get('nodes', node)[pathComponent]
	return node

#
# Tests whether a devicetree node contains matching property names with matching values.
#
# param [in] node is the node which will be tested
# param [in] propertyPatterns is a list of tuples, each with property name pattern and property value pattern, empty
# list accepts all nodes
#
# return True if node contains matching property names with matching values (or if propertyPatterns is an empty list),
# False otherwise
#

def testProperties(node, propertyPatterns):
	if len(propertyPatterns) == 0:
		return True
	for namePattern, valuePattern in propertyPatterns:
		nameRegex = re.compile(namePattern)
		valueRegex = re.compile(valuePattern)
		for name, values in node['properties'].items():
			if nameRegex.match(name) != None:
				for value in values:
					if valueRegex.match(value) != None:
						return True
	return False

#
# Generator which can be used to iterate devicetree nodes.
#
# param [in] dictionary is the dictionary which will be iterated over
# param [in] path is the list with current path components
#
# return node path and node dictionary
#

def iterateNodesImplementation(dictionary, path = []):
	for name, node in dictionary.items():
		if 'nodes' in node and 'properties' in node:
			yield '/'.join(path + [name]).replace('//', '/'), node
			for subpath, subnode in iterateNodesImplementation(node['nodes'], path + [name]):
				yield subpath, subnode

#
# Generator which can be used to iterate devicetree nodes, with optional filtering by node path and/or by presence of
# matching property names with matching values
#
# param [in] dictionary is the dictionary which will be iterated over
# param [in] pathPattern is the path pattern, empty string accepts all paths, default - empty string
# param [in] propertyPatterns is a list of tuples, each with property name pattern and property value pattern, empty
# list accepts all nodes, default - empty list
#
# return node path and node dictionary
#

def iterateNodes(dictionary, pathPattern = '', propertyPatterns = []):
	regex = re.compile(pathPattern)
	for path, node in iterateNodesImplementation(dictionary):
		if regex.match(path) != None:
			if testProperties(node, propertyPatterns) == True:
				yield path, node

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

def sanitize(path, pattern = '[^0-9A-Za-z-]', replacement = '_'):
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

	print()
	print('Reading dts...')

	dictionary = pydts.loadDictionary(arguments.inputFile, arguments.in_format == 'dts')

	print()
	print('Searching for metadata and rendering files...')

	jinjaEnvironment = jinja2.Environment(trim_blocks = True, lstrip_blocks = True, keep_trailing_newline = True,
			loader = jinja2.FileSystemLoader('.'))
	jinjaEnvironment.globals['outputPath'] = arguments.outputPath.rstrip('/')
	jinjaEnvironment.globals['year'] = datetime.date.today().year
	jinjaEnvironment.globals['getNode'] = getNode
	jinjaEnvironment.globals['iterateNodes'] = iterateNodes
	jinjaEnvironment.filters['sanitize'] = sanitize

	for currentDirectory, directories, filenames in os.walk('.'):
		files = [os.path.join(currentDirectory, filename) for filename in filenames]
		for metadataFile in fnmatch.filter(files, '*/boardTemplates/*.metadata'):
			print('Trying {}... '.format(metadataFile))
			metadata = jinjaEnvironment.get_template(metadataFile).render(dictionary = dictionary)
			for templateFile, templateArguments, outputFile in ast.literal_eval('[' + metadata + ']'):
				outputFile = os.path.normpath(os.path.join(arguments.outputPath, outputFile))

				outputDirectory = os.path.dirname(outputFile)
				if os.path.exists(outputDirectory) == False:
					os.makedirs(outputDirectory)

				output = jinjaEnvironment.get_template(templateFile).render(dictionary = dictionary, **templateArguments)
				with open(outputFile, 'w') as file:
					print(' - {} -> {}'.format(templateFile, outputFile))
					file.write(output)

	print()
	print('Done')
