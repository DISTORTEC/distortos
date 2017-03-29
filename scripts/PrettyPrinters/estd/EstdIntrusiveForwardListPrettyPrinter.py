#
# file: EstdIntrusiveForwardListPrettyPrinter.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import gdb
from PrettyPrinters.estd.GenericIntrusiveListIterator import GenericIntrusiveListIterator

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
