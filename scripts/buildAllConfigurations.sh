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

basedir=`dirname $0`

$basedir/forAllConfigurations.sh "make -j`nproc` VERBOSE=1" "$@"

if command -v tup >/dev/null 2>&1; then
	$basedir/forAllConfigurations.sh "tup --verbose" "$@"
fi
