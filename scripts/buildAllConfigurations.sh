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

"${basedir}/forAllConfigurations.sh" \
		"mkdir output && cd output &&
		cmake .. -DCMAKE_TOOLCHAIN_FILE=${basedir}/../cmake/Toolchain-arm-none-eabi.cmake -G \"Unix Makefiles\" &&
		make -j$(nproc) VERBOSE=1 && cd -" \
		"${@}"

"${basedir}/forAllConfigurations.sh" "make -j$(nproc) VERBOSE=1" "${@}"
