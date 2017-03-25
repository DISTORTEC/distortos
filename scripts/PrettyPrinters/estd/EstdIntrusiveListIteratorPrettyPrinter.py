#
# file: EstdIntrusiveListIteratorPrettyPrinter.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from PrettyPrinters.estd.EstdIntrusiveListPrettyPrinter import EstdIntrusiveListPrettyPrinter

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
