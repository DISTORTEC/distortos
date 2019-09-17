/**
 * \file
 * \brief Linker script for ST,32F429IDISCOVERY (ST,STM32F429ZI chip):
 * - 2.0 MiB of flash at 0x8000000;
 * - 192.0 KiB of SRAM at 0x20000000;
 * - 64.0 KiB of CCM at 0x10000000;
 * - 4.0 KiB of BKPSRAM at 0x40024000;
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

__main_stack_size = ALIGN(DISTORTOS_ARCHITECTURE_MAIN_STACK_SIZE, DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT);

__process_stack_size = ALIGN(DISTORTOS_MAIN_THREAD_STACK_SIZE, DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT) +
		ALIGN(DISTORTOS_STACK_GUARD_SIZE, DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT);

/*---------------------------------------------------------------------------------------------------------------------+
| available memories
+---------------------------------------------------------------------------------------------------------------------*/

MEMORY
{
	flash : org = 0x8000000, len = 2097152
	SRAM : org = 0x20000000, len = 196608
	CCM : org = 0x10000000, len = 65536
	BKPSRAM : org = 0x40024000, len = 4096
}

__flash_start = ORIGIN(flash);
__flash_size = LENGTH(flash);
__flash_end = __flash_start + __flash_size;

__SRAM_start = ORIGIN(SRAM);
__SRAM_size = LENGTH(SRAM);
__SRAM_end = __SRAM_start + __SRAM_size;

__CCM_start = ORIGIN(CCM);
__CCM_size = LENGTH(CCM);
__CCM_end = __CCM_start + __CCM_size;

__BKPSRAM_start = ORIGIN(BKPSRAM);
__BKPSRAM_size = LENGTH(BKPSRAM);
__BKPSRAM_end = __BKPSRAM_start + __BKPSRAM_size;

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

#ifdef DISTORTOS_LD_MAIN_STACK_REGION_SRAM
REGION_ALIAS("mainStackVmaRegion", SRAM);
REGION_ALIAS("mainStackLmaRegion", SRAM);
#endif	/* DISTORTOS_LD_MAIN_STACK_REGION_SRAM */

#ifdef DISTORTOS_LD_MAIN_STACK_REGION_CCM
REGION_ALIAS("mainStackVmaRegion", CCM);
REGION_ALIAS("mainStackLmaRegion", CCM);
#endif	/* DISTORTOS_LD_MAIN_STACK_REGION_CCM */

#ifdef DISTORTOS_LD_BSS_REGION_SRAM
REGION_ALIAS("bssVmaRegion", SRAM);
REGION_ALIAS("bssLmaRegion", SRAM);
#endif	/* DISTORTOS_LD_BSS_REGION_SRAM */

#ifdef DISTORTOS_LD_BSS_REGION_CCM
REGION_ALIAS("bssVmaRegion", CCM);
REGION_ALIAS("bssLmaRegion", CCM);
#endif	/* DISTORTOS_LD_BSS_REGION_CCM */

#ifdef DISTORTOS_LD_DATA_VMA_REGION_SRAM
REGION_ALIAS("dataVmaRegion", SRAM);
#endif	/* DISTORTOS_LD_DATA_VMA_REGION_SRAM */

#ifdef DISTORTOS_LD_DATA_VMA_REGION_CCM
REGION_ALIAS("dataVmaRegion", CCM);
#endif	/* DISTORTOS_LD_DATA_VMA_REGION_CCM */

#ifdef DISTORTOS_LD_DATA_LMA_REGION_FLASH
REGION_ALIAS("dataLmaRegion", flash);
#endif	/* DISTORTOS_LD_DATA_LMA_REGION_FLASH */

#ifdef DISTORTOS_LD_NOINIT_REGION_SRAM
REGION_ALIAS("noinitVmaRegion", SRAM);
REGION_ALIAS("noinitLmaRegion", SRAM);
#endif	/* DISTORTOS_LD_NOINIT_REGION_SRAM */

#ifdef DISTORTOS_LD_NOINIT_REGION_CCM
REGION_ALIAS("noinitVmaRegion", CCM);
REGION_ALIAS("noinitLmaRegion", CCM);
#endif	/* DISTORTOS_LD_NOINIT_REGION_CCM */

#ifdef DISTORTOS_LD_NOINIT_REGION_BKPSRAM
REGION_ALIAS("noinitVmaRegion", BKPSRAM);
REGION_ALIAS("noinitLmaRegion", BKPSRAM);
#endif	/* DISTORTOS_LD_NOINIT_REGION_BKPSRAM */

#ifdef DISTORTOS_LD_SRAM_DATA_LMA_REGION_FLASH
REGION_ALIAS("SRAMDataLmaRegion", flash);
#endif	/* DISTORTOS_LD_SRAM_DATA_LMA_REGION_FLASH */

#ifdef DISTORTOS_LD_CCM_DATA_LMA_REGION_FLASH
REGION_ALIAS("CCMDataLmaRegion", flash);
#endif	/* DISTORTOS_LD_CCM_DATA_LMA_REGION_FLASH */

#ifdef DISTORTOS_LD_PROCESS_STACK_REGION_SRAM
__process_stack_end = __SRAM_end / 8 * 8;
#endif	/* def DISTORTOS_LD_PROCESS_STACK_REGION_SRAM */

#ifdef DISTORTOS_LD_PROCESS_STACK_REGION_CCM
__process_stack_end = __CCM_end / 8 * 8;
#endif	/* def DISTORTOS_LD_PROCESS_STACK_REGION_CCM */

__process_stack_start = (__process_stack_end - __process_stack_size) / 8 * 8;

SECTIONS
{
	.text.vectorTable : ALIGN_WITH_INPUT
	{
		__text_vectorTable_start = .;

		KEEP(*(.vectorTable));

		__text_vectorTable_end = .;
	} > textVectorTableVmaRegion AT > textVectorTableLmaRegion

	.text : ALIGN_WITH_INPUT
	{
		__text_start = .;

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
		__data_initializers_start = .;

		LONG(__data_init_start); LONG(__data_start); LONG(__data_end);
		LONG(__SRAM_data_init_start); LONG(__SRAM_data_start); LONG(__SRAM_data_end);
		LONG(__CCM_data_init_start); LONG(__CCM_data_start); LONG(__CCM_data_end);

		. = ALIGN(4);
		__data_initializers_end = .;

		/* end of sub-section: data_initializers */

		/* sub-section: bss_initializers */

		. = ALIGN(4);
		__bss_initializers_start = .;

		LONG(0); LONG(__bss_start); LONG(__bss_end);
		LONG(0xed419f25); LONG(__main_stack_start); LONG(__main_stack_end);
		LONG(0xed419f25); LONG(__process_stack_start); LONG(__process_stack_end);
		LONG(0); LONG(__SRAM_bss_start); LONG(__SRAM_bss_end);
		LONG(0); LONG(__CCM_bss_start); LONG(__CCM_bss_end);

		. = ALIGN(4);
		__bss_initializers_end = .;

		/* end of sub-section: bss_initializers */

		/* sub-sections: low_level_preinitializers, low_level_initializers */

		. = ALIGN(4);
		__low_level_preinitializers_start = .;

		KEEP(*(SORT(.low_level_preinitializers.*)));

		. = ALIGN(4);
		__low_level_preinitializers_end = .;

		. = ALIGN(4);
		__low_level_initializers_start = .;

		KEEP(*(SORT(.low_level_initializers.*)));

		. = ALIGN(4);
		__low_level_initializers_end = .;

		/* end of sub-sections: low_level_preinitializers, low_level_initializers */

		/* sub-sections: init, preinit_array, init_array and fini_array */

		KEEP(*(SORT_NONE(.init)));

		__preinit_array_start = .;

		KEEP(*(.preinit_array));

		__preinit_array_end = .;

		__init_array_start = .;

		KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)));
		KEEP(*(.init_array));

		__init_array_end = .;

		KEEP(*(SORT_NONE(.fini)));

#if DISTORTOS_STATIC_DESTRUCTORS_ENABLE == 1

		__fini_array_start = .;

		KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)));
		KEEP(*(.fini_array));

		__fini_array_end = .;

#endif	/* DISTORTOS_STATIC_DESTRUCTORS_ENABLE == 1 */

		/* end of sub-sections: init, preinit_array, init_array and fini_array */

		__text_end = .;
	} > textVmaRegion AT > textLmaRegion

	.ARM.exidx : ALIGN_WITH_INPUT
	{
		__exidx_start = .;

		*(.ARM.exidx* .gnu.linkonce.armexidx.*);	/* index entries for section unwinding */

		__exidx_end = .;
	} > armExidxVmaRegion AT > armExidxLmaRegion

	.main_stack :
	{
		. = ALIGN(8);
		__main_stack_start = .;

		. += __main_stack_size;

		. = ALIGN(8);
		__main_stack_end = .;
	} > mainStackVmaRegion AT > mainStackLmaRegion

	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		__bss_start = .;

		*(.bss .bss.* .gnu.linkonce.b.*);
		*(COMMON);

		. = ALIGN(4);
		__bss_end = .;
	} > bssVmaRegion AT > bssLmaRegion

	.data :
	{
		. = ALIGN(4);
		__data_init_start = LOADADDR(.data);
		__data_start = .;

		*(.data .data.* .gnu.linkonce.d.*);

		. = ALIGN(4);
		__data_end = .;
	} > dataVmaRegion AT > dataLmaRegion

	.noinit (NOLOAD) :
	{
		. = ALIGN(4);
		__noinit_start = .;

		*(.noinit);

		. = ALIGN(4);
		__noinit_end = .;
	} > noinitVmaRegion AT > noinitLmaRegion

	.SRAM.bss (NOLOAD) :
	{
		. = ALIGN(4);
		__SRAM_bss_start = .;

		*(.SRAM.bss);

		. = ALIGN(4);
		__SRAM_bss_end = .;
	} > SRAM AT > SRAM

	.SRAM.data :
	{
		. = ALIGN(4);
		__SRAM_data_init_start = LOADADDR(.SRAM.data);
		__SRAM_data_start = .;

		*(.SRAM.data);

		. = ALIGN(4);
		__SRAM_data_end = .;
	} > SRAM AT > SRAMDataLmaRegion

	.SRAM.noinit (NOLOAD) :
	{
		. = ALIGN(4);
		__SRAM_noinit_start = .;

		*(.SRAM.noinit);

		. = ALIGN(4);
		__SRAM_noinit_end = .;
	} > SRAM AT > SRAM

	.CCM.bss (NOLOAD) :
	{
		. = ALIGN(4);
		__CCM_bss_start = .;

		*(.CCM.bss);

		. = ALIGN(4);
		__CCM_bss_end = .;
	} > CCM AT > CCM

	.CCM.data :
	{
		. = ALIGN(4);
		__CCM_data_init_start = LOADADDR(.CCM.data);
		__CCM_data_start = .;

		*(.CCM.data);

		. = ALIGN(4);
		__CCM_data_end = .;
	} > CCM AT > CCMDataLmaRegion

	.CCM.noinit (NOLOAD) :
	{
		. = ALIGN(4);
		__CCM_noinit_start = .;

		*(.CCM.noinit);

		. = ALIGN(4);
		__CCM_noinit_end = .;
	} > CCM AT > CCM

	.BKPSRAM.noinit (NOLOAD) :
	{
		. = ALIGN(4);
		__BKPSRAM_noinit_start = .;

		*(.BKPSRAM.noinit);

		. = ALIGN(4);
		__BKPSRAM_noinit_end = .;
	} > BKPSRAM AT > BKPSRAM

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

#ifdef DISTORTOS_LD_HEAP_REGION_SRAM
__heap_start = ALIGN(__SRAM_noinit_end, 8);
#ifdef DISTORTOS_LD_PROCESS_STACK_REGION_SRAM
__heap_end = __process_stack_start / 8 * 8;
#else	/* !def DISTORTOS_LD_PROCESS_STACK_REGION_SRAM */
__heap_end = __SRAM_end / 8 * 8;
#endif	/* !def DISTORTOS_LD_PROCESS_STACK_REGION_SRAM */
#endif	/* def DISTORTOS_LD_HEAP_REGION_SRAM */

#ifdef DISTORTOS_LD_HEAP_REGION_CCM
__heap_start = ALIGN(__CCM_noinit_end, 8);
#ifdef DISTORTOS_LD_PROCESS_STACK_REGION_CCM
__heap_end = __process_stack_start / 8 * 8;
#else	/* !def DISTORTOS_LD_PROCESS_STACK_REGION_CCM */
__heap_end = __CCM_end / 8 * 8;
#endif	/* !def DISTORTOS_LD_PROCESS_STACK_REGION_CCM */
#endif	/* def DISTORTOS_LD_HEAP_REGION_CCM */

__text_vectorTable_size = SIZEOF(.text.vectorTable);
__text_size = SIZEOF(.text);
__exidx_size = SIZEOF(.ARM.exidx);
__bss_size = SIZEOF(.bss);
__data_size = SIZEOF(.data);
__noinit_size = SIZEOF(.noinit);
__SRAM_bss_size = SIZEOF(.SRAM.bss);
__SRAM_data_size = SIZEOF(.SRAM.data);
__SRAM_noinit_size = SIZEOF(.SRAM.noinit);
__CCM_bss_size = SIZEOF(.CCM.bss);
__CCM_data_size = SIZEOF(.CCM.data);
__CCM_noinit_size = SIZEOF(.CCM.noinit);
__BKPSRAM_noinit_size = SIZEOF(.BKPSRAM.noinit);

__bss_start__ = __bss_start;
__bss_end__ = __bss_end;
