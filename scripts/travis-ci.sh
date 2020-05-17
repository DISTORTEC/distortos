#!/bin/bash

#
# file: travis-ci.sh
#
# author: Copyright (C) 2016-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

# toolchain installation phase
installToolchain() {
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
		6)
			installToolchain '6.3.0' '170821'
			;;
		7)
			installToolchain '7.3.0' '180127'
			;;
		8)
			installToolchain '8.3.0' '190223'
			;;
		9)
			installToolchain '9.3.0' '200323'
			;;
		10)
			installToolchain '10.1.0' '200517'
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
	"$(dirname "${0}")/buildAllConfigurations.sh" ${@}
}

# "script generateBoard" phase
scriptGenerateBoard() {
	distortosPath="$(cd $(dirname ${0})/.. && pwd)"

	for yamlFile in $(/usr/bin/find -L "${distortosPath}/source/board" -name '*.yaml')
	do
		${distortosPath}/scripts/generateBoard.py ${yamlFile}
	done

	git add -N .
	git diff --exit-code

	for yamlFile in $(/usr/bin/find -L "${distortosPath}/source/chip" -name '*.yaml')
	do
		${distortosPath}/scripts/generateBoard.py ${yamlFile} -o /tmp/$(basename ${yamlFile} .yaml)
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
