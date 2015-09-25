#!/bin/sh

#
# file: distclean.sh
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-09-25
#

set -e
set -u

if [ -d .tup ]; then
	echo "Removing tup database..."
	rm -rf .tup/
fi

if [ -d output ]; then
	echo "Removing build outputs..."
	rm -rf output/
fi

if [ -d documentation/html ]; then
	echo "Removing doxygen outputs..."
	rm -rf documentation/html/
fi

if [ -f selectedConfiguration.mk ]; then
	echo "Removing selectedConfiguration.mk file..."
	rm -f selectedConfiguration.mk
fi

echo "Project cleaned successfully."

