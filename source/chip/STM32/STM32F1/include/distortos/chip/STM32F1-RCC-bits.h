/**
 * \file
 * \brief Header with definition of bits in RCC control registers
 *
 * This file covers devices as described in following places:
 * - RM0008 reference manual (STM32F101, STM32F102, STM32F103, STM32F105 and STM32F107), Revision 16, 2015-11-26
 * - RM0041 reference manual (STM32F100), Revision 4, 2011-07-21
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_BITS_H_
#define SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_BITS_H_

#include "distortos/architecture/ARMv7-M-bit-banding.h"

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CR - Clock Control Register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CR_PLL3RDY_bit					29
#define RCC_CR_PLL3ON_bit					28
#define RCC_CR_PLL2RDY_bit					27
#define RCC_CR_PLL2ON_bit					26
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

#define RCC_CR_PLL3RDY_bb					BITBAND(&RCC->CR, RCC_CR_PLL3RDY_bit)
#define RCC_CR_PLL3ON_bb					BITBAND(&RCC->CR, RCC_CR_PLL3ON_bit)
#define RCC_CR_PLL2RDY_bb					BITBAND(&RCC->CR, RCC_CR_PLL2RDY_bit)
#define RCC_CR_PLL2ON_bb					BITBAND(&RCC->CR, RCC_CR_PLL2ON_bit)
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
| RCC_CFGR - Clock Configuration Register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CFGR_MCO_bit					24
#define RCC_CFGR_MCO_0_bit					24
#define RCC_CFGR_MCO_1_bit					25
#define RCC_CFGR_MCO_2_bit					26
#define RCC_CFGR_MCO_3_bit					27

#define RCC_CFGR_USBPRE_bit					22
#define RCC_CFGR_OTGFSPRE_bit				22

#define RCC_CFGR_PLLMUL_bit					18
#define RCC_CFGR_PLLMUL_0_bit				18
#define RCC_CFGR_PLLMUL_1_bit				19
#define RCC_CFGR_PLLMUL_2_bit				20
#define RCC_CFGR_PLLMUL_3_bit				21

#define RCC_CFGR_PLLXTPRE_bit				17
#define RCC_CFGR_PLLSRC_bit					16

#define RCC_CFGR_ADCPRE_bit					14
#define RCC_CFGR_ADCPRE_0_bit				14
#define RCC_CFGR_ADCPRE_1_bit				15

#define RCC_CFGR_PPRE2_bit					11
#define RCC_CFGR_PPRE2_0_bit				11
#define RCC_CFGR_PPRE2_1_bit				12
#define RCC_CFGR_PPRE2_2_bit				13

#define RCC_CFGR_PPRE1_bit					8
#define RCC_CFGR_PPRE1_0_bit				8
#define RCC_CFGR_PPRE1_1_bit				9
#define RCC_CFGR_PPRE1_2_bit				10

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

#define RCC_CFGR_MCO_NOCLK_value			0
#define RCC_CFGR_MCO_SYSCLK_value			4
#define RCC_CFGR_MCO_HSI_value				5
#define RCC_CFGR_MCO_HSE_value				6
#define RCC_CFGR_MCO_PLLCLK_DIV2_value		7
#define RCC_CFGR_MCO_PLL2CLK_value			8
#define RCC_CFGR_MCO_PLL3CLK_DIV2_value		9
#define RCC_CFGR_MCO_EXT_HSE_value			10
#define RCC_CFGR_MCO_PLL3CLK_value			11
#define RCC_CFGR_MCO_mask					15

#define RCC_CFGR_ADCPRE_DIV2_value			0
#define RCC_CFGR_ADCPRE_DIV4_value			1
#define RCC_CFGR_ADCPRE_DIV6_value			2
#define RCC_CFGR_ADCPRE_DIV8_value			3
#define RCC_CFGR_ADCPRE_mask				3

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
#define RCC_CFGR_SWS_mask					3

#define RCC_CFGR_SW_HSI_value				0
#define RCC_CFGR_SW_HSE_value				1
#define RCC_CFGR_SW_PLL_value				2
#define RCC_CFGR_SW_mask					3

#define RCC_CFGR_MCO_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO_0_bit)
#define RCC_CFGR_MCO_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO_1_bit)
#define RCC_CFGR_MCO_2_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO_2_bit)
#define RCC_CFGR_MCO_3_bb					BITBAND(&RCC->CFGR, RCC_CFGR_MCO_3_bit)

#define RCC_CFGR_USBPRE_bb					BITBAND(&RCC->CFGR, RCC_CFGR_USBPRE_bit)
#define RCC_CFGR_OTGFSPRE_bb				BITBAND(&RCC->CFGR, RCC_CFGR_OTGFSPRE_bit)

#define RCC_CFGR_PLLMUL_0_bb				BITBAND(&RCC->CFGR, RCC_CFGR_PLLMUL_0_bit)
#define RCC_CFGR_PLLMUL_1_bb				BITBAND(&RCC->CFGR, RCC_CFGR_PLLMUL_1_bit)
#define RCC_CFGR_PLLMUL_2_bb				BITBAND(&RCC->CFGR, RCC_CFGR_PLLMUL_2_bit)
#define RCC_CFGR_PLLMUL_3_bb				BITBAND(&RCC->CFGR, RCC_CFGR_PLLMUL_3_bit)

#define RCC_CFGR_PLLXTPRE_bb				BITBAND(&RCC->CFGR, RCC_CFGR_PLLXTPRE_bit)
#define RCC_CFGR_PLLSRC_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PLLSRC_bit)

#define RCC_CFGR_ADCPRE_0_bb				BITBAND(&RCC->CFGR, RCC_CFGR_ADCPRE_0_bit)
#define RCC_CFGR_ADCPRE_1_bb				BITBAND(&RCC->CFGR, RCC_CFGR_ADCPRE_1_bit)

#define RCC_CFGR_PPRE2_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE2_0_bit)
#define RCC_CFGR_PPRE2_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE2_1_bit)
#define RCC_CFGR_PPRE2_2_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE2_2_bit)

#define RCC_CFGR_PPRE1_0_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE1_0_bit)
#define RCC_CFGR_PPRE1_1_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE1_1_bit)
#define RCC_CFGR_PPRE1_2_bb					BITBAND(&RCC->CFGR, RCC_CFGR_PPRE1_2_bit)

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
#define RCC_CIR_PLL3RDYC_bit				22
#define RCC_CIR_PLL2RDYC_bit				21
#define RCC_CIR_PLLRDYC_bit					20
#define RCC_CIR_HSERDYC_bit					19
#define RCC_CIR_HSIRDYC_bit					18
#define RCC_CIR_LSERDYC_bit					17
#define RCC_CIR_LSIRDYC_bit					16
#define RCC_CIR_PLL3RDYIE_bit				14
#define RCC_CIR_PLL2RDYIE_bit				13
#define RCC_CIR_PLLRDYIE_bit				12
#define RCC_CIR_HSERDYIE_bit				11
#define RCC_CIR_HSIRDYIE_bit				10
#define RCC_CIR_LSERDYIE_bit				9
#define RCC_CIR_LSIRDYIE_bit				8
#define RCC_CIR_CSSF_bit					7
#define RCC_CIR_PLL3RDYF_bit				6
#define RCC_CIR_PLL2RDYF_bit				5
#define RCC_CIR_PLLRDYF_bit					4
#define RCC_CIR_HSERDYF_bit					3
#define RCC_CIR_HSIRDYF_bit					2
#define RCC_CIR_LSERDYF_bit					1
#define RCC_CIR_LSIRDYF_bit					0

#define RCC_CIR_CSSC_bb						BITBAND(&RCC->CIR, RCC_CIR_CSSC_bit)
#define RCC_CIR_PLL3RDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_PLL3RDYC_bit)
#define RCC_CIR_PLL2RDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_PLL2RDYC_bit)
#define RCC_CIR_PLLRDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_PLLRDYC_bit)
#define RCC_CIR_HSERDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_HSERDYC_bit)
#define RCC_CIR_HSIRDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_HSIRDYC_bit)
#define RCC_CIR_LSERDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_LSERDYC_bit)
#define RCC_CIR_LSIRDYC_bb					BITBAND(&RCC->CIR, RCC_CIR_LSIRDYC_bit)
#define RCC_CIR_PLL3RDYIE_bb				BITBAND(&RCC->CIR, RCC_CIR_PLL3RDYIE_bit)
#define RCC_CIR_PLL2RDYIE_bb				BITBAND(&RCC->CIR, RCC_CIR_PLL2RDYIE_bit)
#define RCC_CIR_PLLRDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_PLLRDYIE_bit)
#define RCC_CIR_HSERDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_HSERDYIE_bit)
#define RCC_CIR_HSIRDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_HSIRDYIE_bit)
#define RCC_CIR_LSERDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_LSERDYIE_bit)
#define RCC_CIR_LSIRDYIE_bb					BITBAND(&RCC->CIR, RCC_CIR_LSIRDYIE_bit)
#define RCC_CIR_CSSF_bb						BITBAND(&RCC->CIR, RCC_CIR_CSSF_bit)
#define RCC_CIR_PLL3RDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_PLL3RDYF_bit)
#define RCC_CIR_PLL2RDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_PLL2RDYF_bit)
#define RCC_CIR_PLLRDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_PLLRDYF_bit)
#define RCC_CIR_HSERDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_HSERDYF_bit)
#define RCC_CIR_HSIRDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_HSIRDYF_bit)
#define RCC_CIR_LSERDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_LSERDYF_bit)
#define RCC_CIR_LSIRDYF_bb					BITBAND(&RCC->CIR, RCC_CIR_LSIRDYF_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB2RSTR - APB2 peripheral reset register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB2RSTR_TIM11RST_bit			21
#define RCC_APB2RSTR_TIM10RST_bit			20
#define RCC_APB2RSTR_TIM9RST_bit			19
#define RCC_APB2RSTR_TIM17RST_bit			18
#define RCC_APB2RSTR_TIM16RST_bit			17
#define RCC_APB2RSTR_TIM15RST_bit			16
#define RCC_APB2RSTR_ADC3RST_bit			15
#define RCC_APB2RSTR_USART1RST_bit			14
#define RCC_APB2RSTR_TIM8RST_bit			13
#define RCC_APB2RSTR_SPI1RST_bit			12
#define RCC_APB2RSTR_TIM1RST_bit			11
#define RCC_APB2RSTR_ADC2RST_bit			10
#define RCC_APB2RSTR_ADC1RST_bit			9
#define RCC_APB2RSTR_IOPGRST_bit			8
#define RCC_APB2RSTR_IOPFRST_bit			7
#define RCC_APB2RSTR_IOPERST_bit			6
#define RCC_APB2RSTR_IOPDRST_bit			5
#define RCC_APB2RSTR_IOPCRST_bit			4
#define RCC_APB2RSTR_IOPBRST_bit			3
#define RCC_APB2RSTR_IOPARST_bit			2
#define RCC_APB2RSTR_AFIORST_bit 			0

#define RCC_APB2RSTR_TIM11RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM11RST_bit)
#define RCC_APB2RSTR_TIM10RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM10RST_bit)
#define RCC_APB2RSTR_TIM9RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM9RST_bit)
#define RCC_APB2RSTR_TIM17RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM17RST_bit)
#define RCC_APB2RSTR_TIM16RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM16RST_bit)
#define RCC_APB2RSTR_TIM15RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM15RST_bit)
#define RCC_APB2RSTR_ADC3RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_ADC3RST_bit)
#define RCC_APB2RSTR_USART1RST_bb			BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_USART1RST_bit)
#define RCC_APB2RSTR_TIM8RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM8RST_bit)
#define RCC_APB2RSTR_SPI1RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_SPI1RST_bit)
#define RCC_APB2RSTR_TIM1RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_bit)
#define RCC_APB2RSTR_ADC2RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_ADC2RST_bit)
#define RCC_APB2RSTR_ADC1RST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_ADC1RST_bit)
#define RCC_APB2RSTR_IOPGRST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_IOPGRST_bit)
#define RCC_APB2RSTR_IOPFRST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_IOPFRST_bit)
#define RCC_APB2RSTR_IOPERST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_IOPERST_bit)
#define RCC_APB2RSTR_IOPDRST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_IOPDRST_bit)
#define RCC_APB2RSTR_IOPCRST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_IOPCRST_bit)
#define RCC_APB2RSTR_IOPBRST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_IOPBRST_bit)
#define RCC_APB2RSTR_IOPARST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_IOPARST_bit)
#define RCC_APB2RSTR_AFIORST_bb				BITBAND(&RCC->APB2RSTR, RCC_APB2RSTR_AFIORST_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB1RSTR - APB1 peripheral reset register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB1RSTR_CECRST_bit				30
#define RCC_APB1RSTR_DACRST_bit				29
#define RCC_APB1RSTR_PWRRST_bit				28
#define RCC_APB1RSTR_BKPRST_bit				27
#define RCC_APB1RSTR_CAN2RST_bit			26
#define RCC_APB1RSTR_CAN1RST_bit			25
#define RCC_APB1RSTR_CANRST_bit				25
#define RCC_APB1RSTR_USBRST_bit				23
#define RCC_APB1RSTR_I2C2RST_bit			22
#define RCC_APB1RSTR_I2C1RST_bit			21
#define RCC_APB1RSTR_UART5RST_bit			20
#define RCC_APB1RSTR_UART4RST_bit			19
#define RCC_APB1RSTR_USART3RST_bit			18
#define RCC_APB1RSTR_USART2RST_bit			17
#define RCC_APB1RSTR_SPI3RST_bit			15
#define RCC_APB1RSTR_SPI2RST_bit			14
#define RCC_APB1RSTR_WWDGRST_bit			11
#define RCC_APB1RSTR_TIM14RST_bit			8
#define RCC_APB1RSTR_TIM13RST_bit			7
#define RCC_APB1RSTR_TIM12RST_bit			6
#define RCC_APB1RSTR_TIM7RST_bit			5
#define RCC_APB1RSTR_TIM6RST_bit			4
#define RCC_APB1RSTR_TIM5RST_bit			3
#define RCC_APB1RSTR_TIM4RST_bit			2
#define RCC_APB1RSTR_TIM3RST_bit			1
#define RCC_APB1RSTR_TIM2RST_bit			0

#define RCC_APB1RSTR_CECRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_CECRST_bit)
#define RCC_APB1RSTR_DACRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_DACRST_bit)
#define RCC_APB1RSTR_PWRRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_PWRRST_bit)
#define RCC_APB1RSTR_BKPRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_BKPRST_bit)
#define RCC_APB1RSTR_CAN2RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_CAN2RST_bit)
#define RCC_APB1RSTR_CAN1RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_CAN1RST_bit)
#define RCC_APB1RSTR_CANRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_CANRST_bit)
#define RCC_APB1RSTR_USBRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_USBRST_bit)
#define RCC_APB1RSTR_I2C2RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_I2C2RST_bit)
#define RCC_APB1RSTR_I2C1RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_I2C1RST_bit)
#define RCC_APB1RSTR_UART5RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_UART5RST_bit)
#define RCC_APB1RSTR_UART4RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_UART4RST_bit)
#define RCC_APB1RSTR_USART3RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_USART3RST_bit)
#define RCC_APB1RSTR_USART2RST_bb			BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_USART2RST_bit)
#define RCC_APB1RSTR_SPI3RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_SPI3RST_bit)
#define RCC_APB1RSTR_SPI2RST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_SPI2RST_bit)
#define RCC_APB1RSTR_WWDGRST_bb				BITBAND(&RCC->APB1RSTR, RCC_APB1RSTR_WWDGRST_bit)
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
| RCC_AHBENR - AHB peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHBENR_ETHMACRXEN_bit			16
#define RCC_AHBENR_ETHMACTXEN_bit			15
#define RCC_AHBENR_ETHMACEN_bit				14
#define RCC_AHBENR_OTGFSEN_bit				12
#define RCC_AHBENR_SDIOEN_bit				10
#define RCC_AHBENR_FSMCEN_bit				8
#define RCC_AHBENR_CRCEN_bit				6
#define RCC_AHBENR_FLITFEN_bit				4
#define RCC_AHBENR_SRAMEN_bit				2
#define RCC_AHBENR_DMA2EN_bit				1
#define RCC_AHBENR_DMA1EN_bit				0

#define RCC_AHBENR_ETHMACRXEN_bb			BITBAND(&RCC->AHBENR, RCC_AHBENR_ETHMACRXEN_bit)
#define RCC_AHBENR_ETHMACTXEN_bb			BITBAND(&RCC->AHBENR, RCC_AHBENR_ETHMACTXEN_bit)
#define RCC_AHBENR_ETHMACEN_bb				BITBAND(&RCC->AHBENR, RCC_AHBENR_ETHMACEN_bit)
#define RCC_AHBENR_OTGFSEN_bb				BITBAND(&RCC->AHBENR, RCC_AHBENR_OTGFSEN_bit)
#define RCC_AHBENR_SDIOEN_bb				BITBAND(&RCC->AHBENR, RCC_AHBENR_SDIOEN_bit)
#define RCC_AHBENR_FSMCEN_bb				BITBAND(&RCC->AHBENR, RCC_AHBENR_FSMCEN_bit)
#define RCC_AHBENR_CRCEN_bb					BITBAND(&RCC->AHBENR, RCC_AHBENR_CRCEN_bit)
#define RCC_AHBENR_FLITFEN_bb				BITBAND(&RCC->AHBENR, RCC_AHBENR_FLITFEN_bit)
#define RCC_AHBENR_SRAMEN_bb				BITBAND(&RCC->AHBENR, RCC_AHBENR_SRAMEN_bit)
#define RCC_AHBENR_DMA2EN_bb				BITBAND(&RCC->AHBENR, RCC_AHBENR_DMA2EN_bit)
#define RCC_AHBENR_DMA1EN_bb				BITBAND(&RCC->AHBENR, RCC_AHBENR_DMA1EN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB2ENR - APB2 peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB2ENR_TIM11EN_bit				21
#define RCC_APB2ENR_TIM10EN_bit				20
#define RCC_APB2ENR_TIM9EN_bit				19
#define RCC_APB2ENR_TIM17EN_bit				18
#define RCC_APB2ENR_TIM16EN_bit				17
#define RCC_APB2ENR_TIM15EN_bit				16
#define RCC_APB2ENR_ADC3EN_bit				15
#define RCC_APB2ENR_USART1EN_bit			14
#define RCC_APB2ENR_TIM8EN_bit				13
#define RCC_APB2ENR_SPI1EN_bit				12
#define RCC_APB2ENR_TIM1EN_bit				11
#define RCC_APB2ENR_ADC2EN_bit				10
#define RCC_APB2ENR_ADC1EN_bit				9
#define RCC_APB2ENR_IOPGEN_bit				8
#define RCC_APB2ENR_IOPFEN_bit				7
#define RCC_APB2ENR_IOPEEN_bit				6
#define RCC_APB2ENR_IOPDEN_bit				5
#define RCC_APB2ENR_IOPCEN_bit				4
#define RCC_APB2ENR_IOPBEN_bit				3
#define RCC_APB2ENR_IOPAEN_bit				2
#define RCC_APB2ENR_AFIOEN_bit				0

#define RCC_APB2ENR_TIM11EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM11EN_bit)
#define RCC_APB2ENR_TIM10EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM10EN_bit)
#define RCC_APB2ENR_TIM9EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM9EN_bit)
#define RCC_APB2ENR_TIM17EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM17EN_bit)
#define RCC_APB2ENR_TIM16EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM16EN_bit)
#define RCC_APB2ENR_TIM15EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM15EN_bit)
#define RCC_APB2ENR_ADC3EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_ADC3EN_bit)
#define RCC_APB2ENR_USART1EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_USART1EN_bit)
#define RCC_APB2ENR_TIM8EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM8EN_bit)
#define RCC_APB2ENR_SPI1EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_SPI1EN_bit)
#define RCC_APB2ENR_TIM1EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_TIM1EN_bit)
#define RCC_APB2ENR_ADC2EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_ADC2EN_bit)
#define RCC_APB2ENR_ADC1EN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_ADC1EN_bit)
#define RCC_APB2ENR_IOPGEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_IOPGEN_bit)
#define RCC_APB2ENR_IOPFEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_IOPFEN_bit)
#define RCC_APB2ENR_IOPEEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_IOPEEN_bit)
#define RCC_APB2ENR_IOPDEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_IOPDEN_bit)
#define RCC_APB2ENR_IOPCEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_IOPCEN_bit)
#define RCC_APB2ENR_IOPBEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_IOPBEN_bit)
#define RCC_APB2ENR_IOPAEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_IOPAEN_bit)
#define RCC_APB2ENR_AFIOEN_bb				BITBAND(&RCC->APB2ENR, RCC_APB2ENR_AFIOEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB1ENR - APB1 peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB1ENR_CECEN_bit				30
#define RCC_APB1ENR_DACEN_bit				29
#define RCC_APB1ENR_PWREN_bit				28
#define RCC_APB1ENR_BKPEN_bit				27
#define RCC_APB1ENR_CAN2EN_bit				26
#define RCC_APB1ENR_CAN1EN_bit				25
#define RCC_APB1ENR_CANEN_bit				25
#define RCC_APB1ENR_USBEN_bit				23
#define RCC_APB1ENR_I2C2EN_bit				22
#define RCC_APB1ENR_I2C1EN_bit				21
#define RCC_APB1ENR_UART5EN_bit				20
#define RCC_APB1ENR_UART4EN_bit				19
#define RCC_APB1ENR_USART3EN_bit			18
#define RCC_APB1ENR_USART2EN_bit			17
#define RCC_APB1ENR_SPI3EN_bit				15
#define RCC_APB1ENR_SPI2EN_bit				14
#define RCC_APB1ENR_WWDGEN_bit				11
#define RCC_APB1ENR_TIM14EN_bit				8
#define RCC_APB1ENR_TIM13EN_bit				7
#define RCC_APB1ENR_TIM12EN_bit				6
#define RCC_APB1ENR_TIM7EN_bit				5
#define RCC_APB1ENR_TIM6EN_bit				4
#define RCC_APB1ENR_TIM5EN_bit				3
#define RCC_APB1ENR_TIM4EN_bit				2
#define RCC_APB1ENR_TIM3EN_bit				1
#define RCC_APB1ENR_TIM2EN_bit				0

#define RCC_APB1ENR_CECEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_CECEN_bit)
#define RCC_APB1ENR_DACEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_DACEN_bit)
#define RCC_APB1ENR_PWREN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_PWREN_bit)
#define RCC_APB1ENR_BKPEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_BKPEN_bit)
#define RCC_APB1ENR_CAN2EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_CAN2EN_bit)
#define RCC_APB1ENR_CAN1EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_CAN1EN_bit)
#define RCC_APB1ENR_CANEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_CANEN_bit)
#define RCC_APB1ENR_USBEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_USBEN_bit)
#define RCC_APB1ENR_I2C2EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_I2C2EN_bit)
#define RCC_APB1ENR_I2C1EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_I2C1EN_bit)
#define RCC_APB1ENR_UART5EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_UART5EN_bit)
#define RCC_APB1ENR_UART4EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_UART4EN_bit)
#define RCC_APB1ENR_USART3EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_USART3EN_bit)
#define RCC_APB1ENR_USART2EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_USART2EN_bit)
#define RCC_APB1ENR_SPI3EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_SPI3EN_bit)
#define RCC_APB1ENR_SPI2EN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_SPI2EN_bit)
#define RCC_APB1ENR_WWDGEN_bb				BITBAND(&RCC->APB1ENR, RCC_APB1ENR_WWDGEN_bit)
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
| RCC_BDCR - Backup domain control register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_BDCR_BDRST_bit					16
#define RCC_BDCR_RTCEN_bit					15

#define RCC_BDCR_RTCSEL_bit					8
#define RCC_BDCR_RTCSEL_0_bit				8
#define RCC_BDCR_RTCSEL_1_bit				9

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
#define RCC_CSR_RMVF_bit					24
#define RCC_CSR_LSIRDY_bit					1
#define RCC_CSR_LSION_bit					0

#define RCC_CSR_LPWRRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_LPWRRSTF_bit)
#define RCC_CSR_WWDGRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_WWDGRSTF_bit)
#define RCC_CSR_IWDGRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_IWDGRSTF_bit)
#define RCC_CSR_SFTRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_SFTRSTF_bit)
#define RCC_CSR_PORRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_PORRSTF_bit)
#define RCC_CSR_PINRSTF_bb					BITBAND(&RCC->CSR, RCC_CSR_PINRSTF_bit)
#define RCC_CSR_RMVF_bb						BITBAND(&RCC->CSR, RCC_CSR_RMVF_bit)
#define RCC_CSR_LSIRDY_bb					BITBAND(&RCC->CSR, RCC_CSR_LSIRDY_bit)
#define RCC_CSR_LSION_bb					BITBAND(&RCC->CSR, RCC_CSR_LSION_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHBRSTR - AHB peripheral clock reset register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHBRSTR_ETHMACRST_bit			14
#define RCC_AHBRSTR_OTGFSRST_bit			12

#define RCC_AHBRSTR_ETHMACRST_bb			BITBAND(&RCC->AHBRSTR, RCC_AHBRSTR_ETHMACRST_bit)
#define RCC_AHBRSTR_OTGFSRST_bb				BITBAND(&RCC->AHBRSTR, RCC_AHBRSTR_OTGFSRST_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CFGR2 - Clock configuration register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CFGR2_I2S3SRC_bit				18
#define RCC_CFGR2_I2S2SRC_bit				17
#define RCC_CFGR2_PREDIV1SRC_bit			16
#define RCC_CFGR2_PLL3MUL_bit				12
#define RCC_CFGR2_PLL3MUL_0_bit				12
#define RCC_CFGR2_PLL3MUL_1_bit				13
#define RCC_CFGR2_PLL3MUL_2_bit				14
#define RCC_CFGR2_PLL3MUL_3_bit				15
#define RCC_CFGR2_PLL2MUL_bit				8
#define RCC_CFGR2_PLL2MUL_0_bit				8
#define RCC_CFGR2_PLL2MUL_1_bit				9
#define RCC_CFGR2_PLL2MUL_2_bit				10
#define RCC_CFGR2_PLL2MUL_3_bit				11
#define RCC_CFGR2_PREDIV2_bit				4
#define RCC_CFGR2_PREDIV2_0_bit				4
#define RCC_CFGR2_PREDIV2_1_bit				5
#define RCC_CFGR2_PREDIV2_2_bit				6
#define RCC_CFGR2_PREDIV2_3_bit				7
#define RCC_CFGR2_PREDIV1_bit				0
#define RCC_CFGR2_PREDIV1_0_bit				0
#define RCC_CFGR2_PREDIV1_1_bit				1
#define RCC_CFGR2_PREDIV1_2_bit				2
#define RCC_CFGR2_PREDIV1_3_bit				3

#define RCC_CFGR2_I2S3SRC_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_I2S3SRC_bit)
#define RCC_CFGR2_I2S2SRC_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_I2S2SRC_bit)
#define RCC_CFGR2_PREDIV1SRC_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PREDIV1SRC_bit)
#define RCC_CFGR2_PLL3MUL_0_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PLL3MUL_0_bit)
#define RCC_CFGR2_PLL3MUL_1_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PLL3MUL_1_bit)
#define RCC_CFGR2_PLL3MUL_2_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PLL3MUL_2_bit)
#define RCC_CFGR2_PLL3MUL_3_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PLL3MUL_3_bit)
#define RCC_CFGR2_PLL2MUL_0_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PLL2MUL_0_bit)
#define RCC_CFGR2_PLL2MUL_1_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PLL2MUL_1_bit)
#define RCC_CFGR2_PLL2MUL_2_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PLL2MUL_2_bit)
#define RCC_CFGR2_PLL2MUL_3_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PLL2MUL_3_bit)
#define RCC_CFGR2_PREDIV2_0_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PREDIV2_0_bit)
#define RCC_CFGR2_PREDIV2_1_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PREDIV2_1_bit)
#define RCC_CFGR2_PREDIV2_2_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PREDIV2_2_bit)
#define RCC_CFGR2_PREDIV2_3_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PREDIV2_3_bit)
#define RCC_CFGR2_PREDIV1_0_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PREDIV1_0_bit)
#define RCC_CFGR2_PREDIV1_1_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PREDIV1_1_bit)
#define RCC_CFGR2_PREDIV1_2_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PREDIV1_2_bit)
#define RCC_CFGR2_PREDIV1_3_bb				BITBAND(&RCC->CFGR2, RCC_CFGR2_PREDIV1_3_bit)

#endif /* SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_BITS_H_ */
