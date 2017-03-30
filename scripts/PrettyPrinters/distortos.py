#
# file: distortos.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import gdb
import gdb.printing
import PrettyPrinters.estd

########################################################################################################################
# InternalThreadList class
########################################################################################################################

class InternalThreadList(PrettyPrinters.estd.SortedIntrusiveList):
	'Print distortos::internal::ThreadList'

	def __init__(self, value):
		super().__init__(value, 'distortos::internal::ThreadList')

########################################################################################################################
# registerPrettyPrinters()
########################################################################################################################

def registerPrettyPrinters(obj):
	'Register pretty-printers for distortos'

	prettyPrinters = gdb.printing.RegexpCollectionPrettyPrinter('distortos::internal')
	prettyPrinters.add_printer('distortos::internal::ThreadList', '^distortos::internal::ThreadList$',
			InternalThreadList)
	gdb.printing.register_pretty_printer(obj, prettyPrinters)
