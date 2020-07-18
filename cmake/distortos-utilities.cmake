#
# file: distortos-utilities.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

#
# Adds `flag` string to cache variable `variable`. Useful for cache variables like `CMAKE_CXX_FLAGS` and similar.
#

function(distortosAddFlag variable flag)
	set_property(CACHE "${variable}" PROPERTY VALUE "${${variable}} ${flag}")
endfunction()

#
# Appends one or more `#define`s with distortos configuration named `name` with type `type` to generated header.
#
# Each `#define` is written using `outputName` as name and `outputTypes` (a list) as types. `type` is the same as
# supported by distortosSetConfiguration() and distortosSetFixedConfiguration().
#

function(distortosAppendToHeader name type value outputName outputTypes)
	if(CMAKE_PROJECT_NAME STREQUAL "CMAKE_TRY_COMPILE")	# do nothing during compiler identification and checking
		return()
	endif()

	unset(lines)

	foreach(outputType ${outputTypes})

		if(outputType STREQUAL type)
			set(configurationName ${outputName})
			set(configurationValue ${value})
		elseif(outputType STREQUAL BOOLEAN)
			string(TOUPPER ${outputName}_${value} configurationName)
			set(configurationValue ON)
		elseif(outputType STREQUAL INTEGER)
			distortosCheckInteger("${name}" "${value}" -2147483648 2147483647)
			set(configurationName ${outputName})
			set(configurationValue ${value})
		else()
			message(FATAL_ERROR "Conversion from \"${type}\" to \"${outputType}\" is not implemented/possible")
		endif()

		if(outputType STREQUAL BOOLEAN)
			if(configurationValue)
				set(line "#define ${configurationName} 1\n")
			endif()
		elseif(outputType STREQUAL INTEGER)
			set(line "#define ${configurationName} ${configurationValue}\n")
		elseif(outputType STREQUAL STRING)
			set(line "#define ${configurationName} \"${configurationValue}\"\n")
		endif()

		string(APPEND lines "${line}")
	endforeach()

	if(NOT DEFINED distortos_BINARY_DIR)
		set_property(GLOBAL APPEND_STRING PROPERTY DISTORTOS_APPEND_TO_HEADER_DEFERRED_LINES "${lines}")
		return()
	endif()

	set(distortosConfiguration "${distortos_BINARY_DIR}/include/distortos/distortosConfiguration.h")

	get_property(once GLOBAL PROPERTY DISTORTOS_APPEND_TO_HEADER_ONCE)
	if(NOT once)
		set_property(GLOBAL PROPERTY DISTORTOS_APPEND_TO_HEADER_ONCE ON)

		message(STATUS "Generating ${distortosConfiguration}")
		string(TIMESTAMP timestamp "%Y-%m-%d %H:%M:%S")
		get_property(deferredLines GLOBAL PROPERTY DISTORTOS_APPEND_TO_HEADER_DEFERRED_LINES)
		file(WRITE ${distortosConfiguration}
				"/**\n"
				" * \\file\n"
				" * \\brief distortos configuration\n"
				" *\n"
				" * \\warning\n"
				" * Automatically generated file - do not edit!\n"
				" *\n"
				" * \\date ${timestamp}\n"
				" */\n"
				"\n"
				"#pragma once\n"
				"\n"
				"${deferredLines}")
	endif()

	file(APPEND ${distortosConfiguration} "${lines}")
endfunction()

#
# Appends one CMake cache variable with distortos configuration named `name` to saved configuration.
#

function(distortosAppendToSavedConfiguration name)
	if(CMAKE_PROJECT_NAME STREQUAL "CMAKE_TRY_COMPILE")	# do nothing during compiler identification and checking
		return()
	endif()

	if(NOT DEFINED "${name}")	# do nothing if variable is not defined
		return()
	endif()

	get_property(helpstring CACHE "${name}" PROPERTY HELPSTRING)
	string(REPLACE "\n" "\\n" helpstring "${helpstring}")
	string(REPLACE "\"" "\\\"" helpstring "${helpstring}")
	get_property(type CACHE "${name}" PROPERTY TYPE)
	set(value "${${name}}")

	# this will work only if distortos_SOURCE_DIR is already defined and fail otherwise, however it is acutally used
	# only for CMAKE_TOOLCHAIN_FILE
	if(type STREQUAL FILEPATH)
		file(RELATIVE_PATH value "${distortos_SOURCE_DIR}" "${value}")
		set(value "\${DISTORTOS_PATH}/${value}")
	endif()

	set(entry "set(\"${name}\"\n\t\t\"${value}\"\n\t\tCACHE\n\t\t\"${type}\"\n\t\t\"${helpstring}\")\n")

	if(NOT DEFINED distortos_BINARY_DIR)
		set_property(GLOBAL APPEND_STRING PROPERTY DISTORTOS_APPEND_TO_SAVED_CONFIGURATION_DEFERRED_ENTRIES "${entry}")
		return()
	endif()

	set(distortosConfiguration "${distortos_BINARY_DIR}/distortosConfiguration.cmake")
	file(RELATIVE_PATH distortosPath "${CMAKE_BINARY_DIR}" "${distortos_SOURCE_DIR}")

	get_property(once GLOBAL PROPERTY DISTORTOS_APPEND_TO_SAVED_CONFIGURATION_ONCE)
	if(NOT once)
		set_property(GLOBAL PROPERTY DISTORTOS_APPEND_TO_SAVED_CONFIGURATION_ONCE ON)

		message(STATUS "Generating ${distortosConfiguration}")
		get_property(deferredEntries GLOBAL PROPERTY DISTORTOS_APPEND_TO_SAVED_CONFIGURATION_DEFERRED_ENTRIES)
		file(WRITE ${distortosConfiguration}
				"#\n"
				"# \\file\n"
				"# \\brief distortos configuration\n"
				"#\n"
				"# \\warning\n"
				"# Automatically generated file - do not edit!\n"
				"#\n"
				"\n"
				"if(DEFINED ENV{DISTORTOS_PATH})\n"
				"	set(DISTORTOS_PATH \"\$ENV{DISTORTOS_PATH}\")\n"
				"else()\n"
				"	set(DISTORTOS_PATH \"${distortosPath}\")\n"
				"endif()\n"
				"\n"
				"${deferredEntries}")
	endif()

	file(APPEND ${distortosConfiguration} "${entry}")
endfunction()

#
# Converts output file of `target` to binary file named `binFilename`.
#
# All additional arguments are passed to ${CMAKE_OBJCOPY}.
#

function(distortosBin target binFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJCOPY} ${ARGN} -O binary $<TARGET_FILE:${target}> ${binFilename}
			BYPRODUCTS ${binFilename}
			VERBATIM)
endfunction()

#
# Checks whether `boolean` value is a proper boolean. If it's not, an error message is displayed with `name` as a
# prefix.
#

function(distortosCheckBoolean name boolean)
	if(NOT (boolean STREQUAL ON OR boolean STREQUAL OFF))
		message(SEND_ERROR "\"${name}\": \"${boolean}\" is not a valid boolean")
	endif()
endfunction()

#
# Checks whether `integer` value is a proper integer that is in [`min`; `max`] range. If it's not, an error message is
# displayed with `name` as a prefix.
#

function(distortosCheckInteger name integer min max)
	if(NOT integer MATCHES "^-?[0-9]+$")
		message(SEND_ERROR "\"${name}\": \"${integer}\" is not a valid integer")
	endif()
	if(integer LESS min OR integer GREATER max)
		message(SEND_ERROR "\"${name}\": ${integer} is not in [${min}; ${max}] range")
	endif()
endfunction()

#
# Dumps symbols from output file of `target` to file named `dmpFilename`.
#
# All additional arguments are passed to ${CMAKE_OBJDUMP}.
#

function(distortosDmp target dmpFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJDUMP} ${ARGN} -x --syms --demangle $<TARGET_FILE:${target}> > ${dmpFilename}
			BYPRODUCTS ${dmpFilename}
			VERBATIM)
endfunction()

#
# Converts output file of `target` to Intel HEX file named `hexFilename`.
#
# All additional arguments are passed to ${CMAKE_OBJCOPY}.
#

function(distortosHex target hexFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJCOPY} ${ARGN} -O ihex $<TARGET_FILE:${target}> ${hexFilename}
			BYPRODUCTS ${hexFilename}
			VERBATIM)
endfunction()

#
# Generates disassembly of output file of `target` to file named `lssFilename`.
#
# All additional arguments are passed to ${CMAKE_OBJDUMP}.
#

function(distortosLss target lssFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJDUMP} ${ARGN} --demangle -S $<TARGET_FILE:${target}> > ${lssFilename}
			BYPRODUCTS ${lssFilename}
			VERBATIM)
endfunction()

#
# Requests generation of linker map file named `mapFilename` during linking of `target`.
#

function(distortosMap target mapFilename)
	target_link_libraries(${target} PRIVATE
			-Xlinker -Map="${CMAKE_CURRENT_BINARY_DIR}/${mapFilename}")
endfunction()

#
# Returns max value from the list of integers (passed after named arguments, at least one must be present) in variable
# `output`.
#

function(distortosMax output)
	if(ARGC LESS 2)
		message(FATAL_ERROR "List of integers not provided")
	endif()
	unset(max)
	foreach(item ${ARGN})
		distortosCheckInteger("item" ${item} -2147483648 2147483647)
		if(NOT DEFINED max OR max LESS item)
			set(max ${item})
		endif()
	endforeach()
	set("${output}" "${max}" PARENT_SCOPE)
endfunction()

#
# Removes flag which matches `flag` pattern from cache variable `variable`. The flag is removed with any optional value
# and any leading whitespace. Useful for cache variables like `CMAKE_CXX_FLAGS` and similar.
#

function(distortosRemoveFlag variable flag)
	string(REGEX REPLACE "[ \t]*${flag}=?[0-9A-Za-z._-]*" "" value ${${variable}})
	set_property(CACHE "${variable}" PROPERTY VALUE "${value}")
endfunction()

#
# Implementation of `distortosSetConfiguration(BOOLEAN ...)`. Do not use directly.
#

function(distortosSetBooleanConfiguration name help)
	cmake_parse_arguments(PARSE_ARGV 2 BOOL "FORCE;INTERNAL" "" "DEPENDENTS;OFF_DEPENDENTS")
	list(LENGTH BOOL_UNPARSED_ARGUMENTS length)
	if(NOT length EQUAL 1)
		message(FATAL_ERROR "Invalid arguments: \"${BOOL_UNPARSED_ARGUMENTS}\"")
	endif()

	if(DEFINED BOOL_DEPENDENTS AND DEFINED BOOL_OFF_DEPENDENTS)
		message(FATAL_ERROR "Only one of DEPENDENTS and OFF_DEPENDENTS may be used")
	endif()

	set(defaultValue ${BOOL_UNPARSED_ARGUMENTS})
	distortosCheckBoolean("${name}.DEFAULT" ${defaultValue})

	if(BOOL_FORCE)
		set(force FORCE)
	else()
		unset(force)
	endif()

	if(NOT ${name} AND DEFINED BOOL_DEPENDENTS)
		string(REPLACE ";" ", " dependents "${BOOL_DEPENDENTS}")
		message(STATUS "Auto-enabling ${name}. Dependents: ${dependents}")
		set(defaultValue ON)
		set(force FORCE)
	endif()
	if(${name} AND DEFINED BOOL_OFF_DEPENDENTS)
		string(REPLACE ";" ", " offDependents "${BOOL_OFF_DEPENDENTS}")
		message(STATUS "Auto-disabling ${name}. Off-dependents: ${offDependents}")
		set(defaultValue OFF)
		set(force FORCE)
	endif()

	if(NOT BOOL_INTERNAL)
		set(type BOOL)
	else()
		set(type INTERNAL)
	endif()

	set("${name}" "${defaultValue}" CACHE ${type} "" ${force})
	set_property(CACHE "${name}" PROPERTY HELPSTRING "${help}")
	set_property(CACHE "${name}" PROPERTY TYPE ${type})

	# verify currently set value
	set(currentValue ${${name}})
	distortosCheckBoolean("${name}" ${currentValue})

	distortosAppendToSavedConfiguration("${name}")
endfunction()

#
# Implementation of `distortosSetConfiguration(INTEGER ...)`. Do not use directly.
#

function(distortosSetIntegerConfiguration name help)
	cmake_parse_arguments(PARSE_ARGV 2 INTEGER "FORCE;INTERNAL" "MIN;MAX" "")
	list(LENGTH INTEGER_UNPARSED_ARGUMENTS length)
	if(NOT length EQUAL 1)
		message(FATAL_ERROR "Invalid arguments: \"${INTEGER_UNPARSED_ARGUMENTS}\"")
	endif()

	if(NOT DEFINED INTEGER_MIN)
		set(INTEGER_MIN -2147483648)
	endif()
	if(NOT DEFINED INTEGER_MAX)
		set(INTEGER_MAX 2147483647)
	endif()

	distortosCheckInteger("${name}.MIN" ${INTEGER_MIN} -2147483648 2147483647)
	distortosCheckInteger("${name}.MAX" ${INTEGER_MAX} -2147483648 2147483647)
	if(INTEGER_MIN GREATER INTEGER_MAX)
		message(FATAL_ERROR "[${INTEGER_MIN}; ${INTEGER_MAX}] is not a valid range")
	endif()

	set(defaultValue ${INTEGER_UNPARSED_ARGUMENTS})
	distortosCheckInteger("${name}.DEFAULT" ${defaultValue} ${INTEGER_MIN} ${INTEGER_MAX})

	if(INTEGER_FORCE)
		set(force FORCE)
	else()
		unset(force)
	endif()

	if(NOT INTEGER_INTERNAL)
		set(type STRING)
	else()
		set(type INTERNAL)
	endif()

	if(help)
		string(APPEND help "\n\nAllowed range: [${INTEGER_MIN}; ${INTEGER_MAX}]")
	endif()

	set("${name}" "${defaultValue}" CACHE ${type} "" ${force})
	set_property(CACHE "${name}" PROPERTY HELPSTRING "${help}")
	set_property(CACHE "${name}" PROPERTY TYPE ${type})

	# verify currently set value
	set(currentValue "${${name}}")
	distortosCheckInteger("${name}" "${currentValue}" ${INTEGER_MIN} ${INTEGER_MAX})

	distortosAppendToSavedConfiguration("${name}")
endfunction()

#
# Implementation of `distortosSetConfiguration(STRING ...)`. Do not use directly.
#

function(distortosSetStringConfiguration name help)
	cmake_parse_arguments(PARSE_ARGV 2 STRING "FORCE;INTERNAL" "" "")

	list(FIND STRING_UNPARSED_ARGUMENTS DEFAULT index)
	if(NOT index EQUAL -1)
		if(index EQUAL 0)
			message(FATAL_ERROR "\"DEFAULT\" does not follow a string value")
		endif()
		list(REMOVE_AT STRING_UNPARSED_ARGUMENTS ${index})
		math(EXPR index "${index} - 1")
		list(GET STRING_UNPARSED_ARGUMENTS ${index} defaultValue)
	else()
		list(GET STRING_UNPARSED_ARGUMENTS 0 defaultValue)
	endif()

	list(LENGTH STRING_UNPARSED_ARGUMENTS length)
	if(length EQUAL 0)
		message(FATAL_ERROR "No string values provided")
	endif()

	if(STRING_FORCE)
		set(force FORCE)
	else()
		unset(force)
	endif()

	if(NOT STRING_INTERNAL)
		set(type STRING)
	else()
		set(type INTERNAL)
	endif()

	set("${name}" "${defaultValue}" CACHE ${type} "" ${force})
	set_property(CACHE "${name}" PROPERTY HELPSTRING "${help}")
	set_property(CACHE "${name}" PROPERTY STRINGS "${STRING_UNPARSED_ARGUMENTS}")
	set_property(CACHE "${name}" PROPERTY TYPE ${type})

	# verify currently set value
	set(currentValue ${${name}})
	list(FIND STRING_UNPARSED_ARGUMENTS "${currentValue}" index)
	if(index EQUAL -1)
		message(SEND_ERROR "\"${name}\": \"${currentValue}\" is not an allowed value")
	endif()

	distortosAppendToSavedConfiguration("${name}")
endfunction()

#
# Sets new distortos cache configuration named `name` with type `type`.
#
# `distortosSetConfiguration(BOOLEAN name defaultValue [[OFF_]DEPENDENTS [dependent1 [dependent2 [...]]]]
#		[generic-options])`
# `distortosSetConfiguration(INTEGER name defaultValue [MIN min] [MAX max] [generic-options])`
# `distortosSetConfiguration(STRING name string1 [string2 [string3 [...]]] [generic-options])`
#
# generic options:
# - `[FORCE]` - forces the value of cache entry to `defaultValue`;
# - `[HELP "help message ..."]` - help message displayed in the GUI; all whitespace after newlines is removed, so the
# message may be indented to match other code; all single newlines are replaced with a space, unless they are followed
# by a '-' or '*' character (as in case of a list);
# - `[INTERNAL]` - causes the cache entry to be hidden in the GUI;
# - `[NO_OUTPUT]` - suppress generation of macro in the header; must not be used with `OUTPUT_NAME` and/or
# `OUTPUT_TYPES`;
# - `[OUTPUT_NAME outputName]` - name of macro generated in the header; `name` is used as output name if this option is
# omitted; must not be used with `NO_OUTPUT`;
# - `[OUTPUT_TYPES outputType1 outputType2 ...]` - types of macros generated in the header; possible values are:
# BOOLEAN, INTEGER and STRING; not all conversions are implemented and/or possible; original type is used if this option
# is omitted; must not be used with `NO_OUTPUT`;
#
# `BOOLEAN` variant
# `defaultValue` must be either `ON` or `OFF`. If `DEPENDENTS` or `OFF_DEPENDENTS` is a non-empty list, then this option
# will be auto-enabled or auto-disabled. Only one of `DEPENDENTS` and `OFF_DEPENDENTS` may be used.
#
# `INTEGER` variant
# `defaultValue`, `min` and `max` must be decimal integers in [-2147483648; 2147483647] range. -2147483648 is used as
# `min` if this option is omitted. 2147483647 is used as `max` if this option is omitted. `max` must be greater than or
# equal to `min`. `defaultValue` must be in [`min`; `max`] range.
#
# `STRING` variant
# String followed by `DEFAULT` will be used as the default one. If `DEFAULT` is not present, first string will be used
# as the default one.
#

function(distortosSetConfiguration type name)
	get_property(once GLOBAL PROPERTY DISTORTOS_SET_CONFIGURATION_ONCE)
	if(NOT once)
		set_property(GLOBAL PROPERTY DISTORTOS_SET_CONFIGURATION_ONCE ON)

		foreach(name ${DISTORTOS_ACTIVE_CONFIGURATION_NAMES})
			set_property(CACHE "${name}" PROPERTY TYPE INTERNAL)
		endforeach()

		unset(DISTORTOS_ACTIVE_CONFIGURATION_NAMES CACHE)
	endif()

	list(FIND DISTORTOS_ACTIVE_CONFIGURATION_NAMES "${name}" index)
	if(NOT index EQUAL -1)
		message(FATAL_ERROR "Configuration variable \"${name}\" is already set")
	endif()

	cmake_parse_arguments(PARSE_ARGV 2 PREFIX "NO_OUTPUT" "HELP;OUTPUT_NAME" "OUTPUT_TYPES")
	if(NOT DEFINED PREFIX_HELP)
		set(PREFIX_HELP "")
	endif()
	if(PREFIX_NO_OUTPUT AND (DEFINED PREFIX_OUTPUT_NAME OR DEFINED PREFIX_OUTPUT_TYPES))
		message(FATAL_ERROR "Output name and/or type given for \"no output\" configuration variable \"${name}\"")
	endif()
	if(NOT DEFINED PREFIX_OUTPUT_NAME AND NOT PREFIX_NO_OUTPUT)
		set(PREFIX_OUTPUT_NAME "${name}")
	endif()
	if(NOT DEFINED PREFIX_OUTPUT_TYPES AND NOT PREFIX_NO_OUTPUT)
		set(PREFIX_OUTPUT_TYPES ${type})
	endif()

	string(REGEX REPLACE "\t+" "" PREFIX_HELP "${PREFIX_HELP}")
	string(REGEX REPLACE "([^\n])\n([^\n*-])" "\\1 \\2" PREFIX_HELP "${PREFIX_HELP}")

	if(type STREQUAL BOOLEAN)
		distortosSetBooleanConfiguration("${name}" "${PREFIX_HELP}" ${PREFIX_UNPARSED_ARGUMENTS})
	elseif(type STREQUAL INTEGER)
		distortosSetIntegerConfiguration("${name}" "${PREFIX_HELP}" ${PREFIX_UNPARSED_ARGUMENTS})
	elseif(type STREQUAL STRING)
		distortosSetStringConfiguration("${name}" "${PREFIX_HELP}" ${PREFIX_UNPARSED_ARGUMENTS})
	else()
		message(FATAL_ERROR "\"${type}\" is not a valid type")
	endif()

	set(DISTORTOS_ACTIVE_CONFIGURATION_NAMES ${DISTORTOS_ACTIVE_CONFIGURATION_NAMES} ${name} CACHE INTERNAL "")

	distortosAppendToHeader("${name}" "${type}" "${${name}}" "${PREFIX_OUTPUT_NAME}" "${PREFIX_OUTPUT_TYPES}")
endfunction()

#
# Sets new distortos fixed configuration named `name` with type `type` and value `value`.
#
# `BOOLEAN` type
# `value` must be either `ON` or `OFF`.
#
# `INTEGER` type
# `value` must be decimal integer in [-2147483648; 2147483647] range.
#
# `STRING` type
# Any string is accepted as `value`.
#

function(distortosSetFixedConfiguration type name value)
	list(FIND DISTORTOS_FIXED_CONFIGURATION_NAMES "${name}" index)
	if(NOT index EQUAL -1)
		message(FATAL_ERROR "Fixed configuration variable \"${name}\" is already set")
	endif()

	if(type STREQUAL BOOLEAN)
		distortosCheckBoolean("${name}" "${value}")
	elseif(type STREQUAL INTEGER)
		distortosCheckInteger("${name}" "${value}" -2147483648 2147483647)
	elseif(type STREQUAL STRING)
		# nothing to check, accept everything
	else()
		message(FATAL_ERROR "\"${type}\" is not a valid type")
	endif()

	set("${name}" "${value}" PARENT_SCOPE)

	list(APPEND DISTORTOS_FIXED_CONFIGURATION_NAMES "${name}")
	set(DISTORTOS_FIXED_CONFIGURATION_NAMES ${DISTORTOS_FIXED_CONFIGURATION_NAMES} PARENT_SCOPE)

	distortosAppendToHeader("${name}" "${type}" "${value}" "${name}" "${type}")
endfunction()

#
# Prints size of output file of `target`.
#
# All additional arguments are passed to ${CMAKE_SIZE}.
#

function(distortosSize target)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_SIZE} ${ARGN} -B $<TARGET_FILE:${target}>
			VERBATIM
			USES_TERMINAL)
endfunction()

#
# Specifies linker script(s) used for linking `target`.
#
# `distortosTargetLinkerScripts(target [linker-script.ld ...])`
#
# Most common use will be with single linker script provided by distortos:
# `distortosTargetLinkerScripts(target $ENV{DISTORTOS_LINKER_SCRIPT})`
#
# Proper linker flag is added, as well as CMake dependency via LINK_DEPENDS target property.
#

function(distortosTargetLinkerScripts target)
	foreach(linkerScript IN LISTS ARGN)
		target_link_libraries(${target} PRIVATE
				-T"${linkerScript}")
		get_target_property(linkDepends ${target} LINK_DEPENDS)
		if(NOT linkDepends)
			unset(linkDepends)
		endif()
		list(APPEND linkDepends "${linkerScript}")
		set_target_properties(${target} PROPERTIES
				LINK_DEPENDS "${linkDepends}")
	endforeach()
endfunction()
