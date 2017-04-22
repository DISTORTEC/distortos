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

pydts="$(pwd)/$(dirname "${0}")/pydts.py"
directory=$(mktemp -d)
cd ${directory}

echo 'Downloading package...'

wget https://github.com/RobertCNelson/device-tree-rebasing/archive/master.tar.gz 2>&3
tar -xf master.tar.gz 2>&3
cd device-tree-rebasing-master

tested=0
skipped=0
failedPydts0=0
failedPydts1=0
failedPydts2=0
failedPydts3=0
failedPydts4=0
failedDtc1=0
failedDtc2=0
failedDtb=0

DTCFLAGS='-q -I dts -s -b 0 -H epapr'

dtsFiles="$(find . -name '*.dts' | sort)"
for dtsFile in ${dtsFiles}; do

	tested=$((++tested))
	echo "Testing with ${dtsFile}... "
	basename="$(basename ${dtsFile})"

	# handle includes, resolve macros, remove comments
	CPPFLAGS='-nostdinc -undef -E -x assembler-with-cpp'
	CPPFLAGS="${CPPFLAGS} -I include"
	CPPFLAGS="${CPPFLAGS} -I $(dirname ${dtsFile})"
	cpp ${CPPFLAGS} "${dtsFile}" -o "${basename}" || { skipped=$((++skipped)); echo 'Skipped'; continue; }

	# try to merge the file with dtc - if it fails, it most likely has "/include/" directive (which is currently
	# unsupported), so skip it
	dtc ${DTCFLAGS} -O dts "${basename}" -o "${basename}.dts-dtc" ||
			{ skipped=$((++skipped)); echo 'Skipped'; continue; }

	# Extract phandle values from file merged by dtc and append it to the file used in the test. This way file merged
	# with dtc and file merged with pydts can be both compiled with dtc into a *.dtb and compared. Without this step the
	# phandle values would be different and the comparison would fail.
	{ cat "${basename}" && grep -E '{|}|phandle = <0x[0-9A-Fa-f]+>;' "${basename}.dts-dtc"; } > "${basename}.phandle"
	mv "${basename}.phandle" "${basename}"

	# try to merge the file with pydts
	"${pydts}" -I dts -O dts "${basename}" -o "${basename}.dts-pydts0" ||
			{ failedPydts0=$((++failedPydts0)); echo 'Failed - pydts merge'; continue; }

	# try to merge the file which was just merged - both files should be identical
	{ "${pydts}" -I dts -O dts "${basename}.dts-pydts0" -o "${basename}.dts-pydts1" &&
			cmp -s "${basename}.dts-pydts0" "${basename}.dts-pydts1"; } ||
			{ failedPydts1=$((++failedPydts1)); echo 'Failed - pydts dts->dts'; continue; }

	# try to convert *.dts to dictionary
	"${pydts}" -I dts -O dictionary "${basename}.dts-pydts0" -o "${basename}.dictionary-pydts2" ||
			{ failedPydts2=$((++failedPydts2)); echo 'Failed - pydts dts->dictionary'; continue; }

	# try to convert dictionary to dictionary - both files should be identical
	{ "${pydts}" -I dictionary -O dictionary "${basename}.dictionary-pydts2" -o "${basename}.dictionary-pydts3" &&
			cmp -s "${basename}.dictionary-pydts2" "${basename}.dictionary-pydts3"; } ||
			{ failedPydts3=$((++failedPydts3)); echo 'Failed - pydts dictionary->dictionary'; continue; }

	# try to convert dictionary back to *.dts - the result should be identical to the source *.dts file used to generate
	# the dictionary
	{ "${pydts}" -I dictionary -O dts "${basename}.dictionary-pydts2" -o "${basename}.dts-pydts4" &&
			cmp -s "${basename}.dts-pydts0" "${basename}.dts-pydts4"; } ||
			{ failedPydts4=$((++failedPydts4)); echo 'Failed - pydts dictionary->dts'; continue; }

	# compile *.dts file merged with pydts to *.dtb
	dtc ${DTCFLAGS} -O dtb "${basename}.dts-pydts4" -o "${basename}.dtb0-dtc" ||
			{ failedDtc1=$((++failedDtc1)); echo 'Failed - dtc dts(pydts)->dtb'; continue; }

	# compile original *.dts file (with explicit phandle values) to *.dtb
	dtc ${DTCFLAGS} -O dtb "${basename}" -o "${basename}.dtb1-dtc" ||
			{ failedDtc2=$((++failedDtc2)); echo 'Failed - dtc dts(dtc)->dtb'; continue; }

	# both *.dtb files should be identical
	cmp -s "${basename}.dtb0-dtc" "${basename}.dtb1-dtc" ||
			{ failedDtb=$((++failedDtb)); echo 'Failed - cmp dtb(pydts) & dtb(dtc)'; continue; }

done

failed=$((failedPydts0 + failedPydts1 + failedPydts2 + failedPydts3 + failedPydts4 + failedDtc1 + failedDtc2 +
		failedDtb))
successful=$((tested - skipped - failed))
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

cd ..
rm -rf ${directory}
