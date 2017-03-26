#
# file: GenericIntrusiveListIterator.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from gdb import (lookup_type, parse_and_eval)

class GenericIntrusiveListIterator:
	'Generic iterator for intrusive list'

	def __init__(self, begin, end, nodeValidator, nodePointer, u):
		self.end = end
		self.nodeValidator = nodeValidator
		self.nodePointer = nodePointer
		self.u = u
		self.index = 0
		self.iterator = begin

	def __iter__(self):
		return self

	def __next__(self):
		if self.iterator == self.end:
			raise StopIteration
		if self.nodeValidator(self.iterator.dereference()) == False:
			raise StopIteration
		index = self.index
		sizeType = lookup_type('size_t')
		offset = parse_and_eval('static_cast<%s>(0)->*%s' % (self.u.pointer(), self.nodePointer)).address.cast(sizeType)
		pointer = (self.iterator.cast(sizeType) - offset).cast(self.u.pointer())
		element = pointer.dereference()
		self.index += 1
		self.iterator = self.iterator['nextNode_']
		return ('[%d]' % index, element)
