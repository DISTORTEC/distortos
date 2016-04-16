/**
 * \file
 * \brief Header with definition of bits in PWR control registers
 *
 * This file covers devices as described in following places:
 * - RM0368 reference manual (STM32F401xB/C and STM32F401xD/E), Revision 4, 2015-05-04
 * - RM0090 reference manual (STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439), Revision 11, 2015-10-20
 * - RM0401 reference manual (STM32F410), Revision 2, 2015-10-26
 * - RM0383 reference manual (STM32F411xC/E), Revision 1, 2014-07-24
 * - RM0390 reference manual (STM32F446xx), Revision 2, 2016-01-12
 * - RM0386 reference manual (STM32F469xx and STM32F479xx), Revision 2, 2015-11-19
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_PWR_BITS_H_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_PWR_BITS_H_

#include "distortos/architecture/ARMv7-M-bit-banding.h"

/*---------------------------------------------------------------------------------------------------------------------+
| PWR_CR - PWR power control register
+---------------------------------------------------------------------------------------------------------------------*/

#define PWR_CR_FISSR_bit					21
#define PWR_CR_FMSSR_bit					20

#define PWR_CR_UDEN_bit						18
#define PWR_CR_UDEN_0_bit					18
#define PWR_CR_UDEN_1_bit					19

#define PWR_CR_ODSWEN_bit					17
#define PWR_CR_ODEN_bit						16

#define PWR_CR_VOS_bit						14
#define PWR_CR_VOS_0_bit					14
#define PWR_CR_VOS_1_bit					15

#define PWR_CR_ADCDC1_bit					13
#define PWR_CR_MRLVDS_bit					11
#define PWR_CR_MRUDS_bit					11
#define PWR_CR_LPLVDS_bit					10
#define PWR_CR_LPUDS_bit					10
#define PWR_CR_FPDS_bit						9
#define PWR_CR_DBP_bit						8

#define PWR_CR_PLS_bit						5
#define PWR_CR_PLS_0_bit					5
#define PWR_CR_PLS_1_bit					6
#define PWR_CR_PLS_2_bit					7

#define PWR_CR_PVDE_bit						4
#define PWR_CR_CSBF_bit						3
#define PWR_CR_CWUF_bit						2
#define PWR_CR_PDDS_bit						1
#define PWR_CR_LPDS_bit						0

#define PWR_CR_VOS_SCALE3_value				1
#define PWR_CR_VOS_SCALE2_value				2
#define PWR_CR_VOS_SCALE1_value				3
#define PWR_CR_VOS_mask						3

#define PWR_CR_VOS_SCALE3					(PWR_CR_VOS_SCALE3_value << PWR_CR_VOS_bit)
#define PWR_CR_VOS_SCALE2					(PWR_CR_VOS_SCALE2_value << PWR_CR_VOS_bit)
#define PWR_CR_VOS_SCALE1					(PWR_CR_VOS_SCALE1_value << PWR_CR_VOS_bit)

#define PWR_CR_FISSR_bb						BITBAND(&PWR->CR, PWR_CR_FISSR_bit)
#define PWR_CR_FMSSR_bb						BITBAND(&PWR->CR, PWR_CR_FMSSR_bit)

#define PWR_CR_UDEN_0_bb					BITBAND(&PWR->CR, PWR_CR_UDEN_0_bit)
#define PWR_CR_UDEN_1_bb					BITBAND(&PWR->CR, PWR_CR_UDEN_1_bit)

#define PWR_CR_ODSWEN_bb					BITBAND(&PWR->CR, PWR_CR_ODSWEN_bit)
#define PWR_CR_ODEN_bb						BITBAND(&PWR->CR, PWR_CR_ODEN_bit)

#define PWR_CR_VOS_bb						BITBAND(&PWR->CR, PWR_CR_VOS_bit)
#define PWR_CR_VOS_0_bb						BITBAND(&PWR->CR, PWR_CR_VOS_0_bit)
#define PWR_CR_VOS_1_bb						BITBAND(&PWR->CR, PWR_CR_VOS_1_bit)

#define PWR_CR_ADCDC1_bb					BITBAND(&PWR->CR, PWR_CR_ADCDC1_bit)
#define PWR_CR_MRLVDS_bb					BITBAND(&PWR->CR, PWR_CR_MRLVDS_bit)
#define PWR_CR_MRUDS_bb						BITBAND(&PWR->CR, PWR_CR_MRUDS_bit)
#define PWR_CR_LPLVDS_bb					BITBAND(&PWR->CR, PWR_CR_LPLVDS_bit)
#define PWR_CR_LPUDS_bb						BITBAND(&PWR->CR, PWR_CR_LPUDS_bit)
#define PWR_CR_FPDS_bb						BITBAND(&PWR->CR, PWR_CR_FPDS_bit)
#define PWR_CR_DBP_bb						BITBAND(&PWR->CR, PWR_CR_DBP_bit)

#define PWR_CR_PLS_0_bb						BITBAND(&PWR->CR, PWR_CR_PLS_0_bit)
#define PWR_CR_PLS_1_bb						BITBAND(&PWR->CR, PWR_CR_PLS_1_bit)
#define PWR_CR_PLS_2_bb						BITBAND(&PWR->CR, PWR_CR_PLS_2_bit)

#define PWR_CR_PVDE_bb						BITBAND(&PWR->CR, PWR_CR_PVDE_bit)
#define PWR_CR_CSBF_bb						BITBAND(&PWR->CR, PWR_CR_CSBF_bit)
#define PWR_CR_CWUF_bb						BITBAND(&PWR->CR, PWR_CR_CWUF_bit)
#define PWR_CR_PDDS_bb						BITBAND(&PWR->CR, PWR_CR_PDDS_bit)
#define PWR_CR_LPDS_bb						BITBAND(&PWR->CR, PWR_CR_LPDS_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| PWR_CSR - PWR power control/status register
+---------------------------------------------------------------------------------------------------------------------*/

#define PWR_CSR_UDRDY_bit					18
#define PWR_CSR_UDRDY_0_bit					18
#define PWR_CSR_UDRDY_1_bit					19
#define PWR_CSR_ODSWRDY_bit					17
#define PWR_CSR_ODRDY_bit					16
#define PWR_CSR_VOSRDY_bit					14
#define PWR_CSR_BRE_bit						9
#define PWR_CSR_EWUP_bit					8
#define PWR_CSR_EWUP1_bit					8
#define PWR_CSR_EWUP2_bit					7
#define PWR_CSR_EWUP3_bit					6
#define PWR_CSR_BRR_bit						3
#define PWR_CSR_PVDO_bit					2
#define PWR_CSR_SBF_bit						1
#define PWR_CSR_WUPF_bit					0
#define PWR_CSR_WUF_bit						0

#define PWR_CSR_UDRDY_bb					BITBAND(&PWR->CSR, PWR_CSR_UDRDY_bit)
#define PWR_CSR_UDRDY_0_bb					BITBAND(&PWR->CSR, PWR_CSR_UDRDY_0_bit)
#define PWR_CSR_UDRDY_1_bb					BITBAND(&PWR->CSR, PWR_CSR_UDRDY_1_bit)
#define PWR_CSR_ODSWRDY_bb					BITBAND(&PWR->CSR, PWR_CSR_ODSWRDY_bit)
#define PWR_CSR_ODRDY_bb					BITBAND(&PWR->CSR, PWR_CSR_ODRDY_bit)
#define PWR_CSR_VOSRDY_bb					BITBAND(&PWR->CSR, PWR_CSR_VOSRDY_bit)
#define PWR_CSR_BRE_bb						BITBAND(&PWR->CSR, PWR_CSR_BRE_bit)
#define PWR_CSR_EWUP_bb						BITBAND(&PWR->CSR, PWR_CSR_EWUP_bit)
#define PWR_CSR_EWUP1_bb					BITBAND(&PWR->CSR, PWR_CSR_EWUP1_bit)
#define PWR_CSR_EWUP2_bb					BITBAND(&PWR->CSR, PWR_CSR_EWUP2_bit)
#define PWR_CSR_EWUP3_bb					BITBAND(&PWR->CSR, PWR_CSR_EWUP3_bit)
#define PWR_CSR_BRR_bb						BITBAND(&PWR->CSR, PWR_CSR_BRR_bit)
#define PWR_CSR_PVDO_bb						BITBAND(&PWR->CSR, PWR_CSR_PVDO_bit)
#define PWR_CSR_SBF_bb						BITBAND(&PWR->CSR, PWR_CSR_SBF_bit)
#define PWR_CSR_WUPF_bb						BITBAND(&PWR->CSR, PWR_CSR_WUPF_bit)
#define PWR_CSR_WUF_bb						BITBAND(&PWR->CSR, PWR_CSR_WUF_bit)

#endif	/* SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_PWR_BITS_H_ */
