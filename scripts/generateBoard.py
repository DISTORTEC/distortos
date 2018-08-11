#!/usr/bin/env python

#
# file: generateBoard.py
#
# author: Copyright (C) 2017-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from __future__ import print_function

import argparse
import ast
import collections
import common
import datetime
import fnmatch
import jinja2
import jinja2.ext
import operator
import os
import posixpath
import re
import ruamel.yaml
import sys

class RaiseExtension(jinja2.ext.Extension):
	"""Extension that can raise an exception from within Jinja template"""

	tags = set(['raise'])

	def parse(self, parser):
		lineNumber = next(parser.stream).lineno
		message = parser.parse_expression()
		name = parser.name
		filename = parser.filename
		arguments = [message, jinja2.nodes.Const(lineNumber), jinja2.nodes.Const(name), jinja2.nodes.Const(filename)]
		return jinja2.nodes.CallBlock(self.call_method('raiseImplementation', arguments), [], [], [])

	def raiseImplementation(self, message, lineNumber, name, filename, caller):
		raise jinja2.exceptions.TemplateAssertionError(message, lineNumber, name, filename)

class Reference(object):
	"""Reference to label in YAML"""

	yaml_tag = '!Reference'

	def __init__(self, label):
		self.label = label

	def __repr__(self):
		return "{}(label = {})".format(self.__class__.__name__, self.label)

	def __eq__(self, other):
		if isinstance(self, other.__class__) == True:
			return self.label == other.label
		return False

	def __ne__(self, other):
		return not self.__eq__(other)

	def __hash__(self):
		return self.label.__hash__()

def addPaths(dictionary, path = None):
	"""Extend each node of the dictionary with its path.

	For each node of the dictionary a `'$path'` key is added, which holds node's path - a list of keys.

	* `dictionary` is the dictionary in which paths will be added
	* `path` is the current path (list of keys) in the dictionary, default - `None`
	"""
	path = path or []
	for key, value in dictionary.items():
		if isinstance(value, collections.MutableMapping) == True and '$path' not in value:
			newPath = path + [key]
			value['$path'] = newPath
			addPaths(value, newPath)

def getLabels(dictionary, labels = None):
	"""Build a dictionary with labels and return it.

	Each entry in the dictionary has label as the key and node as value.

	* `dictionary` is the dictionary in which labels are searched for
	* `labels `is the current stage of dictionary with labels, default - `None`
	"""
	labels = labels or {}
	for key, value in dictionary.items():
		if key == '$labels':
			for label in value:
				labels[label] = dictionary
		elif isinstance(value, collections.MutableMapping) == True:
			labels = getLabels(value, labels)
	return labels

def isFullMatch(string, pattern, flags = 0):
	"""Tests whether string fully matches given pattern.

	* `string` is the string that will be tested
	* `pattern` is the pattern which will be used in the test
	* `flags` are flags passed to `re.fullmatch()`, default - `0`
	"""
	# equivalent of re.fullmatch() which works in Python versions prior to 3.4
	return re.match('(?:' + pattern + ')\Z', str(string), flags)

def mergeDictionaries(a, b):
	"""Merge two dictionaries into one and return merged dictionary.

	Recursively handle nested dictionaries. If given key exists in both dictionaries, value from `b` overwrites value
	from `a`.

	* `a` is the dictionary into which `b` will be merged
	* `b` is the dictionary which will be merged into `a`
	"""
	for key in b:
		if key in a:
			if (isinstance(a[key], collections.MutableMapping) == True and
					isinstance(b[key], collections.MutableMapping) == True):
				mergeDictionaries(a[key], b[key])
			elif a[key] != b[key]:
				a[key] = b[key]
		else:
			a[key] = b[key]
	return a

def resolveExtensions(dictionary, distortosPath):
	"""Recursively resolve all `'$extends'` keys and return dictionary with resolved keys.

	* `dictionary` is the dictionary in which `'$extends'` keys will be recursively resolved
	* `distortosPath` is the path to distortos, prepended to all values of `'$extends'` keys
	"""
	if '$extends' in dictionary:
		for filename in dictionary['$extends']:
			with open(os.path.join(distortosPath, filename)) as yamlFile:
				extendedDictionary = yaml.load(yamlFile)
			extendedDictionary = resolveExtensions(extendedDictionary, distortosPath)
			dictionary = mergeDictionaries(extendedDictionary, dictionary)
		del dictionary['$extends']
	return dictionary

def resolveReferences(dictionary, labels):
	"""Resolve all references in a dictionary.

	* `dictionary` is a dictionary of a list in which references will be resolved
	* `labels` is a dictionary with labels
	"""
	keysForDeletion = []
	for key, value in dictionary.items():
		if isinstance(key, Reference) == True:
			mergeDictionaries(labels[key.label], value)
			keysForDeletion.append(key)
		elif isinstance(value, Reference) == True:
			dictionary[key] = labels[value.label]
		elif isinstance(value, collections.MutableMapping) == True:
			resolveReferences(value, labels)

	for keyForDeletion in keysForDeletion:
		del dictionary[keyForDeletion]

########################################################################################################################
# main
########################################################################################################################

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('inputFile', type = argparse.FileType('r'), help = 'input file')
	parser.add_argument('outputPath', help = 'output path')
	parser.add_argument('distortosPath', help = 'distortos path')
	arguments = parser.parse_args()

	yaml = ruamel.yaml.YAML()
	yaml.register_class(Reference)

	dictionary = yaml.load(arguments.inputFile)
	dictionary = resolveExtensions(dictionary, arguments.distortosPath)
	labels = getLabels(dictionary)
	resolveReferences(dictionary, labels)
	addPaths(dictionary)

	# in case of "raw" board - generated directly from chip YAML file - use chip name as board
	board = dictionary.get('board', dictionary['chip'])['compatible'][0]

	relativeOutputPath = posixpath.relpath(posixpath.realpath(arguments.outputPath),
			posixpath.realpath(arguments.distortosPath))

	jinjaEnvironment = jinja2.Environment(trim_blocks = True, lstrip_blocks = True, keep_trailing_newline = True,
			loader = jinja2.FileSystemLoader(['.', arguments.distortosPath]))
	jinjaEnvironment.add_extension(RaiseExtension)
	jinjaEnvironment.filters['sanitize'] = common.sanitize
	jinjaEnvironment.globals['board'] = board
	jinjaEnvironment.globals['outputPath'] = relativeOutputPath
	jinjaEnvironment.globals['sanitizedBoard'] = common.sanitize(board)
	jinjaEnvironment.globals['year'] = datetime.date.today().year
	jinjaEnvironment.tests['fullMatch'] = isFullMatch

	metadata = []

	for currentDirectory, directories, filenames in os.walk('.', followlinks = True):
		# jinja expects forward slashes on all systems - https://github.com/pallets/jinja/issues/767
		currentDirectory = currentDirectory.replace('\\', '/')
		localFilenames = [posixpath.join(currentDirectory, filename) for filename in filenames]
		for metadataFilename in fnmatch.filter(localFilenames, '*/boardTemplates/*.metadata'):
			print('Loading {}'.format(metadataFilename))
			try:
				metadataFragment = jinjaEnvironment.get_template(metadataFilename).render(dictionary = dictionary)
			except jinja2.exceptions.TemplateAssertionError as exception:
				sys.exit("{}:{}: error: {}".format(exception.filename, exception.lineno, exception.message))
			metadata += ast.literal_eval('[' + metadataFragment + ']')

	print()

	metadata.sort(key = operator.itemgetter(2))
	for metadataIndex, metadataRow in enumerate(metadata):
		templateFilename, templateArguments, outputFilename = metadataRow
		relativeOutputFilename = posixpath.normpath(posixpath.join(relativeOutputPath, outputFilename))
		outputFilename = posixpath.normpath(posixpath.join(arguments.outputPath, outputFilename))

		outputDirectory = posixpath.dirname(outputFilename)
		if posixpath.exists(outputDirectory) == False:
			os.makedirs(outputDirectory)

		templateFile = jinjaEnvironment.get_template(templateFilename)
		try:
			output = templateFile.render(dictionary = dictionary, metadata = metadata,
					metadataIndex = metadataIndex, outputFilename = relativeOutputFilename, **templateArguments)
		except jinja2.exceptions.TemplateAssertionError as exception:
			sys.exit("{}:{}: error: {}".format(exception.filename, exception.lineno, exception.message))
		with open(outputFilename, 'w') as outputFile:
			print('Writing {} ({})'.format(outputFilename, templateFile.filename))
			outputFile.write(output)

	print()
	print('Done')
