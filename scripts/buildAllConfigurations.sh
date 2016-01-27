#!/bin/sh

#
# file: distclean.sh
#
# author: Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

make distclean

for configurationPath in `find -name "distortosConfiguration.mk" -printf "%h "`
do

	make configure CONFIG_PATH=$configurationPath
	make -j`nproc` VERBOSE=1
	make distclean

	if command -v tup >/dev/null 2>&1; then
		make configure CONFIG_PATH=$configurationPath
		tup --verbose
		make distclean
	fi

done
