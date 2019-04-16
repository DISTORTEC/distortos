/**
 * \file
 * \brief Linker script for ST,NUCLEO-L432KC (ST,STM32L432KC chip):
 * - 256.0 KiB of flash at 0x8000000;
 * - 64.0 KiB of SRAM1 at 0x20000000;
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/distortosConfiguration.h"

SEARCH_DIR(.);
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm");
OUTPUT_ARCH(arm);

/*---------------------------------------------------------------------------------------------------------------------+
| stacks sizes
+---------------------------------------------------------------------------------------------------------------------*/

PROVIDE(__main_stack_size =
		ALIGN(DISTORTOS_ARCHITECTURE_MAIN_STACK_SIZE, DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT));

PROVIDE(__process_stack_size = ALIGN(DISTORTOS_MAIN_THREAD_STACK_SIZE, DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT) +
		ALIGN(DISTORTOS_STACK_GUARD_SIZE, DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT));

/*---------------------------------------------------------------------------------------------------------------------+
| available memories
+---------------------------------------------------------------------------------------------------------------------*/

MEMORY
{
	flash : org = 0x8000000, len = 262144
	SRAM1 : org = 0x20000000, len = 65536
}

PROVIDE(__flash_start = ORIGIN(flash));
PROVIDE(__flash_size = LENGTH(flash));
PROVIDE(__flash_end = ORIGIN(flash) + LENGTH(flash));

PROVIDE(__SRAM1_start = ORIGIN(SRAM1));
PROVIDE(__SRAM1_size = LENGTH(SRAM1));
PROVIDE(__SRAM1_end = ORIGIN(SRAM1) + LENGTH(SRAM1));

/*---------------------------------------------------------------------------------------------------------------------+
| entry point
+---------------------------------------------------------------------------------------------------------------------*/

ENTRY(Reset_Handler);

/*---------------------------------------------------------------------------------------------------------------------+
| put data in sections
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_LD_TEXT_VECTOR_TABLE_REGION_FLASH
REGION_ALIAS("textVectorTableVmaRegion", flash);
REGION_ALIAS("textVectorTableLmaRegion", flash);
#endif	/* DISTORTOS_LD_TEXT_VECTOR_TABLE_REGION_FLASH */

#ifdef DISTORTOS_LD_TEXT_REGION_FLASH
REGION_ALIAS("textVmaRegion", flash);
REGION_ALIAS("textLmaRegion", flash);
#endif	/* DISTORTOS_LD_TEXT_REGION_FLASH */

#ifdef DISTORTOS_LD_ARM_EXIDX_REGION_FLASH
REGION_ALIAS("armExidxVmaRegion", flash);
REGION_ALIAS("armExidxLmaRegion", flash);
#endif	/* DISTORTOS_LD_ARM_EXIDX_REGION_FLASH */

#ifdef DISTORTOS_LD_MAIN_STACK_REGION_SRAM1
REGION_ALIAS("mainStackVmaRegion", SRAM1);
REGION_ALIAS("mainStackLmaRegion", SRAM1);
#endif	/* DISTORTOS_LD_MAIN_STACK_REGION_SRAM1 */

#ifdef DISTORTOS_LD_BSS_REGION_SRAM1
REGION_ALIAS("bssVmaRegion", SRAM1);
REGION_ALIAS("bssLmaRegion", SRAM1);
#endif	/* DISTORTOS_LD_BSS_REGION_SRAM1 */

#ifdef DISTORTOS_LD_DATA_VMA_REGION_SRAM1
REGION_ALIAS("dataVmaRegion", SRAM1);
#endif	/* DISTORTOS_LD_DATA_VMA_REGION_SRAM1 */

#ifdef DISTORTOS_LD_DATA_LMA_REGION_FLASH
REGION_ALIAS("dataLmaRegion", flash);
#endif	/* DISTORTOS_LD_DATA_LMA_REGION_FLASH */

#ifdef DISTORTOS_LD_NOINIT_REGION_SRAM1
REGION_ALIAS("noinitVmaRegion", SRAM1);
REGION_ALIAS("noinitLmaRegion", SRAM1);
#endif	/* DISTORTOS_LD_NOINIT_REGION_SRAM1 */

#ifdef DISTORTOS_LD_SRAM1_DATA_LMA_REGION_FLASH
REGION_ALIAS("SRAM1DataLmaRegion", flash);
#endif	/* DISTORTOS_LD_SRAM1_DATA_LMA_REGION_FLASH */

#ifdef DISTORTOS_LD_PROCESS_STACK_REGION_SRAM1
PROVIDE(__process_stack_end = __SRAM1_end / 8 * 8);
#endif	/* def DISTORTOS_LD_PROCESS_STACK_REGION_SRAM1 */

PROVIDE(__process_stack_start = (__process_stack_end - __process_stack_size) / 8 * 8);

SECTIONS
{
	.text.vectorTable : ALIGN_WITH_INPUT
	{
		PROVIDE(__text_vectorTable_start = .);

		KEEP(*(.vectorTable));

		PROVIDE(__text_vectorTable_end = .);
	} > textVectorTableVmaRegion AT > textVectorTableLmaRegion

	.text : ALIGN_WITH_INPUT
	{
		PROVIDE(__text_start = .);

		*(.text.unlikely .text.*_unlikely .text.unlikely.*);
		*(.text.exit .text.exit.*);
		*(.text.startup .text.startup.*);
		*(.text.hot .text.hot.*);
		*(.text .stub .text.* .gnu.linkonce.t.*);
		*(.gnu.warning);
		*(.glue_7t);
		*(.glue_7);
		*(.vfp11_veneer);
		*(.v4_bx);
		*(.rodata .rodata.* .gnu.linkonce.r.*);
		*(.ARM.extab* .gnu.linkonce.armextab.*);
		*(.eh_frame_hdr);
		*(.eh_frame_entry .eh_frame_entry.*);
		KEEP(*(.eh_frame));
		*(.eh_frame.*);
		*(.gcc_except_table .gcc_except_table.*);
		*(.gnu_extab*);

		/* sub-section: data_initializers */

		. = ALIGN(4);
		PROVIDE(__data_initializers_start = .);

		LONG(LOADADDR(.data)); LONG(ADDR(.data)); LONG(ADDR(.data) + SIZEOF(.data));
		LONG(LOADADDR(.SRAM1.data)); LONG(ADDR(.SRAM1.data)); LONG(ADDR(.SRAM1.data) + SIZEOF(.SRAM1.data));

		. = ALIGN(4);
		PROVIDE(__data_initializers_end = .);

		/* end of sub-section: data_initializers */

		/* sub-section: bss_initializers */

		. = ALIGN(4);
		PROVIDE(__bss_initializers_start = .);

		LONG(0); LONG(ADDR(.bss)); LONG(ADDR(.bss) + SIZEOF(.bss));
		LONG(0xed419f25); LONG(ADDR(.main_stack)); LONG(ADDR(.main_stack) + SIZEOF(.main_stack));
		LONG(0xed419f25); LONG(ADDR(.process_stack)); LONG(ADDR(.process_stack) + SIZEOF(.process_stack));
		LONG(0); LONG(ADDR(.SRAM1.bss)); LONG(ADDR(.SRAM1.bss) + SIZEOF(.SRAM1.bss));

		. = ALIGN(4);
		PROVIDE(__bss_initializers_end = .);

		/* end of sub-section: bss_initializers */

		/* sub-sections: low_level_preinitializers, low_level_initializers */

		. = ALIGN(4);
		PROVIDE(__low_level_preinitializers_start = .);

		KEEP(*(SORT(.low_level_preinitializers.*)));

		. = ALIGN(4);
		PROVIDE(__low_level_preinitializers_end = .);

		. = ALIGN(4);
		PROVIDE(__low_level_initializers_start = .);

		KEEP(*(SORT(.low_level_initializers.*)));

		. = ALIGN(4);
		PROVIDE(__low_level_initializers_end = .);

		/* end of sub-sections: low_level_preinitializers, low_level_initializers */

		/* sub-sections: init, preinit_array, init_array and fini_array */

		KEEP(*(SORT_NONE(.init)));

		PROVIDE(__preinit_array_start = .);

		KEEP(*(.preinit_array));

		PROVIDE(__preinit_array_end = .);

		PROVIDE(__init_array_start = .);

		KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)));
		KEEP(*(.init_array));

		PROVIDE(__init_array_end = .);

		KEEP(*(SORT_NONE(.fini)));

#if DISTORTOS_STATIC_DESTRUCTORS_ENABLE == 1

		PROVIDE(__fini_array_start = .);

		KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)));
		KEEP(*(.fini_array));

		PROVIDE(__fini_array_end = .);

#endif	/* DISTORTOS_STATIC_DESTRUCTORS_ENABLE == 1 */

		/* end of sub-sections: init, preinit_array, init_array and fini_array */

		PROVIDE(__text_end = .);
	} > textVmaRegion AT > textLmaRegion

	.ARM.exidx :
	{
		PROVIDE(__exidx_start = .);

		*(.ARM.exidx* .gnu.linkonce.armexidx.*);	/* index entries for section unwinding */

		PROVIDE(__exidx_end = .);
	} > armExidxVmaRegion AT > armExidxLmaRegion

	.main_stack :
	{
		. = ALIGN(8);
		PROVIDE(__main_stack_start = .);

		. += __main_stack_size;

		. = ALIGN(8);
		PROVIDE(__main_stack_end = .);
	} > mainStackVmaRegion AT > mainStackLmaRegion

	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		PROVIDE(__bss_start = .);

		*(.bss .bss.* .gnu.linkonce.b.*);
		*(COMMON);

		. = ALIGN(4);
		PROVIDE(__bss_end = .);
	} > bssVmaRegion AT > bssLmaRegion

	.data :
	{
		. = ALIGN(4);
		PROVIDE(__data_init_start = LOADADDR(.data));
		PROVIDE(__data_start = .);

		*(.data .data.* .gnu.linkonce.d.*);

		. = ALIGN(4);
		PROVIDE(__data_end = .);
	} > dataVmaRegion AT > dataLmaRegion

	.noinit (NOLOAD) :
	{
		PROVIDE(__noinit_start = .);

		*(.noinit);

		PROVIDE(__noinit_end = .);
	} > noinitVmaRegion AT > noinitLmaRegion

	.SRAM1.bss (NOLOAD) :
	{
		. = ALIGN(4);
		PROVIDE(__SRAM1_bss_start = .);

		*(.SRAM1.bss);

		. = ALIGN(4);
		PROVIDE(__SRAM1_bss_end = .);
	} > SRAM1 AT > SRAM1

	.SRAM1.data :
	{
		. = ALIGN(4);
		PROVIDE(__SRAM1_data_init_start = LOADADDR(.SRAM1.data));
		PROVIDE(__SRAM1_data_start = .);

		*(.SRAM1.data);

		. = ALIGN(4);
		PROVIDE(__SRAM1_data_end = .);
	} > SRAM1 AT > SRAM1DataLmaRegion

	.SRAM1.noinit (NOLOAD) :
	{
		PROVIDE(__SRAM1_noinit_start = .);

		*(.SRAM1.noinit);

		PROVIDE(__SRAM1_noinit_end = .);
	} > SRAM1 AT > SRAM1

	.process_stack __process_stack_start : AT(__process_stack_start)
	{
		. += __process_stack_size;
	}

	/* Stabs debugging sections. */
	.stab 0 : { *(.stab); }
	.stabstr 0 : { *(.stabstr); }
	.stab.excl 0 : { *(.stab.excl); }
	.stab.exclstr 0 : { *(.stab.exclstr); }
	.stab.index 0 : { *(.stab.index); }
	.stab.indexstr 0 : { *(.stab.indexstr); }
	.comment 0 : { *(.comment); }
	/* DWARF debug sections. */
	/* Symbols in the DWARF debugging sections are relative to the beginning of the section so we begin them at 0. */
	/* DWARF 1 */
	.debug 0 : { *(.debug); }
	.line 0 : { *(.line); }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo 0 : { *(.debug_srcinfo); }
	.debug_sfnames 0 : { *(.debug_sfnames); }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges 0 : { *(.debug_aranges); }
	.debug_pubnames 0 : { *(.debug_pubnames); }
	/* DWARF 2 */
	.debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*); }
	.debug_abbrev 0 : { *(.debug_abbrev); }
	.debug_line 0 : { *(.debug_line .debug_line.* .debug_line_end); }
	.debug_frame 0 : { *(.debug_frame); }
	.debug_str 0 : { *(.debug_str); }
	.debug_loc 0 : { *(.debug_loc); }
	.debug_macinfo 0 : { *(.debug_macinfo); }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames); }
	.debug_funcnames 0 : { *(.debug_funcnames); }
	.debug_typenames 0 : { *(.debug_typenames); }
	.debug_varnames 0 : { *(.debug_varnames); }
	/* DWARF 3 */
	.debug_pubtypes 0 : { *(.debug_pubtypes); }
	.debug_ranges 0 : { *(.debug_ranges); }
	/* DWARF Extension. */
	.debug_macro 0 : { *(.debug_macro); }
	.debug_addr 0 : { *(.debug_addr); }
	.ARM.attributes 0 : { KEEP(*(.ARM.attributes)); KEEP(*(.gnu.attributes)); }
	.note.gnu.arm.ident	0 : { KEEP(*(.note.gnu.arm.ident)); }
	/DISCARD/ : { *(.note.GNU-stack); *(.gnu_debuglink); *(.gnu.lto_*); }
}

#ifdef DISTORTOS_LD_HEAP_REGION_SRAM1
PROVIDE(__heap_start = ALIGN(__SRAM1_noinit_end, 8));
#ifdef DISTORTOS_LD_PROCESS_STACK_REGION_SRAM1
PROVIDE(__heap_end = __process_stack_start / 8 * 8);
#else	/* !def DISTORTOS_LD_PROCESS_STACK_REGION_SRAM1 */
PROVIDE(__heap_end = __SRAM1_end / 8 * 8);
#endif	/* !def DISTORTOS_LD_PROCESS_STACK_REGION_SRAM1 */
#endif	/* def DISTORTOS_LD_HEAP_REGION_SRAM1 */

PROVIDE(__text_vectorTable_size = SIZEOF(.text.vectorTable));
PROVIDE(__text_size = SIZEOF(.text));
PROVIDE(__exidx_size = SIZEOF(.ARM.exidx));
PROVIDE(__bss_size = SIZEOF(.bss));
PROVIDE(__data_size = SIZEOF(.data));
PROVIDE(__noinit_size = SIZEOF(.noinit));
PROVIDE(__SRAM1_bss_size = SIZEOF(.SRAM1.bss));
PROVIDE(__SRAM1_data_size = SIZEOF(.SRAM1.data));
PROVIDE(__SRAM1_noinit_size = SIZEOF(.SRAM1.noinit));

PROVIDE(__bss_start__ = __bss_start);
PROVIDE(__bss_end__ = __bss_end);
