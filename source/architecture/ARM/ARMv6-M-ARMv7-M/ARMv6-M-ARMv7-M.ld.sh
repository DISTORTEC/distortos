#!/bin/sh

#
# file: ARMv6-M-ARMv7-M.ld.sh
#
# author: Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

if [ ${#} -lt 3 ]; then
	echo 'This script requires at least 3 arguments!' >&2
	exit 1
fi

chipName="${1}"
romDescription="${2}"
ramDescription="${3}"

decimalOrHexadecimalRegex='\(\(0x[0-9a-fA-F]\{1,8\}\)\|\([0-9]\{1,9\}\)\)'
addressRegex="\(${decimalOrHexadecimalRegex}\)"
sizeRegex="\(${decimalOrHexadecimalRegex}[kM]\?\)"

if ! expr "${romDescription}" : "\(${addressRegex},${sizeRegex}\)$" > /dev/null; then
	echo "Invalid description of ROM - \"${romDescription}\"!" >&2
	exit 2
fi

if ! expr "${ramDescription}" : "\(${addressRegex},${sizeRegex}\)$" > /dev/null; then
	echo "Invalid description of RAM - \"${ramDescription}\"!" >&2
	exit 3
fi

# the matched string may (technically) be "0" - that's why " || true" is needed
romAddress="$(expr "${romDescription}" : '\([^,]\+\),[^,]\+' || true)"
romSize="$(expr "${romDescription}" : '[^,]\+,\([^,]\+\)' || true)"
ramAddress="$(expr "${ramDescription}" : '\([^,]\+\),[^,]\+' || true)"
ramSize="$(expr "${ramDescription}" : '[^,]\+,\([^,]\+\)' || true)"

shift
shift
shift

headerComments=''
memoryEntries=''
memorySizes=''
dataArrayEntries=''
bssArrayEntries=''
sectionEntries=''
sectionSizes=''

while [ ${#} -gt 0 ]; do

	if ! expr "${1}" : "\([^,]\+,${addressRegex},${sizeRegex}\)$" > /dev/null; then
		echo "Invalid description of memory - \"${1}\"!" >&2
		exit 4
	fi

	memoryName="$(expr "${1}" : '\([^,]\+\),[^,]\+,[^,]\+' || true)"
	memoryAddress="$(expr "${1}" : '[^,]\+,\([^,]\+\),[^,]\+' || true)"
	memorySize="$(expr "${1}" : '[^,]\+,[^,]\+,\([^,]\+\)' || true)"

	headerComments="${headerComments} * - ${memorySize} bytes of ${memoryName} at ${memoryAddress};\n"

	memoryEntries="${memoryEntries}\t${memoryName} : org = ${memoryAddress}, len = ${memorySize}\n"

	memorySizes="${memorySizes}\
PROVIDE(__${memoryName}_start = ORIGIN(${memoryName}));
PROVIDE(__${memoryName}_size = LENGTH(${memoryName}));
PROVIDE(__${memoryName}_end = ORIGIN(${memoryName}) + LENGTH(${memoryName}));\n\n"

	dataArrayEntries="${dataArrayEntries}\
\t\tLONG(LOADADDR(.${memoryName}.data)); LONG(ADDR(.${memoryName}.data)); \
LONG(ADDR(.${memoryName}.data) + SIZEOF(.${memoryName}.data));\n"

	bssArrayEntries="${bssArrayEntries}\
\t\tLONG(0); LONG(ADDR(.${memoryName}.bss)); LONG(ADDR(.${memoryName}.bss) + SIZEOF(.${memoryName}.bss));\n"

	sectionEntries="${sectionEntries}\
	.${memoryName}.bss :
	{
		PROVIDE(__${memoryName}_bss_start = .);

		*(.${memoryName}.bss);

		PROVIDE(__${memoryName}_bss_end = .);
	} > ${memoryName} AT > ${memoryName}

	.${memoryName}.data :
	{
		PROVIDE(__${memoryName}_data_init_start = LOADADDR(.${memoryName}.data));
		PROVIDE(__${memoryName}_data_start = .);

		*(.${memoryName}.data);

		PROVIDE(__${memoryName}_data_end = .);
	} > ${memoryName} AT > rom

	.${memoryName}.noinit (NOLOAD) :
	{
		PROVIDE(__${memoryName}_noinit_start = .);

		*(.${memoryName}.noinit);

		PROVIDE(__${memoryName}_noinit_end = .);
	} > ${memoryName} AT > ${memoryName}\n\n"

	sectionSizes="${sectionSizes}\
PROVIDE(__${memoryName}_bss_size = SIZEOF(.${memoryName}.bss));
PROVIDE(__${memoryName}_data_size = SIZEOF(.${memoryName}.data));
PROVIDE(__${memoryName}_noinit_size = SIZEOF(.${memoryName}.noinit));\n"

	shift
done

cat << EOF
/**
 * \file
 * \brief Linker script for ${chipName} chip:
 * - ${romSize} bytes of rom at ${romAddress};
 * - ${ramSize} bytes of ram at ${ramAddress};
$(printf '%b' "${headerComments}")
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 *
 * \date $(date +'%Y-%m-%d %H:%M:%S')
 */

#include "distortos/distortosConfiguration.h"

SEARCH_DIR(.);
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm");
OUTPUT_ARCH(arm);

/*---------------------------------------------------------------------------------------------------------------------+
| stacks sizes
+---------------------------------------------------------------------------------------------------------------------*/

PROVIDE(__main_stack_size =
		ALIGN(CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE, CONFIG_ARCHITECTURE_STACK_ALIGNMENT));

PROVIDE(__process_stack_size = ALIGN(CONFIG_MAIN_THREAD_STACK_SIZE, CONFIG_ARCHITECTURE_STACK_ALIGNMENT) +
		ALIGN(CONFIG_STACK_GUARD_SIZE, CONFIG_ARCHITECTURE_STACK_ALIGNMENT));

/*---------------------------------------------------------------------------------------------------------------------+
| available memories
+---------------------------------------------------------------------------------------------------------------------*/

MEMORY
{
	rom : org = ${romAddress}, len = ${romSize}
	ram : org = ${ramAddress}, len = ${ramSize}
$(printf '%b' "${memoryEntries}")
}

PROVIDE(__rom_start = ORIGIN(rom));
PROVIDE(__rom_size = LENGTH(rom));
PROVIDE(__rom_end = ORIGIN(rom) + LENGTH(rom));

PROVIDE(__ram_start = ORIGIN(ram));
PROVIDE(__ram_size = LENGTH(ram));
PROVIDE(__ram_end = ORIGIN(ram) + LENGTH(ram));

$(printf '%b' "${memorySizes}")

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
		PROVIDE(__text_start = .);

		/* sub-section: .vectors */

		PROVIDE(__vectors_start = .);

		KEEP(*(.coreVectors));
		KEEP(*(.chipVectors));

		PROVIDE(__vectors_end = .);

		/* end of sub-section: .vectors */

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

		/* sub-section: data_array */

		. = ALIGN(4);
		PROVIDE(__data_array_start = .);

		LONG(LOADADDR(.data)); LONG(ADDR(.data)); LONG(ADDR(.data) + SIZEOF(.data));
$(printf '%b' "${dataArrayEntries}")

		. = ALIGN(4);
		PROVIDE(__data_array_end = .);

		/* end of sub-section: data_array */

		/* sub-section: bss_array */

		. = ALIGN(4);
		PROVIDE(__bss_array_start = .);

		LONG(0); LONG(ADDR(.bss)); LONG(ADDR(.bss) + SIZEOF(.bss));
		LONG(0xed419f25); LONG(ADDR(.main_stack)); LONG(ADDR(.main_stack) + SIZEOF(.main_stack));
		LONG(0xed419f25); LONG(ADDR(.process_stack)); LONG(ADDR(.process_stack) + SIZEOF(.process_stack));
$(printf '%b' "${bssArrayEntries}")

		. = ALIGN(4);
		PROVIDE(__bss_array_end = .);

		/* end of sub-section: bss_array */

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

#if CONFIG_STATIC_DESTRUCTORS_ENABLE == 1

		PROVIDE(__fini_array_start = .);

		KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)));
		KEEP(*(.fini_array));

		PROVIDE(__fini_array_end = .);

#endif	/* CONFIG_STATIC_DESTRUCTORS_ENABLE == 1 */

		/* end of sub-sections: init, preinit_array, init_array and fini_array */

		PROVIDE(__text_end = .);
	} > rom AT > rom

	.ARM.exidx :
	{
		PROVIDE(__exidx_start = .);

		*(.ARM.exidx* .gnu.linkonce.armexidx.*);

		PROVIDE(__exidx_end = .);
	} > rom AT > rom								/* index entries for section unwinding */

	.main_stack :
	{
		. = ALIGN(8);
		PROVIDE(__main_stack_start = .);

		. += __main_stack_size;

		. = ALIGN(8);
		PROVIDE(__main_stack_end = .);
	} > ram AT > ram

	.bss :
	{
		PROVIDE(__bss_start = .);

		*(.bss .bss.* .gnu.linkonce.b.*);
		*(COMMON);

		PROVIDE(__bss_end = .);
	} > ram AT > ram

	.data :
	{
		PROVIDE(__data_init_start = LOADADDR(.data));
		PROVIDE(__data_start = .);

		*(.data .data.* .gnu.linkonce.d.*);

		PROVIDE(__data_end = .);
	} > ram AT > rom

	.noinit (NOLOAD) :
	{
		PROVIDE(__noinit_start = .);

		*(.noinit);

		PROVIDE(__noinit_end = .);
	} > ram AT > ram

	. = ALIGN(8);
	PROVIDE(__heap_start = .);
	. = __ram_end - __process_stack_size;
	PROVIDE(__heap_end = .);

	.process_stack . :
	{
		. = ALIGN(8);
		PROVIDE(__process_stack_start = .);

		. += __process_stack_size;

		. = ALIGN(8);
		PROVIDE(__process_stack_end = .);
	} > ram AT > ram

$(printf '%b' "${sectionEntries}")

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

PROVIDE(__text_size = SIZEOF(.text));
PROVIDE(__exidx_size = SIZEOF(.ARM.exidx));
PROVIDE(__bss_size = SIZEOF(.bss));
PROVIDE(__data_size = SIZEOF(.data));
PROVIDE(__noinit_size = SIZEOF(.noinit));
$(printf '%b' "${sectionSizes}")

PROVIDE(__bss_start__ = __bss_start);
PROVIDE(__bss_end__ = __bss_end);
EOF
