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

if [ ${#} -lt 2 ]; then
	echo 'This script requires at least 2 arguments!' >&2
	exit 1
fi

# "install build" phase
installBuild() {
	mkdir -p "${HOME}/cache"
	mkdir -p "${HOME}/toolchains"
	cd "${HOME}/toolchains"

	echo 'Downloading gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz...'
	wget http://www.freddiechopin.info/en/download/category/11-bleeding-edge-toolchain?download=143%3Ableeding-edge-toolchain-160412-64-bit-linux -O gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz
	echo 'Extracting gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz...'
	tar -xf gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz
	cat > arm-none-eabi-gcc-5.3.1.sh <<- EOF
	export PATH="$(pwd)/gcc-arm-none-eabi-5_3-160412/bin:\${PATH-}"
	EOF

	if [ ! -e "${HOME}"/cache/arm-none-eabi-gcc-6.3.0-*.tar.xz ]; then
		(
		echo 'Downloading bleeding-edge-toolchain-170314.tar.xz...'
		wget http://www.freddiechopin.info/en/download/category/11-bleeding-edge-toolchain?download=155%3Ableeding-edge-toolchain-170314-linux-script -O bleeding-edge-toolchain-170314.tar.xz
		echo 'Extracting bleeding-edge-toolchain-170314.tar.xz...'
		tar -xf bleeding-edge-toolchain-170314.tar.xz
		echo 'Building bleeding-edge-toolchain-170314...'
		cd bleeding-edge-toolchain-170314

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

# "install pydts" phase
installPydts() {
	(
	cd /tmp
	echo 'Installing dtc-1.4.4.tar.gz...'
	wget https://github.com/dgibson/dtc/archive/v1.4.4.tar.gz -O dtc-1.4.4.tar.gz
	echo 'Extracting dtc-1.4.4.tar.gz...'
	tar -xf dtc-1.4.4.tar.gz
	echo 'Building dtc-1.4.4...'
	cd dtc-1.4.4
	make -j$(nproc)
	make  PREFIX="${HOME}/.local" install
	)
	echo 'Installing ply...'
	pip install --user ply
}

# "install" phase
install() {
	case "${1}" in
		build)
			shift
			installBuild "${@}"
			;;
		pydts)
			shift
			installPydts "${@}"
			;;
		*)
			echo "\"${1}\" is not a valid argument!" >&2
			exit 2
	esac
}

# "script build" phase
scriptBuild() {
	toolchains="$(find "${HOME}/toolchains/" -mindepth 1 -maxdepth 1 -name '*.sh' | sort)"
	for toolchain in ${toolchains}; do
		(
		echo "Using ${toolchain}..."
		. "${toolchain}"
		"$(dirname "${0}")/buildAllConfigurations.sh" "${@}"
		)
	done
}

# "script pydts" phase
scriptPydts() {
	"$(dirname "${0}")/test-pydts.sh" "${@}"
}

# "script" phase
script() {
	case "${1}" in
		build)
			shift
			scriptBuild "${@}"
			;;
		pydts)
			shift
			scriptPydts "${@}"
			;;
		*)
			echo "\"${1}\" is not a valid argument!" >&2
			exit 3
	esac
}

case "${1}" in
	install)
		shift
		install "${@}"
		;;
	script)
		shift
		script "${@}"
		;;
	*)
		echo "\"${1}\" is not a valid argument!" >&2
		exit 4
esac
