#!/bin/sh

#
# file: travis-ci.sh
#
# author: Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

if [ ${#} -ne 1 ]; then
	echo 'This script requires 1 argument!' >&2
	exit 1
fi

# "install" phase
install() {
	echo "Downloading gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz..."
	wget http://www.freddiechopin.info/en/download/category/11-bleeding-edge-toolchain?download=143%3Ableeding-edge-toolchain-160412-64-bit-linux -O /tmp/gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz
	echo "Extracting gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz..."
	tar -xf /tmp/gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz -C /tmp/

	echo "Downloading gcc-arm-none-eabi-4_9-150928-linux-x64.tar.xz..."
	wget http://www.freddiechopin.info/en/download/category/11-bleeding-edge-toolchain?download=122%3Ableeding-edge-toolchain-150928-64-bit-linux -O /tmp/gcc-arm-none-eabi-4_9-150928-linux-x64.tar.xz
	echo "Extracting gcc-arm-none-eabi-4_9-150928-linux-x64.tar.xz..."
	tar -xf /tmp/gcc-arm-none-eabi-4_9-150928-linux-x64.tar.xz -C /tmp/
	mkdir /tmp/lib
	gcc -shared -o /tmp/lib/libfl.so.2 -lfl
}

# "script" phase
script() {
	(
	echo "Testing with gcc-arm-none-eabi-5_3-160412"
	export PATH="/tmp/gcc-arm-none-eabi-5_3-160412/bin:${PATH-}"
	./scripts/buildAllConfigurations.sh
	)

	(
	echo "Testing with gcc-arm-none-eabi-4_9-150928"
	export PATH="/tmp/gcc-arm-none-eabi-4_9-150928/bin:${PATH-}"
	export LD_LIBRARY_PATH="/tmp/lib"
	./scripts/buildAllConfigurations.sh
	)
}

case "${1}" in
	install)
		install
		;;
	script)
		script
		;;
	*)
		echo "\"${1}\" is not a valid argument!" >&2
		exit 2
esac
