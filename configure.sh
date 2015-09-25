#!/bin/sh

#
# file: configure.sh
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-09-25
#

set -e
set -u
set -o pipefail

path=.

# If any argument was given, then use it as the path to distortosConfiguration.mk file, otherwise search in current
# directory.
if [ $# -ge 1 ]; then
	path=$1
fi

if [ ! -f $path/distortosConfiguration.mk ]; then
	echo "Trying $path/distortosConfiguration.mk... Not found!"
	path=configurations/$path
	if [ ! -f $path/distortosConfiguration.mk ]; then
		echo "Trying $path/distortosConfiguration.mk... Not found!"
		echo "Error - no suitable configuration found!"
		exit -1
	fi	
fi

echo "Trying $path/distortosConfiguration.mk... Found."
echo "CONFIG_SELECTED_CONFIGURATION=\"$path\"" > selectedConfiguration.mk
echo "\"selectedConfiguration.mk\" file generated successfully."
