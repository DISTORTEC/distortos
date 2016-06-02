/**
 * \file
 * \brief Header with definition of bits in FLASH control registers
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_FLASH_BITS_H_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_FLASH_BITS_H_

#include "distortos/architecture/ARMv7-M-bit-banding.h"

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_ACR - Flash access control register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_ACR_DCRST_bit					12
#define FLASH_ACR_ICRST_bit					11
#define FLASH_ACR_DCEN_bit					10
#define FLASH_ACR_ICEN_bit					9
#define FLASH_ACR_PRFTEN_bit				8
#define FLASH_ACR_LATENCY_bit				0
#define FLASH_ACR_LATENCY_0_bit				0
#define FLASH_ACR_LATENCY_1_bit				1
#define FLASH_ACR_LATENCY_2_bit				2
#define FLASH_ACR_LATENCY_3_bit				3

#define FLASH_ACR_DCRST_bb					BITBAND(&FLASH->ACR, FLASH_ACR_DCRST_bit)
#define FLASH_ACR_ICRST_bb					BITBAND(&FLASH->ACR, FLASH_ACR_ICRST_bit)
#define FLASH_ACR_DCEN_bb					BITBAND(&FLASH->ACR, FLASH_ACR_DCEN_bit)
#define FLASH_ACR_ICEN_bb					BITBAND(&FLASH->ACR, FLASH_ACR_ICEN_bit)
#define FLASH_ACR_PRFTEN_bb					BITBAND(&FLASH->ACR, FLASH_ACR_PRFTEN_bit)
#define FLASH_ACR_LATENCY_0_bb				BITBAND(&FLASH->ACR, FLASH_ACR_LATENCY_0_bit)
#define FLASH_ACR_LATENCY_1_bb				BITBAND(&FLASH->ACR, FLASH_ACR_LATENCY_1_bit)
#define FLASH_ACR_LATENCY_2_bb				BITBAND(&FLASH->ACR, FLASH_ACR_LATENCY_2_bit)
#define FLASH_ACR_LATENCY_3_bb				BITBAND(&FLASH->ACR, FLASH_ACR_LATENCY_3_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_KEYR - Flash key register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_KEYR_KEY1						0x45670123
#define FLASH_KEYR_KEY2						0xCDEF89AB

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_OPTKEYR - Flash option key register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_OPTKEYR_KEY1					0x08192A3B
#define FLASH_OPTKEYR_KEY2					0x4C5D6E7F

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_SR - Flash status register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_SR_BSY_bit					16
#define FLASH_SR_RDERR_bit					8
#define FLASH_SR_PGSERR_bit					7
#define FLASH_SR_PGPERR_bit					6
#define FLASH_SR_PGAERR_bit					5
#define FLASH_SR_WRPERR_bit					4
#define FLASH_SR_OPERR_bit					1
#define FLASH_SR_EOP_bit					0

#define FLASH_SR_BSY_bb						BITBAND(&FLASH->SR, FLASH_SR_BSY_bit)
#define FLASH_SR_RDERR_bb					BITBAND(&FLASH->SR, FLASH_SR_RDERR_bit)
#define FLASH_SR_PGSERR_bb					BITBAND(&FLASH->SR, FLASH_SR_PGSERR_bit)
#define FLASH_SR_PGPERR_bb					BITBAND(&FLASH->SR, FLASH_SR_PGPERR_bit)
#define FLASH_SR_PGAERR_bb					BITBAND(&FLASH->SR, FLASH_SR_PGAERR_bit)
#define FLASH_SR_WRPERR_bb					BITBAND(&FLASH->SR, FLASH_SR_WRPERR_bit)
#define FLASH_SR_OPERR_bb					BITBAND(&FLASH->SR, FLASH_SR_OPERR_bit)
#define FLASH_SR_EOP_bb						BITBAND(&FLASH->SR, FLASH_SR_EOP_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_CR - Flash control register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_CR_LOCK_bit					31
#define FLASH_CR_ERRIE_bit					25
#define FLASH_CR_EOPIE_bit					24
#define FLASH_CR_STRT_bit					16
#define FLASH_CR_MER1_bit					15
#define FLASH_CR_PSIZE_bit					8
#define FLASH_CR_PSIZE_0_bit				8
#define FLASH_CR_PSIZE_1_bit				9
#define FLASH_CR_SNB_bit					3
#define FLASH_CR_SNB_0_bit					3
#define FLASH_CR_SNB_1_bit					4
#define FLASH_CR_SNB_2_bit					5
#define FLASH_CR_SNB_3_bit					6
#define FLASH_CR_SNB_4_bit					7
#define FLASH_CR_MER_bit					2
#define FLASH_CR_SER_bit					1
#define FLASH_CR_PG_bit						0

#define FLASH_CR_PSIZE_x8_value				0
#define FLASH_CR_PSIZE_x16_value			1
#define FLASH_CR_PSIZE_x32_value			2
#define FLASH_CR_PSIZE_x64_value			3
#define FLASH_CR_PSIZE_mask					3

#define FLASH_CR_PSIZE_x8					(FLASH_CR_PSIZE_x8_value << FLASH_CR_PSIZE_bit)
#define FLASH_CR_PSIZE_x16					(FLASH_CR_PSIZE_x16_value << FLASH_CR_PSIZE_bit)
#define FLASH_CR_PSIZE_x32					(FLASH_CR_PSIZE_x32_value << FLASH_CR_PSIZE_bit)
#define FLASH_CR_PSIZE_x64					(FLASH_CR_PSIZE_x64_value << FLASH_CR_PSIZE_bit)

#define FLASH_CR_LOCK_bb					BITBAND(&FLASH->CR, FLASH_CR_LOCK_bit)
#define FLASH_CR_ERRIE_bb					BITBAND(&FLASH->CR, FLASH_CR_ERRIE_bit)
#define FLASH_CR_EOPIE_bb					BITBAND(&FLASH->CR, FLASH_CR_EOPIE_bit)
#define FLASH_CR_STRT_bb					BITBAND(&FLASH->CR, FLASH_CR_STRT_bit)
#define FLASH_CR_MER1_bb					BITBAND(&FLASH->CR, FLASH_CR_MER1_bit)
#define FLASH_CR_PSIZE_0_bb					BITBAND(&FLASH->CR, FLASH_CR_PSIZE_0_bit)
#define FLASH_CR_PSIZE_1_bb					BITBAND(&FLASH->CR, FLASH_CR_PSIZE_1_bit)
#define FLASH_CR_SNB_0_bb					BITBAND(&FLASH->CR, FLASH_CR_SNB_0_bit)
#define FLASH_CR_SNB_1_bb					BITBAND(&FLASH->CR, FLASH_CR_SNB_1_bit)
#define FLASH_CR_SNB_2_bb					BITBAND(&FLASH->CR, FLASH_CR_SNB_2_bit)
#define FLASH_CR_SNB_3_bb					BITBAND(&FLASH->CR, FLASH_CR_SNB_3_bit)
#define FLASH_CR_SNB_4_bb					BITBAND(&FLASH->CR, FLASH_CR_SNB_4_bit)
#define FLASH_CR_MER_bb						BITBAND(&FLASH->CR, FLASH_CR_MER_bit)
#define FLASH_CR_SER_bb						BITBAND(&FLASH->CR, FLASH_CR_SER_bit)
#define FLASH_CR_PG_bb						BITBAND(&FLASH->CR, FLASH_CR_PG_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_OPTCR - Flash option control register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_OPTCR_SPRMOD_bit				31
#define FLASH_OPTCR_DB1M_bit				30
#define FLASH_OPTCR_nWRP_bit				16
#define FLASH_OPTCR_nWRP_0_bit				16
#define FLASH_OPTCR_nWRP_1_bit				17
#define FLASH_OPTCR_nWRP_2_bit				18
#define FLASH_OPTCR_nWRP_3_bit				19
#define FLASH_OPTCR_nWRP_4_bit				20
#define FLASH_OPTCR_nWRP_5_bit				21
#define FLASH_OPTCR_nWRP_6_bit				22
#define FLASH_OPTCR_nWRP_7_bit				23
#define FLASH_OPTCR_nWRP_8_bit				24
#define FLASH_OPTCR_nWRP_9_bit				25
#define FLASH_OPTCR_nWRP_10_bit				26
#define FLASH_OPTCR_nWRP_11_bit				27
#define FLASH_OPTCR_RDP_bit					8
#define FLASH_OPTCR_RDP_0_bit				8
#define FLASH_OPTCR_RDP_1_bit				9
#define FLASH_OPTCR_RDP_2_bit				10
#define FLASH_OPTCR_RDP_3_bit				11
#define FLASH_OPTCR_RDP_4_bit				12
#define FLASH_OPTCR_RDP_5_bit				13
#define FLASH_OPTCR_RDP_6_bit				14
#define FLASH_OPTCR_RDP_7_bit				15
#define FLASH_OPTCR_nRST_STDBY_bit			7
#define FLASH_OPTCR_nRST_STOP_bit			6
#define FLASH_OPTCR_WDG_SW_bit				5
#define FLASH_OPTCR_BFB2_bit				4
#define FLASH_OPTCR_BOR_LEV_bit				2
#define FLASH_OPTCR_BOR_LEV_0_bit			2
#define FLASH_OPTCR_BOR_LEV_1_bit			3
#define FLASH_OPTCR_OPTSTRT_bit				1
#define FLASH_OPTCR_OPTLOCK_bit				0

#define FLASH_OPTCR_SPRMOD_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_SPRMOD_bit)
#define FLASH_OPTCR_DB1M_bb					BITBAND(&FLASH->OPTCR, FLASH_OPTCR_DB1M_bit)
#define FLASH_OPTCR_nWRP_0_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_0_bit)
#define FLASH_OPTCR_nWRP_1_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_1_bit)
#define FLASH_OPTCR_nWRP_2_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_2_bit)
#define FLASH_OPTCR_nWRP_3_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_3_bit)
#define FLASH_OPTCR_nWRP_4_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_4_bit)
#define FLASH_OPTCR_nWRP_5_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_5_bit)
#define FLASH_OPTCR_nWRP_6_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_6_bit)
#define FLASH_OPTCR_nWRP_7_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_7_bit)
#define FLASH_OPTCR_nWRP_8_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_8_bit)
#define FLASH_OPTCR_nWRP_9_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_9_bit)
#define FLASH_OPTCR_nWRP_10_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_10_bit)
#define FLASH_OPTCR_nWRP_11_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nWRP_11_bit)
#define FLASH_OPTCR_RDP_0_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_RDP_0_bit)
#define FLASH_OPTCR_RDP_1_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_RDP_1_bit)
#define FLASH_OPTCR_RDP_2_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_RDP_2_bit)
#define FLASH_OPTCR_RDP_3_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_RDP_3_bit)
#define FLASH_OPTCR_RDP_4_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_RDP_4_bit)
#define FLASH_OPTCR_RDP_5_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_RDP_5_bit)
#define FLASH_OPTCR_RDP_6_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_RDP_6_bit)
#define FLASH_OPTCR_RDP_7_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_RDP_7_bit)
#define FLASH_OPTCR_nRST_STDBY_bb			BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nRST_STDBY_bit)
#define FLASH_OPTCR_nRST_STOP_bb			BITBAND(&FLASH->OPTCR, FLASH_OPTCR_nRST_STOP_bit)
#define FLASH_OPTCR_WDG_SW_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_WDG_SW_bit)
#define FLASH_OPTCR_BFB2_bb					BITBAND(&FLASH->OPTCR, FLASH_OPTCR_BFB2_bit)
#define FLASH_OPTCR_BOR_LEV_0_bb			BITBAND(&FLASH->OPTCR, FLASH_OPTCR_BOR_LEV_0_bit)
#define FLASH_OPTCR_BOR_LEV_1_bb			BITBAND(&FLASH->OPTCR, FLASH_OPTCR_BOR_LEV_1_bit)
#define FLASH_OPTCR_OPTSTRT_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_OPTSTRT_bit)
#define FLASH_OPTCR_OPTLOCK_bb				BITBAND(&FLASH->OPTCR, FLASH_OPTCR_OPTLOCK_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_OPTCR1 - Flash option control register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_OPTCR1_nWRP_bit				16
#define FLASH_OPTCR1_nWRP_0_bit				16
#define FLASH_OPTCR1_nWRP_1_bit				17
#define FLASH_OPTCR1_nWRP_2_bit				18
#define FLASH_OPTCR1_nWRP_3_bit				19
#define FLASH_OPTCR1_nWRP_4_bit				20
#define FLASH_OPTCR1_nWRP_5_bit				21
#define FLASH_OPTCR1_nWRP_6_bit				22
#define FLASH_OPTCR1_nWRP_7_bit				23
#define FLASH_OPTCR1_nWRP_8_bit				24
#define FLASH_OPTCR1_nWRP_9_bit				25
#define FLASH_OPTCR1_nWRP_10_bit			26
#define FLASH_OPTCR1_nWRP_11_bit			27

#define FLASH_OPTCR1_nWRP_0_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_0_bit)
#define FLASH_OPTCR1_nWRP_1_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_1_bit)
#define FLASH_OPTCR1_nWRP_2_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_2_bit)
#define FLASH_OPTCR1_nWRP_3_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_3_bit)
#define FLASH_OPTCR1_nWRP_4_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_4_bit)
#define FLASH_OPTCR1_nWRP_5_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_5_bit)
#define FLASH_OPTCR1_nWRP_6_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_6_bit)
#define FLASH_OPTCR1_nWRP_7_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_7_bit)
#define FLASH_OPTCR1_nWRP_8_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_8_bit)
#define FLASH_OPTCR1_nWRP_9_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_9_bit)
#define FLASH_OPTCR1_nWRP_10_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_10_bit)
#define FLASH_OPTCR1_nWRP_11_bb				BITBAND(&FLASH->OPTCR1, FLASH_OPTCR1_nWRP_11_bit)

#endif	/* SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_FLASH_BITS_H_ */
