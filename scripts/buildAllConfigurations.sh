#!/bin/sh

#
# file: buildAllConfigurations.sh
#
# author: Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

basedir="$(dirname "${0}")"

searchPath='.'

# If any additional argument was given, then use it as the search path, otherwise search in current directory.
if [ ${#} -ge 1 ]; then
	searchPath="${1}"
fi

arguments=${@}
shift

rm -rf output

for configuration in $(find -L "${searchPath}" -name 'distortosConfiguration.cmake' -printf '%p ')
do

	mkdir output
	cd output
	cmake -C ../${configuration} .. -G Ninja
	ninja -v ${@}
	cd -
	rm -rf output

done
