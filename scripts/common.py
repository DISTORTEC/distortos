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

def sanitize(string, pattern = '[^0-9A-Za-z-]', replacement = '_'):
	"""Sanitize and return provided string.

	* `string` is the string that will be sanitized
	* `pattern` is the pattern which will be replaced, default - `'[^0-9A-Za-z-]'`
	* `replacement` is the replacement string, default - `'_'`
	"""
	regex = re.compile(pattern)
	return regex.sub(replacement, str(string))
