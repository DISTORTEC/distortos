#!/bin/sh

#
# file: buildAllConfigurations.sh
#
# author: Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

searchPath=.

# If any argument was given, then use it as the search path, otherwise search in current directory.
if [ $# -ge 1 ]; then
	searchPath=$1
fi

make distclean

for configurationPath in `find $searchPath -name "distortosConfiguration.mk" -printf "%h "`
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
