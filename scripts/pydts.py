#!/usr/bin/env python

#
# file: pydts.py
#
# Grammar was greatly inspired by the original dtc.
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import ast
import argparse
import pickle
import ply.lex
import ply.yacc
import pprint
import sys

########################################################################################################################
# common functions
########################################################################################################################

#
# Prints error with error location information
#
# param [in] error is the error string that will be printed
# param [in] token is the token which caused the error
#

def printError(error, token):
	previousNewline = token.lexer.lexdata.rfind('\n', 0, token.lexpos)
	column = token.lexpos - previousNewline
	location = '%s:%d:%d' % (token.lexer.inputFilename, token.lexer.lineno, column)
	if hasattr(token.lexer, 'trackedFilename') and hasattr(token.lexer, 'trackedLineNumber'):
		location += ' (%s:%d:%d)' % (token.lexer.trackedFilename, token.lexer.trackedLineNumber, column)
	sys.stderr.write('%s: %s\n' % (location, error))
	sys.stderr.write("'%s'\n" % token.lexer.lexdata[previousNewline + 1:token.lexer.lexdata.find('\n', token.lexpos)])
	sys.stderr.write(' ' * column + '^\n')

########################################################################################################################
# lexer
########################################################################################################################

# lexer states
states = (
		# NODEPROPERTYNAME state is started when '{' or ';' are encountered and ends after single match
		('NODEPROPERTYNAME', 'exclusive'),
)

# lexer tokens
tokens = (
		'DTS_V1',
		'NODE_PROPERTY_NAME',
		'INTEGER_LITERAL',
		'STRING_LITERAL',
		'LABEL',
		'LABEL_REFERENCE',
)

# string literal, enclosed in double quotes, may contain escape sequences (including '\"')
quotedString = r'"([^"]|\\.)*"'

# name of label
labelName = r'[A-Za-z_][0-9A-Za-z_]*'

#
# C-preprocessor linemarker - discarded
#
# When a file is preprocessed with `cpp -E`, then linemarkers in the following form are generated:
# '# <line> <file>[ <flags>]'
# where:
# - <line> is an integer;
# - <file> is a quoted string;
# - <flags> (optional) are integers 1-4 separated with spaces;
#
# This info is used to track location in the "source" files
#
# param [in] t is the matched token
#

@ply.lex.TOKEN(r'(?m)^\#\ \d+\ ' + quotedString + r'(\ \d+)*')
def t_ANY_linemarker(t):
	parts = t.value[2:].split()
	t.lexer.trackedLineNumber = int(parts[0]) - 1
	t.lexer.trackedFilename = parts[1].strip('"')

#
# String literal, enclosed in double quotes, may contain escape sequences (including '\"')
#
# param [in] t is the matched token
#
# return matched token with quotes removed
#

@ply.lex.TOKEN(quotedString)
def t_STRING_LITERAL(t):
	t.value = t.value[1:-1]
	return t

# DTS version 1 identifier
t_ANY_DTS_V1 = r'/dts-v1/'

#
# Label
#
# param [in] t is the matched token
#
# return matched token with trailing colon removed
#

@ply.lex.TOKEN(labelName + r':')
def t_ANY_LABEL(t):
	t.value = t.value[0:-1]
	return t

#
# Helper function for integer literal
#
# param [in] t is the matched token
# param [in] offset is the offset since the beginning of token that will be applied before conversion
# param [in] base is the base used for conversion
#
# return matched token with value converted to integer
#

def integerLiteral(t, offset, base):
	t.type  = 'INTEGER_LITERAL'
	t.value = int(t.value[offset:], base)
	return t

#
# Binary integer literal
#
# param [in] t is the matched token
#
# return matched token with value converted to integer
#

def t_binaryLiteral(t):
	r'0[Bb][01]+'
	return integerLiteral(t, 2, 2)

#
# Hexadecimal integer literal
#
# param [in] t is the matched token
#
# return matched token with value converted to integer
#

def t_hexadecimalLiteral(t):
	r'0[Xx][0-9A-Fa-f]+'
	return integerLiteral(t, 2, 16)

#
# Octal integer literal
#
# param [in] t is the matched token
#
# return matched token with value converted to integer
#

def t_octalLiteral(t):
	r'0[0-7]+'
	return integerLiteral(t, 1, 8)

#
# Decimal integer literal
#
# param [in] t is the matched token
#
# return matched token with value converted to integer
#

def t_decimalLiteral(t):
	r'[0-9]+'
	return integerLiteral(t, 0, 10)

#
# Reference to label
#
# param [in] t is the matched token
#
# return matched token with leading '&' removed
#

@ply.lex.TOKEN(r'&' + labelName)
def t_ANY_LABEL_REFERENCE(t):
	t.value = t.value[1:]
	return t

#
# Node or property name
#
# param [in] t is the matched token
#
# return matched token
#

def t_NODEPROPERTYNAME_NODE_PROPERTY_NAME(t):
	r'[0-9A-Za-z\#+,.?@_-]+'
	t.lexer.begin('INITIAL')
	return t

#
# Newline - discarded
#
# param [in] t is the matched token
#

def t_ANY_newline(t):
	r'\n+'
	length = len(t.value)
	t.lexer.lineno += length
	if hasattr(t.lexer, 'trackedLineNumber'):
		t.lexer.trackedLineNumber += length

#
# Begin NODEPROPERTYNAME state
#
# Reuses some of matched literals
#
# param [in] t is the matched token
#
# return matched token
#

def t_ANY_begin_NODEPROPERTYNAME(t):
	r';|{'
	t.type = t.value
	t.lexer.begin('NODEPROPERTYNAME')
	return t

# lexer literals
literals = '()*+,/;<=>{}'

# ignored whitespace
t_ANY_ignore  = '\t '

#
# Lexer error handler
#
# param [in] t is the matched token
#

def t_ANY_error(t):
	printError('Illegal character', t)

########################################################################################################################
# parser
########################################################################################################################

#
# Merges two dictionaries into one
#
# Recursively handles nested dictionaries. If given key exists in both dictionaries, value from b overwrites value from
# a.
#
# param [in] a is the dictionary into which b will be merged
# param [in] b is the dictionary which will be merged into a
#
# return merged dictionary
#

def mergeDictionaries(a, b):
	for key in b:
		if key in a:
			if isinstance(a[key], dict) and isinstance(b[key], dict):
				mergeDictionaries(a[key], b[key])
			elif a[key] != b[key]:
				a[key] = b[key]
		else:
			a[key] = b[key]
	return a

# precedence of arithmetic operators
precedence = (
		('left', '+'),
		('left', '*'),
)

#
# "sourcefile" 0
#
# "headers" must preceede "devicetree".
#
# param [in] p is the matched production
#

def p_sourcefile0(p):
	'''
	sourcefile : headers devicetree
	'''
	p[0] = {p[2][0]: p[2][1]}

#
# "headers" 0
#
# "headers" must consist of at least one "header".
#
# param [in] p is the matched production
#

def p_headers0(p):
	'''
	headers : header
	'''
	pass

#
# "headers" 1
#
# Multiple "header" productions are concatenated into single "headers".
#
# param [in] p is the matched production
#

def p_headers1(p):
	'''
	headers : header headers
	'''
	pass

#
# "header" 0
#
# DTS version 1 identifier is a "header".
#
# param [in] p is the matched production
#

def p_header0(p):
	'''
	header : DTS_V1 ';'
	'''
	pass

#
# "devicetree" 0
#
# Root node label and "nodeDefinition" form "devicetree" (tuple with label as string and node as dictionary).
#
# param [in] p is the matched production
#

def p_devicetree0(p):
	'''
	devicetree : '/' nodeDefinition
	'''
	p[0] = ('/', p[2])

#
# "devicetree" 1
#
# Multiple root nodes are concatenated into single "devicetree".
#
# param [in] p is the matched production
#

def p_devicetree1(p):
	'''
	devicetree : devicetree '/' nodeDefinition
	'''
	mergeDictionaries(p[1][1], p[3])
	p[0] = p[1]

#
# "devicetree" 2
#
# Nodes with label reference are concatenated into "devicetree" in appropriate location using labels dictionary.
#
# param [in] p is the matched production
#

def p_devicetree2(p):
	'''
	devicetree : devicetree LABEL_REFERENCE nodeDefinition
	'''
	mergeDictionaries(p.parser.labels[p[2]]['node'], p[3])
	p[0] = p[1]

#
# "nodeDefinition" 0
#
# "nodeDefinition" consists of zero or more "properties" followed by zero or more "nodes", enclosed in braces and
# followed by semicolon. "nodeDefinition" is a dictionary with two elements: 'properties' (dictionary) and 'nodes'
# (dictionary).
#
# param [in] p is the matched production
#

def p_nodeDefinition0(p):
	'''
	nodeDefinition : '{' properties nodes '}' ';'
	'''
	p[0] = {'properties': p[2], 'nodes': p[3]}

#
# "properties" 0
#
# There may be zero "properties". "properties" are represented as a dictionary of name-value pairs.
#
# param [in] p is the matched production
#

def p_properties0(p):
	'''
	properties : empty
	'''
	p[0] = {}

#
# "properties" 1
#
# Multiple "properties" are concatenated together.
#
# param [in] p is the matched production
#

def p_properties1(p):
	'''
	properties : properties property
	'''
	p[1].update(p[2])
	p[0] = p[1]

#
# "property" 0
#
# "property" may have no value, which is represented as an empty list.
#
# param [in] p is the matched production
#

def p_property0(p):
	'''
	property : NODE_PROPERTY_NAME ';'
	'''
	p[0] = {p[1]: []}

#
# "property" 1
#
# "property" may have one or more values, which are represented as a list.
#
# param [in] p is the matched production
#

def p_property1(p):
	'''
	property : NODE_PROPERTY_NAME '=' propertyValue ';'
	'''
	p[0] = {p[1]: p[3]}

#
# "propertyValue" 0
#
# "propertyValue" may be a string, represented as new item (string) on the list of other values for given property.
#
# param [in] p is the matched production
#

def p_propertyValue0(p):
	'''
	propertyValue : propertyValuePrefix STRING_LITERAL
	'''
	p[1].append(p[2])
	p[0] = p[1]

#
# "propertyValue" 1
#
# "propertyValue" may be an array of cells, represented as new item (list of elements) on the list of other values for
# given property.
#
# param [in] p is the matched production
#

def p_propertyValue1(p):
	'''
	propertyValue : propertyValuePrefix arrayPrefix '>'
	'''
	p[1].append(p[2])
	p[0] = p[1]

#
# "propertyValue" 2
#
# "propertyValue" may be a label reference, represented as new item (dictionary with single 'label' element) on the list
# of other values for given property.
#
# param [in] p is the matched production
#

def p_propertyValue2(p):
	'''
	propertyValue : propertyValuePrefix LABEL_REFERENCE
	'''
	p[1].append({'label': p[2]})
	p[0] = p[1]

#
# "arrayPrefix" 0
#
# Array of cells may be empty, which is represented as an empty list.
#
# param [in] p is the matched production
#

def p_arrayPrefix0(p):
	'''
	arrayPrefix : '<'
	'''
	p[0] = []

#
# "arrayPrefix" 1
#
# Array of cells may be contain integer literal, represented as an integer.
#
# param [in] p is the matched production
#

def p_arrayPrefix1(p):
	'''
	arrayPrefix : arrayPrefix integerLiteral
	'''
	p[1].append(p[2])
	p[0] = p[1]

#
# "arrayPrefix" 2
#
# Array of cells may be contain label reference, represented as a dictionary with single 'label' element.
#
# param [in] p is the matched production
#

def p_arrayPrefix2(p):
	'''
	arrayPrefix : arrayPrefix LABEL_REFERENCE
	'''
	p[1].append({'label': p[2]})
	p[0] = p[1]

#
# "propertyValuePrefix" 0
#
# There must be at least one property value after "=".
#
# param [in] p is the matched production
#

def p_propertyValuePrefix0(p):
	'''
	propertyValuePrefix : empty
	'''
	p[0] = []

#
# "propertyValuePrefix" 1
#
# There may be more than one property value.
#
# param [in] p is the matched production
#

def p_propertyValuePrefix1(p):
	'''
	propertyValuePrefix : propertyValue ','
	'''
	p[0] = p[1]

#
# "integerLiteral" 0
#
# param [in] p is the matched production
#

def p_integerLiteral0(p):
	'''
	integerLiteral	: INTEGER_LITERAL
	'''
	p[0] = p[1]

#
# "integerLiteral" 1
#
# "integerLiteral" enclosed in parentheses is an "integerLiteral".
#
# param [in] p is the matched production
#

def p_integerLiteral1(p):
	'''
	integerLiteral	: '(' integerLiteral ')'
	'''
	p[0] = p[2]

#
# "integerLiteral" 2
#
# Two "integerLiteral" productions can be added together to form an "integerLiteral".
#
# param [in] p is the matched production
#

def p_integerLiteral2(p):
	'''
	integerLiteral : integerLiteral '+' integerLiteral
	'''
	p[0] = p[1] + p[3]

#
# "integerLiteral" 3
#
# Two "integerLiteral" productions can be multiplied together to form an "integerLiteral".
#
# param [in] p is the matched production
#

def p_integerLiteral3(p):
	'''
	integerLiteral : integerLiteral '*' integerLiteral
	'''
	p[0] = p[1] * p[3]

#
# "nodes" 0
#
# There may be zero "nodes". "nodes" are represented as a dictionary of name-value pairs.
#
# param [in] p is the matched production
#

def p_nodes0(p):
	'''
	nodes : empty
	'''
	p[0] = {}

#
# "nodes" 1
#
# Multiple "nodes" are concatenated together.
#
# param [in] p is the matched production
#

def p_nodes1(p):
	'''
	nodes : node nodes
	'''
	p[2].update({p[1][0]: p[1][1]})
	p[0] = p[2]

#
# "node" 0
#
# Node name followed by "nodeDefinition" form a "node".
#
# param [in] p is the matched production
#

def p_node0(p):
	'''
	node : NODE_PROPERTY_NAME nodeDefinition
	'''
	p[0] = (p[1], p[2])

#
# "node" 1
#
# "node" may have a label.
#
# param [in] p is the matched production
#

def p_node1(p):
	'''
	node : LABEL node
	'''
	node = p[2][1]
	node['label'] = p[1]
	p[0] = p[2]
	p.parser.labels[p[1]] = {'path': None, 'node': node}

#
# Empty production
#
# param [in] p is the matched production
#

def p_empty(p):
	'''
	empty :
	'''
	pass

#
# Parser error handler
#
# param [in] p is the matched production
#

def p_error(p):
	if p:
		printError('Syntax error', p)
	raise SyntaxError

########################################################################################################################
# public functionality
########################################################################################################################

#
# Converts cell value to dts.
#
# Integers are converted to hexadecimal representation, labels are prefixed with '&'.
#
# param [in] cell is the cell value
#
# return cell value converted to dts
#

def cellToDts(cell):
	if isinstance(cell, int):
		return '0x%x' % cell
	else:
		return '&%s' % cell['label']

#
# Converts single property value to dts.
#
# Strings are enclosed in double quotes, cell arrays are enclosed in angle brackets, references are converted to quoted
# paths.
#
# param [in] value is the property value
#
# return property value converted to dts
#

def valueToDts(value):
	if isinstance(value, str):
		return '"%s"' % value
	elif isinstance(value, list):
		return '<%s>' % ' '.join(map(cellToDts, value))
	elif isinstance(value, dict):
		return '"%s"' % value['path']

#
# Converts property to dts.
#
# param [in] property is the name of converted property
# param [in] values is the list with property values
# param [in] indention is the current indention
#
# return property converted to dts
#

def propertyToDts(property, values, indention):
	dts = '%s%s' % (indention, property)
	if len(values) != 0:
		dts += ' = %s' % ', '.join(map(valueToDts, values))
	dts += ';\n'
	return dts

#
# Converts node to dts.
#
# param [in] name is the name of converted node
# param [in] node is the dictionary with node contents
# param [in] indention is the current indention, default - ''
#
# return node converted to dts
#

def nodeToDts(name, node, indention = ''):
	if 'label' in node:
		label = '%s: ' % node['label']
	else:
		label = ''
	dts = '%s%s%s {\n' % (indention, label, name)
	for property, values in sorted(node['properties'].items()):
		dts += propertyToDts(property, values, indention + '\t')
	for subName, subNode in sorted(node['nodes'].items()):
		dts += '\n%s' % nodeToDts(subName, subNode, indention + '\t')
	dts += '%s};\n' % indention
	return dts

#
# Converts dictionary to dts.
#
# param [in] dictionary is the converted dictionary
#
# return dictionary converted to dts
#

def dictionaryToDts(dictionary):
	return '/dts-v1/;\n\n%s' % nodeToDts('/', dictionary['/'])

#
# Converts dictionary to pickled representation.
#
# param [in] dictionary is the converted dictionary
#
# return dictionary converted to bytes with pickled representation
#

def dictionaryToPickle(dictionary):
	return pickle.dumps(dictionary, protocol = pickle.HIGHEST_PROTOCOL)

#
# Updates labels' dictionary with paths of the referenced nodes.
#
# param [in] node is currently processed node
# param [in, out] labels is the updated labels' dictionary
# param [in] path is the path of node, default = '/'
#

def updateLabels(node, labels, path = '/'):
	for name, subNode in node['nodes'].items():
		if 'label' in subNode:
			labels[subNode['label']]['path'] = path + name
		updateLabels(subNode, labels, path + name + '/')

#
# Helper to update single reference
#
# param [in] reference is reference dictionary which will be updated
# param [in] labels is the labels' dictionary
#

def updateReference(reference, labels):
	reference['path'] = labels[reference['label']]['path']

#
# Updates 'path' component of references in the node.
#
# param [in] node is currently processed node
# param [in] labels is the labels' dictionary
#

def updateReferences(node, labels):
	for values in node['properties'].values():
		for value in values:
			if isinstance(value, dict):
				updateReference(value, labels)
			elif isinstance(value, list):
				for cell in value:
					if isinstance(cell, dict):
						updateReference(cell, labels)
	for subNode in node['nodes'].values():
		updateReferences(subNode, labels)

#
# Converts dts to dictionary.
#
# param [in] dts are the contents of dts that will be converted
# param [in] inputFilename is the name of input file, used during error reporting only, default - '<string>'
#
# return dictionary parsed from dts
#

def dtsToDictionary(dts, inputFilename = '<string>'):
	lexer = ply.lex.lex()
	lexer.inputFilename = inputFilename
	parser = ply.yacc.yacc(debug = False)
	# labels' dictionary, each entry is a dictionary with 'path' (initially None, updated during second pass) and 'node'
	labels = {}
	parser.labels = labels
	dictionary = parser.parse(dts)
	updateLabels(dictionary['/'], labels)
	updateReferences(dictionary['/'], labels)
	return dictionary

########################################################################################################################
# main
########################################################################################################################

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('-I', '--in-format', choices = ['dictionary', 'dts', 'pickle'], default = 'dts',
			help = 'input format, default - dts')
	parser.add_argument('-O', '--out-format', choices = ['dictionary', 'dts', 'pickle'], default = 'pickle',
			help = 'output format, default - pickle')
	parser.add_argument('-o', '--out', default = '-', help = 'output file, default - stdout')
	parser.add_argument('inputFile', type = argparse.FileType('r'), help = 'input file')
	arguments = parser.parse_args()

	try:
		if arguments.in_format == 'dictionary':
			dictionary = ast.literal_eval(arguments.inputFile.read())
		elif arguments.in_format == 'dts':
			dictionary = dtsToDictionary(arguments.inputFile.read(), arguments.inputFile.name)
		else:
			dictionary = pickle.load(getattr(arguments.inputFile, 'buffer', arguments.inputFile))
	except (ply.lex.LexError, SyntaxError):
		sys.exit(1)
	except Exception as e:
		sys.exit(sys.exc_info())

	if arguments.out_format == 'dictionary':
		output = pprint.pformat(dictionary) + '\n'
	elif arguments.out_format == 'dts':
		output = dictionaryToDts(dictionary)
	else:
		output = dictionaryToPickle(dictionary)

	outputFile = sys.stdout if arguments.out == '-' else open(arguments.out, 'w')
	if isinstance(output, str) == True:
		outputFile.write(output)
	else:
		outputFile.buffer.write(output)

	if outputFile != sys.stdout:
		outputFile.close()
