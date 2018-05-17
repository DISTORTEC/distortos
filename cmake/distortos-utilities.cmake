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
# Converts output file of `target` to Intel HEX file named `hexFilename`.
#

function(hex target hexFilename)
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${target}> ${hexFilename}
			BYPRODUCTS ${hexFilename}
			VERBATIM)
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
