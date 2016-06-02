/**
 * \file
 * \brief Header with definition of bits in FLASH control registers
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_FLASH_BITS_H_
#define SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_FLASH_BITS_H_

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_ACR - Flash access control register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_ACR_PRFTBS_bit				5
#define FLASH_ACR_PRFTBE_bit				4
#define FLASH_ACR_LATENCY_bit				0
#define FLASH_ACR_LATENCY_0_bit				0
#define FLASH_ACR_LATENCY_1_bit				1
#define FLASH_ACR_LATENCY_2_bit				2

#define FLASH_ACR_LATENCY_0WS_value			0
#define FLASH_ACR_LATENCY_1WS_value			1
#define FLASH_ACR_LATENCY_mask				7

#define FLASH_ACR_LATENCY_0WS				(FLASH_ACR_LATENCY_0WS_value << FLASH_ACR_LATENCY_bit)
#define FLASH_ACR_LATENCY_1WS				(FLASH_ACR_LATENCY_1WS_value << FLASH_ACR_LATENCY_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_SR - Flash status register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_SR_EOP_bit					5
#define FLASH_SR_WRPRTERR_bit				4
#define FLASH_SR_PGERR_bit					2
#define FLASH_SR_BSY_bit					0

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_CR - Flash control register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_CR_OBL_LAUNCH_bit				13
#define FLASH_CR_EOPIE_bit					12
#define FLASH_CR_ERRIE_bit					10
#define FLASH_CR_OPTWRE_bit					9
#define FLASH_CR_LOCK_bit					7
#define FLASH_CR_STRT_bit					6
#define FLASH_CR_OPTER_bit					5
#define FLASH_CR_OPTPG_bit					4
#define FLASH_CR_MER_bit					2
#define FLASH_CR_PER_bit					1
#define FLASH_CR_PG_bit						0

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_OBR - Option byte register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_OBR_BOOT_SEL_bit				15
#define FLASH_OBR_RAM_PARITY_CHECK_bit		14
#define FLASH_OBR_VDDA_MONITOR_bit			13
#define FLASH_OBR_nBOOT1_bit				12
#define FLASH_OBR_nBOOT0_bit				11
#define FLASH_OBR_nRST_STDBY_bit			10
#define FLASH_OBR_nRST_STOP_bit				9
#define FLASH_OBR_WDG_SW_bit				8
#define FLASH_OBR_RDPRT_bit					1
#define FLASH_OBR_RDPRT_0_bit				1
#define FLASH_OBR_RDPRT_1_bit				2
#define FLASH_OBR_OPTERR_bit				0

#define FLASH_OBR_RDPRT_LEVEL0_value		0
#define FLASH_OBR_RDPRT_LEVEL1_value		1
#define FLASH_OBR_RDPRT_LEVEL2_value		3
#define FLASH_OBR_RDPRT_mask				3

#define FLASH_OBR_RDPRT_LEVEL0				(FLASH_OBR_RDPRT_LEVEL0_value << FLASH_OBR_RDPRT_bit)
#define FLASH_OBR_RDPRT_LEVEL1				(FLASH_OBR_RDPRT_LEVEL1_value << FLASH_OBR_RDPRT_bit)
#define FLASH_OBR_RDPRT_LEVEL2				(FLASH_OBR_RDPRT_LEVEL2_value << FLASH_OBR_RDPRT_bit)

#endif	/* SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_FLASH_BITS_H_ */
