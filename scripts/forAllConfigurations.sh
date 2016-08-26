#!/bin/sh

#
# file: forAllConfigurations.sh
#
# author: Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

if [ $# -lt 1 ]; then
	echo 'This script requires at least 1 argument!' >&2
	exit 1
fi

command=$1

searchPath=.

# If any additional argument was given, then use it as the search path, otherwise search in current directory.
if [ $# -ge 2 ]; then
	searchPath=$2
fi

make distclean

for configurationPath in $(find $searchPath -name 'distortosConfiguration.mk' -printf '%h ')
do

	make configure CONFIG_PATH=$configurationPath
	$command
	make distclean

done
