/**
 * \file
 * \brief Linker script for STM32F4xxxG chip (1MB Flash, 112kB SRAM, 16kB aux SRAM, 64kB CCM RAM and 4kB backup SRAM).
 * Only main block o SRAM (112kB) is used.
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-21
 */

SEARCH_DIR(.);

/*---------------------------------------------------------------------------------------------------------------------+
| available memories definitions
+---------------------------------------------------------------------------------------------------------------------*/

MEMORY
{
	rom (rx)		: org = 0x08000000, len = 1M
	ram (rwx)		: org = 0x20000000, len = 112k
	aux_ram (rwx)	: org = 0x2001C000, len = 16k
	ccm_ram (rw)	: org = 0x10000000, len = 64k
	bkp_ram (rwx)	: org = 0x40024000, len = 4k
}

/*---------------------------------------------------------------------------------------------------------------------+
| stacks sizes
+---------------------------------------------------------------------------------------------------------------------*/

/* Handler mode (core exceptions / interrupts) can use only main stack */
/* Thread mode can use main stack (default) or process stack - selected in CONTROL special register */

__main_stack_size = 2k;
__process_stack_size = 2k;

/*---------------------------------------------------------------------------------------------------------------------+
| include generic linker script
+---------------------------------------------------------------------------------------------------------------------*/

INCLUDE ARMv7-M.ld
