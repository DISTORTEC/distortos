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
import common
import datetime
import fnmatch
import jinja2
import os
import posixpath
import ruamel.yaml

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
		if isinstance(value, dict) == True:
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
		elif isinstance(value, dict) == True:
			labels = getLabels(value, labels)
	return labels

def mergeDictionaries(a, b):
	"""Merge two dictionaries into one and return merged dictionary.

	Recursively handle nested dictionaries. If given key exists in both dictionaries, value from `b` overwrites value
	from `a`.

	* `a` is the dictionary into which `b` will be merged
	* `b` is the dictionary which will be merged into `a`
	"""
	for key in b:
		if key in a:
			if isinstance(a[key], dict) == True and isinstance(b[key], dict) == True:
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
	for key, value in dictionary.items():
		if isinstance(key, Reference) == True:
			mergeDictionaries(labels[key.label], value)
			del dictionary[key]
		elif isinstance(value, Reference) == True:
			dictionary[key] = labels[value.label]
		elif isinstance(value, dict) == True:
			resolveReferences(value, labels)

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
	addPaths(dictionary)
	labels = getLabels(dictionary)
	resolveReferences(dictionary, labels)

	# in case of "raw" board - generated directly from chip YAML file - use chip name as board
	board = dictionary.get('board', dictionary['chip'])['compatible'][0]

	relativeOutputPath = posixpath.relpath(posixpath.realpath(arguments.outputPath),
			posixpath.realpath(arguments.distortosPath))

	jinjaEnvironment = jinja2.Environment(trim_blocks = True, lstrip_blocks = True, keep_trailing_newline = True,
			loader = jinja2.FileSystemLoader(['.', arguments.distortosPath]))
	jinjaEnvironment.filters['sanitize'] = common.sanitize
	jinjaEnvironment.globals['board'] = board
	jinjaEnvironment.globals['outputPath'] = relativeOutputPath
	jinjaEnvironment.globals['sanitizedBoard'] = common.sanitize(board)
	jinjaEnvironment.globals['year'] = datetime.date.today().year

	metadata = []

	for currentDirectory, directories, filenames in os.walk('.', followlinks = True):
		# jinja expects forward slashes on all systems - https://github.com/pallets/jinja/issues/767
		currentDirectory = currentDirectory.replace('\\', '/')
		files = [posixpath.join(currentDirectory, filename) for filename in filenames]
		for metadataFile in fnmatch.filter(files, '*/boardTemplates/*.metadata'):
			print('Loading {}'.format(metadataFile))
			metadataFragment = jinjaEnvironment.get_template(metadataFile).render(dictionary = dictionary)
			metadata += ast.literal_eval('[' + metadataFragment + ']')

	print()

	for metadataIndex, metadataRow in enumerate(metadata):
		templateFile, templateArguments, outputFile = metadataRow
		relativeOutputFile = posixpath.normpath(posixpath.join(relativeOutputPath, outputFile))
		outputFile = posixpath.normpath(posixpath.join(arguments.outputPath, outputFile))

		outputDirectory = posixpath.dirname(outputFile)
		if posixpath.exists(outputDirectory) == False:
			os.makedirs(outputDirectory)

		output = jinjaEnvironment.get_template(templateFile).render(dictionary = dictionary, metadata = metadata,
				metadataIndex = metadataIndex, outputFile = relativeOutputFile, **templateArguments)
		with open(outputFile, 'w') as file:
			print('Writing {} ({})'.format(outputFile, templateFile))
			file.write(output)

	print()
	print('Done')
