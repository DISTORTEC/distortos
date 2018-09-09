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

set(DISTORTOS_RAW_LINKER_SCRIPT "source/board/ST_NUCLEO-F446RE/ST_NUCLEO-F446RE.ld")

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
		CONFIG_ARCHITECTURE_ARMV7_M
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_ARCHITECTURE_ARM_CORTEX_M4
		ON
		INTERNAL)

distortosSetConfiguration(BOOLEAN
		CONFIG_ARCHITECTURE_ARM_CORTEX_M4_R0P1
		ON
		INTERNAL)

distortosSetConfiguration(INTEGER
		distortos_Architecture_00_Interrupt_stack_size
		1024
		MIN 8
		HELP "Size (in bytes) of \"main\" stack used by core exceptions and interrupts in Handler mode."
		OUTPUT_NAME CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE)

distortosSetConfiguration(INTEGER
		distortos_Architecture_01_Interrupt_priority_disabled_in_critical_sections
		0
		MIN 0
		MAX 15
		HELP "Interrupt priority disabled in critical sections.

		Minimal numerical priority (inclusive) of interrupt handlers that can use system's functions.

		During critical sections all interrupts with numerical priority above or equal to this value will be disabled.
		Interrupts with numerical priority below this value are never disabled, but they may not use any system's
		functions.

		Note - \"lower\" logical priority has \"higher\" numeric value! If this option is set to \"x\", then interrupts
		with priorities between \"x\" and 255 (both inclusive) may use system's functions, while interrupts with
		priorities between 0 and \"x - 1\" (both inclusive) may not. If 0 is chosen, then all interrupts (except
		HardFault and NMI) are disabled during critical sections, so they may use system's functions."
		OUTPUT_NAME CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI)

distortosSetConfiguration(STRING
		distortos_Memory_regions_00_text_vectorTable
		"flash"
		HELP "Memory region for .text.vectorTable section in linker script"
		OUTPUT_NAME CONFIG_LD_TEXT_VECTOR_TABLE_REGION
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
		"BKPSRAM"
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
