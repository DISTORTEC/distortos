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
# Dumps symbols from output file of `target` to file named `dmpFilename`.
#

function(dmp target dmpFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJDUMP} -x --syms --demangle $<TARGET_FILE:${target}> > ${dmpFilename}
			BYPRODUCTS ${dmpFilename}
			VERBATIM)
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
# Converts output file of `target` to Intel HEX file named `hexFilename`.
#

function(hex target hexFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${target}> ${hexFilename}
			BYPRODUCTS ${hexFilename}
			VERBATIM)
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
# Generates disassembly of output file of `target` to file named `lssFilename`.
#

function(lss target lssFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJDUMP} --demangle -S $<TARGET_FILE:${target}> > ${lssFilename}
			BYPRODUCTS ${lssFilename}
			VERBATIM)
endfunction()

#
# Prints size of output file of `target`.
#

function(size target)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_SIZE} -B $<TARGET_FILE:${target}>
			VERBATIM
			USES_TERMINAL)
endfunction()
