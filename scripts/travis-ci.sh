#!/bin/bash

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

	if [ ! -e "${HOME}"/cache/arm-none-eabi-gcc-6.3.0-*.tar.xz ]; then
		(
		echo 'Downloading bleeding-edge-toolchain-master.tar.gz...'
		wget https://github.com/FreddieChopin/bleeding-edge-toolchain/archive/master.tar.gz -O bleeding-edge-toolchain-master.tar.gz
		echo 'Extracting bleeding-edge-toolchain-master.tar.gz...'
		tar -xf bleeding-edge-toolchain-master.tar.gz
		echo 'Building bleeding-edge-toolchain-master...'
		cd bleeding-edge-toolchain-master

		{ time='0'; while true; do sleep 60; time="$((${time} + 1))"; echo "${time} minute(s)..."; done } &
		keepAlivePid="${!}"
		timeout -k 1m 45m ./build-bleeding-edge-toolchain.sh --skip-nano-libraries > >(tee /tmp/stdout.log) 2> /tmp/stderr.log | grep '[*-]\{10,10\} '
		kill "${keepAlivePid}"
		wait "${keepAlivePid}" || true
		cp arm-none-eabi-gcc-6.3.0-*.tar.xz "${HOME}/cache"
		)
	fi
	echo 'Extracting arm-none-eabi-gcc-6.3.0-*.tar.xz...'
	tar -xf ${HOME}/cache/arm-none-eabi-gcc-6.3.0-*.tar.xz
	cat > arm-none-eabi-gcc-6.3.0.sh <<- EOF
	export PATH="$(cd arm-none-eabi-gcc-6.3.0-*/bin && pwd):\${PATH-}"
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
