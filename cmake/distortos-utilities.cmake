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

function(distortosBin target binFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${target}> ${binFilename}
			BYPRODUCTS ${binFilename}
			VERBATIM)
endfunction()

#
# Dumps symbols from output file of `target` to file named `dmpFilename`.
#

function(distortosDmp target dmpFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJDUMP} -x --syms --demangle $<TARGET_FILE:${target}> > ${dmpFilename}
			BYPRODUCTS ${dmpFilename}
			VERBATIM)
endfunction()

#
# Converts output file of `target` to Intel HEX file named `hexFilename`.
#

function(distortosHex target hexFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${target}> ${hexFilename}
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
