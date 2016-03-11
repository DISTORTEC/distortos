#!/bin/sh

#
# file: regenerateKconfig.sh
#
# author: Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

distortosPath=.

# If any argument was given, then use it as the path to distortos, otherwise assume the path to be "."
if [ $# -ge 1 ]; then
	distortosPath=$1
fi

for file in `find $distortosPath -name "Kconfig*"`
do
	grep '### begin ' $file | while read -r _ _ pattern
	do
		echo "Regenerating pattern \"$pattern\" in \"$file\""
		temp=`mktemp`
		find $distortosPath -name $pattern -printf 'source "$DISTORTOS_PATH/%P"\n' | sort > $temp
		sed -i -e '/^### begin '$pattern'$/,/^### end '$pattern'$/{//!d}' -e '/^### begin '$pattern'$/r '$temp $file
		rm $temp
	done
done
