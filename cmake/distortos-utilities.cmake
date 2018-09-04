#
# file: distortos-utilities.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

#
# Adds `flag` string to cache variable `variable`. Useful for cache variables like `CMAKE_CXX_FLAGS` and similar.
#

function(distortosAddFlag variable flag)
	set_property(CACHE "${variable}" PROPERTY VALUE "${${variable}} ${flag}")
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
# Checks whether `boolean` value is a proper boolean. If it's not, a fatal error message is displayed with `name` as a
# prefix.
#

function(distortosCheckBoolean name boolean)
	if(NOT (boolean STREQUAL ON OR boolean STREQUAL OFF))
		message(FATAL_ERROR "\"${name}\": \"${boolean}\" is not a valid boolean")
	endif()
endfunction()

#
# Checks whether `integer` value is a proper integer that is in [`min`; `max`] range. If it's not, a fatal error message
# is displayed with `name` as a prefix.
#

function(distortosCheckInteger name integer min max)
	if(NOT integer MATCHES "^-?[0-9]+$")
		message(FATAL_ERROR "\"${name}\": \"${integer}\" is not a valid integer")
	endif()
	if(integer LESS min OR integer GREATER max)
		message(FATAL_ERROR "\"${name}\": ${integer} is not in [${min}; ${max}] range")
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

function(distortosLss target lssFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJDUMP} --demangle -S $<TARGET_FILE:${target}> > ${lssFilename}
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
# Removes flag which matches `flag` pattern from cache variable `variable`. The flag is removed with any optional value
# and any leading whitespace. Useful for cache variables like `CMAKE_CXX_FLAGS` and similar.
#

function(distortosRemoveFlag variable flag)
	string(REGEX REPLACE "[ \t]*${flag}=?[0-9A-Za-z._-]*" "" value ${${variable}})
	set_property(CACHE "${variable}" PROPERTY VALUE "${value}")
endfunction()

#
# Implementation of `distortosSetConfiguration(BOOLEAN ...`. Do not use directly.
#

function(distortosSetBooleanConfiguration name help)
	cmake_parse_arguments(PARSE_ARGV 2 BOOL "INTERNAL" "" "")
	list(LENGTH BOOL_UNPARSED_ARGUMENTS length)
	if(NOT length EQUAL 1)
		message(FATAL_ERROR "Invalid arguments: \"${BOOL_UNPARSED_ARGUMENTS}\"")
	endif()

	set(defaultValue ${BOOL_UNPARSED_ARGUMENTS})
	distortosCheckBoolean("${name}.DEFAULT" ${defaultValue})

	if(NOT BOOL_INTERNAL)
		set(type BOOL)
	else()
		set(type INTERNAL)
	endif()

	set("${name}" "${defaultValue}" CACHE ${type} "")
	set_property(CACHE "${name}" PROPERTY HELPSTRING "${help}")
	get_property(currentType CACHE "${name}" PROPERTY TYPE)
	if(NOT currentType STREQUAL type)
		set_property(CACHE "${name}" PROPERTY TYPE ${type})
	endif()

	# verify currently set value
	set(currentValue ${${name}})
	distortosCheckBoolean("${name}" ${currentValue})
endfunction()

#
# Implementation of `distortosSetConfiguration(INTEGER ...`. Do not use directly.
#

function(distortosSetIntegerConfiguration name help)
	cmake_parse_arguments(PARSE_ARGV 2 INTEGER "INTERNAL" "MIN;MAX" "")
	list(LENGTH INTEGER_UNPARSED_ARGUMENTS length)
	if(NOT length EQUAL 1)
		message(FATAL_ERROR "Invalid arguments: \"${INTEGER_UNPARSED_ARGUMENTS}\"")
	endif()

	if(NOT INTEGER_MIN)
		set(INTEGER_MIN -2147483648)
	endif()
	if(NOT INTEGER_MAX)
		set(INTEGER_MAX 2147483647)
	endif()

	distortosCheckInteger("${name}.MIN" ${INTEGER_MIN} -2147483648 2147483647)
	distortosCheckInteger("${name}.MAX" ${INTEGER_MAX} -2147483648 2147483647)
	if(INTEGER_MIN GREATER INTEGER_MAX)
		message(FATAL_ERROR "[${INTEGER_MIN}; ${INTEGER_MAX}] is not a valid range")
	endif()

	set(defaultValue ${INTEGER_UNPARSED_ARGUMENTS})
	distortosCheckInteger("${name}.DEFAULT" ${defaultValue} ${INTEGER_MIN} ${INTEGER_MAX})

	if(NOT INTEGER_INTERNAL)
		set(type STRING)
	else()
		set(type INTERNAL)
	endif()

	if(help)
		string(APPEND help "\n\nAllowed range: [${INTEGER_MIN}; ${INTEGER_MAX}]")
	endif()

	set("${name}" "${defaultValue}" CACHE ${type} "")
	set_property(CACHE "${name}" PROPERTY HELPSTRING "${help}")
	get_property(currentType CACHE "${name}" PROPERTY TYPE)
	if(NOT currentType STREQUAL type)
		set_property(CACHE "${name}" PROPERTY TYPE ${type})
	endif()

	# verify currently set value
	set(currentValue "${${name}}")
	distortosCheckInteger("${name}" "${currentValue}" ${INTEGER_MIN} ${INTEGER_MAX})
endfunction()

#
# Implementation of `distortosSetConfiguration(STRING ...`. Do not use directly.
#

function(distortosSetStringConfiguration name help)
	cmake_parse_arguments(PARSE_ARGV 2 STRING "INTERNAL" "" "")

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

	if(NOT STRING_INTERNAL)
		set(type STRING)
	else()
		set(type INTERNAL)
	endif()

	set("${name}" "${defaultValue}" CACHE ${type} "")
	set_property(CACHE "${name}" PROPERTY HELPSTRING "${help}")
	set_property(CACHE "${name}" PROPERTY STRINGS "${STRING_UNPARSED_ARGUMENTS}")
	get_property(currentType CACHE "${name}" PROPERTY TYPE)
	if(NOT currentType STREQUAL type)
		set_property(CACHE "${name}" PROPERTY TYPE ${type})
	endif()

	# verify currently set value
	set(currentValue ${${name}})
	list(FIND STRING_UNPARSED_ARGUMENTS "${currentValue}" index)
	if(index EQUAL -1)
		message(FATAL_ERROR "\"${name}\": \"${currentValue}\" is not an allowed value")
	endif()
endfunction()

#
# Sets new distortos cache configuration named `name` with type `type`.
#
# `distortosSetConfiguration(BOOLEAN name defaultValue [generic-options])`
# `distortosSetConfiguration(INTEGER name defaultValue [MIN min] [MAX max] [generic-options])`
# `distortosSetConfiguration(STRING name string1 [[[string2] string3] ...] [generic-options])`
#
# generic options:
# - `[HELP "help message ..."]` - help message displayed in the GUI; all whitespace after newlines is removed, so the
# message may be indented to match other code;
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
# `defaultValue` must be either `ON` or `OFF`.
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
	list(FIND DISTORTOS_CONFIGURATION_NAMES "${name}" index)
	if(NOT index EQUAL -1)
		message(FATAL_ERROR "Configuration variable \"${name}\" is already set")
	endif()

	cmake_parse_arguments(PARSE_ARGV 2 PREFIX "NO_OUTPUT" "HELP;OUTPUT_NAME" "OUTPUT_TYPES")
	if(NOT PREFIX_HELP)
		set(PREFIX_HELP "")
	endif()
	if(PREFIX_NO_OUTPUT AND (PREFIX_OUTPUT_NAME OR PREFIX_OUTPUT_TYPES))
		message(FATAL_ERROR "Output name and/or type given for \"no output\" configuration variable \"${name}\"")
	endif()
	if(NOT PREFIX_OUTPUT_NAME AND NOT PREFIX_NO_OUTPUT)
		set(PREFIX_OUTPUT_NAME "${name}")
	endif()
	if(NOT PREFIX_OUTPUT_TYPES AND NOT PREFIX_NO_OUTPUT)
		set(PREFIX_OUTPUT_TYPES ${type})
	endif()

	string(REGEX REPLACE "\t+" "" PREFIX_HELP "${PREFIX_HELP}")

	if(type STREQUAL BOOLEAN)
		distortosSetBooleanConfiguration("${name}" "${PREFIX_HELP}" ${PREFIX_UNPARSED_ARGUMENTS})
	elseif(type STREQUAL INTEGER)
		distortosSetIntegerConfiguration("${name}" "${PREFIX_HELP}" ${PREFIX_UNPARSED_ARGUMENTS})
	elseif(type STREQUAL STRING)
		distortosSetStringConfiguration("${name}" "${PREFIX_HELP}" ${PREFIX_UNPARSED_ARGUMENTS})
	else()
		message(FATAL_ERROR "\"${type}\" is not a valid type")
	endif()

	list(APPEND DISTORTOS_CONFIGURATION_NAMES "${name}")
	set(DISTORTOS_CONFIGURATION_NAMES ${DISTORTOS_CONFIGURATION_NAMES} PARENT_SCOPE)
	set(${name}_TYPE ${type} PARENT_SCOPE)
	set(${name}_OUTPUT_NAME ${PREFIX_OUTPUT_NAME} PARENT_SCOPE)
	set(${name}_OUTPUT_TYPES ${PREFIX_OUTPUT_TYPES} PARENT_SCOPE)
endfunction()

#
# Prints size of output file of `target`.
#

function(distortosSize target)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_SIZE} -B $<TARGET_FILE:${target}>
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
