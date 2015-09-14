/**
 * \file
 * \brief Header with definitions for bit-banding in ARMv7-M core.
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-14
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV7_M_INCLUDE_DISTORTOS_ARCHITECTURE_ARMV7_M_BIT_BANDING_H_
#define SOURCE_ARCHITECTURE_ARM_ARMV7_M_INCLUDE_DISTORTOS_ARCHITECTURE_ARMV7_M_BIT_BANDING_H_

/*---------------------------------------------------------------------------------------------------------------------+
| global defines
+---------------------------------------------------------------------------------------------------------------------*/

/** beginning of SRAM region which is aliased in bit-band region */
enum { BITBAND_SRAM_BEGIN = 0x20000000 };

/** end of SRAM region which is aliased in bit-band region */
enum { BITBAND_SRAM_END = 0x200fffff };

/** start of bit-band alias region of SRAM */
enum { BITBAND_SRAM_BASE = 0x22000000 };

/** beginning of peripheral region which is aliased in bit-band region */
enum { BITBAND_PERIPHERAL_BEGIN = 0x40000000 };

/** end of peripheral region which is aliased in bit-band region */
enum { BITBAND_PERIPHERAL_END = 0x400fffff };

/** start of bit-band alias region of peripherals */
enum { BITBAND_PERIPHERAL_BASE = 0x42000000 };

/** address of bit-band alias for SRAM region */
#define BITBAND_SRAM_ADDRESS(address, bit) \
	(BITBAND_SRAM_BASE + (((unsigned long)address) - BITBAND_SRAM_BEGIN) * 32 + (bit) * 4)

/** address of bit-band alias for peripheral region */
#define BITBAND_PERIPHERAL_ADDRESS(address, bit) \
	(BITBAND_PERIPHERAL_BASE + (((unsigned long)address) - BITBAND_PERIPHERAL_BEGIN) * 32 + (bit) * 4)

/** address of bit-band alias for any region */
#define BITBAND_ADDRESS(address, bit)		( \
	(((unsigned long)address) >= BITBAND_SRAM_BEGIN) && (((unsigned long)address) <= BITBAND_SRAM_END) ? \
		BITBAND_SRAM_ADDRESS(address, bit) : \
	(((unsigned long)address) >= BITBAND_PERIPHERAL_BEGIN) && (((unsigned long)address) <= BITBAND_PERIPHERAL_END) ? \
		BITBAND_PERIPHERAL_ADDRESS(address, bit) : \
	0 /* fail */ \
	)

/** bit-band alias in SRAM region */
#define BITBAND_SRAM(address, bit)			(*(volatile unsigned long*)BITBAND_SRAM_ADDRESS(address, bit))

/** bit-band alias in peripheral region */
#define BITBAND_PERIPH(address, bit)		(*(volatile unsigned long*)BITBAND_PERIPHERAL_ADDRESS(address, bit))

/** bit-band alias in any region */
#define BITBAND(address, bit)				(*(volatile unsigned long*)BITBAND_ADDRESS(address, bit))

#endif /* SOURCE_ARCHITECTURE_ARM_ARMV7_M_INCLUDE_DISTORTOS_ARCHITECTURE_ARMV7_M_BIT_BANDING_H_ */
