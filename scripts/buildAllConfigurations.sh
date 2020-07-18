#!/bin/sh

#
# file: buildAllConfigurations.sh
#
# author: Copyright (C) 2016-2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

if [ ${#} -lt 1 ]; then
	echo 'This script requires at least 1 argument!' >&2
	exit 1
fi

searchPath="${1}"
shift

basedir="$(dirname "${0}")"
"${basedir}/forAllConfigurations.sh" "${searchPath}" ninja -v ${@}
