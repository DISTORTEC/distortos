#!/bin/sh

#
# file: ARMv7-M.ld.sh
#
# author: Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set -e
set -u

if [ $# -lt 5 ]; then
	echo "This script requires at least 5 arguments!" >&2
	exit 1
fi

chipName=$1
romDescription=$2
ramDescription=$3
mainStackSize=$4
processStackSize=$5

decimalOrHexadecimalRegex='\(\(0x[0-9a-fA-F]\{1,8\}\)\|\([0-9]\{1,9\}\)\)'
addressRegex="\($decimalOrHexadecimalRegex\)"
sizeRegex="\($decimalOrHexadecimalRegex[kM]\?\)"

if ! expr "$romDescription" : "\($addressRegex,$sizeRegex\)$" > /dev/null; then
	echo "Invalid description of ROM - \"$romDescription\"!" >&2
	exit 2
fi

if ! expr "$ramDescription" : "\($addressRegex,$sizeRegex\)$" > /dev/null; then
	echo "Invalid description of RAM - \"$ramDescription\"!" >&2
	exit 3
fi

if ! expr "$mainStackSize" : "\($sizeRegex\)$" > /dev/null; then
	echo "Invalid size of main stack - \"$mainStackSize\"!" >&2
	exit 4
fi

if ! expr "$processStackSize" : "\($sizeRegex\)$" > /dev/null; then
	echo "Invalid size of process stack - \"$processStackSize\"!" >&2
	exit 5
fi

# the matched string may (technically) be "0" - that's why " || true" is needed
romAddress=`expr "$romDescription" : '\([^,]\+\),[^,]\+' || true`
romSize=`expr "$romDescription" : '[^,]\+,\([^,]\+\)' || true`
ramAddress=`expr "$ramDescription" : '\([^,]\+\),[^,]\+' || true`
ramSize=`expr "$ramDescription" : '[^,]\+,\([^,]\+\)' || true`

shift
shift
shift
shift
shift

headerComments=
memoryEntries=
memorySizes=
dataArrayEntries=
bssArrayEntries=
sectionEntries=
sectionSizes=

while [ $# -gt 0 ]; do

	if ! expr "$1" : "\([^,]\+,$addressRegex,$sizeRegex\)$" > /dev/null; then
		echo "Invalid description of memory - \"$1\"!" >&2
		exit 6
	fi

	memoryName=`expr "$1" : '\([^,]\+\),[^,]\+,[^,]\+' || true`
	memoryAddress=`expr "$1" : '[^,]\+,\([^,]\+\),[^,]\+' || true`
	memorySize=`expr "$1" : '[^,]\+,[^,]\+,\([^,]\+\)' || true`

	headerComments="$headerComments * - $memorySize bytes of $memoryName;\n"

	memoryEntries="$memoryEntries\t$memoryName : org = $memoryAddress, len = $memorySize\n"

	memorySizes="${memorySizes}\
PROVIDE(__${memoryName}_start = ORIGIN(${memoryName}));
PROVIDE(__${memoryName}_size = LENGTH(${memoryName}));
PROVIDE(__${memoryName}_end = __${memoryName}_start + __${memoryName}_size);\n\n"

	dataArrayEntries="$dataArrayEntries\
\t\tLONG(LOADADDR(.${memoryName}.data)); LONG(ADDR(.${memoryName}.data)); \
LONG(ADDR(.${memoryName}.data) + SIZEOF(.${memoryName}.data));\n"

	bssArrayEntries="$bssArrayEntries\
\t\tLONG(ADDR(.${memoryName}.bss)); LONG(ADDR(.${memoryName}.bss) + SIZEOF(.${memoryName}.bss));\n"

	sectionEntries="$sectionEntries\
	.${memoryName}.bss :
	{
		. = ALIGN(4);
		PROVIDE(__${memoryName}_bss_start = .);

		*(.${memoryName}.bss)

		. = ALIGN(4);
		PROVIDE(__${memoryName}_bss_end = .);
	} > ${memoryName} AT > ${memoryName}

	.${memoryName}.data :
	{
		. = ALIGN(4);
		PROVIDE(__${memoryName}_data_init_start = LOADADDR(.${memoryName}.data));
		PROVIDE(__${memoryName}_data_start = .);

		*(.${memoryName}.data)

		. = ALIGN(4);
		PROVIDE(__${memoryName}_data_end = .);
	} > ${memoryName} AT > rom

	.${memoryName}.noinit :
	{
		. = ALIGN(4);
		PROVIDE(__${memoryName}_noinit_start = .);

		*(.${memoryName}.noinit)

		. = ALIGN(4);
		PROVIDE(__${memoryName}_noinit_end = .);
	} > ${memoryName} AT > ${memoryName}\n\n"

	sectionSizes="${sectionSizes}\
PROVIDE(__${memoryName}_bss_size = __${memoryName}_bss_end - __${memoryName}_bss_start);
PROVIDE(__${memoryName}_data_size = __${memoryName}_data_end - __${memoryName}_data_start);
PROVIDE(__${memoryName}_noinit_size = __${memoryName}_noinit_end - __${memoryName}_noinit_start);\n"

	shift
done

cat<<EOF
/**
 * \file
 * \brief Linker script for $chipName chip:
 * - $romSize bytes of rom;
 * - $ramSize bytes of ram;
EOF

printf "%b" "$headerComments"

cat<<EOF
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
PROVIDE(__main_stack_size = $mainStackSize);

/* Thread mode can use main stack (default after reset) or process stack - selected in CONTROL special register */
PROVIDE(__process_stack_size = $processStackSize);

/*---------------------------------------------------------------------------------------------------------------------+
| available memories
+---------------------------------------------------------------------------------------------------------------------*/

MEMORY
{
	rom : org = $romAddress, len = $romSize
	ram : org = $ramAddress, len = $ramSize
EOF

printf "%b" "$memoryEntries"

cat<<EOF
}

PROVIDE(__rom_start = ORIGIN(rom));
PROVIDE(__rom_size = LENGTH(rom));
PROVIDE(__rom_end = __rom_start + __rom_size);

PROVIDE(__ram_start = ORIGIN(ram));
PROVIDE(__ram_size = LENGTH(ram));
PROVIDE(__ram_end = __ram_start + __ram_size);

EOF

printf "%b" "$memorySizes"

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
		PROVIDE(__text_start = .);

		/* sub-section: .vectors */

		. = ALIGN(4);
		PROVIDE(__vectors_start = .);

		KEEP(*(.coreVectors));
		KEEP(*(.chipVectors));

		. = ALIGN(4);
		PROVIDE(__vectors_end = .);

		/* end of sub-section: .vectors */

		*(.text* .gnu.linkonce.t.*);
		*(.rodata* .gnu.linkonce.r.*);
		*(.glue_7t .glue_7);

		*(.ARM.extab* .gnu.linkonce.armextab.*);	/* exception unwinding information */
		*(.gcc_except_table);						/* information used for stack unwinding during exception */
		*(.eh_frame_hdr);							/* additional information about .ex_frame section */
		*(.eh_frame);								/* information used for stack unwinding during exception */

		/* sub-section: data_array */

		. = ALIGN(4);
		PROVIDE(__data_array_start = .);

		LONG(LOADADDR(.data)); LONG(ADDR(.data)); LONG(ADDR(.data) + SIZEOF(.data));
EOF

printf "%b" "$dataArrayEntries"

cat<<EOF

		. = ALIGN(4);
		PROVIDE(__data_array_end = .);

		/* end of sub-section: data_array */

		/* sub-section: bss_array */

		. = ALIGN(4);
		PROVIDE(__bss_array_start = .);

		LONG(ADDR(.bss)); LONG(ADDR(.bss) + SIZEOF(.bss));
		LONG(ADDR(.stack)); LONG(ADDR(.stack) + SIZEOF(.stack));
EOF

printf "%b" "$bssArrayEntries"

cat<<EOF

		. = ALIGN(4);
		PROVIDE(__bss_array_end = .);

		/* end of sub-section: bss_array */

		/* sub-sections: init, preinit_array, init_array and fini_array */

		KEEP(*(.init));

		. = ALIGN(4);
		PROVIDE(__preinit_array_start = .);

		KEEP(*(.preinit_array));

		. = ALIGN(4);
		PROVIDE(__preinit_array_end = .);

		. = ALIGN(4);
		PROVIDE(__init_array_start = .);

		KEEP(*(SORT(.init_array.*)));
		KEEP(*(.init_array));

		. = ALIGN(4);
		PROVIDE(__init_array_end = .);

		KEEP(*(.fini));

		. = ALIGN(4);
		PROVIDE(__fini_array_start = .);

		KEEP(*(.fini_array));
		KEEP(*(SORT(.fini_array.*)));

		. = ALIGN(4);
		PROVIDE(__fini_array_end = .);

		/* end of sub-sections: init, preinit_array, init_array and fini_array */

		. = ALIGN(4);
		PROVIDE(__text_end = .);
	} > rom AT > rom

	. = ALIGN(4);
	PROVIDE(__exidx_start = .);

	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*);
	} > rom AT > rom								/* index entries for section unwinding */

	. = ALIGN(4);
	PROVIDE(__exidx_end = .);

	.data :
	{
		. = ALIGN(4);
		PROVIDE(__data_init_start = LOADADDR(.data));
		PROVIDE(__data_start = .);

		*(.data* .gnu.linkonce.d.*)

		. = ALIGN(4);
		PROVIDE(__data_end = .);
	} > ram AT > rom

	.bss :
	{
		. = ALIGN(4);
		PROVIDE(__bss_start = .);

		*(.bss* .gnu.linkonce.b.*)
		*(COMMON);

		. = ALIGN(4);
		PROVIDE(__bss_end = .);
	} > ram AT > ram

	.stack :
	{
		. = ALIGN(8);
		PROVIDE(__stack_start = .);
		PROVIDE(__main_stack_start = .);

		. += __main_stack_size;

		. = ALIGN(8);
		PROVIDE(__main_stack_end = .);
		PROVIDE(__process_stack_start = .);

		. += __process_stack_size;

		. = ALIGN(8);
		PROVIDE(__process_stack_end = .);
		PROVIDE(__stack_end = .);
	} > ram AT > ram

	. = ALIGN(4);
	PROVIDE(__heap_start = .);
	PROVIDE(__heap_end = __ram_end);

EOF

printf "%b" "$sectionEntries"

cat<<EOF
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
PROVIDE(__data_array_size = __data_array_end - __data_array_start);
PROVIDE(__bss_array_size = __bss_array_end - __bss_array_start);
PROVIDE(__exidx_size = __exidx_end - __exidx_start);
PROVIDE(__data_size = __data_end - __data_start);
PROVIDE(__bss_size = __bss_end - __bss_start);
PROVIDE(__stack_size = __stack_end - __stack_start);
PROVIDE(__heap_size = __heap_end - __heap_start);
EOF

printf "%b" "$sectionSizes"

cat<<EOF

PROVIDE(__bss_start__ = __bss_start);
PROVIDE(__bss_end__ = __bss_end);
EOF
