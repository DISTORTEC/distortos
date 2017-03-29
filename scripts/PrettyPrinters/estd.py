#
# file: estd.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import gdb
import gdb.printing

########################################################################################################################
# GenericIntrusiveListIterator class
########################################################################################################################

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
		sizeType = gdb.lookup_type('size_t')
		offset = gdb.parse_and_eval('static_cast<%s>(0)->*%s' % (self.u.pointer(), self.nodePointer)).address.cast(sizeType)
		pointer = (self.iterator.cast(sizeType) - offset).cast(self.u.pointer())
		element = pointer.dereference()
		self.index += 1
		self.iterator = self.iterator['nextNode_']
		return ('[%d]' % index, element)

########################################################################################################################
# EstdContiguousRangePrettyPrinter class
########################################################################################################################

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

########################################################################################################################
# EstdIntrusiveListPrettyPrinter class
########################################################################################################################

class EstdIntrusiveListPrettyPrinter:
	'Print estd::IntrusiveList'

	class Iterator(GenericIntrusiveListIterator):
		'Iterate over estd::IntrusiveList'

		def __init__(self, begin, end, nodePointer, u):
			super().__init__(begin, end, EstdIntrusiveListPrettyPrinter.isNodeValid, nodePointer, u)

	def __init__(self, value, name = 'estd::IntrusiveList'):
		self.value = value
		self.name = name

	def children(self):
		rootNode = self.value['intrusiveListBase_']['rootNode_']
		if EstdIntrusiveListPrettyPrinter.isNodeValid(rootNode) == False:
			return iter([])
		return self.Iterator(rootNode['nextNode_'], rootNode.address, self.value.type.template_argument(1),
				self.value.type.template_argument(2).strip_typedefs())

	def display_hint(self):
		# https://bugs.eclipse.org/bugs/show_bug.cgi?id=512795
		return 'array'

	@staticmethod
	def isNodeValid(node):
		nextNode = node['nextNode_']
		sizeType = gdb.lookup_type('size_t')
		# value of "next" pointer must be  properly aligned
		if nextNode.cast(sizeType) % nextNode.type.sizeof != 0:
			return False
		previousNode_ = node['previousNode_']
		# value of "previous" pointer must be  properly aligned
		if previousNode_.cast(sizeType) % previousNode_.type.sizeof != 0:
			return False
		# "previous" link of next node must point to this node
		if nextNode['previousNode_'] != node.address:
			return False
		# "next" link of previous node must point to this node
		if previousNode_['nextNode_'] != node.address:
			return False
		return True

	def to_string(self):
		return self.name

########################################################################################################################
# EstdIntrusiveListIteratorPrettyPrinter class
########################################################################################################################

class EstdIntrusiveListIteratorPrettyPrinter(EstdIntrusiveListPrettyPrinter):
	'Print estd::IntrusiveListIterator'

	def __init__(self, value):
		super().__init__(value, 'estd::IntrusiveListIterator')

	def children(self):
		node = self.value['node_'];
		if node == 0:
			return iter([])
		return self.Iterator(node, node['nextNode_'], self.value.type.template_argument(1),
				self.value.type.template_argument(2).strip_typedefs())

########################################################################################################################
# EstdSortedIntrusiveListPrettyPrinter class
########################################################################################################################

class EstdSortedIntrusiveListPrettyPrinter(EstdIntrusiveListPrettyPrinter):
	'Print estd::SortedIntrusiveList'

	def __init__(self, value):
		super().__init__(value['implementation_']['intrusiveList'], 'estd::SortedIntrusiveList')

########################################################################################################################
# EstdIntrusiveForwardListPrettyPrinter class
########################################################################################################################

class EstdIntrusiveForwardListPrettyPrinter:
	'Print estd::IntrusiveForwardList'

	class Iterator(GenericIntrusiveListIterator):
		'Iterate over estd::IntrusiveForwardList'

		def __init__(self, begin, end, nodePointer, u):
			super().__init__(begin, end, EstdIntrusiveForwardListPrettyPrinter.isNodeValid, nodePointer, u)

	def __init__(self, value, name = 'estd::IntrusiveForwardList'):
		self.value = value
		self.name = name

	def children(self):
		rootNode = self.value['intrusiveForwardListBase_']['rootNode_']
		if EstdIntrusiveForwardListPrettyPrinter.isNodeValid(rootNode) == False:
			return iter([])
		return self.Iterator(rootNode['nextNode_'], 0, self.value.type.template_argument(1),
				self.value.type.template_argument(2).strip_typedefs())

	def display_hint(self):
		# https://bugs.eclipse.org/bugs/show_bug.cgi?id=512795
		return 'array'

	@staticmethod
	def isNodeValid(node):
		nextNode = node['nextNode_']
		sizeType = gdb.lookup_type('size_t')
		# value of "next" pointer must be  properly aligned
		if nextNode.cast(sizeType) % nextNode.type.sizeof != 0:
			return False
		return True

	def to_string(self):
		return self.name

########################################################################################################################
# EstdIntrusiveForwardListIteratorPrettyPrinter class
########################################################################################################################

class EstdIntrusiveForwardListIteratorPrettyPrinter(EstdIntrusiveForwardListPrettyPrinter):
	'Print estd::IntrusiveForwardListIterator'

	def __init__(self, value):
		super().__init__(value, 'estd::IntrusiveForwardListIterator')

	def children(self):
		node = self.value['node_'];
		if node == 0:
			return iter([])
		return self.Iterator(node, node['nextNode_'], self.value.type.template_argument(1),
				self.value.type.template_argument(2).strip_typedefs())

########################################################################################################################
# EstdSortedIntrusiveForwardListPrettyPrinter class
########################################################################################################################

class EstdSortedIntrusiveForwardListPrettyPrinter(EstdIntrusiveForwardListPrettyPrinter):
	'Print estd::SortedIntrusiveForwardList'

	def __init__(self, value):
		super().__init__(value['implementation_']['intrusiveForwardList'], 'estd::SortedIntrusiveForwardList')

########################################################################################################################
# registerEstdPrettyPrinters()
########################################################################################################################

def registerEstdPrettyPrinters(obj):
	'Register pretty-printers for estd'

	prettyPrinters = gdb.printing.RegexpCollectionPrettyPrinter('estd')
	prettyPrinters.add_printer('estd::ContiguousRange', '^estd::ContiguousRange<.*>$', EstdContiguousRangePrettyPrinter)
	prettyPrinters.add_printer('estd::IntrusiveForwardListIterator', '^estd::IntrusiveForwardListIterator<.*>$',
			EstdIntrusiveForwardListIteratorPrettyPrinter)
	prettyPrinters.add_printer('estd::IntrusiveForwardList', '^estd::IntrusiveForwardList<.*>$',
			EstdIntrusiveForwardListPrettyPrinter)
	prettyPrinters.add_printer('estd::IntrusiveListIterator', '^estd::IntrusiveListIterator<.*>$',
			EstdIntrusiveListIteratorPrettyPrinter)
	prettyPrinters.add_printer('estd::IntrusiveList', '^estd::IntrusiveList<.*>$', EstdIntrusiveListPrettyPrinter)
	prettyPrinters.add_printer('estd::SortedIntrusiveForwardList', '^estd::SortedIntrusiveForwardList<.*>$',
			EstdSortedIntrusiveForwardListPrettyPrinter)
	prettyPrinters.add_printer('estd::SortedIntrusiveList', '^estd::SortedIntrusiveList<.*>$',
			EstdSortedIntrusiveListPrettyPrinter)
	gdb.printing.register_pretty_printer(obj, prettyPrinters)
