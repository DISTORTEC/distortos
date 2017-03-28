#
# file: EstdIntrusiveListPrettyPrinter.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from gdb import lookup_type
from PrettyPrinters.estd.GenericIntrusiveListIterator import GenericIntrusiveListIterator

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
		sizeType = lookup_type('size_t')
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
