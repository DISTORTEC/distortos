#
# file: EstdContiguousRangePrettyPrinter.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

class EstdContiguousRangePrettyPrinter:
	'Print estd::ContiguousRange'

	class Iterator:
		'Iterate over estd::ContiguousRange'

		def __init__(self, begin, end):
			self.end = end
			self.index = 0
			self.iterator = begin

		def __iter__(self):
			return self

		def __next__(self):
			if self.iterator == self.end:
				raise StopIteration
			index = self.index
			element = self.iterator.dereference()
			self.index += 1
			self.iterator += 1
			return ('[%d]' % index, element)

	def __init__(self, value):
		self.value = value

	def children(self):
		if self.getLength() == 0:
			return iter([])
		return self.Iterator(self.value['begin_'], self.value['end_'])

	def display_hint(self):
		return 'array'

	def getLength(self):
		begin = self.value['begin_']
		end = self.value['end_']
		# value of "begin" must be less than or equal to "end"
		if begin > end :
			return 0
		sizeType = gdb.lookup_type('size_t')
		rawLength = end.cast(sizeType) - begin.cast(sizeType)
		# raw difference between values of "begin" and "end" must be a multiple of element size
		if rawLength % begin.type.target().sizeof != 0:
			return 0
		return end - begin

	def to_string(self):
		return ('estd::ContiguousRange of length %d' % self.getLength())
