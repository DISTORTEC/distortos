#
# file: printers.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from gdb.printing import (RegexpCollectionPrettyPrinter, register_pretty_printer)
from PrettyPrinters.estd.EstdContiguousRangePrettyPrinter import EstdContiguousRangePrettyPrinter

def registerEstdPrettyPrinters(obj):
	'Register pretty-printers for estd'

	prettyPrinters = RegexpCollectionPrettyPrinter('estd')
	prettyPrinters.add_printer('estd::ContiguousRange', '^estd::ContiguousRange<.*>$', EstdContiguousRangePrettyPrinter)
	register_pretty_printer(obj, prettyPrinters)
