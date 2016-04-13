/**
 * \file
 * \brief Header with definition of bits in FLASH control registers
 *
 * This file covers devices as described in following places:
 * - PM0063 programming manual (STM32F100 value line), Revision 2, 2010-10-12
 * - PM0068 programming manual (STM32F1 XL-density), Revision 2, 2012-08-27
 * - PM0075 programming manual (STM32F1), Revision 2, 2012-01-13
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_FLASH_BITS_H_
#define SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_FLASH_BITS_H_

#include "distortos/architecture/ARMv7-M-bit-banding.h"

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_ACR - Flash access control register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_ACR_PRFTBS_bit				5
#define FLASH_ACR_PRFTBE_bit				4
#define FLASH_ACR_HLFCYA_bit				3
#define FLASH_ACR_LATENCY_bit				0
#define FLASH_ACR_LATENCY_0_bit				0
#define FLASH_ACR_LATENCY_1_bit				1
#define FLASH_ACR_LATENCY_2_bit				2

#define FLASH_ACR_PRFTBS_bb					BITBAND(&FLASH->ACR, FLASH_ACR_PRFTBS_bit)
#define FLASH_ACR_PRFTBE_bb					BITBAND(&FLASH->ACR, FLASH_ACR_PRFTBE_bit)
#define FLASH_ACR_HLFCYA_bb					BITBAND(&FLASH->ACR, FLASH_ACR_HLFCYA_bit)
#define FLASH_ACR_LATENCY_0_bb				BITBAND(&FLASH->ACR, FLASH_ACR_LATENCY_0_bit)
#define FLASH_ACR_LATENCY_1_bb				BITBAND(&FLASH->ACR, FLASH_ACR_LATENCY_1_bit)
#define FLASH_ACR_LATENCY_2_bb				BITBAND(&FLASH->ACR, FLASH_ACR_LATENCY_2_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_KEYR - FPEC key register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_KEYR_KEY1						0x45670123
#define FLASH_KEYR_KEY2						0xcdef89ab

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_OPTKEYR - Flash OPTKEY register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_OPTKEYR_KEY1					0x45670123
#define FLASH_OPTKEYR_KEY2					0xcdef89ab

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_SR - Flash status register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_SR_EOP_bit					5
#define FLASH_SR_WRPRTERR_bit				4
#define FLASH_SR_PGERR_bit					2
#define FLASH_SR_BSY_bit					0

#define FLASH_SR_EOP_bb						BITBAND(&FLASH->SR, FLASH_SR_EOP_bit)
#define FLASH_SR_WRPRTERR_bb				BITBAND(&FLASH->SR, FLASH_SR_WRPRTERR_bit)
#define FLASH_SR_PGERR_bb					BITBAND(&FLASH->SR, FLASH_SR_PGERR_bit)
#define FLASH_SR_BSY_bb						BITBAND(&FLASH->SR, FLASH_SR_BSY_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_CR - Flash control register
+---------------------------------------------------------------------------------------------------------------------*/

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

#define FLASH_CR_EOPIE_bb					BITBAND(&FLASH->CR, FLASH_CR_EOPIE_bit)
#define FLASH_CR_ERRIE_bb					BITBAND(&FLASH->CR, FLASH_CR_ERRIE_bit)
#define FLASH_CR_OPTWRE_bb					BITBAND(&FLASH->CR, FLASH_CR_OPTWRE_bit)
#define FLASH_CR_LOCK_bb					BITBAND(&FLASH->CR, FLASH_CR_LOCK_bit)
#define FLASH_CR_STRT_bb					BITBAND(&FLASH->CR, FLASH_CR_STRT_bit)
#define FLASH_CR_OPTER_bb					BITBAND(&FLASH->CR, FLASH_CR_OPTER_bit)
#define FLASH_CR_OPTPG_bb					BITBAND(&FLASH->CR, FLASH_CR_OPTPG_bit)
#define FLASH_CR_MER_bb						BITBAND(&FLASH->CR, FLASH_CR_MER_bit)
#define FLASH_CR_PER_bb						BITBAND(&FLASH->CR, FLASH_CR_PER_bit)
#define FLASH_CR_PG_bb						BITBAND(&FLASH->CR, FLASH_CR_PG_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_OBR - Option byte register
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_OBR_BFB2_bit					5
#define FLASH_OBR_nRST_STDBY_bit			4
#define FLASH_OBR_nRST_STOP_bit				3
#define FLASH_OBR_WDG_SW_bit				2
#define FLASH_OBR_RDPRT_bit					1
#define FLASH_OBR_OPTERR_bit				0

#define FLASH_OBR_BFB2_bb					BITBAND(&FLASH->OBR, FLASH_OBR_BFB2_bit)
#define FLASH_OBR_nRST_STDBY_bb				BITBAND(&FLASH->OBR, FLASH_OBR_nRST_STDBY_bit)
#define FLASH_OBR_nRST_STOP_bb				BITBAND(&FLASH->OBR, FLASH_OBR_nRST_STOP_bit)
#define FLASH_OBR_WDG_SW_bb					BITBAND(&FLASH->OBR, FLASH_OBR_WDG_SW_bit)
#define FLASH_OBR_RDPRT_bb					BITBAND(&FLASH->OBR, FLASH_OBR_RDPRT_bit)
#define FLASH_OBR_OPTERR_bb					BITBAND(&FLASH->OBR, FLASH_OBR_OPTERR_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_KEYR2 - FPEC key register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_KEYR2_KEY1					0x45670123
#define FLASH_KEYR2_KEY2					0xcdef89ab

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_SR2 - Flash status register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_SR2_EOP_bit					5
#define FLASH_SR2_WRPRTERR_bit				4
#define FLASH_SR2_PGERR_bit					2
#define FLASH_SR2_BSY_bit					0

#define FLASH_SR2_EOP_bb					BITBAND(&FLASH->SR2, FLASH_SR2_EOP_bit)
#define FLASH_SR2_WRPRTERR_bb				BITBAND(&FLASH->SR2, FLASH_SR2_WRPRTERR_bit)
#define FLASH_SR2_PGERR_bb					BITBAND(&FLASH->SR2, FLASH_SR2_PGERR_bit)
#define FLASH_SR2_BSY_bb					BITBAND(&FLASH->SR2, FLASH_SR2_BSY_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| FLASH_CR2 - Flash control register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define FLASH_CR2_EOPIE_bit					12
#define FLASH_CR2_ERRIE_bit					10
#define FLASH_CR2_LOCK_bit					7
#define FLASH_CR2_STRT_bit					6
#define FLASH_CR2_MER_bit					2
#define FLASH_CR2_PER_bit					1
#define FLASH_CR2_PG_bit					0

#define FLASH_CR2_EOPIE_bb					BITBAND(&FLASH->CR2, FLASH_CR2_EOPIE_bit)
#define FLASH_CR2_ERRIE_bb					BITBAND(&FLASH->CR2, FLASH_CR2_ERRIE_bit)
#define FLASH_CR2_LOCK_bb					BITBAND(&FLASH->CR2, FLASH_CR2_LOCK_bit)
#define FLASH_CR2_STRT_bb					BITBAND(&FLASH->CR2, FLASH_CR2_STRT_bit)
#define FLASH_CR2_MER_bb					BITBAND(&FLASH->CR2, FLASH_CR2_MER_bit)
#define FLASH_CR2_PER_bb					BITBAND(&FLASH->CR2, FLASH_CR2_PER_bit)
#define FLASH_CR2_PG_bb						BITBAND(&FLASH->CR2, FLASH_CR2_PG_bit)

#endif	/* SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_FLASH_BITS_H_ */
