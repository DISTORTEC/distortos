#!/bin/bash

#
# file: test-pydts.sh
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

exec 3>&2

if [ ${#} -gt 0 ]; then
	case "${1}" in
		--quiet)
			exec 2>/dev/null
			;;
		*)
			echo "\"${1}\" is not a valid argument!" >&2
			exit 1
	esac
fi

distortosPath="$(cd $(dirname ${0})/.. && pwd)"
pydts="${distortosPath}/scripts/pydts.py"

testPydts() {
	local dtsPath="${1}"
	local cppFlags="${2}"

	local tested=0
	local skipped=0
	local failedPydts0=0
	local failedPydts1=0
	local failedPydts2=0
	local failedPydts3=0
	local failedPydts4=0
	local failedDtc1=0
	local failedDtc2=0
	local failedDtb=0

	local DTCFLAGS='-q -I dts -s -b 0 -H epapr'

	local dtsFiles="$(find ${dtsPath} -name '*.dts' | sort)"
	for dtsFile in ${dtsFiles}; do

		tested=$((++tested))
		echo "Testing with ${dtsFile}... "
		local name="$(basename ${dtsFile})"

		# handle includes, resolve macros, remove comments
		local CPPFLAGS="-nostdinc -undef -E -x assembler-with-cpp ${cppFlags} -I $(dirname ${dtsFile})"
		cpp ${CPPFLAGS} "${dtsFile}" -o "${name}" || { skipped=$((++skipped)); echo 'Skipped'; continue; }

		# try to merge the file with dtc - if it fails, it most likely has "/include/" directive (which is currently
		# unsupported), so skip it
		dtc ${DTCFLAGS} -O dts "${name}" -o "${name}.dts-dtc" || { skipped=$((++skipped)); echo 'Skipped'; continue; }

		# Extract phandle values from file merged by dtc and append it to the file used in the test. This way file
		# merged with dtc and file merged with pydts can be both compiled with dtc into a *.dtb and compared. Without
		# this step the phandle values would be different and the comparison would fail.
		{ cat "${name}" && grep -E '{|}|phandle = <0x[0-9A-Fa-f]+>;' "${name}.dts-dtc"; } > "${name}.phandle"
		mv "${name}.phandle" "${name}"

		# try to merge the file with pydts
		"${pydts}" -I dts -O dts "${name}" -o "${name}.dts-pydts0" ||
				{ failedPydts0=$((++failedPydts0)); echo 'Failed - pydts merge'; continue; }

		# try to merge the file which was just merged - both files should be identical
		{ "${pydts}" -I dts -O dts "${name}.dts-pydts0" -o "${name}.dts-pydts1" &&
				cmp -s "${name}.dts-pydts0" "${name}.dts-pydts1"; } ||
				{ failedPydts1=$((++failedPydts1)); echo 'Failed - pydts dts->dts'; continue; }

		# try to convert *.dts to dictionary
		"${pydts}" -I dts -O dictionary "${name}.dts-pydts0" -o "${name}.dictionary-pydts2" ||
				{ failedPydts2=$((++failedPydts2)); echo 'Failed - pydts dts->dictionary'; continue; }

		# try to convert dictionary to dictionary - both files should be identical
		{ "${pydts}" -I dictionary -O dictionary "${name}.dictionary-pydts2" -o "${name}.dictionary-pydts3" &&
				cmp -s "${name}.dictionary-pydts2" "${name}.dictionary-pydts3"; } ||
				{ failedPydts3=$((++failedPydts3)); echo 'Failed - pydts dictionary->dictionary'; continue; }

		# try to convert dictionary back to *.dts - the result should be identical to the source *.dts file used to
		# generate the dictionary
		{ "${pydts}" -I dictionary -O dts "${name}.dictionary-pydts2" -o "${name}.dts-pydts4" &&
				cmp -s "${name}.dts-pydts0" "${name}.dts-pydts4"; } ||
				{ failedPydts4=$((++failedPydts4)); echo 'Failed - pydts dictionary->dts'; continue; }

		# compile *.dts file merged with pydts to *.dtb
		dtc ${DTCFLAGS} -O dtb "${name}.dts-pydts4" -o "${name}.dtb0-dtc" ||
				{ failedDtc1=$((++failedDtc1)); echo 'Failed - dtc dts(pydts)->dtb'; continue; }

		# compile original *.dts file (with explicit phandle values) to *.dtb
		dtc ${DTCFLAGS} -O dtb "${name}" -o "${name}.dtb1-dtc" ||
				{ failedDtc2=$((++failedDtc2)); echo 'Failed - dtc dts(dtc)->dtb'; continue; }

		# both *.dtb files should be identical
		cmp -s "${name}.dtb0-dtc" "${name}.dtb1-dtc" ||
				{ failedDtb=$((++failedDtb)); echo 'Failed - cmp dtb(pydts) & dtb(dtc)'; continue; }

	done

	local failed=$((failedPydts0 + failedPydts1 + failedPydts2 + failedPydts3 + failedPydts4 + failedDtc1 + failedDtc2 +
			failedDtb))
	local successful=$((tested - skipped - failed))
	echo '================================================================================'
	echo "Tested ${tested}"
	echo "- skipped ${skipped}"
	echo "- successful ${successful}"
	echo "- failed ${failed}:"
	echo "  - pydts merge ${failedPydts0}"
	echo "  - pydts dts->dts ${failedPydts1}"
	echo "  - pydts dts->dictionary ${failedPydts2}"
	echo "  - pydts dictionary->dictionary ${failedPydts3}"
	echo "  - pydts dictionary->dts ${failedPydts4}"
	echo "  - dtc dts(pydts)->dtb ${failedDtc1}"
	echo "  - dtc dts(dtc)->dtb ${failedDtc2}"
	echo "  - cmp dtb(pydts) & dtb(dtc) ${failedDtb}"
}

directory=$(mktemp -d)
cd ${directory}

echo 'Testing with device-tree-rebasing...'
echo
mkdir device-tree-rebasing
cd device-tree-rebasing
echo 'Downloading package...'
wget https://github.com/RobertCNelson/device-tree-rebasing/archive/master.tar.gz 2>&3
tar -xf master.tar.gz 2>&3
cd device-tree-rebasing-master
testPydts '.' '-I include'
cd ..

echo 'Testing with distortos...'
echo
mkdir distortos
cd distortos
testPydts "${distortosPath}" "$(find ${distortosPath} -type d -name 'dtsi' -exec echo -I {} \;)"
cd ..

cd ..
rm -rf ${directory}
