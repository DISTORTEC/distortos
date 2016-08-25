#!/bin/sh

#
# file: makeDistortosConfiguration.sh
#
# author: Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

if [ $# -ne 1 ]; then
	echo 'This script requires 1 argument!' >&2
	exit 1
fi

echo '/**'
echo ' * \file'
echo ' * \brief distortos configuration'
echo ' *'
echo ' * \warning'
echo ' * Automatically generated file - do not edit!'
echo ' *'
echo " * \\date $(date +'%Y-%m-%d %H:%M:%S')"
echo ' */'
echo ''
echo '#ifndef INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_'
echo '#define INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_'
echo ''

sed -n \
	-e 's/^\(CONFIG_[A-Za-z0-9_]\{1,\}\)=y$/#define \1/p' \
	-e 's/^\(CONFIG_[A-Za-z0-9_]\{1,\}\)=\(.*\)$/#define \1 \2/p' \
	-e 's/^# \(CONFIG_[A-Za-z0-9_]\{1,\}\) is not set$/#undef \1/p' \
	$1

echo ''
echo '#endif	/* INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_ */'
