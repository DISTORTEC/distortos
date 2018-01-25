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

#
# Sanitizes provided string
#
# param [in] string is the string that will be sanitized
# param [in] pattern is the pattern which will be replaced, default - '[^0-9A-Za-z-]'
# param [in] replacement is the replacement string, default - '_'
#
# return sanitized string
#

def sanitize(string, pattern = '[^0-9A-Za-z-]', replacement = '_'):
	regex = re.compile(pattern)
	return regex.sub(replacement, string)
