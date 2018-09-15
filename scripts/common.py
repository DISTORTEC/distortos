#!/usr/bin/env python

#
# file: common.py
#
# author: Copyright (C) 2017-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import re

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

def sanitize(string, pattern = '[^0-9A-Za-z-]', replacement = '_'):
	"""Sanitize and return provided string.

	* `string` is the string that will be sanitized
	* `pattern` is the pattern which will be replaced, default - `'[^0-9A-Za-z-]'`
	* `replacement` is the replacement string, default - `'_'`
	"""
	regex = re.compile(pattern)
	return regex.sub(replacement, str(string))
