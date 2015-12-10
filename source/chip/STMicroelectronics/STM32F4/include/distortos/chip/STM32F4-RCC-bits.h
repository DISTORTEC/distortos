/**
 * \file
 * \brief Header with definition of bits in RCC control registers
 *
 * This file covers devices as described in following places:
 * - RM0368 reference manual (STM32F401xB/C and STM32F401xD/E), Revision 4, 2015-05-04
 * - RM0090 reference manual (STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439), Revision 11, 2015-10-20
 * - RM0401 reference manual (STM32F410), Revision 2, 2015-10-26
 * - RM0383 reference manual (STM32F411xC/E), Revision 1, 2014-07-24
 * - RM0390 reference manual (STM32F446xx), Revision 1, 2015-03-17
 * - RM0386 reference manual (STM32F469xx and STM32F479xx), Revision 2, 2015-11-19
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-10
 */

#ifndef SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_RCC_BITS_H_
#define SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_RCC_BITS_H_

#include "distortos/architecture/ARMv7-M-bit-banding.h"

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CR - Clock Control Register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CR_PLLSAIRDY_bit				29
#define RCC_CR_PLLSAION_bit					28
#define RCC_CR_PLLI2SRDY_bit				27
#define RCC_CR_PLLI2SON_bit					26
#define RCC_CR_PLLRDY_bit					25
#define RCC_CR_PLLON_bit					24
#define RCC_CR_CSSON_bit					19
#define RCC_CR_HSEBYP_bit					18
#define RCC_CR_HSERDY_bit					17
#define RCC_CR_HSEON_bit					16

#define RCC_CR_HSICAL_bit					8
#define RCC_CR_HSICAL_0_bit					8
#define RCC_CR_HSICAL_1_bit					9
#define RCC_CR_HSICAL_2_bit					10
#define RCC_CR_HSICAL_3_bit					11
#define RCC_CR_HSICAL_4_bit					12
#define RCC_CR_HSICAL_5_bit					13
#define RCC_CR_HSICAL_6_bit					14
#define RCC_CR_HSICAL_7_bit					15

#define RCC_CR_HSITRIM_bit					3
#define RCC_CR_HSITRIM_0_bit				3
#define RCC_CR_HSITRIM_1_bit				4
#define RCC_CR_HSITRIM_2_bit				5
#define RCC_CR_HSITRIM_3_bit				6
#define RCC_CR_HSITRIM_4_bit				7

#define RCC_CR_HSIRDY_bit					1
#define RCC_CR_HSION_bit					0

#define RCC_CR_PLLSAIRDY_bb					BITBAND(&RCC->CR, RCC_CR_PLLSAIRDY_bit)
#define RCC_CR_PLLSAION_bb					BITBAND(&RCC->CR, RCC_CR_PLLSAION_bit)
#define RCC_CR_PLLI2SRDY_bb					BITBAND(&RCC->CR,RCC_CR_PLLI2SRDY_bit)
#define RCC_CR_PLLI2SON_bb					BITBAND(&RCC->CR,RCC_CR_PLLI2SON_bit)
#define RCC_CR_PLLRDY_bb					BITBAND(&RCC->CR, RCC_CR_PLLRDY_bit)
#define RCC_CR_PLLON_bb						BITBAND(&RCC->CR, RCC_CR_PLLON_bit)
#define RCC_CR_CSSON_bb						BITBAND(&RCC->CR, RCC_CR_CSSON_bit)
#define RCC_CR_HSEBYP_bb					BITBAND(&RCC->CR, RCC_CR_HSEBYP_bit)
#define RCC_CR_HSERDY_bb					BITBAND(&RCC->CR, RCC_CR_HSERDY_bit)
#define RCC_CR_HSEON_bb						BITBAND(&RCC->CR, RCC_CR_HSEON_bit)

#define RCC_CR_HSICAL_0_bb					BITBAND(&RCC->CR, RCC_CR_HSICAL_0_bit)
#define RCC_CR_HSICAL_1_bb					BITBAND(&RCC->CR, RCC_CR_HSICAL_1_bit)
#define RCC_CR_HSICAL_2_bb					BITBAND(&RCC->CR, RCC_CR_HSICAL_2_bit)
#define RCC_CR_HSICAL_3_bb					BITBAND(&RCC->CR, RCC_CR_HSICAL_3_bit)
#define RCC_CR_HSICAL_4_bb					BITBAND(&RCC->CR, RCC_CR_HSICAL_4_bit)
#define RCC_CR_HSICAL_5_bb					BITBAND(&RCC->CR, RCC_CR_HSICAL_5_bit)
#define RCC_CR_HSICAL_6_bb					BITBAND(&RCC->CR, RCC_CR_HSICAL_6_bit)
#define RCC_CR_HSICAL_7_bb					BITBAND(&RCC->CR, RCC_CR_HSICAL_7_bit)

#define RCC_CR_HSITRIM_0_bb					BITBAND(&RCC->CR, RCC_CR_HSITRIM_0_bit)
#define RCC_CR_HSITRIM_1_bb					BITBAND(&RCC->CR, RCC_CR_HSITRIM_1_bit)
#define RCC_CR_HSITRIM_2_bb					BITBAND(&RCC->CR, RCC_CR_HSITRIM_2_bit)
#define RCC_CR_HSITRIM_3_bb					BITBAND(&RCC->CR, RCC_CR_HSITRIM_3_bit)
#define RCC_CR_HSITRIM_4_bb					BITBAND(&RCC->CR, RCC_CR_HSITRIM_4_bit)

#define RCC_CR_HSIRDY_bb					BITBAND(&RCC->CR, RCC_CR_HSIRDY_bit)
#define RCC_CR_HSION_bb						BITBAND(&RCC->CR, RCC_CR_HSION_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_PLLCFGR - RCC PLL configuration register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_PLLCFGR_PLLR_bit				28
#define RCC_PLLCFGR_PLLR_0_bit				28
#define RCC_PLLCFGR_PLLR_1_bit				29
#define RCC_PLLCFGR_PLLR_2_bit				30

#define RCC_PLLCFGR_PLLQ_bit				24
#define RCC_PLLCFGR_PLLQ_0_bit				24
#define RCC_PLLCFGR_PLLQ_1_bit				25
#define RCC_PLLCFGR_PLLQ_2_bit				26
#define RCC_PLLCFGR_PLLQ_3_bit				27

#define RCC_PLLCFGR_PLLSRC_bit				22

#define RCC_PLLCFGR_PLLP_bit				16
#define RCC_PLLCFGR_PLLP_0_bit				16
#define RCC_PLLCFGR_PLLP_1_bit				17

#define RCC_PLLCFGR_PLLN_bit				6
#define RCC_PLLCFGR_PLLN_0_bit				6
#define RCC_PLLCFGR_PLLN_1_bit				7
#define RCC_PLLCFGR_PLLN_2_bit				8
#define RCC_PLLCFGR_PLLN_3_bit				9
#define RCC_PLLCFGR_PLLN_4_bit				10
#define RCC_PLLCFGR_PLLN_5_bit				11
#define RCC_PLLCFGR_PLLN_6_bit				12
#define RCC_PLLCFGR_PLLN_7_bit				13
#define RCC_PLLCFGR_PLLN_8_bit				14

#define RCC_PLLCFGR_PLLM_bit				0
#define RCC_PLLCFGR_PLLM_0_bit				0
#define RCC_PLLCFGR_PLLM_1_bit				1
#define RCC_PLLCFGR_PLLM_2_bit				2
#define RCC_PLLCFGR_PLLM_3_bit				3
#define RCC_PLLCFGR_PLLM_4_bit				4
#define RCC_PLLCFGR_PLLM_5_bit				5

#define RCC_PLLCFGR_PLLP_DIV2_value			0
#define RCC_PLLCFGR_PLLP_DIV4_value			1
#define RCC_PLLCFGR_PLLP_DIV6_value			2
#define RCC_PLLCFGR_PLLP_DIV8_value			3
#define RCC_PLLCFGR_PLLP_mask				3

#define RCC_PLLCFGR_PLLP_DIV2				(RCC_PLLCFGR_PLLP_DIV2_value << RCC_PLLCFGR_PLLP_bit)
#define RCC_PLLCFGR_PLLP_DIV4				(RCC_PLLCFGR_PLLP_DIV4_value << RCC_PLLCFGR_PLLP_bit)
#define RCC_PLLCFGR_PLLP_DIV6				(RCC_PLLCFGR_PLLP_DIV6_value << RCC_PLLCFGR_PLLP_bit)
#define RCC_PLLCFGR_PLLP_DIV8				(RCC_PLLCFGR_PLLP_DIV8_value << RCC_PLLCFGR_PLLP_bit)

#define RCC_PLLCFGR_PLLR_0_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLR_0_bit)
#define RCC_PLLCFGR_PLLR_1_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLR_1_bit)
#define RCC_PLLCFGR_PLLR_2_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLR_2_bit)

#define RCC_PLLCFGR_PLLQ_0_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_0_bit)
#define RCC_PLLCFGR_PLLQ_1_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_1_bit)
#define RCC_PLLCFGR_PLLQ_2_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_2_bit)
#define RCC_PLLCFGR_PLLQ_3_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_3_bit)

#define RCC_PLLCFGR_PLLSRC_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_bit)

#define RCC_PLLCFGR_PLLP_0_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLP_0_bit)
#define RCC_PLLCFGR_PLLP_1_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLP_1_bit)

#define RCC_PLLCFGR_PLLN_0_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLN_0_bit)
#define RCC_PLLCFGR_PLLN_1_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLN_1_bit)
#define RCC_PLLCFGR_PLLN_2_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLN_2_bit)
#define RCC_PLLCFGR_PLLN_3_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLN_3_bit)
#define RCC_PLLCFGR_PLLN_4_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLN_4_bit)
#define RCC_PLLCFGR_PLLN_5_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLN_5_bit)
#define RCC_PLLCFGR_PLLN_6_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLN_6_bit)
#define RCC_PLLCFGR_PLLN_7_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLN_7_bit)
#define RCC_PLLCFGR_PLLN_8_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLN_8_bit)

#define RCC_PLLCFGR_PLLM_0_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLM_0_bit)
#define RCC_PLLCFGR_PLLM_1_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLM_1_bit)
#define RCC_PLLCFGR_PLLM_2_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLM_2_bit)
#define RCC_PLLCFGR_PLLM_3_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLM_3_bit)
#define RCC_PLLCFGR_PLLM_4_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLM_4_bit)
#define RCC_PLLCFGR_PLLM_5_bb				BITBAND(&RCC->PLLCFGR, RCC_PLLCFGR_PLLM_5_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CFGR - Clock Configuration Register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CFGR_MCO2_bit					30
#define RCC_CFGR_MCO2_0_bit					30
#define RCC_CFGR_MCO2_1_bit					31

#define RCC_CFGR_MCO2PRE_bit				27
#define RCC_CFGR_MCO2PRE_0_bit				27
#define RCC_CFGR_MCO2PRE_1_bit				28
#define RCC_CFGR_MCO2PRE_2_bit				29

#define RCC_CFGR_MCO1PRE_bit				24
#define RCC_CFGR_MCO1PRE_0_bit				24
#define RCC_CFGR_MCO1PRE_1_bit				25
#define RCC_CFGR_MCO1PRE_2_bit				26

#define RCC_CFGR_I2SSRC_bit					23

#define RCC_CFGR_MCO1_bit					21
#define RCC_CFGR_MCO1_0_bit					21
#define RCC_CFGR_MCO1_1_bit					22

#define RCC_CFGR_RTCPRE_bit					16
#define RCC_CFGR_RTCPRE_0_bit				16
#define RCC_CFGR_RTCPRE_1_bit				17
#define RCC_CFGR_RTCPRE_2_bit				18
#define RCC_CFGR_RTCPRE_3_bit				19
#define RCC_CFGR_RTCPRE_4_bit				20

#define RCC_CFGR_PPRE2_bit					13
#define RCC_CFGR_PPRE2_0_bit				13
#define RCC_CFGR_PPRE2_1_bit				14
#define RCC_CFGR_PPRE2_2_bit				15

#define RCC_CFGR_PPRE1_bit					10
#define RCC_CFGR_PPRE1_0_bit				10
#define RCC_CFGR_PPRE1_1_bit				11
#define RCC_CFGR_PPRE1_2_bit				12

#define RCC_CFGR_MCO2EN_bit					9
#define RCC_CFGR_MCO1EN_bit					8

#define RCC_CFGR_HPRE_bit					4
#define RCC_CFGR_HPRE_0_bit					4
#define RCC_CFGR_HPRE_1_bit					5
#define RCC_CFGR_HPRE_2_bit					6
#define RCC_CFGR_HPRE_3_bit					7

#define RCC_CFGR_SWS_bit					2
#define RCC_CFGR_SWS_0_bit					2
#define RCC_CFGR_SWS_1_bit					3

#define RCC_CFGR_SW_bit						0
#define RCC_CFGR_SW_0_bit					0
#define RCC_CFGR_SW_1_bit					1

#define RCC_CFGR_MCO2_SYSCLK_value			0
#define RCC_CFGR_MCO2_PLLI2S_value			1
#define RCC_CFGR_MCO2_HSE_value				2
#define RCC_CFGR_MCO2_PLL_value				3
#define RCC_CFGR_MCO2_mask					3

#define RCC_CFGR_I2SSRC_PLLI2S_value		0
#define RCC_CFGR_I2SSRC_CKIN_value			1
#define RCC_CFGR_I2SSRC_mask				1

#define RCC_CFGR_MCO1_HSI_value				0
#define RCC_CFGR_MCO1_LSE_value				1
#define RCC_CFGR_MCO1_HSE_value				2
#define RCC_CFGR_MCO1_PLL_value				3
#define RCC_CFGR_MCO1_mask					3

#define RCC_CFGR_PPRE2_DIV1_value			0
#define RCC_CFGR_PPRE2_DIV2_value			4
#define RCC_CFGR_PPRE2_DIV4_value			5
#define RCC_CFGR_PPRE2_DIV8_value			6
#define RCC_CFGR_PPRE2_DIV16_value			7
#define RCC_CFGR_PPRE2_mask					7

#define RCC_CFGR_PPRE1_DIV1_value			0
#define RCC_CFGR_PPRE1_DIV2_value			4
#define RCC_CFGR_PPRE1_DIV4_value			5
#define RCC_CFGR_PPRE1_DIV8_value			6
#define RCC_CFGR_PPRE1_DIV16_value			7
#define RCC_CFGR_PPRE1_mask					7

#define RCC_CFGR_HPRE_DIV1_value			0
#define RCC_CFGR_HPRE_DIV2_value			8
#define RCC_CFGR_HPRE_DIV4_value			9
#define RCC_CFGR_HPRE_DIV8_value			10
#define RCC_CFGR_HPRE_DIV16_value			11
#define RCC_CFGR_HPRE_DIV64_value			12
#define RCC_CFGR_HPRE_DIV128_value			13
#define RCC_CFGR_HPRE_DIV256_value			14
#define RCC_CFGR_HPRE_DIV512_value			15
#define RCC_CFGR_HPRE_mask					15

#define RCC_CFGR_SWS_HSI_value				0
#define RCC_CFGR_SWS_HSE_value				1
#define RCC_CFGR_SWS_PLL_value				2
#define RCC_CFGR_SWS_PLL_R_value			3
#define RCC_CFGR_SWS_mask					3

#define RCC_CFGR_SW_HSI_value				0
#define RCC_CFGR_SW_HSE_value				1
#define RCC_CFGR_SW_PLL_value				2
#define RCC_CFGR_SW_PLL_R_value				3
#define RCC_CFGR_SW_mask					3

#define RCC_CFGR_MCO2_SYSCLK				(RCC_CFGR_MCO2_SYSCLK_value << RCC_CFGR_MCO2_bit)
#define RCC_CFGR_MCO2_PLLI2S				(RCC_CFGR_MCO2_PLLI2S_value << RCC_CFGR_MCO2_bit)
#define RCC_CFGR_MCO2_HSE					(RCC_CFGR_MCO2_HSE_value << RCC_CFGR_MCO2_bit)
#define RCC_CFGR_MCO2_PLL					(RCC_CFGR_MCO2_PLL_value << RCC_CFGR_MCO2_bit)

#define RCC_CFGR_I2SSRC_PLLI2S				(RCC_CFGR_I2SSRC_PLLI2S_value << RCC_CFGR_I2SSRC_bit)
#define RCC_CFGR_I2SSRC_CKIN				(RCC_CFGR_I2SSRC_CKIN_value << RCC_CFGR_I2SSRC_bit)

#define RCC_CFGR_MCO1_HSI					(RCC_CFGR_MCO1_HSI_value << RCC_CFGR_MCO1_bit)
#define RCC_CFGR_MCO1_LSE					(RCC_CFGR_MCO1_LSE_value << RCC_CFGR_MCO1_bit)
#define RCC_CFGR_MCO1_HSE					(RCC_CFGR_MCO1_HSE_value << RCC_CFGR_MCO1_bit)
#define RCC_CFGR_MCO1_PLL					(RCC_CFGR_MCO1_PLL_value << RCC_CFGR_MCO1_bit)

#define RCC_CFGR_SWS_PLL_R					(RCC_CFGR_SWS_PLL_R_value << RCC_CFGR_SWS_bit)

#define RCC_CFGR_SW_PLL_R					(RCC_CFGR_SW_PLL_R_value << RCC_CFGR_SW_bit)

#define RCC_CFGR_MCO2_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO2_0_bit)
#define RCC_CFGR_MCO2_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO2_1_bit)

#define RCC_CFGR_MCO2PRE_0_bb				BITBAND(&RCC->CFGR, RCC_CFGR_MCO2PRE_0_bit)
#define RCC_CFGR_MCO2PRE_1_bb				BITBAND(&RCC->CFGR, RCC_CFGR_MCO2PRE_1_bit)
#define RCC_CFGR_MCO2PRE_2_bb				BITBAND(&RCC->CFGR, RCC_CFGR_MCO2PRE_2_bit)

#define RCC_CFGR_MCO1PRE_0_bb				BITBAND(&RCC->CFGR, RCC_CFGR_MCO1PRE_0_bit)
#define RCC_CFGR_MCO1PRE_1_bb				BITBAND(&RCC->CFGR, RCC_CFGR_MCO1PRE_1_bit)
#define RCC_CFGR_MCO1PRE_2_bb				BITBAND(&RCC->CFGR, RCC_CFGR_MCO1PRE_2_bit)

#define RCC_CFGR_I2SSRC_bb					BITBAND(&RCC->CFGR, RCC_CFGR_I2SSRC_bit)

#define RCC_CFGR_MCO1_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO1_0_bit)
#define RCC_CFGR_MCO1_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO1_1_bit)

#define RCC_CFGR_RTCPRE_0_bb				BITBAND(&RCC->CFGR, RCC_CFGR_RTCPRE_0_bit)
#define RCC_CFGR_RTCPRE_1_bb				BITBAND(&RCC->CFGR, RCC_CFGR_RTCPRE_1_bit)
#define RCC_CFGR_RTCPRE_2_bb				BITBAND(&RCC->CFGR, RCC_CFGR_RTCPRE_2_bit)
#define RCC_CFGR_RTCPRE_3_bb				BITBAND(&RCC->CFGR, RCC_CFGR_RTCPRE_3_bit)
#define RCC_CFGR_RTCPRE_4_bb				BITBAND(&RCC->CFGR, RCC_CFGR_RTCPRE_4_bit)

#define RCC_CFGR_PPRE2_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE2_0_bit)
#define RCC_CFGR_PPRE2_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE2_1_bit)
#define RCC_CFGR_PPRE2_2_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE2_2_bit)

#define RCC_CFGR_PPRE1_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE1_0_bit)
#define RCC_CFGR_PPRE1_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE1_1_bit)
#define RCC_CFGR_PPRE1_2_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE1_2_bit)

#define RCC_CFGR_MCO2EN_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO2EN_bit)
#define RCC_CFGR_MCO1EN_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO1EN_bit)

#define RCC_CFGR_HPRE_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_HPRE_0_bit)
#define RCC_CFGR_HPRE_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_HPRE_1_bit)
#define RCC_CFGR_HPRE_2_bb					BITBAND(&RCC->CFGR, RCC_CFGR_HPRE_2_bit)
#define RCC_CFGR_HPRE_3_bb					BITBAND(&RCC->CFGR, RCC_CFGR_HPRE_3_bit)

#define RCC_CFGR_SWS_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_SWS_0_bit)
#define RCC_CFGR_SWS_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_SWS_1_bit)

#define RCC_CFGR_SW_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_SW_0_bit)
#define RCC_CFGR_SW_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_SW_1_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CIR - Clock interrupt register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CIR_CSSC_bit					23
#define RCC_CIR_PLLSAIRDYC_bit				22
#define RCC_CIR_PLLI2SRDYC_bit				21
#define RCC_CIR_PLLRDYC_bit					20
#define RCC_CIR_HSERDYC_bit					19
#define RCC_CIR_HSIRDYC_bit					18
#define RCC_CIR_LSERDYC_bit					17
#define RCC_CIR_LSIRDYC_bit					16
#define RCC_CIR_PLLSAIRDYIE_bit				14
#define RCC_CIR_PLLI2SRDYIE_bit				13
#define RCC_CIR_PLLRDYIE_bit				12
#define RCC_CIR_HSERDYIE_bit				11
#define RCC_CIR_HSIRDYIE_bit				10
#define RCC_CIR_LSERDYIE_bit				9
#define RCC_CIR_LSIRDYIE_bit				8
#define RCC_CIR_CSSF_bit					7
#define RCC_CIR_PLLSAIRDYF_bit				6
#define RCC_CIR_PLLI2SRDYF_bit				5
#define RCC_CIR_PLLRDYF_bit					4
#define RCC_CIR_HSERDYF_bit					3
#define RCC_CIR_HSIRDYF_bit					2
#define RCC_CIR_LSERDYF_bit					1
#define RCC_CIR_LSIRDYF_bit					0

#define RCC_CIR_CSSC_bb						BITBAND(&RCC->CIR, RCC_CIR_CSSC_bit)
#define RCC_CIR_PLLSAIRDYC_bb				BITBAND(&RCC->CIR, RCC_CIR_PLLSAIRDYC_bit)
#define RCC_CIR_PLLI2SRDYC_bb				BITBAND(&RCC->CIR, RCC_CIR_PLLI2SRDYC_bit)
#define RCC_CIR_PLLRDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_PLLRDYC_bit)
#define RCC_CIR_HSERDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_HSERDYC_bit)
#define RCC_CIR_HSIRDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_HSIRDYC_bit)
#define RCC_CIR_LSERDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_LSERDYC_bit)
#define RCC_CIR_LSIRDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_LSIRDYC_bit)
#define RCC_CIR_PLLSAIRDYIE_bb				BITBAND(&RCC->CIR, RCC_CIR_PLLSAIRDYIE_bit)
#define RCC_CIR_PLLI2SRDYIE_bb				BITBAND(&RCC->CIR, RCC_CIR_PLLI2SRDYIE_bit)
#define RCC_CIR_PLLRDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_PLLRDYIE_bit)
#define RCC_CIR_HSERDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_HSERDYIE_bit)
#define RCC_CIR_HSIRDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_HSIRDYIE_bit)
#define RCC_CIR_LSERDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_LSERDYIE_bit)
#define RCC_CIR_LSIRDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_LSIRDYIE_bit)
#define RCC_CIR_CSSF_bb						BITBAND(&RCC->CIR, RCC_CIR_CSSF_bit)
#define RCC_CIR_PLLSAIRDYF_bb				BITBAND(&RCC->CIR, RCC_CIR_PLLSAIRDYF_bit)
#define RCC_CIR_PLLI2SRDYF_bb				BITBAND(&RCC->CIR, RCC_CIR_PLLI2SRDYF_bit)
#define RCC_CIR_PLLRDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_PLLRDYF_bit)
#define RCC_CIR_HSERDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_HSERDYF_bit)
#define RCC_CIR_HSIRDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_HSIRDYF_bit)
#define RCC_CIR_LSERDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_LSERDYF_bit)
#define RCC_CIR_LSIRDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_LSIRDYF_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHB1RSTR - RCC AHB1 peripheral reset register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHB1RSTR_RNGRST_bit				31
#define RCC_AHB1RSTR_OTGHSRST_bit			29
#define RCC_AHB1RSTR_ETHMACRST_bit			25

#define RCC_AHB1RSTR_DMA2DRST_bit			23
#define RCC_AHB1RSTR_DMA2RST_bit			22
#define RCC_AHB1RSTR_DMA1RST_bit			21

#define RCC_AHB1RSTR_CRCRST_bit				12

#define RCC_AHB1RSTR_GPIOKRST_bit			10
#define RCC_AHB1RSTR_GPIOJRST_bit			9
#define RCC_AHB1RSTR_GPIOIRST_bit			8
#define RCC_AHB1RSTR_GPIOHRST_bit			7
#define RCC_AHB1RSTR_GPIOGRST_bit			6
#define RCC_AHB1RSTR_GPIOFRST_bit			5
#define RCC_AHB1RSTR_GPIOERST_bit			4
#define RCC_AHB1RSTR_GPIODRST_bit			3
#define RCC_AHB1RSTR_GPIOCRST_bit			2
#define RCC_AHB1RSTR_GPIOBRST_bit			1
#define RCC_AHB1RSTR_GPIOARST_bit			0

#define RCC_AHB1RSTR_RNGRST_bb				BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_RNGRST_bit)
#define RCC_AHB1RSTR_OTGHSRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_OTGHSRST_bit)
#define RCC_AHB1RSTR_ETHMACRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_ETHMACRST_bit)

#define RCC_AHB1RSTR_DMA2DRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_DMA2DRST_bit)
#define RCC_AHB1RSTR_DMA2RST_bb				BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_DMA2RST_bit)
#define RCC_AHB1RSTR_DMA1RST_bb				BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_DMA1RST_bit)

#define RCC_AHB1RSTR_CRCRST_bb				BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_CRCRST_bit)

#define RCC_AHB1RSTR_GPIOKRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOKRST_bit)
#define RCC_AHB1RSTR_GPIOJRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOJRST_bit)
#define RCC_AHB1RSTR_GPIOIRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOIRST_bit)
#define RCC_AHB1RSTR_GPIOHRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOHRST_bit)
#define RCC_AHB1RSTR_GPIOGRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOGRST_bit)
#define RCC_AHB1RSTR_GPIOFRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOFRST_bit)
#define RCC_AHB1RSTR_GPIOERST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOERST_bit)
#define RCC_AHB1RSTR_GPIODRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIODRST_bit)
#define RCC_AHB1RSTR_GPIOCRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOCRST_bit)
#define RCC_AHB1RSTR_GPIOBRST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOBRST_bit)
#define RCC_AHB1RSTR_GPIOARST_bb			BITBAND(&RCC->AHB1RSTR, RCC_AHB1RSTR_GPIOARST_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHB2RSTR - RCC AHB2 peripheral reset register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHB2RSTR_OTGFSRST_bit			7
#define RCC_AHB2RSTR_RNGRST_bit				6
#define RCC_AHB2RSTR_HASHRST_bit			5
#define RCC_AHB2RSTR_CRYPRST_bit			4
#define RCC_AHB2RSTR_DCMIRST_bit			0

#define RCC_AHB2RSTR_OTGFSRST_bb			BITBAND(&RCC->AHB2RSTR, RCC_AHB2RSTR_OTGFSRST_bit)
#define RCC_AHB2RSTR_RNGRST_bb				BITBAND(&RCC->AHB2RSTR, RCC_AHB2RSTR_RNGRST_bit)
#define RCC_AHB2RSTR_HASHRST_bb				BITBAND(&RCC->AHB2RSTR, RCC_AHB2RSTR_HASHRST_bit)
#define RCC_AHB2RSTR_CRYPRST_bb				BITBAND(&RCC->AHB2RSTR, RCC_AHB2RSTR_CRYPRST_bit)
#define RCC_AHB2RSTR_DCMIRST_bb				BITBAND(&RCC->AHB2RSTR, RCC_AHB2RSTR_DCMIRST_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHB3RSTR - RCC AHB3 peripheral reset register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHB3RSTR_QSPIRST_bit			1
#define RCC_AHB3RSTR_FMCRST_bit				0
#define RCC_AHB3RSTR_FSMCRST_bit			0

#define RCC_AHB3RSTR_QSPIRST_bb				BITBAND(&RCC->AHB3RSTR, RCC_AHB3RSTR_QSPIRST_bit)
#define RCC_AHB3RSTR_FMCRST_bb				BITBAND(&RCC->AHB3RSTR, RCC_AHB3RSTR_FMCRST_bit)
#define RCC_AHB3RSTR_FSMCRST_bb				BITBAND(&RCC->AHB3RSTR, RCC_AHB3RSTR_FSMCRST_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB1RSTR - APB1 peripheral reset register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB1RSTR_UART8RST_bit			31
#define RCC_APB1RSTR_UART7RST_bit			30

#define RCC_APB1RSTR_DACRST_bit				29
#define RCC_APB1RSTR_PWRRST_bit				28
#define RCC_APB1RSTR_CECRST_bit				27

#define RCC_APB1RSTR_CAN2RST_bit			26
#define RCC_APB1RSTR_CAN1RST_bit			25

#define RCC_APB1RSTR_FMPI2C1RST_bit			24
#define RCC_APB1RSTR_I2C3RST_bit			23
#define RCC_APB1RSTR_I2C2RST_bit			22
#define RCC_APB1RSTR_I2C1RST_bit			21

#define RCC_APB1RSTR_UART5RST_bit			20
#define RCC_APB1RSTR_UART4RST_bit			19
#define RCC_APB1RSTR_USART3RST_bit			18
#define RCC_APB1RSTR_USART2RST_bit			17

#define RCC_APB1RSTR_SPDIFRXRST_bit			16

#define RCC_APB1RSTR_SPI3RST_bit			15
#define RCC_APB1RSTR_SPI2RST_bit			14

#define RCC_APB1RSTR_WWDGRST_bit			11

#define RCC_APB1RSTR_LPTIM1RST_bit			9
#define RCC_APB1RSTR_TIM14RST_bit			8
#define RCC_APB1RSTR_TIM13RST_bit			7
#define RCC_APB1RSTR_TIM12RST_bit			6
#define RCC_APB1RSTR_TIM7RST_bit			5
#define RCC_APB1RSTR_TIM6RST_bit			4
#define RCC_APB1RSTR_TIM5RST_bit			3
#define RCC_APB1RSTR_TIM4RST_bit			2
#define RCC_APB1RSTR_TIM3RST_bit			1
#define RCC_APB1RSTR_TIM2RST_bit			0

#define RCC_APB1RSTR_UART8RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_UART8RST_bit)
#define RCC_APB1RSTR_UART7RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_UART7RST_bit)

#define RCC_APB1RSTR_DACRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_DACRST_bit)
#define RCC_APB1RSTR_PWRRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_PWRRST_bit)
#define RCC_APB1RSTR_CECRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_CECRST_bit)

#define RCC_APB1RSTR_CAN2RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_CAN2RST_bit)
#define RCC_APB1RSTR_CAN1RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_CAN1RST_bit)

#define RCC_APB1RSTR_FMPI2C1RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_FMPI2C1RST_bit)
#define RCC_APB1RSTR_I2C3RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_I2C3RST_bit)
#define RCC_APB1RSTR_I2C2RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_I2C2RST_bit)
#define RCC_APB1RSTR_I2C1RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_I2C1RST_bit)

#define RCC_APB1RSTR_UART5RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_UART5RST_bit)
#define RCC_APB1RSTR_UART4RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_UART4RST_bit)
#define RCC_APB1RSTR_USART3RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_USART3RST_bit)
#define RCC_APB1RSTR_USART2RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_USART2RST_bit)

#define RCC_APB1RSTR_SPDIFRXRST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_SPDIFRXRST_bit)

#define RCC_APB1RSTR_SPI3RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_SPI3RST_bit)
#define RCC_APB1RSTR_SPI2RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_SPI2RST_bit)

#define RCC_APB1RSTR_WWDGRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_WWDGRST_bit)

#define RCC_APB1RSTR_LPTIM1RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_LPTIM1RST_bit)
#define RCC_APB1RSTR_TIM14RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_TIM14RST_bit)
#define RCC_APB1RSTR_TIM13RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_TIM13RST_bit)
#define RCC_APB1RSTR_TIM12RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_TIM12RST_bit)
#define RCC_APB1RSTR_TIM7RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_TIM7RST_bit)
#define RCC_APB1RSTR_TIM6RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_TIM6RST_bit)
#define RCC_APB1RSTR_TIM5RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_TIM5RST_bit)
#define RCC_APB1RSTR_TIM4RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_TIM4RST_bit)
#define RCC_APB1RSTR_TIM3RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_TIM3RST_bit)
#define RCC_APB1RSTR_TIM2RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_TIM2RST_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB2RSTR - APB2 peripheral reset register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB2RSTR_DSIRST_bit				27
#define RCC_APB2RSTR_LTDCRST_bit			26

#define RCC_APB2RSTR_SAI2RST_bit			23
#define RCC_APB2RSTR_SAI1RST_bit			22

#define RCC_APB2RSTR_SPI6RST_bit			21
#define RCC_APB2RSTR_SPI5RST_bit			20

#define RCC_APB2RSTR_TIM11RST_bit			18
#define RCC_APB2RSTR_TIM10RST_bit			17
#define RCC_APB2RSTR_TIM9RST_bit			16

#define RCC_APB2RSTR_SYSCFGRST_bit			14

#define RCC_APB2RSTR_SPI4RST_bit			13
#define RCC_APB2RSTR_SPI1RST_bit			12

#define RCC_APB2RSTR_SDIORST_bit			11

#define RCC_APB2RSTR_ADC1RST_bit			8
#define RCC_APB2RSTR_ADCRST_bit				8

#define RCC_APB2RSTR_USART6RST_bit			5
#define RCC_APB2RSTR_USART1RST_bit			4

#define RCC_APB2RSTR_TIM8RST_bit			1
#define RCC_APB2RSTR_TIM1RST_bit			0

#define RCC_APB2RSTR_DSIRST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_DSIRST_bit)
#define RCC_APB2RSTR_LTDCRST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_LTDCRST_bit)

#define RCC_APB2RSTR_SAI2RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_SAI2RST_bit)
#define RCC_APB2RSTR_SAI1RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_SAI1RST_bit)

#define RCC_APB2RSTR_SPI6RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_SPI6RST_bit)
#define RCC_APB2RSTR_SPI5RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_SPI5RST_bit)

#define RCC_APB2RSTR_TIM11RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM11RST_bit)
#define RCC_APB2RSTR_TIM10RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM10RST_bit)
#define RCC_APB2RSTR_TIM9RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM9RST_bit)

#define RCC_APB2RSTR_SYSCFGRST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_SYSCFGRST_bit)

#define RCC_APB2RSTR_SPI4RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_SPI4RST_bit)
#define RCC_APB2RSTR_SPI1RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_SPI1RST_bit)

#define RCC_APB2RSTR_SDIORST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_SDIORST_bit)

#define RCC_APB2RSTR_ADC1RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_ADC1RST_bit)
#define RCC_APB2RSTR_ADCRST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_ADCRST_bit)

#define RCC_APB2RSTR_USART6RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_USART6RST_bit)
#define RCC_APB2RSTR_USART1RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_USART1RST_bit)

#define RCC_APB2RSTR_TIM8RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM8RST_bit)
#define RCC_APB2RSTR_TIM1RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHB1ENR - RCC AHB1 peripheral clock register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHB1ENR_RNGEN_bit				31

#define RCC_AHB1ENR_OTGHSULPIEN_bit			30
#define RCC_AHB1ENR_OTGHSEN_bit				29

#define RCC_AHB1ENR_ETHMACPTPEN_bit			28
#define RCC_AHB1ENR_ETHMACRXEN_bit			27
#define RCC_AHB1ENR_ETHMACTXEN_bit			26
#define RCC_AHB1ENR_ETHMACEN_bit			25

#define RCC_AHB1ENR_DMA2DEN_bit				23
#define RCC_AHB1ENR_DMA2EN_bit				22
#define RCC_AHB1ENR_DMA1EN_bit				21

#define RCC_AHB1ENR_CCMDATARAMEN_bit		20
#define RCC_AHB1ENR_BKPSRAMEN_bit			18
#define RCC_AHB1ENR_CRCEN_bit				12

#define RCC_AHB1ENR_GPIOKEN_bit				10
#define RCC_AHB1ENR_GPIOJEN_bit				9
#define RCC_AHB1ENR_GPIOIEN_bit				8
#define RCC_AHB1ENR_GPIOHEN_bit				7
#define RCC_AHB1ENR_GPIOGEN_bit				6
#define RCC_AHB1ENR_GPIOFEN_bit				5
#define RCC_AHB1ENR_GPIOEEN_bit				4
#define RCC_AHB1ENR_GPIODEN_bit				3
#define RCC_AHB1ENR_GPIOCEN_bit				2
#define RCC_AHB1ENR_GPIOBEN_bit				1
#define RCC_AHB1ENR_GPIOAEN_bit				0

#define RCC_AHB1ENR_RNGEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_RNGEN_bit)

#define RCC_AHB1ENR_OTGHSULPIEN_bb			BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_OTGHSULPIEN_bit)
#define RCC_AHB1ENR_OTGHSEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN_bit)

#define RCC_AHB1ENR_ETHMACPTPEN_bb			BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_ETHMACPTPEN_bit)
#define RCC_AHB1ENR_ETHMACRXEN_bb			BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_ETHMACRXEN_bit)
#define RCC_AHB1ENR_ETHMACTXEN_bb			BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_ETHMACTXEN_bit)
#define RCC_AHB1ENR_ETHMACEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_ETHMACEN_bit)

#define RCC_AHB1ENR_DMA2DEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_DMA2DEN_bit)
#define RCC_AHB1ENR_DMA2EN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN_bit)
#define RCC_AHB1ENR_DMA1EN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN_bit)

#define RCC_AHB1ENR_CCMDATARAMEN_bb			BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_CCMDATARAMEN_bit)
#define RCC_AHB1ENR_BKPSRAMEN_bb			BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_BKPSRAMEN_bit)
#define RCC_AHB1ENR_CRCEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_CRCEN_bit)

#define RCC_AHB1ENR_GPIOKEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOKEN_bit)
#define RCC_AHB1ENR_GPIOJEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOJEN_bit)
#define RCC_AHB1ENR_GPIOIEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOIEN_bit)
#define RCC_AHB1ENR_GPIOHEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOHEN_bit)
#define RCC_AHB1ENR_GPIOGEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN_bit)
#define RCC_AHB1ENR_GPIOFEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN_bit)
#define RCC_AHB1ENR_GPIOEEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN_bit)
#define RCC_AHB1ENR_GPIODEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN_bit)
#define RCC_AHB1ENR_GPIOCEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN_bit)
#define RCC_AHB1ENR_GPIOBEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN_bit)
#define RCC_AHB1ENR_GPIOAEN_bb				BITBAND(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHB2ENR - RCC AHB2 peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHB2ENR_OTGFSEN_bit				7
#define RCC_AHB2ENR_RNGEN_bit				6
#define RCC_AHB2ENR_HASHEN_bit				5
#define RCC_AHB2ENR_CRYPEN_bit				4
#define RCC_AHB2ENR_DCMIEN_bit				0

#define RCC_AHB2ENR_OTGFSEN_bb				BITBAND(&RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN_bit)
#define RCC_AHB2ENR_RNGEN_bb				BITBAND(&RCC->AHB2ENR, RCC_AHB2ENR_RNGEN_bit)
#define RCC_AHB2ENR_HASHEN_bb				BITBAND(&RCC->AHB2ENR, RCC_AHB2ENR_HASHEN_bit)
#define RCC_AHB2ENR_CRYPEN_bb				BITBAND(&RCC->AHB2ENR, RCC_AHB2ENR_CRYPEN_bit)
#define RCC_AHB2ENR_DCMIEN_bb				BITBAND(&RCC->AHB2ENR, RCC_AHB2ENR_DCMIEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHB3ENR - RCC AHB3 peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHB3ENR_QSPIEN_bit				1
#define RCC_AHB3ENR_FMCEN_bit				0
#define RCC_AHB3ENR_FSMCEN_bit				0

#define RCC_AHB3ENR_QSPIEN_bb				BITBAND(&RCC->AHB3ENR, RCC_AHB3ENR_QSPIEN_bit)
#define RCC_AHB3ENR_FMCEN_bb				BITBAND(&RCC->AHB3ENR, RCC_AHB3ENR_FMCEN_bit)
#define RCC_AHB3ENR_FSMCEN_bb				BITBAND(&RCC->AHB3ENR, RCC_AHB3ENR_FSMCEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB1ENR - RCC APB1 peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB1ENR_UART8EN_bit				31
#define RCC_APB1ENR_UART7EN_bit				30

#define RCC_APB1ENR_DACEN_bit				29
#define RCC_APB1ENR_PWREN_bit				28
#define RCC_APB1ENR_CECEN_bit				27

#define RCC_APB1ENR_CAN2EN_bit				26
#define RCC_APB1ENR_CAN1EN_bit				25

#define RCC_APB1ENR_FMPI2C1EN_bit			24
#define RCC_APB1ENR_I2C3EN_bit				23
#define RCC_APB1ENR_I2C2EN_bit				22
#define RCC_APB1ENR_I2C1EN_bit				21

#define RCC_APB1ENR_UART5EN_bit				20
#define RCC_APB1ENR_UART4EN_bit				19
#define RCC_APB1ENR_USART3EN_bit			18
#define RCC_APB1ENR_USART2EN_bit			17

#define RCC_APB1ENR_SPDIFRXEN_bit			16

#define RCC_APB1ENR_SPI3EN_bit				15
#define RCC_APB1ENR_SPI2EN_bit				14

#define RCC_APB1ENR_WWDGEN_bit				11

#define RCC_APB1ENR_RTCAPBEN_bit			10

#define RCC_APB1ENR_LPTIM1EN_bit			9
#define RCC_APB1ENR_TIM14EN_bit				8
#define RCC_APB1ENR_TIM13EN_bit				7
#define RCC_APB1ENR_TIM12EN_bit				6
#define RCC_APB1ENR_TIM7EN_bit				5
#define RCC_APB1ENR_TIM6EN_bit				4
#define RCC_APB1ENR_TIM5EN_bit				3
#define RCC_APB1ENR_TIM4EN_bit				2
#define RCC_APB1ENR_TIM3EN_bit				1
#define RCC_APB1ENR_TIM2EN_bit				0

#define RCC_APB1ENR_UART8EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_UART8EN_bit)
#define RCC_APB1ENR_UART7EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_UART7EN_bit)

#define RCC_APB1ENR_DACEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_DACEN_bit)
#define RCC_APB1ENR_PWREN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_PWREN_bit)
#define RCC_APB1ENR_CECEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_CECEN_bit)

#define RCC_APB1ENR_CAN2EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_CAN2EN_bit)
#define RCC_APB1ENR_CAN1EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_CAN1EN_bit)

#define RCC_APB1ENR_FMPI2C1EN_bb			BITBAND(&RCC->APB1ENR, RCC_APB1ENR_FMPI2C1EN_bit)
#define RCC_APB1ENR_I2C3EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_I2C3EN_bit)
#define RCC_APB1ENR_I2C2EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_I2C2EN_bit)
#define RCC_APB1ENR_I2C1EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_I2C1EN_bit)

#define RCC_APB1ENR_UART5EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_UART5EN_bit)
#define RCC_APB1ENR_UART4EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_UART4EN_bit)
#define RCC_APB1ENR_USART3EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_USART3EN_bit)
#define RCC_APB1ENR_USART2EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_USART2EN_bit)

#define RCC_APB1ENR_SPDIFRXEN_bb			BITBAND(&RCC->APB1ENR, RCC_APB1ENR_SPDIFRXEN_bit)

#define RCC_APB1ENR_SPI3EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_SPI3EN_bit)
#define RCC_APB1ENR_SPI2EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_SPI2EN_bit)

#define RCC_APB1ENR_WWDGEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_WWDGEN_bit)

#define RCC_APB1ENR_RTCAPBEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_RTCAPBEN_bit)

#define RCC_APB1ENR_LPTIM1EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_LPTIM1EN_bit)
#define RCC_APB1ENR_TIM14EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_TIM14EN_bit)
#define RCC_APB1ENR_TIM13EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_TIM13EN_bit)
#define RCC_APB1ENR_TIM12EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_TIM12EN_bit)
#define RCC_APB1ENR_TIM7EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_TIM7EN_bit)
#define RCC_APB1ENR_TIM6EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_TIM6EN_bit)
#define RCC_APB1ENR_TIM5EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_TIM5EN_bit)
#define RCC_APB1ENR_TIM4EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_TIM4EN_bit)
#define RCC_APB1ENR_TIM3EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_TIM3EN_bit)
#define RCC_APB1ENR_TIM2EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_TIM2EN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB2ENR - RCC APB2 peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB2ENR_DSIEN_bit				27
#define RCC_APB2ENR_LTDCEN_bit				26

#define RCC_APB2ENR_SAI2EN_bit				23
#define RCC_APB2ENR_SAI1EN_bit				22

#define RCC_APB2ENR_SPI6EN_bit				21
#define RCC_APB2ENR_SPI5EN_bit				20

#define RCC_APB2ENR_TIM11EN_bit				18
#define RCC_APB2ENR_TIM10EN_bit				17
#define RCC_APB2ENR_TIM9EN_bit				16

#define RCC_APB2ENR_EXTIEN_bit				15
#define RCC_APB2ENR_SYSCFGEN_bit			14

#define RCC_APB2ENR_SPI4EN_bit				13
#define RCC_APB2ENR_SPI1EN_bit				12

#define RCC_APB2ENR_SDIOEN_bit				11

#define RCC_APB2ENR_ADC3EN_bit				10
#define RCC_APB2ENR_ADC2EN_bit				9
#define RCC_APB2ENR_ADC1EN_bit				8

#define RCC_APB2ENR_USART6EN_bit			5
#define RCC_APB2ENR_USART1EN_bit			4

#define RCC_APB2ENR_TIM8EN_bit				1
#define RCC_APB2ENR_TIM1EN_bit				0

#define RCC_APB2ENR_DSIEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_DSIEN_bit)
#define RCC_APB2ENR_LTDCEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_LTDCEN_bit)

#define RCC_APB2ENR_SAI2EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_SAI2EN_bit)
#define RCC_APB2ENR_SAI1EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_SAI1EN_bit)

#define RCC_APB2ENR_SPI6EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_SPI6EN_bit)
#define RCC_APB2ENR_SPI5EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_SPI5EN_bit)

#define RCC_APB2ENR_TIM11EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM11EN_bit)
#define RCC_APB2ENR_TIM10EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM10EN_bit)
#define RCC_APB2ENR_TIM9EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM9EN_bit)

#define RCC_APB2ENR_EXTIEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_EXTIEN_bit)
#define RCC_APB2ENR_SYSCFGEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN_bit)

#define RCC_APB2ENR_SPI4EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_SPI4EN_bit)
#define RCC_APB2ENR_SPI1EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_SPI1EN_bit)

#define RCC_APB2ENR_SDIOEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_SDIOEN_bit)

#define RCC_APB2ENR_ADC3EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_ADC3EN_bit)
#define RCC_APB2ENR_ADC2EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_ADC2EN_bit)
#define RCC_APB2ENR_ADC1EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_ADC1EN_bit)

#define RCC_APB2ENR_USART6EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_USART6EN_bit)
#define RCC_APB2ENR_USART1EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_USART1EN_bit)

#define RCC_APB2ENR_TIM8EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM8EN_bit)
#define RCC_APB2ENR_TIM1EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM1EN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHB1LPENR - RCC AHB1 peripheral clock enable in low power mode register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHB1LPENR_RNGLPEN_bit			31

#define RCC_AHB1LPENR_OTGHSULPILPEN_bit		30
#define RCC_AHB1LPENR_OTGHSLPEN_bit			29

#define RCC_AHB1LPENR_ETHMACPTPLPEN_bit		28
#define RCC_AHB1LPENR_ETHMACRXLPEN_bit		27
#define RCC_AHB1LPENR_ETHMACTXLPEN_bit		26
#define RCC_AHB1LPENR_ETHMACLPEN_bit		25

#define RCC_AHB1LPENR_DMA2DLPEN_bit			23
#define RCC_AHB1LPENR_DMA2LPEN_bit			22
#define RCC_AHB1LPENR_DMA1LPEN_bit			21

#define RCC_AHB1LPENR_SRAM3LPEN_bit			19
#define RCC_AHB1LPENR_BKPSRAMLPEN_bit		18
#define RCC_AHB1LPENR_SRAM2LPEN_bit			17
#define RCC_AHB1LPENR_SRAM1LPEN_bit			16

#define RCC_AHB1LPENR_FLITFLPEN_bit			15
#define RCC_AHB1LPENR_CRCLPEN_bit			12

#define RCC_AHB1LPENR_GPIOKLPEN_bit			10
#define RCC_AHB1LPENR_GPIOJLPEN_bit			9
#define RCC_AHB1LPENR_GPIOILPEN_bit			8
#define RCC_AHB1LPENR_GPIOHLPEN_bit			7
#define RCC_AHB1LPENR_GPIOGLPEN_bit			6
#define RCC_AHB1LPENR_GPIOFLPEN_bit			5
#define RCC_AHB1LPENR_GPIOELPEN_bit			4
#define RCC_AHB1LPENR_GPIODLPEN_bit			3
#define RCC_AHB1LPENR_GPIOCLPEN_bit			2
#define RCC_AHB1LPENR_GPIOBLPEN_bit			1
#define RCC_AHB1LPENR_GPIOALPEN_bit			0

#define RCC_AHB1LPENR_RNGLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_RNGLPEN_bit)

#define RCC_AHB1LPENR_OTGHSULPILPEN_bb		BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_OTGHSULPILPEN_bit)
#define RCC_AHB1LPENR_OTGHSLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_OTGHSLPEN_bit)

#define RCC_AHB1LPENR_ETHMACPTPLPEN_bb		BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_ETHMACPTPLPEN_bit)
#define RCC_AHB1LPENR_ETHMACRXLPEN_bb		BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_ETHMACRXLPEN_bit)
#define RCC_AHB1LPENR_ETHMACTXLPEN_bb		BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_ETHMACTXLPEN_bit)
#define RCC_AHB1LPENR_ETHMACLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_ETHMACLPEN_bit)

#define RCC_AHB1LPENR_DMA2DLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_DMA2DLPEN_bit)
#define RCC_AHB1LPENR_DMA2LPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_DMA2LPEN_bit)
#define RCC_AHB1LPENR_DMA1LPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_DMA1LPEN_bit)

#define RCC_AHB1LPENR_SRAM3LPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_SRAM3LPEN_bit)
#define RCC_AHB1LPENR_BKPSRAMLPEN_bb		BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_BKPSRAMLPEN_bit)
#define RCC_AHB1LPENR_SRAM2LPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_SRAM2LPEN_bit)
#define RCC_AHB1LPENR_SRAM1LPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_SRAM1LPEN_bit)

#define RCC_AHB1LPENR_FLITFLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_FLITFLPEN_bit)
#define RCC_AHB1LPENR_CRCLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_CRCLPEN_bit)

#define RCC_AHB1LPENR_GPIOKLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOKLPEN_bit)
#define RCC_AHB1LPENR_GPIOJLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOJLPEN_bit)
#define RCC_AHB1LPENR_GPIOILPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOILPEN_bit)
#define RCC_AHB1LPENR_GPIOHLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOHLPEN_bit)
#define RCC_AHB1LPENR_GPIOGLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOGLPEN_bit)
#define RCC_AHB1LPENR_GPIOFLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOFLPEN_bit)
#define RCC_AHB1LPENR_GPIOELPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOELPEN_bit)
#define RCC_AHB1LPENR_GPIODLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIODLPEN_bit)
#define RCC_AHB1LPENR_GPIOCLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOCLPEN_bit)
#define RCC_AHB1LPENR_GPIOBLPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOBLPEN_bit)
#define RCC_AHB1LPENR_GPIOALPEN_bb			BITBAND(&RCC->AHB1LPENR, RCC_AHB1LPENR_GPIOALPEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHB2LPENR - RCC AHB2 peripheral clock enable in low power mode register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHB2LPENR_OTGFSLPEN_bit			7
#define RCC_AHB2LPENR_RNGLPEN_bit			6
#define RCC_AHB2LPENR_HASHLPEN_bit			5
#define RCC_AHB2LPENR_CRYPLPEN_bit			4
#define RCC_AHB2LPENR_DCMILPEN_bit			0

#define RCC_AHB2LPENR_OTGFSLPEN_bb			BITBAND(&RCC->AHB2LPENR, RCC_AHB2LPENR_OTGFSLPEN_bit)
#define RCC_AHB2LPENR_RNGLPEN_bb			BITBAND(&RCC->AHB2LPENR, RCC_AHB2LPENR_RNGLPEN_bit)
#define RCC_AHB2LPENR_HASHLPEN_bb			BITBAND(&RCC->AHB2LPENR, RCC_AHB2LPENR_HASHLPEN_bit)
#define RCC_AHB2LPENR_CRYPLPEN_bb			BITBAND(&RCC->AHB2LPENR, RCC_AHB2LPENR_CRYPLPEN_bit)
#define RCC_AHB2LPENR_DCMILPEN_bb			BITBAND(&RCC->AHB2LPENR, RCC_AHB2LPENR_DCMILPEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHB3LPENR - RCC AHB3 peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHB3LPENR_QSPILPEN_bit			1
#define RCC_AHB3LPENR_FMCLPEN_bit			0
#define RCC_AHB3LPENR_FSMCLPEN_bit			0

#define RCC_AHB3LPENR_QSPILPEN_bb			BITBAND(&RCC->AHB3LPENR, RCC_AHB3LPENR_QSPILPEN_bit)
#define RCC_AHB3LPENR_FMCLPEN_bb			BITBAND(&RCC->AHB3LPENR, RCC_AHB3LPENR_FMCLPEN_bit)
#define RCC_AHB3LPENR_FSMCLPEN_bb			BITBAND(&RCC->AHB3LPENR, RCC_AHB3LPENR_FSMCLPEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB1LPENR - RCC APB1 peripheral clock enable in low power mode register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB1LPENR_UART8LPEN_bit			31
#define RCC_APB1LPENR_UART7LPEN_bit			30

#define RCC_APB1LPENR_DACLPEN_bit			29
#define RCC_APB1LPENR_PWRLPEN_bit			28

#define RCC_APB1LPENR_CECLPEN_bit			27

#define RCC_APB1LPENR_CAN2LPEN_bit			26
#define RCC_APB1LPENR_CAN1LPEN_bit			25

#define RCC_APB1LPENR_FMPI2C1LPEN_bit		24
#define RCC_APB1LPENR_I2C3LPEN_bit			23
#define RCC_APB1LPENR_I2C2LPEN_bit			22
#define RCC_APB1LPENR_I2C1LPEN_bit			21

#define RCC_APB1LPENR_UART5LPEN_bit			20
#define RCC_APB1LPENR_UART4LPEN_bit			19
#define RCC_APB1LPENR_USART3LPEN_bit		18
#define RCC_APB1LPENR_USART2LPEN_bit		17

#define RCC_APB1LPENR_SPDIFRXLPEN_bit		16

#define RCC_APB1LPENR_SPI3LPEN_bit			15
#define RCC_APB1LPENR_SPI2LPEN_bit			14

#define RCC_APB1LPENR_WWDGLPEN_bit			11

#define RCC_APB1LPENR_RTCAPBLPEN_bit		10

#define RCC_APB1LPENR_LPTIM1LPEN_bit		9
#define RCC_APB1LPENR_TIM14LPEN_bit			8
#define RCC_APB1LPENR_TIM13LPEN_bit			7
#define RCC_APB1LPENR_TIM12LPEN_bit			6
#define RCC_APB1LPENR_TIM7LPEN_bit			5
#define RCC_APB1LPENR_TIM6LPEN_bit			4
#define RCC_APB1LPENR_TIM5LPEN_bit			3
#define RCC_APB1LPENR_TIM4LPEN_bit			2
#define RCC_APB1LPENR_TIM3LPEN_bit			1
#define RCC_APB1LPENR_TIM2LPEN_bit			0

#define RCC_APB1LPENR_UART8LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_UART8LPEN_bit)
#define RCC_APB1LPENR_UART7LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_UART7LPEN_bit)

#define RCC_APB1LPENR_DACLPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_DACLPEN_bit)
#define RCC_APB1LPENR_PWRLPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_PWRLPEN_bit)

#define RCC_APB1LPENR_CECLPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_CECLPEN_bit)

#define RCC_APB1LPENR_CAN2LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_CAN2LPEN_bit)
#define RCC_APB1LPENR_CAN1LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_CAN1LPEN_bit)

#define RCC_APB1LPENR_FMPI2C1LPEN_bb		BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_FMPI2C1LPEN_bit)
#define RCC_APB1LPENR_I2C3LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_I2C3LPEN_bit)
#define RCC_APB1LPENR_I2C2LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_I2C2LPEN_bit)
#define RCC_APB1LPENR_I2C1LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_I2C1LPEN_bit)

#define RCC_APB1LPENR_UART5LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_UART5LPEN_bit)
#define RCC_APB1LPENR_UART4LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_UART4LPEN_bit)
#define RCC_APB1LPENR_USART3LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_USART3LPEN_bit)
#define RCC_APB1LPENR_USART2LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_USART2LPEN_bit)

#define RCC_APB1LPENR_SPDIFRXLPEN_bb		BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_SPDIFRXLPEN_bit)

#define RCC_APB1LPENR_SPI3LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_SPI3LPEN_bit)
#define RCC_APB1LPENR_SPI2LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_SPI2LPEN_bit)

#define RCC_APB1LPENR_WWDGLPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_WWDGLPEN_bit)

#define RCC_APB1LPENR_RTCAPBLPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_RTCAPBLPEN_bit)

#define RCC_APB1LPENR_LPTIM1LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_LPTIM1LPEN_bit)
#define RCC_APB1LPENR_TIM14LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_TIM14LPEN_bit)
#define RCC_APB1LPENR_TIM13LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_TIM13LPEN_bit)
#define RCC_APB1LPENR_TIM12LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_TIM12LPEN_bit)
#define RCC_APB1LPENR_TIM7LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_TIM7LPEN_bit)
#define RCC_APB1LPENR_TIM6LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_TIM6LPEN_bit)
#define RCC_APB1LPENR_TIM5LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_TIM5LPEN_bit)
#define RCC_APB1LPENR_TIM4LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_TIM4LPEN_bit)
#define RCC_APB1LPENR_TIM3LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_TIM3LPEN_bit)
#define RCC_APB1LPENR_TIM2LPEN_bb			BITBAND(&RCC->APB1LPENR, RCC_APB1LPENR_TIM2LPEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB2LPENR - RCC APB2 peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB2LPENR_DSILPEN_bit			27
#define RCC_APB2LPENR_LTDCLPEN_bit			26

#define RCC_APB2LPENR_SAI2LPEN_bit			23
#define RCC_APB2LPENR_SAI1LPEN_bit			22

#define RCC_APB2LPENR_SPI6LPEN_bit			21
#define RCC_APB2LPENR_SPI5LPEN_bit			20

#define RCC_APB2LPENR_TIM11LPEN_bit			18
#define RCC_APB2LPENR_TIM10LPEN_bit			17
#define RCC_APB2LPENR_TIM9LPEN_bit			16

#define RCC_APB2LPENR_EXTILPEN_bit			15

#define RCC_APB2LPENR_SYSCFGLPEN_bit		14

#define RCC_APB2LPENR_SPI4LPEN_bit			13
#define RCC_APB2LPENR_SPI1LPEN_bit			12

#define RCC_APB2LPENR_SDIOLPEN_bit			11

#define RCC_APB2LPENR_ADC3LPEN_bit			10
#define RCC_APB2LPENR_ADC2LPEN_bit			9
#define RCC_APB2LPENR_ADC1LPEN_bit			8

#define RCC_APB2LPENR_USART6LPEN_bit		5
#define RCC_APB2LPENR_USART1LPEN_bit		4

#define RCC_APB2LPENR_TIM8LPEN_bit			1
#define RCC_APB2LPENR_TIM1LPEN_bit			0

#define RCC_APB2LPENR_DSILPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_DSILPEN_bit)
#define RCC_APB2LPENR_LTDCLPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_LTDCLPEN_bit)

#define RCC_APB2LPENR_SAI2LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_SAI2LPEN_bit)
#define RCC_APB2LPENR_SAI1LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_SAI1LPEN_bit)

#define RCC_APB2LPENR_SPI6LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_SPI6LPEN_bit)
#define RCC_APB2LPENR_SPI5LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_SPI5LPEN_bit)

#define RCC_APB2LPENR_TIM11LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_TIM11LPEN_bit)
#define RCC_APB2LPENR_TIM10LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_TIM10LPEN_bit)
#define RCC_APB2LPENR_TIM9LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_TIM9LPEN_bit)

#define RCC_APB2LPENR_EXTILPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_EXTILPEN_bit)

#define RCC_APB2LPENR_SYSCFGLPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_SYSCFGLPEN_bit)

#define RCC_APB2LPENR_SPI4LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_SPI4LPEN_bit)
#define RCC_APB2LPENR_SPI1LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_SPI1LPEN_bit)

#define RCC_APB2LPENR_SDIOLPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_SDIOLPEN_bit)

#define RCC_APB2LPENR_ADC3LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_ADC3LPEN_bit)
#define RCC_APB2LPENR_ADC2LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_ADC2LPEN_bit)
#define RCC_APB2LPENR_ADC1LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_ADC1LPEN_bit)

#define RCC_APB2LPENR_USART6LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_USART6LPEN_bit)
#define RCC_APB2LPENR_USART1LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_USART1LPEN_bit)

#define RCC_APB2LPENR_TIM8LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_TIM8LPEN_bit)
#define RCC_APB2LPENR_TIM1LPEN_bb			BITBAND(&RCC->APB2LPENR, RCC_APB2LPENR_TIM1LPEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_BDCR - Backup domain control register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_BDCR_BDRST_bit					16
#define RCC_BDCR_RTCEN_bit					15

#define RCC_BDCR_RTCSEL_bit					8
#define RCC_BDCR_RTCSEL_0_bit				8
#define RCC_BDCR_RTCSEL_1_bit				9

#define RCC_BDCR_LSEMOD_bit					3
#define RCC_BDCR_LSEBYP_bit					2
#define RCC_BDCR_LSERDY_bit					1
#define RCC_BDCR_LSEON_bit					0

#define RCC_BDCR_RTCSEL_NOCLOCK_value		0
#define RCC_BDCR_RTCSEL_LSE_value			1
#define RCC_BDCR_RTCSEL_LSI_value			2
#define RCC_BDCR_RTCSEL_HSE_value			3
#define RCC_BDCR_RTCSEL_mask				3

#define RCC_BDCR_BDRST_bb					BITBAND(&RCC->BDCR, RCC_BDCR_BDRST_bit)
#define RCC_BDCR_RTCEN_bb					BITBAND(&RCC->BDCR, RCC_BDCR_RTCEN_bit)
#define RCC_BDCR_RTCSEL_0_bb				BITBAND(&RCC->BDCR, RCC_BDCR_RTCSEL_0_bit)
#define RCC_BDCR_RTCSEL_1_bb				BITBAND(&RCC->BDCR, RCC_BDCR_RTCSEL_1_bit)
#define RCC_BDCR_LSEMOD_bb					BITBAND(&RCC->BDCR, RCC_BDCR_LSEMOD_bit)
#define RCC_BDCR_LSEBYP_bb					BITBAND(&RCC->BDCR, RCC_BDCR_LSEBYP_bit)
#define RCC_BDCR_LSERDY_bb					BITBAND(&RCC->BDCR, RCC_BDCR_LSERDY_bit)
#define RCC_BDCR_LSEON_bb					BITBAND(&RCC->BDCR, RCC_BDCR_LSEON_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CSR - Control/status register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CSR_LPWRRSTF_bit				31
#define RCC_CSR_WWDGRSTF_bit				30
#define RCC_CSR_IWDGRSTF_bit				29
#define RCC_CSR_SFTRSTF_bit					28
#define RCC_CSR_PORRSTF_bit					27
#define RCC_CSR_PINRSTF_bit					26
#define RCC_CSR_BORRSTF_bit					25
#define RCC_CSR_RMVF_bit					24

#define RCC_CSR_LSIRDY_bit					1
#define RCC_CSR_LSION_bit					0

#define RCC_CSR_LPWRRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_LPWRRSTF_bit)
#define RCC_CSR_WWDGRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_WWDGRSTF_bit)
#define RCC_CSR_IWDGRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_IWDGRSTF_bit)
#define RCC_CSR_SFTRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_SFTRSTF_bit)
#define RCC_CSR_PORRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_PORRSTF_bit)
#define RCC_CSR_PINRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_PINRSTF_bit)
#define RCC_CSR_BORRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_BORRSTF_bit)
#define RCC_CSR_RMVF_bb						BITBAND(&RCC->CSR, RCC_CSR_RMVF_bit)

#define RCC_CSR_LSIRDY_bb					BITBAND(&RCC->CSR, RCC_CSR_LSIRDY_bit)
#define RCC_CSR_LSION_bb					BITBAND(&RCC->CSR, RCC_CSR_LSION_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_SSCGR - RCC spread spectrum clock generation register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_SSCGR_SSCGEN_bit				31
#define RCC_SSCGR_SPREADSEL_bit				30

#define RCC_SSCGR_INCSTEP_bit				13
#define RCC_SSCGR_INCSTEP_0_bit				13
#define RCC_SSCGR_INCSTEP_1_bit				14
#define RCC_SSCGR_INCSTEP_2_bit				15
#define RCC_SSCGR_INCSTEP_3_bit				16
#define RCC_SSCGR_INCSTEP_4_bit				17
#define RCC_SSCGR_INCSTEP_5_bit				18
#define RCC_SSCGR_INCSTEP_6_bit				19
#define RCC_SSCGR_INCSTEP_7_bit				20
#define RCC_SSCGR_INCSTEP_8_bit				21
#define RCC_SSCGR_INCSTEP_9_bit				22
#define RCC_SSCGR_INCSTEP_10_bit			23
#define RCC_SSCGR_INCSTEP_11_bit			24
#define RCC_SSCGR_INCSTEP_12_bit			25
#define RCC_SSCGR_INCSTEP_13_bit			26
#define RCC_SSCGR_INCSTEP_14_bit			27

#define RCC_SSCGR_MODPER_bit				0
#define RCC_SSCGR_MODPER_0_bit				0
#define RCC_SSCGR_MODPER_1_bit				1
#define RCC_SSCGR_MODPER_2_bit				2
#define RCC_SSCGR_MODPER_3_bit				3
#define RCC_SSCGR_MODPER_4_bit				4
#define RCC_SSCGR_MODPER_5_bit				5
#define RCC_SSCGR_MODPER_6_bit				6
#define RCC_SSCGR_MODPER_7_bit				7
#define RCC_SSCGR_MODPER_8_bit				8
#define RCC_SSCGR_MODPER_9_bit				9
#define RCC_SSCGR_MODPER_10_bit				10
#define RCC_SSCGR_MODPER_11_bit				11
#define RCC_SSCGR_MODPER_12_bit				12

#define RCC_SSCGR_SSCGEN_bb					BITBAND(&RCC->SSCGR, RCC_SSCGR_SSCGEN_bit)
#define RCC_SSCGR_SPREADSEL_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_SPREADSEL_bit)

#define RCC_SSCGR_INCSTEP_0_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_0_bit)
#define RCC_SSCGR_INCSTEP_1_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_1_bit)
#define RCC_SSCGR_INCSTEP_2_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_2_bit)
#define RCC_SSCGR_INCSTEP_3_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_3_bit)
#define RCC_SSCGR_INCSTEP_4_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_4_bit)
#define RCC_SSCGR_INCSTEP_5_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_5_bit)
#define RCC_SSCGR_INCSTEP_6_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_6_bit)
#define RCC_SSCGR_INCSTEP_7_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_7_bit)
#define RCC_SSCGR_INCSTEP_8_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_8_bit)
#define RCC_SSCGR_INCSTEP_9_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_9_bit)
#define RCC_SSCGR_INCSTEP_10_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_10_bit)
#define RCC_SSCGR_INCSTEP_11_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_11_bit)
#define RCC_SSCGR_INCSTEP_12_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_12_bit)
#define RCC_SSCGR_INCSTEP_13_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_13_bit)
#define RCC_SSCGR_INCSTEP_14_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_INCSTEP_14_bit)

#define RCC_SSCGR_MODPER_0_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_0_bit)
#define RCC_SSCGR_MODPER_1_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_1_bit)
#define RCC_SSCGR_MODPER_2_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_2_bit)
#define RCC_SSCGR_MODPER_3_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_3_bit)
#define RCC_SSCGR_MODPER_4_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_4_bit)
#define RCC_SSCGR_MODPER_5_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_5_bit)
#define RCC_SSCGR_MODPER_6_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_6_bit)
#define RCC_SSCGR_MODPER_7_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_7_bit)
#define RCC_SSCGR_MODPER_8_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_8_bit)
#define RCC_SSCGR_MODPER_9_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_9_bit)
#define RCC_SSCGR_MODPER_10_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_10_bit)
#define RCC_SSCGR_MODPER_11_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_11_bit)
#define RCC_SSCGR_MODPER_12_bb				BITBAND(&RCC->SSCGR, RCC_SSCGR_MODPER_12_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_PLLI2SCFGR - RCC PLLI2S configuration register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_PLLI2SCFGR_PLLI2SR_bit			28
#define RCC_PLLI2SCFGR_PLLI2SR_0_bit		28
#define RCC_PLLI2SCFGR_PLLI2SR_1_bit		29
#define RCC_PLLI2SCFGR_PLLI2SR_2_bit		30

#define RCC_PLLI2SCFGR_PLLI2SQ_bit			24
#define RCC_PLLI2SCFGR_PLLI2SQ_0_bit		24
#define RCC_PLLI2SCFGR_PLLI2SQ_1_bit		25
#define RCC_PLLI2SCFGR_PLLI2SQ_2_bit		26
#define RCC_PLLI2SCFGR_PLLI2SQ_3_bit		27

#define RCC_PLLI2SCFGR_PLLI2SP_bit			16
#define RCC_PLLI2SCFGR_PLLI2SP_0_bit		16
#define RCC_PLLI2SCFGR_PLLI2SP_1_bit		17

#define RCC_PLLI2SCFGR_PLLI2SN_bit			6
#define RCC_PLLI2SCFGR_PLLI2SN_0_bit		6
#define RCC_PLLI2SCFGR_PLLI2SN_1_bit		7
#define RCC_PLLI2SCFGR_PLLI2SN_2_bit		8
#define RCC_PLLI2SCFGR_PLLI2SN_3_bit		9
#define RCC_PLLI2SCFGR_PLLI2SN_4_bit		10
#define RCC_PLLI2SCFGR_PLLI2SN_5_bit		11
#define RCC_PLLI2SCFGR_PLLI2SN_6_bit		12
#define RCC_PLLI2SCFGR_PLLI2SN_7_bit		13
#define RCC_PLLI2SCFGR_PLLI2SN_8_bit		14

#define RCC_PLLI2SCFGR_PLLI2SM_bit			0
#define RCC_PLLI2SCFGR_PLLI2SM_0_bit		0
#define RCC_PLLI2SCFGR_PLLI2SM_1_bit		1
#define RCC_PLLI2SCFGR_PLLI2SM_2_bit		2
#define RCC_PLLI2SCFGR_PLLI2SM_3_bit		3
#define RCC_PLLI2SCFGR_PLLI2SM_4_bit		4
#define RCC_PLLI2SCFGR_PLLI2SM_5_bit		5

#define RCC_PLLI2SCFGR_PLLI2SP_DIV2_value	0
#define RCC_PLLI2SCFGR_PLLI2SP_DIV4_value	1
#define RCC_PLLI2SCFGR_PLLI2SP_DIV6_value	2
#define RCC_PLLI2SCFGR_PLLI2SP_DIV8_value	3
#define RCC_PLLI2SCFGR_PLLI2SP_mask			3

#define RCC_PLLI2SCFGR_PLLI2SP_DIV2			(RCC_PLLI2SCFGR_PLLI2SP_DIV2_value << RCC_PLLI2SCFGR_PLLI2SP_bit)
#define RCC_PLLI2SCFGR_PLLI2SP_DIV4			(RCC_PLLI2SCFGR_PLLI2SP_DIV4_value << RCC_PLLI2SCFGR_PLLI2SP_bit)
#define RCC_PLLI2SCFGR_PLLI2SP_DIV6			(RCC_PLLI2SCFGR_PLLI2SP_DIV6_value << RCC_PLLI2SCFGR_PLLI2SP_bit)
#define RCC_PLLI2SCFGR_PLLI2SP_DIV8			(RCC_PLLI2SCFGR_PLLI2SP_DIV8_value << RCC_PLLI2SCFGR_PLLI2SP_bit)

#define RCC_PLLI2SCFGR_PLLI2SR_0_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SR_0_bit)
#define RCC_PLLI2SCFGR_PLLI2SR_1_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SR_1_bit)
#define RCC_PLLI2SCFGR_PLLI2SR_2_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SR_2_bit)

#define RCC_PLLI2SCFGR_PLLI2SQ_0_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SQ_0_bit)
#define RCC_PLLI2SCFGR_PLLI2SQ_1_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SQ_1_bit)
#define RCC_PLLI2SCFGR_PLLI2SQ_2_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SQ_2_bit)
#define RCC_PLLI2SCFGR_PLLI2SQ_3_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SQ_3_bit)

#define RCC_PLLI2SCFGR_PLLI2SP_0_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SP_0_bit)
#define RCC_PLLI2SCFGR_PLLI2SP_1_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SP_1_bit)

#define RCC_PLLI2SCFGR_PLLI2SN_0_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SN_0_bit)
#define RCC_PLLI2SCFGR_PLLI2SN_1_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SN_1_bit)
#define RCC_PLLI2SCFGR_PLLI2SN_2_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SN_2_bit)
#define RCC_PLLI2SCFGR_PLLI2SN_3_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SN_3_bit)
#define RCC_PLLI2SCFGR_PLLI2SN_4_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SN_4_bit)
#define RCC_PLLI2SCFGR_PLLI2SN_5_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SN_5_bit)
#define RCC_PLLI2SCFGR_PLLI2SN_6_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SN_6_bit)
#define RCC_PLLI2SCFGR_PLLI2SN_7_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SN_7_bit)
#define RCC_PLLI2SCFGR_PLLI2SN_8_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SN_8_bit)

#define RCC_PLLI2SCFGR_PLLI2SM_0_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SM_0_bit)
#define RCC_PLLI2SCFGR_PLLI2SM_1_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SM_1_bit)
#define RCC_PLLI2SCFGR_PLLI2SM_2_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SM_2_bit)
#define RCC_PLLI2SCFGR_PLLI2SM_3_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SM_3_bit)
#define RCC_PLLI2SCFGR_PLLI2SM_4_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SM_4_bit)
#define RCC_PLLI2SCFGR_PLLI2SM_5_bb			BITBAND(&RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SM_5_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_PLLSAICFGR - RCC PLL configuration register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_PLLSAICFGR_PLLSAIR_bit			28
#define RCC_PLLSAICFGR_PLLSAIR_0_bit		28
#define RCC_PLLSAICFGR_PLLSAIR_1_bit		29
#define RCC_PLLSAICFGR_PLLSAIR_2_bit		30

#define RCC_PLLSAICFGR_PLLSAIQ_bit			24
#define RCC_PLLSAICFGR_PLLSAIQ_0_bit		24
#define RCC_PLLSAICFGR_PLLSAIQ_1_bit		25
#define RCC_PLLSAICFGR_PLLSAIQ_2_bit		26
#define RCC_PLLSAICFGR_PLLSAIQ_3_bit		27

#define RCC_PLLSAICFGR_PLLSAIP_bit			16
#define RCC_PLLSAICFGR_PLLSAIP_0_bit		16
#define RCC_PLLSAICFGR_PLLSAIP_1_bit		17

#define RCC_PLLSAICFGR_PLLSAIN_bit			6
#define RCC_PLLSAICFGR_PLLSAIN_0_bit		6
#define RCC_PLLSAICFGR_PLLSAIN_1_bit		7
#define RCC_PLLSAICFGR_PLLSAIN_2_bit		8
#define RCC_PLLSAICFGR_PLLSAIN_3_bit		9
#define RCC_PLLSAICFGR_PLLSAIN_4_bit		10
#define RCC_PLLSAICFGR_PLLSAIN_5_bit		11
#define RCC_PLLSAICFGR_PLLSAIN_6_bit		12
#define RCC_PLLSAICFGR_PLLSAIN_7_bit		13
#define RCC_PLLSAICFGR_PLLSAIN_8_bit		14

#define RCC_PLLSAICFGR_PLLSAIM_bit			0
#define RCC_PLLSAICFGR_PLLSAIM_0_bit		0
#define RCC_PLLSAICFGR_PLLSAIM_1_bit		1
#define RCC_PLLSAICFGR_PLLSAIM_2_bit		2
#define RCC_PLLSAICFGR_PLLSAIM_3_bit		3
#define RCC_PLLSAICFGR_PLLSAIM_4_bit		4
#define RCC_PLLSAICFGR_PLLSAIM_5_bit		5

#define RCC_PLLSAICFGR_PLLSAIP_DIV2_value	2
#define RCC_PLLSAICFGR_PLLSAIP_DIV4_value	3
#define RCC_PLLSAICFGR_PLLSAIP_DIV6_value	4
#define RCC_PLLSAICFGR_PLLSAIP_DIV8_value	5
#define RCC_PLLSAICFGR_PLLSAIP_mask			3

#define RCC_PLLSAICFGR_PLLSAIP_DIV2			(RCC_PLLSAICFGR_PLLSAIP_DIV2_value << RCC_PLLSAICFGR_PLLSAIP_bit)
#define RCC_PLLSAICFGR_PLLSAIP_DIV4			(RCC_PLLSAICFGR_PLLSAIP_DIV4_value << RCC_PLLSAICFGR_PLLSAIP_bit)
#define RCC_PLLSAICFGR_PLLSAIP_DIV6			(RCC_PLLSAICFGR_PLLSAIP_DIV6_value << RCC_PLLSAICFGR_PLLSAIP_bit)
#define RCC_PLLSAICFGR_PLLSAIP_DIV8			(RCC_PLLSAICFGR_PLLSAIP_DIV8_value << RCC_PLLSAICFGR_PLLSAIP_bit)

#define RCC_PLLSAICFGR_PLLSAIR_0_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIR_0_bit)
#define RCC_PLLSAICFGR_PLLSAIR_1_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIR_1_bit)
#define RCC_PLLSAICFGR_PLLSAIR_2_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIR_2_bit)

#define RCC_PLLSAICFGR_PLLSAIQ_0_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIQ_0_bit)
#define RCC_PLLSAICFGR_PLLSAIQ_1_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIQ_1_bit)
#define RCC_PLLSAICFGR_PLLSAIQ_2_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIQ_2_bit)
#define RCC_PLLSAICFGR_PLLSAIQ_3_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIQ_3_bit)

#define RCC_PLLSAICFGR_PLLSAIP_0_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIP_0_bit)
#define RCC_PLLSAICFGR_PLLSAIP_1_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIP_1_bit)

#define RCC_PLLSAICFGR_PLLSAIN_0_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIN_0_bit)
#define RCC_PLLSAICFGR_PLLSAIN_1_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIN_1_bit)
#define RCC_PLLSAICFGR_PLLSAIN_2_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIN_2_bit)
#define RCC_PLLSAICFGR_PLLSAIN_3_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIN_3_bit)
#define RCC_PLLSAICFGR_PLLSAIN_4_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIN_4_bit)
#define RCC_PLLSAICFGR_PLLSAIN_5_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIN_5_bit)
#define RCC_PLLSAICFGR_PLLSAIN_6_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIN_6_bit)
#define RCC_PLLSAICFGR_PLLSAIN_7_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIN_7_bit)
#define RCC_PLLSAICFGR_PLLSAIN_8_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIN_8_bit)

#define RCC_PLLSAICFGR_PLLSAIM_0_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIM_0_bit)
#define RCC_PLLSAICFGR_PLLSAIM_1_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIM_1_bit)
#define RCC_PLLSAICFGR_PLLSAIM_2_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIM_2_bit)
#define RCC_PLLSAICFGR_PLLSAIM_3_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIM_3_bit)
#define RCC_PLLSAICFGR_PLLSAIM_4_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIM_4_bit)
#define RCC_PLLSAICFGR_PLLSAIM_5_bb			BITBAND(&RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIM_5_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_DCKCFGR - RCC Dedicated Clock Configuration Register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_DCKCFGR_DSISEL_bit				29
#define RCC_DCKCFGR_SDIOSEL_bit				28
#define RCC_DCKCFGR_48MSEL_bit				27

#define RCC_DCKCFGR_I2S2SRC_bit				27
#define RCC_DCKCFGR_I2S2SRC_0_bit			27
#define RCC_DCKCFGR_I2S2SRC_1_bit			28

#define RCC_DCKCFGR_I2S1SRC_bit				25
#define RCC_DCKCFGR_I2S1SRC_0_bit			25
#define RCC_DCKCFGR_I2S1SRC_1_bit			26

#define RCC_DCKCFGR_I2SSRC_bit				25
#define RCC_DCKCFGR_I2SSRC_0_bit			25
#define RCC_DCKCFGR_I2SSRC_1_bit			26

#define RCC_DCKCFGR_TIMPRE_bit				24

#define RCC_DCKCFGR_SAI1BSRC_bit			22
#define RCC_DCKCFGR_SAI1BSRC_0_bit			22
#define RCC_DCKCFGR_SAI1BSRC_1_bit			23

#define RCC_DCKCFGR_SAI1ASRC_bit			20
#define RCC_DCKCFGR_SAI1ASRC_0_bit			20
#define RCC_DCKCFGR_SAI1ASRC_1_bit			21

#define RCC_DCKCFGR_SAI2SRC_bit				22
#define RCC_DCKCFGR_SAI2SRC_0_bit			22
#define RCC_DCKCFGR_SAI2SRC_1_bit			23

#define RCC_DCKCFGR_SAI1SRC_bit				20
#define RCC_DCKCFGR_SAI1SRC_0_bit			20
#define RCC_DCKCFGR_SAI1SRC_1_bit			21

#define RCC_DCKCFGR_PLLSAIDIVR_bit			16
#define RCC_DCKCFGR_PLLSAIDIVR_0_bit		16
#define RCC_DCKCFGR_PLLSAIDIVR_1_bit		17

#define RCC_DCKCFGR_PLLSAIDIVQ_bit			8
#define RCC_DCKCFGR_PLLSAIDIVQ_0_bit		8
#define RCC_DCKCFGR_PLLSAIDIVQ_1_bit		9
#define RCC_DCKCFGR_PLLSAIDIVQ_2_bit		10
#define RCC_DCKCFGR_PLLSAIDIVQ_3_bit		11
#define RCC_DCKCFGR_PLLSAIDIVQ_4_bit		12

#define RCC_DCKCFGR_PLLI2SDIVQ_bit			0
#define RCC_DCKCFGR_PLLI2SDIVQ_0_bit		0
#define RCC_DCKCFGR_PLLI2SDIVQ_1_bit		1
#define RCC_DCKCFGR_PLLI2SDIVQ_2_bit		2
#define RCC_DCKCFGR_PLLI2SDIVQ_3_bit		3
#define RCC_DCKCFGR_PLLI2SDIVQ_4_bit		4

#define RCC_DCKCFGR_I2S2SRC_PLLI2S_R_value	0
#define RCC_DCKCFGR_I2S2SRC_I2S_CKIN_value	1
#define RCC_DCKCFGR_I2S2SRC_PLL_R_value		2
#define RCC_DCKCFGR_I2S2SRC_HSI_HSE_value	3
#define RCC_DCKCFGR_I2S2SRC_mask			3

#define RCC_DCKCFGR_I2S1SRC_PLLI2S_R_value	0
#define RCC_DCKCFGR_I2S1SRC_I2S_CKIN_value	1
#define RCC_DCKCFGR_I2S1SRC_PLL_R_value		2
#define RCC_DCKCFGR_I2S1SRC_HSI_HSE_value	3
#define RCC_DCKCFGR_I2S1SRC_mask			3

#define RCC_DCKCFGR_I2SSRC_PLL_R_value		0
#define RCC_DCKCFGR_I2SSRC_I2S_CKIN_value	1
#define RCC_DCKCFGR_I2SSRC_HSI_HSE_value	3
#define RCC_DCKCFGR_I2SSRC_mask				3

#define RCC_DCKCFGR_SAI1BSRC_PLLSAI_Q_DIV_value	0
#define RCC_DCKCFGR_SAI1BSRC_PLLI2S_Q_DIV_value	1
#define RCC_DCKCFGR_SAI1BSRC_I2S_CKIN_value		2
#define RCC_DCKCFGR_SAI1BSRC_mask				3

#define RCC_DCKCFGR_SAI1ASRC_PLLSAI_Q_DIV_value	0
#define RCC_DCKCFGR_SAI1ASRC_PLLI2S_Q_DIV_value	1
#define RCC_DCKCFGR_SAI1ASRC_I2S_CKIN_value		2
#define RCC_DCKCFGR_SAI1ASRC_mask				3

#define RCC_DCKCFGR_SAI2SRC_PLLSAI_Q_DIV_value	0
#define RCC_DCKCFGR_SAI2SRC_PLLI2S_Q_DIV_value	1
#define RCC_DCKCFGR_SAI2SRC_PLL_R_value			2
#define RCC_DCKCFGR_SAI2SRC_HSI_HSE_value		3
#define RCC_DCKCFGR_SAI2SRC_mask				3

#define RCC_DCKCFGR_SAI1SRC_PLLSAI_Q_DIV_value	0
#define RCC_DCKCFGR_SAI1SRC_PLLI2S_Q_DIV_value	1
#define RCC_DCKCFGR_SAI1SRC_PLL_R_value			2
#define RCC_DCKCFGR_SAI1SRC_I2S_CKIN_value		3
#define RCC_DCKCFGR_SAI1SRC_mask				3

#define RCC_DCKCFGR_PLLSAIDIVR_DIV2_value	0
#define RCC_DCKCFGR_PLLSAIDIVR_DIV4_value	1
#define RCC_DCKCFGR_PLLSAIDIVR_DIV8_value	2
#define RCC_DCKCFGR_PLLSAIDIVR_DIV16_value	3
#define RCC_DCKCFGR_PLLSAIDIVR_mask			3

#define RCC_DCKCFGR_I2S2SRC_PLLI2S_R		(RCC_DCKCFGR_I2S2SRC_PLLI2S_R_value << RCC_DCKCFGR_I2S2SRC_bit)
#define RCC_DCKCFGR_I2S2SRC_I2S_CKIN		(RCC_DCKCFGR_I2S2SRC_I2S_CKIN_value << RCC_DCKCFGR_I2S2SRC_bit)
#define RCC_DCKCFGR_I2S2SRC_PLL_R			(RCC_DCKCFGR_I2S2SRC_PLL_R_value << RCC_DCKCFGR_I2S2SRC_bit)
#define RCC_DCKCFGR_I2S2SRC_HSI_HSE			(RCC_DCKCFGR_I2S2SRC_HSI_HSE_value << RCC_DCKCFGR_I2S2SRC_bit)

#define RCC_DCKCFGR_I2S1SRC_PLLI2S_R		(RCC_DCKCFGR_I2S1SRC_PLLI2S_R_value << RCC_DCKCFGR_I2S1SRC_bit)
#define RCC_DCKCFGR_I2S1SRC_I2S_CKIN		(RCC_DCKCFGR_I2S1SRC_I2S_CKIN_value << RCC_DCKCFGR_I2S1SRC_bit)
#define RCC_DCKCFGR_I2S1SRC_PLL_R			(RCC_DCKCFGR_I2S1SRC_PLL_R_value << RCC_DCKCFGR_I2S1SRC_bit)
#define RCC_DCKCFGR_I2S1SRC_HSI_HSE			(RCC_DCKCFGR_I2S1SRC_HSI_HSE_value << RCC_DCKCFGR_I2S1SRC_bit)

#define RCC_DCKCFGR_I2SSRC_PLL_R			(RCC_DCKCFGR_I2SSRC_PLL_R_value << RCC_DCKCFGR_I2SSRC_bit)
#define RCC_DCKCFGR_I2SSRC_I2S_CKIN			(RCC_DCKCFGR_I2SSRC_I2S_CKIN_value << RCC_DCKCFGR_I2SSRC_bit)
#define RCC_DCKCFGR_I2SSRC_HSI_HSE			(RCC_DCKCFGR_I2SSRC_HSI_HSE_value << RCC_DCKCFGR_I2SSRC_bit)

#define RCC_DCKCFGR_SAI1BSRC_PLLSAI_Q_DIV	(RCC_DCKCFGR_SAI1BSRC_PLLSAI_Q_DIV_value << RCC_DCKCFGR_SAI1BSRC_bit)
#define RCC_DCKCFGR_SAI1BSRC_PLLI2S_Q_DIV	(RCC_DCKCFGR_SAI1BSRC_PLLI2S_Q_DIV_value << RCC_DCKCFGR_SAI1BSRC_bit)
#define RCC_DCKCFGR_SAI1BSRC_I2S_CKIN		(RCC_DCKCFGR_SAI1BSRC_I2S_CKIN_value << RCC_DCKCFGR_SAI1BSRC_bit)

#define RCC_DCKCFGR_SAI1ASRC_PLLSAI_Q_DIV	(RCC_DCKCFGR_SAI1ASRC_PLLSAI_Q_DIV_value << RCC_DCKCFGR_SAI1ASRC_bit)
#define RCC_DCKCFGR_SAI1ASRC_PLLI2S_Q_DIV	(RCC_DCKCFGR_SAI1ASRC_PLLI2S_Q_DIV_value << RCC_DCKCFGR_SAI1ASRC_bit)
#define RCC_DCKCFGR_SAI1ASRC_I2S_CKIN		(RCC_DCKCFGR_SAI1ASRC_I2S_CKIN_value << RCC_DCKCFGR_SAI1ASRC_bit)

#define RCC_DCKCFGR_SAI2SRC_PLLSAI_Q_DIV	(RCC_DCKCFGR_SAI2SRC_PLLSAI_Q_DIV_value << RCC_DCKCFGR_SAI2SRC_bit)
#define RCC_DCKCFGR_SAI2SRC_PLLI2S_Q_DIV	(RCC_DCKCFGR_SAI2SRC_PLLI2S_Q_DIV_value << RCC_DCKCFGR_SAI2SRC_bit)
#define RCC_DCKCFGR_SAI2SRC_PLL_R			(RCC_DCKCFGR_SAI2SRC_PLL_R_value << RCC_DCKCFGR_SAI2SRC_bit)
#define RCC_DCKCFGR_SAI2SRC_HSI_HSE			(RCC_DCKCFGR_SAI2SRC_HSI_HSE_value << RCC_DCKCFGR_SAI2SRC_bit)

#define RCC_DCKCFGR_SAI1SRC_PLLSAI_Q_DIV	(RCC_DCKCFGR_SAI1SRC_PLLSAI_Q_DIV_value << RCC_DCKCFGR_SAI1SRC_bit)
#define RCC_DCKCFGR_SAI1SRC_PLLI2S_Q_DIV	(RCC_DCKCFGR_SAI1SRC_PLLI2S_Q_DIV_value << RCC_DCKCFGR_SAI1SRC_bit)
#define RCC_DCKCFGR_SAI1SRC_PLL_R			(RCC_DCKCFGR_SAI1SRC_PLL_R_value << RCC_DCKCFGR_SAI1SRC_bit)
#define RCC_DCKCFGR_SAI1SRC_I2S_CKIN		(RCC_DCKCFGR_SAI1SRC_I2S_CKIN_value << RCC_DCKCFGR_SAI1SRC_bit)

#define RCC_DCKCFGR_PLLSAIDIVR_DIV2			(RCC_DCKCFGR_PLLSAIDIVR_DIV2_value << RCC_DCKCFGR_PLLSAIDIVR_bit)
#define RCC_DCKCFGR_PLLSAIDIVR_DIV4			(RCC_DCKCFGR_PLLSAIDIVR_DIV4_value << RCC_DCKCFGR_PLLSAIDIVR_bit)
#define RCC_DCKCFGR_PLLSAIDIVR_DIV8			(RCC_DCKCFGR_PLLSAIDIVR_DIV8_value << RCC_DCKCFGR_PLLSAIDIVR_bit)
#define RCC_DCKCFGR_PLLSAIDIVR_DIV16		(RCC_DCKCFGR_PLLSAIDIVR_DIV16_value << RCC_DCKCFGR_PLLSAIDIVR_bit)

#define RCC_DCKCFGR_DSISEL_bb				BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_DSISEL_bit)
#define RCC_DCKCFGR_SDIOSEL_bb				BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_SDIOSEL_bit)
#define RCC_DCKCFGR_48MSEL_bb				BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_48MSEL_bit)

#define RCC_DCKCFGR_I2S2SRC_0_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_I2S2SRC_0_bit)
#define RCC_DCKCFGR_I2S2SRC_1_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_I2S2SRC_1_bit)

#define RCC_DCKCFGR_I2S1SRC_0_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_I2S1SRC_0_bit)
#define RCC_DCKCFGR_I2S1SRC_1_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_I2S1SRC_1_bit)

#define RCC_DCKCFGR_I2SSRC_0_bb				BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_I2SSRC_0_bit)
#define RCC_DCKCFGR_I2SSRC_1_bb				BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_I2SSRC_1_bit)

#define RCC_DCKCFGR_TIMPRE_bb				BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_TIMPRE_bit)

#define RCC_DCKCFGR_SAI1BSRC_0_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_SAI1BSRC_0_bit)
#define RCC_DCKCFGR_SAI1BSRC_1_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_SAI1BSRC_1_bit)

#define RCC_DCKCFGR_SAI1ASRC_0_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_SAI1ASRC_0_bit)
#define RCC_DCKCFGR_SAI1ASRC_1_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_SAI1ASRC_1_bit)

#define RCC_DCKCFGR_SAI2SRC_0_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_SAI2SRC_0_bit)
#define RCC_DCKCFGR_SAI2SRC_1_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_SAI2SRC_1_bit)

#define RCC_DCKCFGR_SAI1SRC_0_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_SAI1SRC_0_bit)
#define RCC_DCKCFGR_SAI1SRC_1_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_SAI1SRC_1_bit)

#define RCC_DCKCFGR_PLLSAIDIVR_0_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVR_0_bit)
#define RCC_DCKCFGR_PLLSAIDIVR_1_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVR_1_bit)

#define RCC_DCKCFGR_PLLSAIDIVQ_0_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVQ_0_bit)
#define RCC_DCKCFGR_PLLSAIDIVQ_1_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVQ_1_bit)
#define RCC_DCKCFGR_PLLSAIDIVQ_2_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVQ_2_bit)
#define RCC_DCKCFGR_PLLSAIDIVQ_3_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVQ_3_bit)
#define RCC_DCKCFGR_PLLSAIDIVQ_4_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVQ_4_bit)

#define RCC_DCKCFGR_PLLI2SDIVQ_0_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLI2SDIVQ_0_bit)
#define RCC_DCKCFGR_PLLI2SDIVQ_1_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLI2SDIVQ_1_bit)
#define RCC_DCKCFGR_PLLI2SDIVQ_2_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLI2SDIVQ_2_bit)
#define RCC_DCKCFGR_PLLI2SDIVQ_3_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLI2SDIVQ_3_bit)
#define RCC_DCKCFGR_PLLI2SDIVQ_4_bb			BITBAND(&RCC->DCKCFGR, RCC_DCKCFGR_PLLI2SDIVQ_4_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CKGATENR - RCC clocks gated enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CKGATENR_RCC_CKEN_bit			6
#define RCC_CKGATENR_FLITF_CKEN_bit			5
#define RCC_CKGATENR_SRAM_CKEN_bit			4
#define RCC_CKGATENR_SPARE_CKEN_bit			3
#define RCC_CKGATENR_CM4DBG_CKEN_bit		2
#define RCC_CKGATENR_AHB2APB2_CKEN_bit		1
#define RCC_CKGATENR_AHB2APB1_CKEN_bit		0

#define RCC_CKGATENR_RCC_CKEN_bb			BITBAND(&RCC->CKGATENR, RCC_CKGATENR_RCC_CKEN_bit)
#define RCC_CKGATENR_FLITF_CKEN_bb			BITBAND(&RCC->CKGATENR, RCC_CKGATENR_FLITF_CKEN_bit)
#define RCC_CKGATENR_SRAM_CKEN_bb			BITBAND(&RCC->CKGATENR, RCC_CKGATENR_SRAM_CKEN_bit)
#define RCC_CKGATENR_SPARE_CKEN_bb			BITBAND(&RCC->CKGATENR, RCC_CKGATENR_SPARE_CKEN_bit)
#define RCC_CKGATENR_CM4DBG_CKEN_bb			BITBAND(&RCC->CKGATENR, RCC_CKGATENR_CM4DBG_CKEN_bit)
#define RCC_CKGATENR_AHB2APB2_CKEN_bb		BITBAND(&RCC->CKGATENR, RCC_CKGATENR_AHB2APB2_CKEN_bit)
#define RCC_CKGATENR_AHB2APB1_CKEN_bb		BITBAND(&RCC->CKGATENR, RCC_CKGATENR_AHB2APB1_CKEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_DCKCFGR2 - RCC dedicated clocks configuration register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_DCKCFGR2_LPTIM1SEL_bit			30
#define RCC_DCKCFGR2_LPTIM1SEL_0_bit		30
#define RCC_DCKCFGR2_LPTIM1SEL_1_bit		31

#define RCC_DCKCFGR2_SPDIFRXSEL_bit			29
#define RCC_DCKCFGR2_SDIOSEL_bit			28
#define RCC_DCKCFGR2_CK48MSEL_bit			27
#define RCC_DCKCFGR2_CECSEL_bit				26

#define RCC_DCKCFGR2_FMPI2C1SEL_bit			22
#define RCC_DCKCFGR2_FMPI2C1SEL_0_bit		22
#define RCC_DCKCFGR2_FMPI2C1SEL_1_bit		23

#define RCC_DCKCFGR2_LPTIM1SEL_APB_value	0
#define RCC_DCKCFGR2_LPTIM1SEL_HSI_value	1
#define RCC_DCKCFGR2_LPTIM1SEL_LSI_value	2
#define RCC_DCKCFGR2_LPTIM1SEL_LSE_value	3
#define RCC_DCKCFGR2_LPTIM1SEL_mask			3

#define RCC_DCKCFGR2_FMPI2C1SEL_APB_value		0
#define RCC_DCKCFGR2_FMPI2C1SEL_SYSCLK_value	1
#define RCC_DCKCFGR2_FMPI2C1SEL_HSI_value		2
#define RCC_DCKCFGR2_FMPI2C1SEL_mask			3

#define RCC_DCKCFGR2_LPTIM1SEL_APB			(RCC_DCKCFGR2_LPTIM1SEL_APB_value << RCC_DCKCFGR2_LPTIM1SEL_bit)
#define RCC_DCKCFGR2_LPTIM1SEL_HSI			(RCC_DCKCFGR2_LPTIM1SEL_HSI_value << RCC_DCKCFGR2_LPTIM1SEL_bit)
#define RCC_DCKCFGR2_LPTIM1SEL_LSI			(RCC_DCKCFGR2_LPTIM1SEL_LSI_value << RCC_DCKCFGR2_LPTIM1SEL_bit)
#define RCC_DCKCFGR2_LPTIM1SEL_LSE			(RCC_DCKCFGR2_LPTIM1SEL_LSE_value << RCC_DCKCFGR2_LPTIM1SEL_bit)

#define RCC_DCKCFGR2_FMPI2C1SEL_APB			(RCC_DCKCFGR2_FMPI2C1SEL_APB_value << RCC_DCKCFGR2_FMPI2C1SEL_bit)
#define RCC_DCKCFGR2_FMPI2C1SEL_SYSCLK		(RCC_DCKCFGR2_FMPI2C1SEL_SYSCLK_value << RCC_DCKCFGR2_FMPI2C1SEL_bit)
#define RCC_DCKCFGR2_FMPI2C1SEL_HSI			(RCC_DCKCFGR2_FMPI2C1SEL_HSI_value << RCC_DCKCFGR2_FMPI2C1SEL_bit)

#define RCC_DCKCFGR2_LPTIM1SEL_0_bb			BITBAND(&RCC->DCKCFGR2, RCC_DCKCFGR2_LPTIM1SEL_0_bit)
#define RCC_DCKCFGR2_LPTIM1SEL_1_bb			BITBAND(&RCC->DCKCFGR2, RCC_DCKCFGR2_LPTIM1SEL_1_bit)

#define RCC_DCKCFGR2_SPDIFRXSEL_bb			BITBAND(&RCC->DCKCFGR2, RCC_DCKCFGR2_SPDIFRXSEL_bit)
#define RCC_DCKCFGR2_SDIOSEL_bb				BITBAND(&RCC->DCKCFGR2, RCC_DCKCFGR2_SDIOSEL_bit)
#define RCC_DCKCFGR2_CK48MSEL_bb			BITBAND(&RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL_bit)
#define RCC_DCKCFGR2_CECSEL_bb				BITBAND(&RCC->DCKCFGR2, RCC_DCKCFGR2_CECSEL_bit)

#define RCC_DCKCFGR2_FMPI2C1SEL_0_bb		BITBAND(&RCC->DCKCFGR2, RCC_DCKCFGR2_FMPI2C1SEL_0_bit)
#define RCC_DCKCFGR2_FMPI2C1SEL_1_bb		BITBAND(&RCC->DCKCFGR2, RCC_DCKCFGR2_FMPI2C1SEL_1_bit)

#endif /* SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_RCC_BITS_H_ */
