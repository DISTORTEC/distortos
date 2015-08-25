#!/usr/bin/awk -f

#
# file: makeDistortosConfiguration.awk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-08-25
#

BEGIN {
	configPattern = "CONFIG_[A-Za-z0-9_]+"
	booleanTrueConfigPattern = "^" configPattern "=y$"
	booleanFalseConfigPattern = "^" configPattern "=n$"
	notSetConfigPattern = "^# " configPattern " is not set$"
	numericConfigPattern = "^" configPattern "=(([[:digit:]]+)|(0x[[:xdigit:]]+))$"
	stringConfigPattern = "^" configPattern "=\"([^\"\\\\]|\\\\.)*\"$"

	print "/**"
	print " * \\file"
	print " * \\brief distortos configuration"
	print " *"
	print " * \\warning"
	print " * Automatically generated file - do not edit!"
	print " *"
	print " * \\date " strftime("%Y-%m-%d %H:%M:%S")
	print " */"
	print ""
	print "#ifndef INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_"
	print "#define INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_"
	print ""
}

$0 ~ booleanTrueConfigPattern {
	split($0, array, "=")
	print "#define " array[1]
}

$0 ~ booleanFalseConfigPattern {
	split($0, array, "=")
	print "#undef " array[1]
}

$0 ~ notSetConfigPattern {
	print "#undef " $2
}

$0 ~ numericConfigPattern {
	split($0, array, "=")
	print "#define " array[1] " " array[2]
}

$0 ~ stringConfigPattern {
	split($0, array, "=")
	print "#define " array[1] " " array[2]
}

END {
	print ""
	print "#endif	/* INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_ */"
}
