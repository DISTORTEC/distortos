#!/bin/sh

#
# file: distclean.sh
#
# author: Copyright (C) 2015-2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

set -e
set -u

if [ -d output ]; then
	echo 'Removing build outputs...'
	rm -rf output/
fi

if [ -d documentation/html ]; then
	echo 'Removing doxygen outputs...'
	rm -rf documentation/html/
fi

if [ -f selectedConfiguration.mk ]; then
	echo 'Removing selectedConfiguration.mk file...'
	rm -f selectedConfiguration.mk
fi

echo 'Project cleaned successfully.'
