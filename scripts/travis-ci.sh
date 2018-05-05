#!/bin/bash

#
# file: travis-ci.sh
#
# author: Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

if [ "${2}" = 'build' ] && [ ${#} -lt 3 ]; then
	echo '"build" subphase requires at least 3 arguments!' >&2
	exit 2
fi

# "install build 5" phase
installBuild5() {
	echo 'Downloading gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz...'
	wget http://www.freddiechopin.info/en/download/category/11-bleeding-edge-toolchain?download=143%3Ableeding-edge-toolchain-160412-64-bit-linux -O gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz
	echo 'Extracting gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz...'
	tar -xf gcc-arm-none-eabi-5_3-160412-linux-x64.tar.xz
	cat > arm-none-eabi-gcc-5.3.1.sh <<- EOF
	export LD_LIBRARY_PATH="$(pwd)/gcc-arm-none-eabi-5_3-160412/bin/lib"
	export PATH="$(pwd)/gcc-arm-none-eabi-5_3-160412/bin:\${PATH-}"
	EOF
}

# "install build 6", "install build 7" and "install build 8" phase
installBuild678() {
	local gccVersion="${1}"
	local buildDate="${2}"
	echo "Downloading arm-none-eabi-gcc-${gccVersion}-${buildDate}.tar.xz..."
	wget http://distortos.org/files/travis-ci/arm-none-eabi-gcc-${gccVersion}-${buildDate}.tar.xz
	echo "Extracting arm-none-eabi-gcc-${gccVersion}-${buildDate}.tar.xz..."
	tar -xf arm-none-eabi-gcc-${gccVersion}-${buildDate}.tar.xz
	cat > "arm-none-eabi-gcc-${gccVersion}.sh" <<- EOF
	export PATH="$(cd arm-none-eabi-gcc-${gccVersion}-${buildDate}/bin && pwd):\${PATH-}"
	EOF
}

# "install build" phase
installBuild() {
	mkdir -p "${HOME}/toolchain"
	cd "${HOME}/toolchain"

	case "${1}" in
		5)
			installBuild5
			;;
		6)
			installBuild678 '6.3.0' '170821'
			;;
		7)
			installBuild678 '7.3.0' '180127'
			;;
		8)
			installBuild678 '8.1.0' '180504'
			;;
		*)
			echo "\"${1}\" is not a valid argument!" >&2
			exit 3
	esac
}

# "install generateBoard" phase
installGenerateBoard() {
	echo 'Installing jinja2...'
	pip install jinja2
	echo 'Installing ruamel.yaml...'
	pip install ruamel.yaml
}

# "install" phase
install() {
	case "${1}" in
		build)
			shift
			installBuild "${@}"
			;;
		generateBoard)
			shift
			installGenerateBoard "${@}"
			;;
		unit-test)
			;;
		*)
			echo "\"${1}\" is not a valid argument!" >&2
			exit 4
	esac
}

# "script build" phase
scriptBuild() {
	shift

	toolchain="$(find "${HOME}/toolchain/" -mindepth 1 -maxdepth 1 -name '*.sh')"
	echo "Using ${toolchain}..."
	. "${toolchain}"
	"$(dirname "${0}")/buildAllConfigurations.sh" "${@}"
}

# "script generateBoard" phase
scriptGenerateBoard() {
	distortosPath="$(cd $(dirname ${0})/.. && pwd)"

	for yamlFile in $(/usr/bin/find -L "${distortosPath}/source/board" -name '*.yaml')
	do
		(
			cd "${distortosPath}"
			make board CONFIG_FILE="${yamlFile}"
		)
	done

	git add -N .
	git diff --exit-code

	for yamlFile in $(/usr/bin/find -L "${distortosPath}/source/chip" -name '*.yaml')
	do
		(
			cd "${distortosPath}"
			make board CONFIG_FILE="${yamlFile}" OUTPUT_PATH="/tmp/$(basename ${yamlFile} .yaml)"
		)
	done
}

unitTest() {
	mkdir output
	cd output
	cmake -G Ninja ../unit-test
	ninja all run
}

# "script" phase
script() {
	case "${1}" in
		build)
			shift
			scriptBuild "${@}"
			;;
		generateBoard)
			shift
			scriptGenerateBoard "${@}"
			;;
		unit-test)
			shift
			unitTest "${@}"
			;;
		*)
			echo "\"${1}\" is not a valid argument!" >&2
			exit 5
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
		exit 6
esac
