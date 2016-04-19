#!/bin/sh

#
# file: generateKconfig.sh
#
# author: Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

output=output

# If any argument was given, then use it as the output path, otherwise assume the output path to be "./output"
if [ $# -ge 1 ]; then
	output=`realpath --relative-base=. $1`
fi

mkdir -p $output
pattern="### don't remove this line! "
for file in \
		`/usr/bin/find -path "./$output" -prune -o -name 'Kconfig*' -exec grep "$pattern" {} + | cut -d ' ' -f 6 | uniq`
do

cat << EOF > $output/$file
#
# file: $file
#
# Automatically generated file - do not edit!
#
# date: `date +'%Y-%m-%d %H:%M:%S'`
#

`/usr/bin/find -path "./$output" -prune -o -name $file -printf 'source "%p"\n' | sort`
EOF

done
