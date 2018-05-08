#
# file: distortos-utilities.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

#
# Converts output file of `target` to binary file named `binFilename`.
#

function(bin target binFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${target}> ${binFilename}
			BYPRODUCTS ${binFilename}
			VERBATIM)
endfunction()

#
# Dumps symbols from ELF file named `elfFilename` to file named `dmpFilename`.
#

function(dmp elfFilename dmpFilename)
	add_custom_command(OUTPUT ${dmpFilename}
			COMMAND ${CMAKE_OBJDUMP} -x --syms --demangle ${elfFilename} > ${dmpFilename}
			DEPENDS ${elfFilename}
			USES_TERMINAL)
	add_custom_target(${elfFilename}-to-${dmpFilename} ALL DEPENDS ${dmpFilename})
endfunction()

#
# Adds given folders to doxygen `INPUT`, `INCLUDE_PATH` and/or `EXCLUDE`.
#
# `doxygen([INPUT inputFolder ...] [INCLUDE_PATH includePathFolder ...] [EXCLUDE excludeFolder ...])`
#

function(doxygen)
	cmake_parse_arguments(PARSE_ARGV 0 DOXYGEN "" "" "EXCLUDE;INCLUDE_PATH;INPUT")
	if(DOXYGEN_EXCLUDE)
		set(envDoxygenExclude "$ENV{doxygenExclude}")
		list(APPEND envDoxygenExclude ${DOXYGEN_EXCLUDE})
		set(ENV{doxygenExclude} "${envDoxygenExclude}")
	endif()
	if(DOXYGEN_INCLUDE_PATH)
		set(envDoxygenIncludePath "$ENV{doxygenIncludePath}")
		list(APPEND envDoxygenIncludePath ${DOXYGEN_INCLUDE_PATH})
		set(ENV{doxygenIncludePath} "${envDoxygenIncludePath}")
	endif()
	if(DOXYGEN_INPUT)
		set(envDoxygenInput "$ENV{doxygenInput}")
		list(APPEND envDoxygenInput ${DOXYGEN_INPUT})
		set(ENV{doxygenInput} "${envDoxygenInput}")
	endif()
endfunction()

#
# Converts ELF file named `elfFilename` to Intel HEX file named `hexFilename`.
#

function(hex elfFilename hexFilename)
	add_custom_command(OUTPUT ${hexFilename}
			COMMAND ${CMAKE_OBJCOPY} -O ihex ${elfFilename} ${hexFilename}
			DEPENDS ${elfFilename}
			USES_TERMINAL)
	add_custom_target(${elfFilename}-to-${hexFilename} ALL DEPENDS ${hexFilename})
endfunction()

#
# Loads configuration variables from kconfig file named `filename`.
#

function(loadConfiguration filename)
	file(STRINGS ${filename} lines)
	foreach(line ${lines})
		if("${line}" MATCHES "^([^=]+)=(.*)$")
			set(key ${CMAKE_MATCH_1})
			set(value ${CMAKE_MATCH_2})
			if(${value} STREQUAL y)	# bool? convert to ON
				set(value ON)
			elseif(${value} MATCHES "^\"(.*)\"$")	# string? remove quotes and convert to a ;-list
				separate_arguments(value UNIX_COMMAND ${CMAKE_MATCH_1})
			endif()
			set(${key} ${value} PARENT_SCOPE)
		endif()
	endforeach()
endfunction()

#
# Generates disassembly of ELF file named `elfFilename` to file named `lssFilename`.
#

function(lss elfFilename lssFilename)
	add_custom_command(OUTPUT ${lssFilename}
			COMMAND ${CMAKE_OBJDUMP} --demangle -S ${elfFilename} > ${lssFilename}
			DEPENDS ${elfFilename}
			USES_TERMINAL)
	add_custom_target(${elfFilename}-to-${lssFilename} ALL DEPENDS ${lssFilename})
endfunction()

#
# Prints size of ELF file named `elfFilename`.
#

function(size elfFilename)
	add_custom_target(${elfFilename}-size ALL
			COMMAND ${CMAKE_SIZE} -B ${elfFilename}
			DEPENDS ${elfFilename}
			USES_TERMINAL)
endfunction()
