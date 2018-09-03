#
# file: cmake/90-ARMv6-M-ARMv7-M.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

set(DISTORTOS_RAW_LINKER_SCRIPT "source/board/ST_NUCLEO-L073RZ/ST_NUCLEO-L073RZ.ld")

distortosSetConfiguration(BOOLEAN
		CONFIG_ARCHITECTURE_ASCENDING_STACK
		OFF
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_ARCHITECTURE_EMPTY_STACK
		OFF
		INTERNAL)

distortosSetConfiguration(INTEGER
		CONFIG_ARCHITECTURE_STACK_ALIGNMENT
		8
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_ARCHITECTURE_ARM
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_ARCHITECTURE_ARMV6_M
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_ARCHITECTURE_ARM_CORTEX_M0PLUS
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_ARCHITECTURE_ARM_CORTEX_M0PLUS_R0P1
		ON
		INTERNAL)

distortosSetConfiguration(INTEGER
		distortos_Architecture_00_Interrupt_stack_size
		1024
		MIN 8
		HELP "Size (in bytes) of \"main\" stack used by core exceptions and interrupts in Handler mode."
		OUTPUT_NAME CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE)

distortosSetConfiguration(STRING
		distortos_Memory_regions_00_text_vectors
		"flash"
		HELP "Memory region for .text.vectors section in linker script"
		OUTPUT_NAME CONFIG_LD_TEXT_VECTORS_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_01_text
		"flash"
		HELP "Memory region for .text section in linker script"
		OUTPUT_NAME CONFIG_LD_TEXT_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_02_ARM_exidx
		"flash"
		HELP "Memory region for .ARM.exidx section in linker script"
		OUTPUT_NAME CONFIG_LD_ARM_EXIDX_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_03_Main_stack
		"SRAM"
		HELP "Memory region for main stack in linker script"
		OUTPUT_NAME CONFIG_LD_MAIN_STACK_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_04_bss
		"SRAM"
		HELP "Memory region for .bss section in linker script"
		OUTPUT_NAME CONFIG_LD_BSS_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_05_data_VMA
		"SRAM"
		HELP "VMA memory region for .data section in linker script"
		OUTPUT_NAME CONFIG_LD_DATA_VMA_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_06_data_LMA
		"flash"
		HELP "LMA memory region for .data section in linker script"
		OUTPUT_NAME CONFIG_LD_DATA_LMA_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_07_noinit
		"SRAM"
		HELP "Memory region for .noinit section in linker script"
		OUTPUT_NAME CONFIG_LD_NOINIT_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_08_SRAM_data_LMA
		"flash"
		HELP "LMA memory region for .SRAM.data section in linker script"
		OUTPUT_NAME CONFIG_LD_SRAM_DATA_LMA_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_09_Process_stack
		"SRAM"
		HELP "Memory region for process stack in linker script"
		OUTPUT_NAME CONFIG_LD_PROCESS_STACK_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_10_Heap
		"SRAM"
		HELP "Memory region for heap in linker script"
		OUTPUT_NAME CONFIG_LD_HEAP_REGION
		OUTPUT_TYPES BOOLEAN)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/architecture/ARM/ARMv6-M-ARMv7-M/distortos-sources.cmake")
