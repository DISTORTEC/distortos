#!/bin/sh

#
# file: travis-ci.sh
#
# author: Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

# "install" phase
install() {
	mkdir -p "${HOME}/cache"
	mkdir -p "${HOME}/toolchains"
	cd "${HOME}/toolchains"

	echo 'Downloading gcc-arm-none-eabi-4_9-150928-linux-x64.tar.xz...'
	wget http://www.freddiechopin.info/en/download/category/11-bleeding-edge-toolchain?download=122%3Ableeding-edge-toolchain-150928-64-bit-linux -O gcc-arm-none-eabi-4_9-150928-linux-x64.tar.xz
	echo 'Extracting gcc-arm-none-eabi-4_9-150928-linux-x64.tar.xz...'
	tar -xf gcc-arm-none-eabi-4_9-150928-linux-x64.tar.xz
	mkdir gcc-arm-none-eabi-4_9-150928/bin/lib
	gcc -shared -o gcc-arm-none-eabi-4_9-150928/bin/lib/libfl.so.2 -lfl
	cat > arm-none-eabi-gcc-4.9.3.sh <<- EOF
	export LD_LIBRARY_PATH="$(pwd)/gcc-arm-none-eabi-4_9-150928/bin/lib"
	export PATH="$(pwd)/gcc-arm-none-eabi-4_9-150928/bin:\${PATH-}"
	EOF

	echo 'Downloading gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz...'
	wget http://www.freddiechopin.info/en/download/category/11-bleeding-edge-toolchain?download=143%3Ableeding-edge-toolchain-160412-64-bit-linux -O gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz
	echo 'Extracting gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz...'
	tar -xf gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz
	cat > arm-none-eabi-gcc-5.3.1.sh <<- EOF
	export PATH="$(pwd)/gcc-arm-none-eabi-5_3-160412/bin:\${PATH-}"
	EOF
}

# "script" phase
script() {
	toolchains="$(find "${HOME}/toolchains/" -mindepth 1 -maxdepth 1 -name '*.sh' | sort)"
	for toolchain in ${toolchains}; do
		(
		echo "Using ${toolchain}..."
		. "${toolchain}"
		"$(dirname "${0}")/buildAllConfigurations.sh" "${@}"
		)
	done
}

case "${1}" in
	install)
		install
		;;
	script)
		shift
		script "${@}"
		;;
	*)
		echo "\"${1}\" is not a valid argument!" >&2
		exit 2
esac
