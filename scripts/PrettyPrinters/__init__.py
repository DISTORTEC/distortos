#
# file: __init__.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

########################################################################################################################
# registerPrettyPrinters()
########################################################################################################################

def registerPrettyPrinters(obj):
	"""Register pretty-printers."""
	import PrettyPrinters.estd
	PrettyPrinters.estd.registerPrettyPrinters(obj)
	import PrettyPrinters.distortos
	PrettyPrinters.distortos.registerPrettyPrinters(obj)
