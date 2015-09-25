#!/bin/sh

#
# file: STM32F4.ld.sh
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
set -o pipefail

if [ $# -ne 1 ]; then
	echo "This script requires exactly one argument!" >&2
	exit -1
fi

if [ ! -f $1 ]; then
	echo "Configuation file $1 does not exist!" >&2
	exit -1
fi

# "execute" the configuration file provided as argument to read all the configuration variables
. $1

if [ $CONFIG_CHIP_STM32F4_FLASH_SIZE -eq 0 ] || [ $CONFIG_CHIP_STM32F4_SRAM1_SIZE -eq 0 ]; then
	echo "CONFIG_CHIP_STM32F4_FLASH_SIZE and CONFIG_CHIP_STM32F4_SRAM1_SIZE cannot be 0!" >&2
	exit -1
fi

PROCESS_STACK_SIZE=$CONFIG_MAIN_THREAD_STACK_SIZE

cat<<EOF
/**
 * \file
 * \brief Linker script for $CONFIG_CHIP chip:
 * - $CONFIG_CHIP_STM32F4_FLASH_SIZE bytes of flash;
 * - $CONFIG_CHIP_STM32F4_SRAM1_SIZE bytes of SRAM1;
EOF

if [ $CONFIG_CHIP_STM32F4_SRAM2_SIZE -ne 0 ]; then
	echo " * - $CONFIG_CHIP_STM32F4_SRAM2_SIZE bytes of SRAM2;"
fi
if [ $CONFIG_CHIP_STM32F4_SRAM3_SIZE -ne 0 ]; then
	echo " * - $CONFIG_CHIP_STM32F4_SRAM3_SIZE bytes of SRAM3;"
fi
if [ $CONFIG_CHIP_STM32F4_CCM_SIZE -ne 0 ]; then
	echo " * - $CONFIG_CHIP_STM32F4_CCM_SIZE bytes of CCM;"
fi
if [ $CONFIG_CHIP_STM32F4_BKPSRAM_SIZE -ne 0 ]; then
	echo " * - $CONFIG_CHIP_STM32F4_BKPSRAM_SIZE bytes of BKPSRAM;"
fi

cat<<EOF
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 *
 * \date `date +"%Y-%m-%d %H:%M:%S"`
 */

SEARCH_DIR(.);
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm");
OUTPUT_ARCH(arm);

/*---------------------------------------------------------------------------------------------------------------------+
| stacks sizes
+---------------------------------------------------------------------------------------------------------------------*/

/* Handler mode (core exceptions / interrupts) can use only main stack */
/* Thread mode can use main stack (default after reset) or process stack - selected in CONTROL special register */

__main_stack_size = $CONFIG_ARCHITECTURE_ARMV7_M_MAIN_STACK_SIZE;
__process_stack_size = $PROCESS_STACK_SIZE;

PROVIDE(__main_stack_size = __main_stack_size);
PROVIDE(__process_stack_size = __process_stack_size);

/*---------------------------------------------------------------------------------------------------------------------+
| available memories
+---------------------------------------------------------------------------------------------------------------------*/

MEMORY
{
	rom (rx)		: org = $CONFIG_CHIP_STM32F4_FLASH_ADDRESS, len = $CONFIG_CHIP_STM32F4_FLASH_SIZE
	ram (rwx)		: org = $CONFIG_CHIP_STM32F4_SRAM1_ADDRESS, len = $CONFIG_CHIP_STM32F4_SRAM1_SIZE
EOF

if [ $CONFIG_CHIP_STM32F4_SRAM2_SIZE -ne 0 ]; then
	echo "	sram2 (rwx)		: org = $CONFIG_CHIP_STM32F4_SRAM2_ADDRESS, len = $CONFIG_CHIP_STM32F4_SRAM2_SIZE"
fi
if [ $CONFIG_CHIP_STM32F4_SRAM3_SIZE -ne 0 ]; then
	echo "	sram3 (rwx)		: org = $CONFIG_CHIP_STM32F4_SRAM3_ADDRESS, len = $CONFIG_CHIP_STM32F4_SRAM3_SIZE"
fi
if [ $CONFIG_CHIP_STM32F4_CCM_SIZE -ne 0 ]; then
	echo "	ccm (rw)		: org = $CONFIG_CHIP_STM32F4_CCM_ADDRESS, len = $CONFIG_CHIP_STM32F4_CCM_SIZE"
fi
if [ $CONFIG_CHIP_STM32F4_BKPSRAM_SIZE -ne 0 ]; then
	echo "	bkpsram (rwx)	: org = $CONFIG_CHIP_STM32F4_BKPSRAM_ADDRESS, len = $CONFIG_CHIP_STM32F4_BKPSRAM_SIZE"
fi

cat<<EOF
}

__rom_start = ORIGIN(rom);
__rom_size = LENGTH(rom);
__rom_end = __rom_start + __rom_size;
PROVIDE(__rom_start = __rom_start);
PROVIDE(__rom_size = __rom_size);
PROVIDE(__rom_end = __rom_end);

__ram_start = ORIGIN(ram);
__ram_size = LENGTH(ram);
__ram_end = __ram_start + __ram_size;
PROVIDE(__ram_start = __ram_start);
PROVIDE(__ram_size = __ram_size);
PROVIDE(__ram_end = __ram_end);

EOF

if [ $CONFIG_CHIP_STM32F4_SRAM2_SIZE -ne 0 ]; then
cat<<EOF
__sram2_start = ORIGIN(sram2);
__sram2_size = LENGTH(sram2);
__sram2_end = __sram2_start + __sram2_size;
PROVIDE(__sram2_start = __sram2_start);
PROVIDE(__sram2_size = __sram2_size);
PROVIDE(__sram2_end = __sram2_end);

EOF
fi

if [ $CONFIG_CHIP_STM32F4_SRAM3_SIZE -ne 0 ]; then
cat<<EOF
__sram3_start = ORIGIN(sram3);
__sram3_size = LENGTH(sram3);
__sram3_end = __sram3_start + __sram3_size;
PROVIDE(__sram3_start = __sram3_start);
PROVIDE(__sram3_size = __sram3_size);
PROVIDE(__sram3_end = __sram3_end);

EOF
fi

if [ $CONFIG_CHIP_STM32F4_CCM_SIZE -ne 0 ]; then
cat<<EOF
__ccm_start = ORIGIN(ccm);
__ccm_size = LENGTH(ccm);
__ccm_end = __ccm_start + __ccm_size;
PROVIDE(__ccm_start = __ccm_start);
PROVIDE(__ccm_size = __ccm_size);
PROVIDE(__ccm_end = __ccm_end);

EOF
fi

if [ $CONFIG_CHIP_STM32F4_BKPSRAM_SIZE -ne 0 ]; then
cat<<EOF
__bkpsram_start = ORIGIN(bkpsram);
__bkpsram_size = LENGTH(bkpsram);
__bkpsram_end = __bkpsram_start + __bkpsram_size;
PROVIDE(__bkpsram_start = __bkpsram_start);
PROVIDE(__bkpsram_size = __bkpsram_size);
PROVIDE(__bkpsram_end = __bkpsram_end);

EOF
fi

cat<<EOF
/*---------------------------------------------------------------------------------------------------------------------+
| entry point
+---------------------------------------------------------------------------------------------------------------------*/

ENTRY(Reset_Handler);

/*---------------------------------------------------------------------------------------------------------------------+
| put data in sections
+---------------------------------------------------------------------------------------------------------------------*/

SECTIONS
{
	.text :
	{
		. = ALIGN(4);
		__text_start = .;
		PROVIDE(__text_start = __text_start);

		/* sub-section: .vectors */

		. = ALIGN(4);
		__vectors_start = .;
		PROVIDE(__vectors_start = __vectors_start);

		. = ALIGN(4);
		KEEP(*(.coreVectors));
		KEEP(*(.chipVectors));

		. = ALIGN(4);
		__vectors_end = .;
		PROVIDE(__vectors_end = __vectors_end);

		/* end of sub-section: .vectors */

		. = ALIGN(4);
		*(.text);
		. = ALIGN(4);
		*(.text.*);
		. = ALIGN(4);
		*(.gnu.linkonce.t.*);
		. = ALIGN(4);
		*(.glue_7t .glue_7);
		. = ALIGN(4);
		*(.rodata .rodata.* .gnu.linkonce.r.*);

		. = ALIGN(4);
		*(.ARM.extab* .gnu.linkonce.armextab.*);	/* exception unwinding information */
		. = ALIGN(4);
		*(.gcc_except_table);						/* information used for stack unwinding during exception */
		. = ALIGN(4);
		*(.eh_frame_hdr);							/* additional information about .ex_frame section */
		. = ALIGN(4);
		*(.eh_frame);								/* information used for stack unwinding during exception */

		/* sub-section: data_array */

		. = ALIGN(4);
		__data_array_start = .;
		PROVIDE(__data_array_start = __data_array_start);

		LONG(LOADADDR(.data)); LONG(ADDR(.data)); LONG(ADDR(.data) + SIZEOF(.data));

		. = ALIGN(4);
		__data_array_end = .;
		PROVIDE(__data_array_end = __data_array_end);

		/* end of sub-section: data_array */

		/* sub-section: bss_array */

		. = ALIGN(4);
		__bss_array_start = .;
		PROVIDE(__bss_array_start = __bss_array_start);

		LONG(ADDR(.bss)); LONG(ADDR(.bss) + SIZEOF(.bss));
		LONG(ADDR(.stack)); LONG(ADDR(.stack) + SIZEOF(.stack));

		. = ALIGN(4);
		__bss_array_end = .;
		PROVIDE(__bss_array_end = __bss_array_end);

		/* end of sub-section: bss_array */

		/* sub-sections: init, preinit_array, init_array and fini_array */

		. = ALIGN(4);
		KEEP(*(.init));
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP(*(.preinit_array));
		. = ALIGN(4);
		__preinit_array_end = .;
		__init_array_start = .;
		KEEP(*(SORT(.init_array.*)));
		. = ALIGN(4);
		KEEP(*(.init_array));
		. = ALIGN(4);
		__init_array_end = .;
		KEEP(*(.fini));
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP(*(.fini_array));
		. = ALIGN(4);
		KEEP(*(SORT(.fini_array.*)));
		. = ALIGN(4);
		__fini_array_end = .;

		/* end of sub-sections: init, preinit_array, init_array and fini_array */

		. = ALIGN(4);
		__text_end = .;
		PROVIDE(__text_end = __text_end);
	} > rom AT > rom

	. = ALIGN(4);
	__exidx_start = .;
	PROVIDE(__exidx_start = __exidx_start);

	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*);
	} > rom AT > rom								/* index entries for section unwinding */

	. = ALIGN(4);
	__exidx_end = .;
	PROVIDE(__exidx_end = __exidx_end);

	.data :
	{
		. = ALIGN(4);
		__data_init_start = LOADADDR(.data);
		PROVIDE(__data_init_start = __data_init_start);
		__data_start = .;
		PROVIDE(__data_start = __data_start);

		. = ALIGN(4);
		*(.data .data.* .gnu.linkonce.d.*)

		. = ALIGN(4);
		__data_end = .;
		PROVIDE(__data_end = __data_end);
	} > ram AT > rom

	.bss :
	{
		. = ALIGN(4);
		__bss_start = .;
		PROVIDE(__bss_start = __bss_start);

		. = ALIGN(4);
		*(.bss .bss.* .gnu.linkonce.b.*)
		. = ALIGN(4);
		*(COMMON);

		. = ALIGN(4);
		__bss_end = .;
		PROVIDE(__bss_end = __bss_end);
	} > ram AT > ram

	.stack :
	{
		. = ALIGN(8);
		__stack_start = .;
		PROVIDE(__stack_start = __stack_start);

		. = ALIGN(8);
		__main_stack_start = .;
		PROVIDE(__main_stack_start = __main_stack_start);

		. += __main_stack_size;

		. = ALIGN(8);
		__main_stack_end = .;
		PROVIDE(__main_stack_end = __main_stack_end);

		. = ALIGN(8);
		__process_stack_start = .;
		PROVIDE(__process_stack_start = __process_stack_start);

		. += __process_stack_size;

		. = ALIGN(8);
		__process_stack_end = .;
		PROVIDE(__process_stack_end = __process_stack_end);

		. = ALIGN(8);
		__stack_end = .;
		PROVIDE(__stack_end = __stack_end);
	} > ram AT > ram

	. = ALIGN(4);
	__heap_start = .;
	PROVIDE(__heap_start = __heap_start);

	. = ALIGN(4);
	__heap_end = __ram_end;
	PROVIDE(__heap_end = __heap_end);

	.stab 				0 (NOLOAD) : { *(.stab) }
	.stabstr 			0 (NOLOAD) : { *(.stabstr) }
	/* DWARF debug sections.
	* Symbols in the DWARF debugging sections are relative to the beginning
	* of the section so we begin them at 0. */
	/* DWARF 1 */
	.debug				0 : { *(.debug) }
	.line				0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo		0 : { *(.debug_srcinfo) }
	.debug_sfnames		0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges		0 : { *(.debug_aranges) }
	.debug_pubnames		0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info			0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev		0 : { *(.debug_abbrev) }
	.debug_line			0 : { *(.debug_line) }
	.debug_frame		0 : { *(.debug_frame) }
	.debug_str			0 : { *(.debug_str) }
	.debug_loc			0 : { *(.debug_loc) }
	.debug_macinfo		0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames	0 : { *(.debug_weaknames) }
	.debug_funcnames	0 : { *(.debug_funcnames) }
	.debug_typenames	0 : { *(.debug_typenames) }
	.debug_varnames		0 : { *(.debug_varnames) }

	.note.gnu.arm.ident	0 : { KEEP(*(.note.gnu.arm.ident)) }
	.ARM.attributes		0 : { KEEP(*(.ARM.attributes)) }
	/DISCARD/				: { *(.note.GNU-stack) }
}

PROVIDE(__text_size = __text_end - __text_start);
	PROVIDE(__vectors_size = __vectors_end - __vectors_start);
PROVIDE(__exidx_size = __exidx_end - __exidx_start);
PROVIDE(__data_size = __data_end - __data_start);
PROVIDE(__bss_size = __bss_end - __bss_start);
PROVIDE(__stack_size = __stack_end - __stack_start);
PROVIDE(__heap_size = __heap_end - __heap_start);

PROVIDE(__bss_start__ = __bss_start);
PROVIDE(__bss_end__ = __bss_end);
EOF
