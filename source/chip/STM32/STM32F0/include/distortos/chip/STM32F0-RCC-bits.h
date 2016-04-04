/**
 * \file
 * \brief Header with definition of bits in RCC control registers
 *
 * This file covers devices as described in following places:
 * - RM0091 reference manual (STM32F0x1, STM32F0x2, STM32F0x8), Revision 8, 2015-07-29
 * - RM0360 reference manual (STM32F030x4/6/8/C, STM32F070x6/B), Revision 3, 2015-05-19
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_RCC_BITS_H_
#define SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_RCC_BITS_H_

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CR - Clock Control Register
+---------------------------------------------------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CFGR - Clock Configuration Register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CFGR_PLLNODIV_bit				31

#define RCC_CFGR_MCOPRE_bit					28
#define RCC_CFGR_MCOPRE_0_bit				28
#define RCC_CFGR_MCOPRE_1_bit				29
#define RCC_CFGR_MCOPRE_2_bit				30

#define RCC_CFGR_MCO_bit					24
#define RCC_CFGR_MCO_0_bit					24
#define RCC_CFGR_MCO_1_bit					25
#define RCC_CFGR_MCO_2_bit					26
#define RCC_CFGR_MCO_3_bit					27

#define RCC_CFGR_PLLMUL_bit					18
#define RCC_CFGR_PLLMUL_0_bit				18
#define RCC_CFGR_PLLMUL_1_bit				19
#define RCC_CFGR_PLLMUL_2_bit				20
#define RCC_CFGR_PLLMUL_3_bit				21

#define RCC_CFGR_PLLXTPRE_bit				17

#define RCC_CFGR_PLLSRC_bit					15
#define RCC_CFGR_PLLSRC_0_bit				15
#define RCC_CFGR_PLLSRC_1_bit				16

#define RCC_CFGR_ADCPRE_bit					14

#define RCC_CFGR_PPRE_bit					8
#define RCC_CFGR_PPRE_0_bit					8
#define RCC_CFGR_PPRE_1_bit					9
#define RCC_CFGR_PPRE_2_bit					10

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

#define RCC_CFGR_MCOPRE_DIV1_value			0
#define RCC_CFGR_MCOPRE_DIV2_value			1
#define RCC_CFGR_MCOPRE_DIV4_value			2
#define RCC_CFGR_MCOPRE_DIV8_value			3
#define RCC_CFGR_MCOPRE_DIV16_value			4
#define RCC_CFGR_MCOPRE_DIV32_value			5
#define RCC_CFGR_MCOPRE_DIV64_value			6
#define RCC_CFGR_MCOPRE_DIV128_value		7
#define RCC_CFGR_MCOPRE_mask				7

#define RCC_CFGR_MCO_NOCLOCK_value			0
#define RCC_CFGR_MCO_HSI14_value			1
#define RCC_CFGR_MCO_LSI_value				2
#define RCC_CFGR_MCO_LSE_value				3
#define RCC_CFGR_MCO_SYSCLK_value			4
#define RCC_CFGR_MCO_HSI_value				5
#define RCC_CFGR_MCO_HSE_value				6
#define RCC_CFGR_MCO_PLL_value				7
#define RCC_CFGR_MCO_HSI48_value			8
#define RCC_CFGR_MCO_mask					15

#define RCC_CFGR_PLLMUL2_value				0
#define RCC_CFGR_PLLMUL3_value				1
#define RCC_CFGR_PLLMUL4_value				2
#define RCC_CFGR_PLLMUL5_value				3
#define RCC_CFGR_PLLMUL6_value				4
#define RCC_CFGR_PLLMUL7_value				5
#define RCC_CFGR_PLLMUL8_value				6
#define RCC_CFGR_PLLMUL9_value				7
#define RCC_CFGR_PLLMUL10_value				8
#define RCC_CFGR_PLLMUL11_value				9
#define RCC_CFGR_PLLMUL12_value				10
#define RCC_CFGR_PLLMUL13_value				11
#define RCC_CFGR_PLLMUL14_value				12
#define RCC_CFGR_PLLMUL15_value				13
#define RCC_CFGR_PLLMUL16_value				14
#define RCC_CFGR_PLLMUL_mask				15

#define RCC_CFGR_PLLSRC_HSI_DIV2_value		0
#define RCC_CFGR_PLLSRC_HSI_PREDIV_value	1
#define RCC_CFGR_PLLSRC_HSE_PREDIV_value	2
#define RCC_CFGR_PLLSRC_HSI48_PREDIV_value	3
#define RCC_CFGR_PLLSRC_mask				3

#define RCC_CFGR_PPRE_DIV1_value			0
#define RCC_CFGR_PPRE_DIV2_value			4
#define RCC_CFGR_PPRE_DIV4_value			5
#define RCC_CFGR_PPRE_DIV8_value			6
#define RCC_CFGR_PPRE_DIV16_value			7
#define RCC_CFGR_PPRE_mask					7

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
#define RCC_CFGR_SWS_HSI48_value			3
#define RCC_CFGR_SWS_mask					3

#define RCC_CFGR_SW_HSI_value				0
#define RCC_CFGR_SW_HSE_value				1
#define RCC_CFGR_SW_PLL_value				2
#define RCC_CFGR_SW_HSI48_value				3
#define RCC_CFGR_SW_mask					3


/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CIR - Clock interrupt register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CIR_CSSC_bit					23
#define RCC_CIR_HSI48RDYC_bit				22
#define RCC_CIR_HSI14RDYC_bit				21
#define RCC_CIR_PLLRDYC_bit					20
#define RCC_CIR_HSERDYC_bit					19
#define RCC_CIR_HSIRDYC_bit					18
#define RCC_CIR_LSERDYC_bit					17
#define RCC_CIR_LSIRDYC_bit					16
#define RCC_CIR_HSI48RDYIE_bit				14
#define RCC_CIR_HSI14RDYIE_bit				13
#define RCC_CIR_PLLRDYIE_bit				12
#define RCC_CIR_HSERDYIE_bit				11
#define RCC_CIR_HSIRDYIE_bit				10
#define RCC_CIR_LSERDYIE_bit				9
#define RCC_CIR_LSIRDYIE_bit				8
#define RCC_CIR_CSSF_bit					7
#define RCC_CIR_HSI48RDYF_bit				6
#define RCC_CIR_HSI14RDYF_bit				5
#define RCC_CIR_PLLRDYF_bit					4
#define RCC_CIR_HSERDYF_bit					3
#define RCC_CIR_HSIRDYF_bit					2
#define RCC_CIR_LSERDYF_bit					1
#define RCC_CIR_LSIRDYF_bit					0

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB2RSTR - APB peripheral reset register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB2RSTR_DBGMCURST_bit			22
#define RCC_APB2RSTR_TIM17RST_bit			18
#define RCC_APB2RSTR_TIM16RST_bit			17
#define RCC_APB2RSTR_TIM15RST_bit			16
#define RCC_APB2RSTR_USART1RST_bit			14
#define RCC_APB2RSTR_SPI1RST_bit			12
#define RCC_APB2RSTR_TIM1RST_bit			11
#define RCC_APB2RSTR_ADCRST_bit				9
#define RCC_APB2RSTR_USART8RST_bit			7
#define RCC_APB2RSTR_USART7RST_bit			6
#define RCC_APB2RSTR_USART6RST_bit			5
#define RCC_APB2RSTR_SYSCFGRST_bit			0

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB1RSTR - APB peripheral reset register 1
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB1RSTR_CECRST_bit				30
#define RCC_APB1RSTR_DACRST_bit				29
#define RCC_APB1RSTR_PWRRST_bit				28
#define RCC_APB1RSTR_CRSRST_bit				27
#define RCC_APB1RSTR_CANRST_bit				25
#define RCC_APB1RSTR_USBRST_bit				23
#define RCC_APB1RSTR_I2C2RST_bit			22
#define RCC_APB1RSTR_I2C1RST_bit			21
#define RCC_APB1RSTR_USART5RST_bit			20
#define RCC_APB1RSTR_USART4RST_bit			19
#define RCC_APB1RSTR_USART3RST_bit			18
#define RCC_APB1RSTR_USART2RST_bit			17
#define RCC_APB1RSTR_SPI2RST_bit			14
#define RCC_APB1RSTR_WWDGRST_bit			11
#define RCC_APB1RSTR_TIM14RST_bit			8
#define RCC_APB1RSTR_TIM7RST_bit			5
#define RCC_APB1RSTR_TIM6RST_bit			4
#define RCC_APB1RSTR_TIM3RST_bit			1
#define RCC_APB1RSTR_TIM2RST_bit			0

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHBENR - AHB peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHBENR_TSCEN_bit				24
#define RCC_AHBENR_IOPFEN_bit				22
#define RCC_AHBENR_IOPEEN_bit				21
#define RCC_AHBENR_IOPDEN_bit				20
#define RCC_AHBENR_IOPCEN_bit				19
#define RCC_AHBENR_IOPBEN_bit				18
#define RCC_AHBENR_IOPAEN_bit				17
#define RCC_AHBENR_CRCEN_bit				6
#define RCC_AHBENR_FLITFEN_bit				4
#define RCC_AHBENR_SRAMEN_bit				2
#define RCC_AHBENR_DMA2EN_bit				1
#define RCC_AHBENR_DMAEN_bit				0

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB2ENR - APB peripheral clock enable register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB2ENR_DBGMCUEN_bit			22
#define RCC_APB2ENR_TIM17EN_bit				18
#define RCC_APB2ENR_TIM16EN_bit				17
#define RCC_APB2ENR_TIM15EN_bit				16
#define RCC_APB2ENR_USART1EN_bit			14
#define RCC_APB2ENR_SPI1EN_bit				12
#define RCC_APB2ENR_TIM1EN_bit				11
#define RCC_APB2ENR_ADCEN_bit				9
#define RCC_APB2ENR_USART8EN_bit			7
#define RCC_APB2ENR_USART7EN_bit			6
#define RCC_APB2ENR_USART6EN_bit			5
#define RCC_APB2ENR_SYSCFGCOMPEN_bit		0

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_APB1ENR - APB peripheral clock enable register 1
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_APB1ENR_CECEN_bit				30
#define RCC_APB1ENR_DACEN_bit				29
#define RCC_APB1ENR_PWREN_bit				28
#define RCC_APB1ENR_CRSEN_bit				27
#define RCC_APB1ENR_CANEN_bit				25
#define RCC_APB1ENR_USBEN_bit				23
#define RCC_APB1ENR_I2C2EN_bit				22
#define RCC_APB1ENR_I2C1EN_bit				21
#define RCC_APB1ENR_USART5EN_bit			20
#define RCC_APB1ENR_USART4EN_bit			19
#define RCC_APB1ENR_USART3EN_bit			18
#define RCC_APB1ENR_USART2EN_bit			17
#define RCC_APB1ENR_SPI2EN_bit				14
#define RCC_APB1ENR_WWDGEN_bit				11
#define RCC_APB1ENR_TIM14EN_bit				8
#define RCC_APB1ENR_TIM7EN_bit				5
#define RCC_APB1ENR_TIM6EN_bit				4
#define RCC_APB1ENR_TIM3EN_bit				1
#define RCC_APB1ENR_TIM2EN_bit				0

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_BDCR - Backup domain control register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_BDCR_BDRST_bit					16
#define RCC_BDCR_RTCEN_bit					15

#define RCC_BDCR_RTCSEL_bit					8
#define RCC_BDCR_RTCSEL_0_bit				8
#define RCC_BDCR_RTCSEL_1_bit				9

#define RCC_BDCR_LSEDRV_bit					3
#define RCC_BDCR_LSEDRV_0_bit				3
#define RCC_BDCR_LSEDRV_1_bit				4

#define RCC_BDCR_LSEBYP_bit					2
#define RCC_BDCR_LSERDY_bit					1
#define RCC_BDCR_LSEON_bit					0

#define RCC_BDCR_RTCSEL_NOCLOCK_value		0
#define RCC_BDCR_RTCSEL_LSE_value			1
#define RCC_BDCR_RTCSEL_LSI_value			2
#define RCC_BDCR_RTCSEL_HSE_value			3
#define RCC_BDCR_RTCSEL_mask				3

#define RCC_BDCR_LSEDRV_LOW_value			0
#define RCC_BDCR_LSEDRV_MEDIUM_LOW_value	1
#define RCC_BDCR_LSEDRV_MEDIUM_HIGH_value	2
#define RCC_BDCR_LSEDRV_HIGH_value			3
#define RCC_BDCR_LSEDRV_mask				3

#define RCC_BDCR_LSEDRV_LOW					(RCC_BDCR_LSEDRV_LOW_value << RCC_BDCR_LSEDRV_bit)
#define RCC_BDCR_LSEDRV_MEDIUM_LOW			(RCC_BDCR_LSEDRV_MEDIUM_LOW_value << RCC_BDCR_LSEDRV_bit)
#define RCC_BDCR_LSEDRV_MEDIUM_HIGH			(RCC_BDCR_LSEDRV_MEDIUM_HIGH_value << RCC_BDCR_LSEDRV_bit)
#define RCC_BDCR_LSEDRV_HIGH				(RCC_BDCR_LSEDRV_HIGH_value << RCC_BDCR_LSEDRV_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CSR - Control/status register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CSR_LPWRRSTF_bit				31
#define RCC_CSR_WWDGRSTF_bit				30
#define RCC_CSR_IWDGRSTF_bit				29
#define RCC_CSR_SFTRSTF_bit					28
#define RCC_CSR_PORRSTF_bit					27
#define RCC_CSR_PINRSTF_bit					26
#define RCC_CSR_OBLRSTF_bit					25
#define RCC_CSR_RMVF_bit					24
#define RCC_CSR_V18PWRRSTF_bit				23
#define RCC_CSR_LSIRDY_bit					1
#define RCC_CSR_LSION_bit					0

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_AHBRSTR - AHB peripheral clock enable register
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_AHBRSTR_TSCRST_bit				24
#define RCC_AHBRSTR_IOPFRST_bit				22
#define RCC_AHBRSTR_IOPERST_bit				21
#define RCC_AHBRSTR_IOPDRST_bit				20
#define RCC_AHBRSTR_IOPCRST_bit				19
#define RCC_AHBRSTR_IOPBRST_bit				18
#define RCC_AHBRSTR_IOPARST_bit				17

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CFGR2 - Clock configuration register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CFGR2_PREDIV_bit				0
#define RCC_CFGR2_PREDIV_0_bit				0
#define RCC_CFGR2_PREDIV_1_bit				1
#define RCC_CFGR2_PREDIV_2_bit				2
#define RCC_CFGR2_PREDIV_3_bit				3

#define RCC_CFGR2_PREDIV_mask				15
#define RCC_CFGR2_PREDIV_DIV1_value			0
#define RCC_CFGR2_PREDIV_DIV2_value			1
#define RCC_CFGR2_PREDIV_DIV3_value			2
#define RCC_CFGR2_PREDIV_DIV4_value			3
#define RCC_CFGR2_PREDIV_DIV5_value			4
#define RCC_CFGR2_PREDIV_DIV6_value			5
#define RCC_CFGR2_PREDIV_DIV7_value			6
#define RCC_CFGR2_PREDIV_DIV8_value			7
#define RCC_CFGR2_PREDIV_DIV9_value			8
#define RCC_CFGR2_PREDIV_DIV10_value		9
#define RCC_CFGR2_PREDIV_DIV11_value		10
#define RCC_CFGR2_PREDIV_DIV12_value		11
#define RCC_CFGR2_PREDIV_DIV13_value		12
#define RCC_CFGR2_PREDIV_DIV14_value		13
#define RCC_CFGR2_PREDIV_DIV15_value		14
#define RCC_CFGR2_PREDIV_DIV16_value		15

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CFGR3 - Clock configuration register 3
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CFGR3_USART3SW_bit				18
#define RCC_CFGR3_USART3SW_0_bit			18
#define RCC_CFGR3_USART3SW_1_bit			19

#define RCC_CFGR3_USART2SW_bit				16
#define RCC_CFGR3_USART2SW_0_bit			16
#define RCC_CFGR3_USART2SW_1_bit			17

#define RCC_CFGR3_ADCSW_bit					8
#define RCC_CFGR3_USBSW_bit					7
#define RCC_CFGR3_CECSW_bit					6
#define RCC_CFGR3_I2C1SW_bit				4

#define RCC_CFGR3_USART1SW_bit				0
#define RCC_CFGR3_USART1SW_0_bit			0
#define RCC_CFGR3_USART1SW_1_bit			1

#define RCC_CFGR3_USARTxSW_PCLK_value		0
#define RCC_CFGR3_USARTxSW_SYSCLK_value		1
#define RCC_CFGR3_USARTxSW_LSE_value		2
#define RCC_CFGR3_USARTxSW_HSI_value		3
#define RCC_CFGR3_USARTxSW_mask				3

/*---------------------------------------------------------------------------------------------------------------------+
| RCC_CR2 - Clock control register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define RCC_CR2_HSI48CAL_bit				24
#define RCC_CR2_HSI48CAL_0_bit				24
#define RCC_CR2_HSI48CAL_1_bit				25
#define RCC_CR2_HSI48CAL_2_bit				26
#define RCC_CR2_HSI48CAL_3_bit				27
#define RCC_CR2_HSI48CAL_4_bit				28
#define RCC_CR2_HSI48CAL_5_bit				29
#define RCC_CR2_HSI48CAL_6_bit				30
#define RCC_CR2_HSI48CAL_7_bit				31

#define RCC_CR2_HSI48RDY_bit				17
#define RCC_CR2_HSI48ON_bit					16

#define RCC_CR2_HSI14CAL_bit				8
#define RCC_CR2_HSI14CAL_0_bit				8
#define RCC_CR2_HSI14CAL_1_bit				9
#define RCC_CR2_HSI14CAL_2_bit				10
#define RCC_CR2_HSI14CAL_3_bit				11
#define RCC_CR2_HSI14CAL_4_bit				12
#define RCC_CR2_HSI14CAL_5_bit				13
#define RCC_CR2_HSI14CAL_6_bit				14
#define RCC_CR2_HSI14CAL_7_bit				15

#define RCC_CR2_HSI14TRIM_bit				3
#define RCC_CR2_HSI14TRIM_0_bit				3
#define RCC_CR2_HSI14TRIM_1_bit				4
#define RCC_CR2_HSI14TRIM_2_bit				5
#define RCC_CR2_HSI14TRIM_3_bit				6
#define RCC_CR2_HSI14TRIM_4_bit				7

#define RCC_CR2_HSI14DIS_bit				2
#define RCC_CR2_HSI14RDY_bit				1
#define RCC_CR2_HSI14ON_bit					0

#endif /* SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_RCC_BITS_H_ */
