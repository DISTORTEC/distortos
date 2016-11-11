/**
 * \file
 * \brief Header with definition of bits in STM32 SPIv2 control registers
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_STM32_SPIV2_BITS_H_
#define SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_STM32_SPIV2_BITS_H_

/*---------------------------------------------------------------------------------------------------------------------+
| SPI_CR1 - SPI control register 1
+---------------------------------------------------------------------------------------------------------------------*/

#define SPI_CR1_BIDIMODE_bit				15
#define SPI_CR1_BIDIOE_bit					14
#define SPI_CR1_CRCEN_bit					13
#define SPI_CR1_CRCNEXT_bit					12
#define SPI_CR1_CRCL_bit					11
#define SPI_CR1_RXONLY_bit					10
#define SPI_CR1_SSM_bit						9
#define SPI_CR1_SSI_bit						8
#define SPI_CR1_LSBFIRST_bit				7
#define SPI_CR1_SPE_bit						6
#define SPI_CR1_BR_bit						3
#define SPI_CR1_BR_0_bit					3
#define SPI_CR1_BR_1_bit					4
#define SPI_CR1_BR_2_bit					5
#define SPI_CR1_MSTR_bit					2
#define SPI_CR1_CPOL_bit					1
#define SPI_CR1_CPHA_bit					0

#define SPI_CR1_BR_DIV2_value				0
#define SPI_CR1_BR_DIV4_value				1
#define SPI_CR1_BR_DIV8_value				2
#define SPI_CR1_BR_DIV16_value				3
#define SPI_CR1_BR_DIV32_value				4
#define SPI_CR1_BR_DIV64_value				5
#define SPI_CR1_BR_DIV128_value				6
#define SPI_CR1_BR_DIV256_value				7
#define SPI_CR1_BR_mask						7

#define SPI_CR1_BR_DIV2						(SPI_CR1_BR_DIV2_value << SPI_CR1_BR_bit)
#define SPI_CR1_BR_DIV4						(SPI_CR1_BR_DIV4_value << SPI_CR1_BR_bit)
#define SPI_CR1_BR_DIV8						(SPI_CR1_BR_DIV8_value << SPI_CR1_BR_bit)
#define SPI_CR1_BR_DIV16					(SPI_CR1_BR_DIV16_value << SPI_CR1_BR_bit)
#define SPI_CR1_BR_DIV32					(SPI_CR1_BR_DIV32_value << SPI_CR1_BR_bit)
#define SPI_CR1_BR_DIV64					(SPI_CR1_BR_DIV64_value << SPI_CR1_BR_bit)
#define SPI_CR1_BR_DIV128					(SPI_CR1_BR_DIV128_value << SPI_CR1_BR_bit)
#define SPI_CR1_BR_DIV256					(SPI_CR1_BR_DIV256_value << SPI_CR1_BR_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| SPI_CR2 - SPI control register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define SPI_CR2_LDMA_TX_bit					14
#define SPI_CR2_LDMA_RX_bit					13
#define SPI_CR2_FRXTH_bit					12
#define SPI_CR2_DS_bit						8
#define SPI_CR2_DS_0_bit					8
#define SPI_CR2_DS_1_bit					9
#define SPI_CR2_DS_2_bit					10
#define SPI_CR2_DS_3_bit					11
#define SPI_CR2_TXEIE_bit					7
#define SPI_CR2_RXNEIE_bit					6
#define SPI_CR2_ERRIE_bit					5
#define SPI_CR2_FRF_bit						4
#define SPI_CR2_NSSP_bit					3
#define SPI_CR2_SSOE_bit					2
#define SPI_CR2_TXDMAEN_bit					1
#define SPI_CR2_RXDMAEN_bit					0

#define SPI_CR2_DS_4BIT_value				3
#define SPI_CR2_DS_5BIT_value				4
#define SPI_CR2_DS_6BIT_value				5
#define SPI_CR2_DS_7BIT_value				6
#define SPI_CR2_DS_8BIT_value				7
#define SPI_CR2_DS_9BIT_value				8
#define SPI_CR2_DS_10BIT_value				9
#define SPI_CR2_DS_11BIT_value				10
#define SPI_CR2_DS_12BIT_value				11
#define SPI_CR2_DS_13BIT_value				12
#define SPI_CR2_DS_14BIT_value				13
#define SPI_CR2_DS_15BIT_value				14
#define SPI_CR2_DS_16BIT_value				15
#define SPI_CR2_DS_mask						15

#define SPI_CR2_DS_4BIT						(SPI_CR2_DS_4BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_5BIT						(SPI_CR2_DS_5BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_6BIT						(SPI_CR2_DS_6BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_7BIT						(SPI_CR2_DS_7BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_8BIT						(SPI_CR2_DS_8BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_9BIT						(SPI_CR2_DS_9BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_10BIT					(SPI_CR2_DS_10BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_11BIT					(SPI_CR2_DS_11BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_12BIT					(SPI_CR2_DS_12BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_13BIT					(SPI_CR2_DS_13BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_14BIT					(SPI_CR2_DS_14BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_15BIT					(SPI_CR2_DS_15BIT_value << SPI_CR2_DS_bit)
#define SPI_CR2_DS_16BIT					(SPI_CR2_DS_16BIT_value << SPI_CR2_DS_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| SPI_SR - SPI status register
+---------------------------------------------------------------------------------------------------------------------*/

#define SPI_SR_FTLVL_bit					11
#define SPI_SR_FTLVL_0_bit					11
#define SPI_SR_FTLVL_1_bit					12
#define SPI_SR_FRLVL_bit					9
#define SPI_SR_FRLVL_0_bit					9
#define SPI_SR_FRLVL_1_bit					10
#define SPI_SR_FRE_bit						8
#define SPI_SR_BSY_bit						7
#define SPI_SR_OVR_bit						6
#define SPI_SR_MODF_bit						5
#define SPI_SR_CRCERR_bit					4
#define SPI_SR_UDR_bit						3
#define SPI_SR_CHSIDE_bit					2
#define SPI_SR_TXE_bit						1
#define SPI_SR_RXNE_bit						0

#define SPI_SR_FxLVL_EMPTY_value			0
#define SPI_SR_FxLVL_QUARTER_value			1
#define SPI_SR_FxLVL_HALF_value				2
#define SPI_SR_FxLVL_FULL_value				3
#define SPI_SR_FxLVL_mask					3

#define SPI_SR_FTLVL_EMPTY					(SPI_SR_FxLVL_EMPTY_value << SPI_SR_FTLVL_bit)
#define SPI_SR_FTLVL_QUARTER				(SPI_SR_FxLVL_QUARTER_value << SPI_SR_FTLVL_bit)
#define SPI_SR_FTLVL_HALF					(SPI_SR_FxLVL_HALF_value << SPI_SR_FTLVL_bit)
#define SPI_SR_FTLVL_FULL					(SPI_SR_FxLVL_FULL_value << SPI_SR_FTLVL_bit)

#define SPI_SR_FRLVL_EMPTY					(SPI_SR_FxLVL_EMPTY_value << SPI_SR_FRLVL_bit)
#define SPI_SR_FRLVL_QUARTER				(SPI_SR_FxLVL_QUARTER_value << SPI_SR_FRLVL_bit)
#define SPI_SR_FRLVL_HALF					(SPI_SR_FxLVL_HALF_value << SPI_SR_FRLVL_bit)
#define SPI_SR_FRLVL_FULL					(SPI_SR_FxLVL_FULL_value << SPI_SR_FRLVL_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| SPI_I2SCFGR - SPI_I2S configuration register
+---------------------------------------------------------------------------------------------------------------------*/

#define SPI_I2SCFGR_I2SMOD_bit				11
#define SPI_I2SCFGR_I2SE_bit				10
#define SPI_I2SCFGR_I2SCFG_bit				8
#define SPI_I2SCFGR_I2SCFG_0_bit			8
#define SPI_I2SCFGR_I2SCFG_1_bit			9
#define SPI_I2SCFGR_PCMSYNC_bit				7
#define SPI_I2SCFGR_I2SSTD_bit				4
#define SPI_I2SCFGR_I2SSTD_0_bit			4
#define SPI_I2SCFGR_I2SSTD_1_bit			5
#define SPI_I2SCFGR_CKPOL_bit				3
#define SPI_I2SCFGR_DATLEN_bit				1
#define SPI_I2SCFGR_DATLEN_0_bit			1
#define SPI_I2SCFGR_DATLEN_1_bit			2
#define SPI_I2SCFGR_CHLEN_bit				0

#define SPI_I2SCFGR_I2SCFG_SLV_TX_value		0
#define SPI_I2SCFGR_I2SCFG_SLV_RX_value		1
#define SPI_I2SCFGR_I2SCFG_MSTR_TX_value	2
#define SPI_I2SCFGR_I2SCFG_MSTR_RX_value	3
#define SPI_I2SCFGR_I2SCFG_mask				3

#define SPI_I2SCFGR_I2SSTD_I2S_value		0
#define SPI_I2SCFGR_I2SSTD_MSB_value		1
#define SPI_I2SCFGR_I2SSTD_LSB_value		2
#define SPI_I2SCFGR_I2SSTD_PCM_value		3
#define SPI_I2SCFGR_I2SSTD_mask				3

#define SPI_I2SCFGR_DATLEN_16_value			0
#define SPI_I2SCFGR_DATLEN_24_value			1
#define SPI_I2SCFGR_DATLEN_32_value			2
#define SPI_I2SCFGR_DATLEN_mask				3

#define SPI_I2SCFGR_I2SCFG_SLV_TX			(SPI_I2SCFGR_I2SCFG_SLV_TX_value << SPI_I2SCFGR_I2SCFG_bit)
#define SPI_I2SCFGR_I2SCFG_SLV_RX			(SPI_I2SCFGR_I2SCFG_SLV_RX_value << SPI_I2SCFGR_I2SCFG_bit)
#define SPI_I2SCFGR_I2SCFG_MSTR_TX			(SPI_I2SCFGR_I2SCFG_MSTR_TX_value << SPI_I2SCFGR_I2SCFG_bit)
#define SPI_I2SCFGR_I2SCFG_MSTR_RX			(SPI_I2SCFGR_I2SCFG_MSTR_RX_value << SPI_I2SCFGR_I2SCFG_bit)

#define SPI_I2SCFGR_I2SSTD_I2S				(SPI_I2SCFGR_I2SSTD_I2S_value << SPI_I2SCFGR_I2SSTD_bit)
#define SPI_I2SCFGR_I2SSTD_MSB				(SPI_I2SCFGR_I2SSTD_MSB_value << SPI_I2SCFGR_I2SSTD_bit)
#define SPI_I2SCFGR_I2SSTD_LSB				(SPI_I2SCFGR_I2SSTD_LSB_value << SPI_I2SCFGR_I2SSTD_bit)
#define SPI_I2SCFGR_I2SSTD_PCM				(SPI_I2SCFGR_I2SSTD_PCM_value << SPI_I2SCFGR_I2SSTD_bit)

#define SPI_I2SCFGR_DATLEN_16				(SPI_I2SCFGR_DATLEN_16_value << SPI_I2SCFGR_DATLEN_bit)
#define SPI_I2SCFGR_DATLEN_24				(SPI_I2SCFGR_DATLEN_24_value << SPI_I2SCFGR_DATLEN_bit)
#define SPI_I2SCFGR_DATLEN_32				(SPI_I2SCFGR_DATLEN_32_value << SPI_I2SCFGR_DATLEN_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| SPI_I2SPR - SPI_I2S prescaler register
+---------------------------------------------------------------------------------------------------------------------*/

#define SPI_I2SPR_MCKOE_bit					9
#define SPI_I2SPR_ODD_bit					8
#define SPI_I2SPR_I2SDIV_bit				0
#define SPI_I2SPR_I2SDIV_0_bit				0
#define SPI_I2SPR_I2SDIV_1_bit				1
#define SPI_I2SPR_I2SDIV_2_bit				2
#define SPI_I2SPR_I2SDIV_3_bit				3
#define SPI_I2SPR_I2SDIV_4_bit				4
#define SPI_I2SPR_I2SDIV_5_bit				5
#define SPI_I2SPR_I2SDIV_6_bit				6
#define SPI_I2SPR_I2SDIV_7_bit				7

#define SPI_I2SPR_I2SDIV_0					(1 << SPI_I2SPR_I2SDIV_0_bit)
#define SPI_I2SPR_I2SDIV_1					(1 << SPI_I2SPR_I2SDIV_1_bit)
#define SPI_I2SPR_I2SDIV_2					(1 << SPI_I2SPR_I2SDIV_2_bit)
#define SPI_I2SPR_I2SDIV_3					(1 << SPI_I2SPR_I2SDIV_3_bit)
#define SPI_I2SPR_I2SDIV_4					(1 << SPI_I2SPR_I2SDIV_4_bit)
#define SPI_I2SPR_I2SDIV_5					(1 << SPI_I2SPR_I2SDIV_5_bit)
#define SPI_I2SPR_I2SDIV_6					(1 << SPI_I2SPR_I2SDIV_6_bit)
#define SPI_I2SPR_I2SDIV_7					(1 << SPI_I2SPR_I2SDIV_7_bit)

#endif /* SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_STM32_SPIV2_BITS_H_ */
