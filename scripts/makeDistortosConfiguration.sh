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

cat << EOF
/**
 * \file
 * \brief distortos configuration
 *
 * \warning
 * Automatically generated file - do not edit!
 *
 * \date $(date +'%Y-%m-%d %H:%M:%S')
 */

#ifndef INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_
#define INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_

EOF

sed -n \
	-e 's/^\(CONFIG_[A-Za-z0-9_]\{1,\}\)=y$/#define \1/p' \
	-e 's/^\(CONFIG_[A-Za-z0-9_]\{1,\}\)=\(.*\)$/#define \1 \2/p' \
	-e 's/^# \(CONFIG_[A-Za-z0-9_]\{1,\}\) is not set$/#undef \1/p' \
	$1

cat << EOF

#endif	/* INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_ */
EOF