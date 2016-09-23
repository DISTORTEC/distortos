/**
 * \file
 * \brief Header with definition of bits in STM32 SPIv1 control registers
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SPIV1_INCLUDE_DISTORTOS_CHIP_STM32_SPIV1_BITS_H_
#define SOURCE_CHIP_STM32_PERIPHERALS_SPIV1_INCLUDE_DISTORTOS_CHIP_STM32_SPIV1_BITS_H_

#include "distortos/architecture/ARMv7-M-bit-banding.h"

/*---------------------------------------------------------------------------------------------------------------------+
| SPI_CR1 - SPI control register 1
+---------------------------------------------------------------------------------------------------------------------*/

#define SPI_CR1_BIDIMODE_bit				15
#define SPI_CR1_BIDIOE_bit					14
#define SPI_CR1_CRCEN_bit					13
#define SPI_CR1_CRCNEXT_bit					12
#define SPI_CR1_DFF_bit						11
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

#define SPIx_CR1_BIDIMODE_bb(base)			BITBAND(&(base)->CR1, SPI_CR1_BIDIMODE_bit)
#define SPIx_CR1_BIDIOE_bb(base)			BITBAND(&(base)->CR1, SPI_CR1_BIDIOE_bit)
#define SPIx_CR1_CRCEN_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_CRCEN_bit)
#define SPIx_CR1_CRCNEXT_bb(base)			BITBAND(&(base)->CR1, SPI_CR1_CRCNEXT_bit)
#define SPIx_CR1_DFF_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_DFF_bit)
#define SPIx_CR1_RXONLY_bb(base)			BITBAND(&(base)->CR1, SPI_CR1_RXONLY_bit)
#define SPIx_CR1_SSM_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_SSM_bit)
#define SPIx_CR1_SSI_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_SSI_bit)
#define SPIx_CR1_LSBFIRST_bb(base)			BITBAND(&(base)->CR1, SPI_CR1_LSBFIRST_bit)
#define SPIx_CR1_SPE_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_SPE_bit)
#define SPIx_CR1_BR_0_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_BR_0_bit)
#define SPIx_CR1_BR_1_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_BR_1_bit)
#define SPIx_CR1_BR_2_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_BR_2_bit)
#define SPIx_CR1_MSTR_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_MSTR_bit)
#define SPIx_CR1_CPOL_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_CPOL_bit)
#define SPIx_CR1_CPHA_bb(base)				BITBAND(&(base)->CR1, SPI_CR1_CPHA_bit)

#define SPI1_CR1_BIDIMODE_bb				SPIx_CR1_BIDIMODE_bb(SPI1)
#define SPI1_CR1_BIDIOE_bb					SPIx_CR1_BIDIOE_bb(SPI1)
#define SPI1_CR1_CRCEN_bb					SPIx_CR1_CRCEN_bb(SPI1)
#define SPI1_CR1_CRCNEXT_bb					SPIx_CR1_CRCNEXT_bb(SPI1)
#define SPI1_CR1_DFF_bb						SPIx_CR1_DFF_bb(SPI1)
#define SPI1_CR1_RXONLY_bb					SPIx_CR1_RXONLY_bb(SPI1)
#define SPI1_CR1_SSM_bb						SPIx_CR1_SSM_bb(SPI1)
#define SPI1_CR1_SSI_bb						SPIx_CR1_SSI_bb(SPI1)
#define SPI1_CR1_LSBFIRST_bb				SPIx_CR1_LSBFIRST_bb(SPI1)
#define SPI1_CR1_SPE_bb						SPIx_CR1_SPE_bb(SPI1)
#define SPI1_CR1_BR_0_bb					SPIx_CR1_BR_0_bb(SPI1)
#define SPI1_CR1_BR_1_bb					SPIx_CR1_BR_1_bb(SPI1)
#define SPI1_CR1_BR_2_bb					SPIx_CR1_BR_2_bb(SPI1)
#define SPI1_CR1_MSTR_bb					SPIx_CR1_MSTR_bb(SPI1)
#define SPI1_CR1_CPOL_bb					SPIx_CR1_CPOL_bb(SPI1)
#define SPI1_CR1_CPHA_bb					SPIx_CR1_CPHA_bb(SPI1)

#define SPI2_CR1_BIDIMODE_bb				SPIx_CR1_BIDIMODE_bb(SPI2)
#define SPI2_CR1_BIDIOE_bb					SPIx_CR1_BIDIOE_bb(SPI2)
#define SPI2_CR1_CRCEN_bb					SPIx_CR1_CRCEN_bb(SPI2)
#define SPI2_CR1_CRCNEXT_bb					SPIx_CR1_CRCNEXT_bb(SPI2)
#define SPI2_CR1_DFF_bb						SPIx_CR1_DFF_bb(SPI2)
#define SPI2_CR1_RXONLY_bb					SPIx_CR1_RXONLY_bb(SPI2)
#define SPI2_CR1_SSM_bb						SPIx_CR1_SSM_bb(SPI2)
#define SPI2_CR1_SSI_bb						SPIx_CR1_SSI_bb(SPI2)
#define SPI2_CR1_LSBFIRST_bb				SPIx_CR1_LSBFIRST_bb(SPI2)
#define SPI2_CR1_SPE_bb						SPIx_CR1_SPE_bb(SPI2)
#define SPI2_CR1_BR_0_bb					SPIx_CR1_BR_0_bb(SPI2)
#define SPI2_CR1_BR_1_bb					SPIx_CR1_BR_1_bb(SPI2)
#define SPI2_CR1_BR_2_bb					SPIx_CR1_BR_2_bb(SPI2)
#define SPI2_CR1_MSTR_bb					SPIx_CR1_MSTR_bb(SPI2)
#define SPI2_CR1_CPOL_bb					SPIx_CR1_CPOL_bb(SPI2)
#define SPI2_CR1_CPHA_bb					SPIx_CR1_CPHA_bb(SPI2)

#define SPI3_CR1_BIDIMODE_bb				SPIx_CR1_BIDIMODE_bb(SPI3)
#define SPI3_CR1_BIDIOE_bb					SPIx_CR1_BIDIOE_bb(SPI3)
#define SPI3_CR1_CRCEN_bb					SPIx_CR1_CRCEN_bb(SPI3)
#define SPI3_CR1_CRCNEXT_bb					SPIx_CR1_CRCNEXT_bb(SPI3)
#define SPI3_CR1_DFF_bb						SPIx_CR1_DFF_bb(SPI3)
#define SPI3_CR1_RXONLY_bb					SPIx_CR1_RXONLY_bb(SPI3)
#define SPI3_CR1_SSM_bb						SPIx_CR1_SSM_bb(SPI3)
#define SPI3_CR1_SSI_bb						SPIx_CR1_SSI_bb(SPI3)
#define SPI3_CR1_LSBFIRST_bb				SPIx_CR1_LSBFIRST_bb(SPI3)
#define SPI3_CR1_SPE_bb						SPIx_CR1_SPE_bb(SPI3)
#define SPI3_CR1_BR_0_bb					SPIx_CR1_BR_0_bb(SPI3)
#define SPI3_CR1_BR_1_bb					SPIx_CR1_BR_1_bb(SPI3)
#define SPI3_CR1_BR_2_bb					SPIx_CR1_BR_2_bb(SPI3)
#define SPI3_CR1_MSTR_bb					SPIx_CR1_MSTR_bb(SPI3)
#define SPI3_CR1_CPOL_bb					SPIx_CR1_CPOL_bb(SPI3)
#define SPI3_CR1_CPHA_bb					SPIx_CR1_CPHA_bb(SPI3)

#define SPI4_CR1_BIDIMODE_bb				SPIx_CR1_BIDIMODE_bb(SPI4)
#define SPI4_CR1_BIDIOE_bb					SPIx_CR1_BIDIOE_bb(SPI4)
#define SPI4_CR1_CRCEN_bb					SPIx_CR1_CRCEN_bb(SPI4)
#define SPI4_CR1_CRCNEXT_bb					SPIx_CR1_CRCNEXT_bb(SPI4)
#define SPI4_CR1_DFF_bb						SPIx_CR1_DFF_bb(SPI4)
#define SPI4_CR1_RXONLY_bb					SPIx_CR1_RXONLY_bb(SPI4)
#define SPI4_CR1_SSM_bb						SPIx_CR1_SSM_bb(SPI4)
#define SPI4_CR1_SSI_bb						SPIx_CR1_SSI_bb(SPI4)
#define SPI4_CR1_LSBFIRST_bb				SPIx_CR1_LSBFIRST_bb(SPI4)
#define SPI4_CR1_SPE_bb						SPIx_CR1_SPE_bb(SPI4)
#define SPI4_CR1_BR_0_bb					SPIx_CR1_BR_0_bb(SPI4)
#define SPI4_CR1_BR_1_bb					SPIx_CR1_BR_1_bb(SPI4)
#define SPI4_CR1_BR_2_bb					SPIx_CR1_BR_2_bb(SPI4)
#define SPI4_CR1_MSTR_bb					SPIx_CR1_MSTR_bb(SPI4)
#define SPI4_CR1_CPOL_bb					SPIx_CR1_CPOL_bb(SPI4)
#define SPI4_CR1_CPHA_bb					SPIx_CR1_CPHA_bb(SPI4)

#define SPI5_CR1_BIDIMODE_bb				SPIx_CR1_BIDIMODE_bb(SPI5)
#define SPI5_CR1_BIDIOE_bb					SPIx_CR1_BIDIOE_bb(SPI5)
#define SPI5_CR1_CRCEN_bb					SPIx_CR1_CRCEN_bb(SPI5)
#define SPI5_CR1_CRCNEXT_bb					SPIx_CR1_CRCNEXT_bb(SPI5)
#define SPI5_CR1_DFF_bb						SPIx_CR1_DFF_bb(SPI5)
#define SPI5_CR1_RXONLY_bb					SPIx_CR1_RXONLY_bb(SPI5)
#define SPI5_CR1_SSM_bb						SPIx_CR1_SSM_bb(SPI5)
#define SPI5_CR1_SSI_bb						SPIx_CR1_SSI_bb(SPI5)
#define SPI5_CR1_LSBFIRST_bb				SPIx_CR1_LSBFIRST_bb(SPI5)
#define SPI5_CR1_SPE_bb						SPIx_CR1_SPE_bb(SPI5)
#define SPI5_CR1_BR_0_bb					SPIx_CR1_BR_0_bb(SPI5)
#define SPI5_CR1_BR_1_bb					SPIx_CR1_BR_1_bb(SPI5)
#define SPI5_CR1_BR_2_bb					SPIx_CR1_BR_2_bb(SPI5)
#define SPI5_CR1_MSTR_bb					SPIx_CR1_MSTR_bb(SPI5)
#define SPI5_CR1_CPOL_bb					SPIx_CR1_CPOL_bb(SPI5)
#define SPI5_CR1_CPHA_bb					SPIx_CR1_CPHA_bb(SPI5)

#define SPI6_CR1_BIDIMODE_bb				SPIx_CR1_BIDIMODE_bb(SPI6)
#define SPI6_CR1_BIDIOE_bb					SPIx_CR1_BIDIOE_bb(SPI6)
#define SPI6_CR1_CRCEN_bb					SPIx_CR1_CRCEN_bb(SPI6)
#define SPI6_CR1_CRCNEXT_bb					SPIx_CR1_CRCNEXT_bb(SPI6)
#define SPI6_CR1_DFF_bb						SPIx_CR1_DFF_bb(SPI6)
#define SPI6_CR1_RXONLY_bb					SPIx_CR1_RXONLY_bb(SPI6)
#define SPI6_CR1_SSM_bb						SPIx_CR1_SSM_bb(SPI6)
#define SPI6_CR1_SSI_bb						SPIx_CR1_SSI_bb(SPI6)
#define SPI6_CR1_LSBFIRST_bb				SPIx_CR1_LSBFIRST_bb(SPI6)
#define SPI6_CR1_SPE_bb						SPIx_CR1_SPE_bb(SPI6)
#define SPI6_CR1_BR_0_bb					SPIx_CR1_BR_0_bb(SPI6)
#define SPI6_CR1_BR_1_bb					SPIx_CR1_BR_1_bb(SPI6)
#define SPI6_CR1_BR_2_bb					SPIx_CR1_BR_2_bb(SPI6)
#define SPI6_CR1_MSTR_bb					SPIx_CR1_MSTR_bb(SPI6)
#define SPI6_CR1_CPOL_bb					SPIx_CR1_CPOL_bb(SPI6)
#define SPI6_CR1_CPHA_bb					SPIx_CR1_CPHA_bb(SPI6)

/*---------------------------------------------------------------------------------------------------------------------+
| SPI_CR2 - SPI control register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define SPI_CR2_TXEIE_bit					7
#define SPI_CR2_RXNEIE_bit					6
#define SPI_CR2_ERRIE_bit					5
#define SPI_CR2_FRF_bit						4
#define SPI_CR2_SSOE_bit					2
#define SPI_CR2_TXDMAEN_bit					1
#define SPI_CR2_RXDMAEN_bit					0

#define SPIx_CR2_TXEIE_bb(base)				BITBAND(&(base)->CR2, SPI_CR2_TXEIE_bit)
#define SPIx_CR2_RXNEIE_bb(base)			BITBAND(&(base)->CR2, SPI_CR2_RXNEIE_bit)
#define SPIx_CR2_ERRIE_bb(base)				BITBAND(&(base)->CR2, SPI_CR2_ERRIE_bit)
#define SPIx_CR2_FRF_bb(base)				BITBAND(&(base)->CR2, SPI_CR2_FRF_bit)
#define SPIx_CR2_SSOE_bb(base)				BITBAND(&(base)->CR2, SPI_CR2_SSOE_bit)
#define SPIx_CR2_TXDMAEN_bb(base)			BITBAND(&(base)->CR2, SPI_CR2_TXDMAEN_bit)
#define SPIx_CR2_RXDMAEN_bb(base)			BITBAND(&(base)->CR2, SPI_CR2_RXDMAEN_bit)

#define SPI1_CR2_TXEIE_bb					SPIx_CR2_TXEIE_bb(SPI1)
#define SPI1_CR2_RXNEIE_bb					SPIx_CR2_RXNEIE_bb(SPI1)
#define SPI1_CR2_ERRIE_bb					SPIx_CR2_ERRIE_bb(SPI1)
#define SPI1_CR2_FRF_bb						SPIx_CR2_FRF_bb(SPI1)
#define SPI1_CR2_SSOE_bb					SPIx_CR2_SSOE_bb(SPI1)
#define SPI1_CR2_TXDMAEN_bb					SPIx_CR2_TXDMAEN_bb(SPI1)
#define SPI1_CR2_RXDMAEN_bb					SPIx_CR2_RXDMAEN_bb(SPI1)

#define SPI2_CR2_TXEIE_bb					SPIx_CR2_TXEIE_bb(SPI2)
#define SPI2_CR2_RXNEIE_bb					SPIx_CR2_RXNEIE_bb(SPI2)
#define SPI2_CR2_ERRIE_bb					SPIx_CR2_ERRIE_bb(SPI2)
#define SPI2_CR2_FRF_bb						SPIx_CR2_FRF_bb(SPI2)
#define SPI2_CR2_SSOE_bb					SPIx_CR2_SSOE_bb(SPI2)
#define SPI2_CR2_TXDMAEN_bb					SPIx_CR2_TXDMAEN_bb(SPI2)
#define SPI2_CR2_RXDMAEN_bb					SPIx_CR2_RXDMAEN_bb(SPI2)

#define SPI3_CR2_TXEIE_bb					SPIx_CR2_TXEIE_bb(SPI3)
#define SPI3_CR2_RXNEIE_bb					SPIx_CR2_RXNEIE_bb(SPI3)
#define SPI3_CR2_ERRIE_bb					SPIx_CR2_ERRIE_bb(SPI3)
#define SPI3_CR2_FRF_bb						SPIx_CR2_FRF_bb(SPI3)
#define SPI3_CR2_SSOE_bb					SPIx_CR2_SSOE_bb(SPI3)
#define SPI3_CR2_TXDMAEN_bb					SPIx_CR2_TXDMAEN_bb(SPI3)
#define SPI3_CR2_RXDMAEN_bb					SPIx_CR2_RXDMAEN_bb(SPI3)

#define SPI4_CR2_TXEIE_bb					SPIx_CR2_TXEIE_bb(SPI4)
#define SPI4_CR2_RXNEIE_bb					SPIx_CR2_RXNEIE_bb(SPI4)
#define SPI4_CR2_ERRIE_bb					SPIx_CR2_ERRIE_bb(SPI4)
#define SPI4_CR2_FRF_bb						SPIx_CR2_FRF_bb(SPI4)
#define SPI4_CR2_SSOE_bb					SPIx_CR2_SSOE_bb(SPI4)
#define SPI4_CR2_TXDMAEN_bb					SPIx_CR2_TXDMAEN_bb(SPI4)
#define SPI4_CR2_RXDMAEN_bb					SPIx_CR2_RXDMAEN_bb(SPI4)

#define SPI5_CR2_TXEIE_bb					SPIx_CR2_TXEIE_bb(SPI5)
#define SPI5_CR2_RXNEIE_bb					SPIx_CR2_RXNEIE_bb(SPI5)
#define SPI5_CR2_ERRIE_bb					SPIx_CR2_ERRIE_bb(SPI5)
#define SPI5_CR2_FRF_bb						SPIx_CR2_FRF_bb(SPI5)
#define SPI5_CR2_SSOE_bb					SPIx_CR2_SSOE_bb(SPI5)
#define SPI5_CR2_TXDMAEN_bb					SPIx_CR2_TXDMAEN_bb(SPI5)
#define SPI5_CR2_RXDMAEN_bb					SPIx_CR2_RXDMAEN_bb(SPI5)

#define SPI6_CR2_TXEIE_bb					SPIx_CR2_TXEIE_bb(SPI6)
#define SPI6_CR2_RXNEIE_bb					SPIx_CR2_RXNEIE_bb(SPI6)
#define SPI6_CR2_ERRIE_bb					SPIx_CR2_ERRIE_bb(SPI6)
#define SPI6_CR2_FRF_bb						SPIx_CR2_FRF_bb(SPI6)
#define SPI6_CR2_SSOE_bb					SPIx_CR2_SSOE_bb(SPI6)
#define SPI6_CR2_TXDMAEN_bb					SPIx_CR2_TXDMAEN_bb(SPI6)
#define SPI6_CR2_RXDMAEN_bb					SPIx_CR2_RXDMAEN_bb(SPI6)

/*---------------------------------------------------------------------------------------------------------------------+
| SPI_SR - SPI status register
+---------------------------------------------------------------------------------------------------------------------*/

#define SPI_SR_FRE_bit						8
#define SPI_SR_BSY_bit						7
#define SPI_SR_OVR_bit						6
#define SPI_SR_MODF_bit						5
#define SPI_SR_CRCERR_bit					4
#define SPI_SR_UDR_bit						3
#define SPI_SR_CHSIDE_bit					2
#define SPI_SR_TXE_bit						1
#define SPI_SR_RXNE_bit						0

#define SPIx_SR_FRE_bb(base)				BITBAND(&(base)->SR, SPI_SR_FRE_bit)
#define SPIx_SR_BSY_bb(base)				BITBAND(&(base)->SR, SPI_SR_BSY_bit)
#define SPIx_SR_OVR_bb(base)				BITBAND(&(base)->SR, SPI_SR_OVR_bit)
#define SPIx_SR_MODF_bb(base)				BITBAND(&(base)->SR, SPI_SR_MODF_bit)
#define SPIx_SR_CRCERR_bb(base)				BITBAND(&(base)->SR, SPI_SR_CRCERR_bit)
#define SPIx_SR_UDR_bb(base)				BITBAND(&(base)->SR, SPI_SR_UDR_bit)
#define SPIx_SR_CHSIDE_bb(base)				BITBAND(&(base)->SR, SPI_SR_CHSIDE_bit)
#define SPIx_SR_TXE_bb(base)				BITBAND(&(base)->SR, SPI_SR_TXE_bit)
#define SPIx_SR_RXNE_bb(base)				BITBAND(&(base)->SR, SPI_SR_RXNE_bit)

#define SPI1_SR_FRE_bb						SPIx_SR_FRE_bb(SPI1)
#define SPI1_SR_BSY_bb						SPIx_SR_BSY_bb(SPI1)
#define SPI1_SR_OVR_bb						SPIx_SR_OVR_bb(SPI1)
#define SPI1_SR_MODF_bb						SPIx_SR_MODF_bb(SPI1)
#define SPI1_SR_CRCERR_bb					SPIx_SR_CRCERR_bb(SPI1)
#define SPI1_SR_UDR_bb						SPIx_SR_UDR_bb(SPI1)
#define SPI1_SR_CHSIDE_bb					SPIx_SR_CHSIDE_bb(SPI1)
#define SPI1_SR_TXE_bb						SPIx_SR_TXE_bb(SPI1)
#define SPI1_SR_RXNE_bb						SPIx_SR_RXNE_bb(SPI1)

#define SPI2_SR_FRE_bb						SPIx_SR_FRE_bb(SPI2)
#define SPI2_SR_BSY_bb						SPIx_SR_BSY_bb(SPI2)
#define SPI2_SR_OVR_bb						SPIx_SR_OVR_bb(SPI2)
#define SPI2_SR_MODF_bb						SPIx_SR_MODF_bb(SPI2)
#define SPI2_SR_CRCERR_bb					SPIx_SR_CRCERR_bb(SPI2)
#define SPI2_SR_UDR_bb						SPIx_SR_UDR_bb(SPI2)
#define SPI2_SR_CHSIDE_bb					SPIx_SR_CHSIDE_bb(SPI2)
#define SPI2_SR_TXE_bb						SPIx_SR_TXE_bb(SPI2)
#define SPI2_SR_RXNE_bb						SPIx_SR_RXNE_bb(SPI2)

#define SPI3_SR_FRE_bb						SPIx_SR_FRE_bb(SPI3)
#define SPI3_SR_BSY_bb						SPIx_SR_BSY_bb(SPI3)
#define SPI3_SR_OVR_bb						SPIx_SR_OVR_bb(SPI3)
#define SPI3_SR_MODF_bb						SPIx_SR_MODF_bb(SPI3)
#define SPI3_SR_CRCERR_bb					SPIx_SR_CRCERR_bb(SPI3)
#define SPI3_SR_UDR_bb						SPIx_SR_UDR_bb(SPI3)
#define SPI3_SR_CHSIDE_bb					SPIx_SR_CHSIDE_bb(SPI3)
#define SPI3_SR_TXE_bb						SPIx_SR_TXE_bb(SPI3)
#define SPI3_SR_RXNE_bb						SPIx_SR_RXNE_bb(SPI3)

#define SPI4_SR_FRE_bb						SPIx_SR_FRE_bb(SPI4)
#define SPI4_SR_BSY_bb						SPIx_SR_BSY_bb(SPI4)
#define SPI4_SR_OVR_bb						SPIx_SR_OVR_bb(SPI4)
#define SPI4_SR_MODF_bb						SPIx_SR_MODF_bb(SPI4)
#define SPI4_SR_CRCERR_bb					SPIx_SR_CRCERR_bb(SPI4)
#define SPI4_SR_UDR_bb						SPIx_SR_UDR_bb(SPI4)
#define SPI4_SR_CHSIDE_bb					SPIx_SR_CHSIDE_bb(SPI4)
#define SPI4_SR_TXE_bb						SPIx_SR_TXE_bb(SPI4)
#define SPI4_SR_RXNE_bb						SPIx_SR_RXNE_bb(SPI4)

#define SPI5_SR_FRE_bb						SPIx_SR_FRE_bb(SPI5)
#define SPI5_SR_BSY_bb						SPIx_SR_BSY_bb(SPI5)
#define SPI5_SR_OVR_bb						SPIx_SR_OVR_bb(SPI5)
#define SPI5_SR_MODF_bb						SPIx_SR_MODF_bb(SPI5)
#define SPI5_SR_CRCERR_bb					SPIx_SR_CRCERR_bb(SPI5)
#define SPI5_SR_UDR_bb						SPIx_SR_UDR_bb(SPI5)
#define SPI5_SR_CHSIDE_bb					SPIx_SR_CHSIDE_bb(SPI5)
#define SPI5_SR_TXE_bb						SPIx_SR_TXE_bb(SPI5)
#define SPI5_SR_RXNE_bb						SPIx_SR_RXNE_bb(SPI5)

#define SPI6_SR_FRE_bb						SPIx_SR_FRE_bb(SPI6)
#define SPI6_SR_BSY_bb						SPIx_SR_BSY_bb(SPI6)
#define SPI6_SR_OVR_bb						SPIx_SR_OVR_bb(SPI6)
#define SPI6_SR_MODF_bb						SPIx_SR_MODF_bb(SPI6)
#define SPI6_SR_CRCERR_bb					SPIx_SR_CRCERR_bb(SPI6)
#define SPI6_SR_UDR_bb						SPIx_SR_UDR_bb(SPI6)
#define SPI6_SR_CHSIDE_bb					SPIx_SR_CHSIDE_bb(SPI6)
#define SPI6_SR_TXE_bb						SPIx_SR_TXE_bb(SPI6)
#define SPI6_SR_RXNE_bb						SPIx_SR_RXNE_bb(SPI6)

/*---------------------------------------------------------------------------------------------------------------------+
| SPI_I2SCFGR - SPI_I2S configuration register
+---------------------------------------------------------------------------------------------------------------------*/

#define SPI_I2SCFGR_ASTREN_bit				12
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

#define SPI_I2SCFGR_DATLEN_16BIT_value		0
#define SPI_I2SCFGR_DATLEN_24BIT_value		1
#define SPI_I2SCFGR_DATLEN_32BIT_value		2
#define SPI_I2SCFGR_DATLEN_mask				3

#define SPI_I2SCFGR_I2SCFG_SLV_TX			(SPI_I2SCFGR_I2SCFG_SLV_TX_value << SPI_I2SCFGR_I2SCFG_bit)
#define SPI_I2SCFGR_I2SCFG_SLV_RX			(SPI_I2SCFGR_I2SCFG_SLV_RX_value << SPI_I2SCFGR_I2SCFG_bit)
#define SPI_I2SCFGR_I2SCFG_MSTR_TX			(SPI_I2SCFGR_I2SCFG_MSTR_TX_value << SPI_I2SCFGR_I2SCFG_bit)
#define SPI_I2SCFGR_I2SCFG_MSTR_RX			(SPI_I2SCFGR_I2SCFG_MSTR_RX_value << SPI_I2SCFGR_I2SCFG_bit)

#define SPI_I2SCFGR_I2SSTD_I2S				(SPI_I2SCFGR_I2SSTD_I2S_value << SPI_I2SCFGR_I2SSTD_bit)
#define SPI_I2SCFGR_I2SSTD_MSB				(SPI_I2SCFGR_I2SSTD_MSB_value << SPI_I2SCFGR_I2SSTD_bit)
#define SPI_I2SCFGR_I2SSTD_LSB				(SPI_I2SCFGR_I2SSTD_LSB_value << SPI_I2SCFGR_I2SSTD_bit)
#define SPI_I2SCFGR_I2SSTD_PCM				(SPI_I2SCFGR_I2SSTD_PCM_value << SPI_I2SCFGR_I2SSTD_bit)

#define SPI_I2SCFGR_DATLEN_16BIT			(SPI_I2SCFGR_DATLEN_16BIT_value << SPI_I2SCFGR_DATLEN_bit)
#define SPI_I2SCFGR_DATLEN_24BIT			(SPI_I2SCFGR_DATLEN_24BIT_value << SPI_I2SCFGR_DATLEN_bit)
#define SPI_I2SCFGR_DATLEN_32BIT			(SPI_I2SCFGR_DATLEN_32BIT_value << SPI_I2SCFGR_DATLEN_bit)

#define SPIx_I2SCFGR_ASTREN_bb(base)		BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_ASTREN_bit)
#define SPIx_I2SCFGR_I2SMOD_bb(base)		BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_I2SMOD_bit)
#define SPIx_I2SCFGR_I2SE_bb(base)			BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_I2SE_bit)
#define SPIx_I2SCFGR_I2SCFG_0_bb(base)		BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_I2SCFG_0_bit)
#define SPIx_I2SCFGR_I2SCFG_1_bb(base)		BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_I2SCFG_1_bit)
#define SPIx_I2SCFGR_PCMSYNC_bb(base)		BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_PCMSYNC_bit)
#define SPIx_I2SCFGR_I2SSTD_0_bb(base)		BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_I2SSTD_0_bit)
#define SPIx_I2SCFGR_I2SSTD_1_bb(base)		BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_I2SSTD_1_bit)
#define SPIx_I2SCFGR_CKPOL_bb(base)			BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_CKPOL_bit)
#define SPIx_I2SCFGR_DATLEN_0_bb(base)		BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_DATLEN_0_bit)
#define SPIx_I2SCFGR_DATLEN_1_bb(base)		BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_DATLEN_1_bit)
#define SPIx_I2SCFGR_CHLEN_bb(base)			BITBAND(&(base)->I2SCFGR, SPI_I2SCFGR_CHLEN_bit)

#define SPI1_I2SCFGR_ASTREN_bb				SPIx_I2SCFGR_ASTREN_bb(SPI1)
#define SPI1_I2SCFGR_I2SMOD_bb				SPIx_I2SCFGR_I2SMOD_bb(SPI1)
#define SPI1_I2SCFGR_I2SE_bb				SPIx_I2SCFGR_I2SE_bb(SPI1)
#define SPI1_I2SCFGR_I2SCFG_0_bb			SPIx_I2SCFGR_I2SCFG_0_bb(SPI1)
#define SPI1_I2SCFGR_I2SCFG_1_bb			SPIx_I2SCFGR_I2SCFG_1_bb(SPI1)
#define SPI1_I2SCFGR_PCMSYNC_bb				SPIx_I2SCFGR_PCMSYNC_bb(SPI1)
#define SPI1_I2SCFGR_I2SSTD_0_bb			SPIx_I2SCFGR_I2SSTD_0_bb(SPI1)
#define SPI1_I2SCFGR_I2SSTD_1_bb			SPIx_I2SCFGR_I2SSTD_1_bb(SPI1)
#define SPI1_I2SCFGR_CKPOL_bb				SPIx_I2SCFGR_CKPOL_bb(SPI1)
#define SPI1_I2SCFGR_DATLEN_0_bb			SPIx_I2SCFGR_DATLEN_0_bb(SPI1)
#define SPI1_I2SCFGR_DATLEN_1_bb			SPIx_I2SCFGR_DATLEN_1_bb(SPI1)
#define SPI1_I2SCFGR_CHLEN_bb				SPIx_I2SCFGR_CHLEN_bb(SPI1)

#define SPI2_I2SCFGR_ASTREN_bb				SPIx_I2SCFGR_ASTREN_bb(SPI2)
#define SPI2_I2SCFGR_I2SMOD_bb				SPIx_I2SCFGR_I2SMOD_bb(SPI2)
#define SPI2_I2SCFGR_I2SE_bb				SPIx_I2SCFGR_I2SE_bb(SPI2)
#define SPI2_I2SCFGR_I2SCFG_0_bb			SPIx_I2SCFGR_I2SCFG_0_bb(SPI2)
#define SPI2_I2SCFGR_I2SCFG_1_bb			SPIx_I2SCFGR_I2SCFG_1_bb(SPI2)
#define SPI2_I2SCFGR_PCMSYNC_bb				SPIx_I2SCFGR_PCMSYNC_bb(SPI2)
#define SPI2_I2SCFGR_I2SSTD_0_bb			SPIx_I2SCFGR_I2SSTD_0_bb(SPI2)
#define SPI2_I2SCFGR_I2SSTD_1_bb			SPIx_I2SCFGR_I2SSTD_1_bb(SPI2)
#define SPI2_I2SCFGR_CKPOL_bb				SPIx_I2SCFGR_CKPOL_bb(SPI2)
#define SPI2_I2SCFGR_DATLEN_0_bb			SPIx_I2SCFGR_DATLEN_0_bb(SPI2)
#define SPI2_I2SCFGR_DATLEN_1_bb			SPIx_I2SCFGR_DATLEN_1_bb(SPI2)
#define SPI2_I2SCFGR_CHLEN_bb				SPIx_I2SCFGR_CHLEN_bb(SPI2)

#define SPI3_I2SCFGR_ASTREN_bb				SPIx_I2SCFGR_ASTREN_bb(SPI3)
#define SPI3_I2SCFGR_I2SMOD_bb				SPIx_I2SCFGR_I2SMOD_bb(SPI3)
#define SPI3_I2SCFGR_I2SE_bb				SPIx_I2SCFGR_I2SE_bb(SPI3)
#define SPI3_I2SCFGR_I2SCFG_0_bb			SPIx_I2SCFGR_I2SCFG_0_bb(SPI3)
#define SPI3_I2SCFGR_I2SCFG_1_bb			SPIx_I2SCFGR_I2SCFG_1_bb(SPI3)
#define SPI3_I2SCFGR_PCMSYNC_bb				SPIx_I2SCFGR_PCMSYNC_bb(SPI3)
#define SPI3_I2SCFGR_I2SSTD_0_bb			SPIx_I2SCFGR_I2SSTD_0_bb(SPI3)
#define SPI3_I2SCFGR_I2SSTD_1_bb			SPIx_I2SCFGR_I2SSTD_1_bb(SPI3)
#define SPI3_I2SCFGR_CKPOL_bb				SPIx_I2SCFGR_CKPOL_bb(SPI3)
#define SPI3_I2SCFGR_DATLEN_0_bb			SPIx_I2SCFGR_DATLEN_0_bb(SPI3)
#define SPI3_I2SCFGR_DATLEN_1_bb			SPIx_I2SCFGR_DATLEN_1_bb(SPI3)
#define SPI3_I2SCFGR_CHLEN_bb				SPIx_I2SCFGR_CHLEN_bb(SPI3)

#define SPI4_I2SCFGR_ASTREN_bb				SPIx_I2SCFGR_ASTREN_bb(SPI4)
#define SPI4_I2SCFGR_I2SMOD_bb				SPIx_I2SCFGR_I2SMOD_bb(SPI4)
#define SPI4_I2SCFGR_I2SE_bb				SPIx_I2SCFGR_I2SE_bb(SPI4)
#define SPI4_I2SCFGR_I2SCFG_0_bb			SPIx_I2SCFGR_I2SCFG_0_bb(SPI4)
#define SPI4_I2SCFGR_I2SCFG_1_bb			SPIx_I2SCFGR_I2SCFG_1_bb(SPI4)
#define SPI4_I2SCFGR_PCMSYNC_bb				SPIx_I2SCFGR_PCMSYNC_bb(SPI4)
#define SPI4_I2SCFGR_I2SSTD_0_bb			SPIx_I2SCFGR_I2SSTD_0_bb(SPI4)
#define SPI4_I2SCFGR_I2SSTD_1_bb			SPIx_I2SCFGR_I2SSTD_1_bb(SPI4)
#define SPI4_I2SCFGR_CKPOL_bb				SPIx_I2SCFGR_CKPOL_bb(SPI4)
#define SPI4_I2SCFGR_DATLEN_0_bb			SPIx_I2SCFGR_DATLEN_0_bb(SPI4)
#define SPI4_I2SCFGR_DATLEN_1_bb			SPIx_I2SCFGR_DATLEN_1_bb(SPI4)
#define SPI4_I2SCFGR_CHLEN_bb				SPIx_I2SCFGR_CHLEN_bb(SPI4)

#define SPI5_I2SCFGR_ASTREN_bb				SPIx_I2SCFGR_ASTREN_bb(SPI5)
#define SPI5_I2SCFGR_I2SMOD_bb				SPIx_I2SCFGR_I2SMOD_bb(SPI5)
#define SPI5_I2SCFGR_I2SE_bb				SPIx_I2SCFGR_I2SE_bb(SPI5)
#define SPI5_I2SCFGR_I2SCFG_0_bb			SPIx_I2SCFGR_I2SCFG_0_bb(SPI5)
#define SPI5_I2SCFGR_I2SCFG_1_bb			SPIx_I2SCFGR_I2SCFG_1_bb(SPI5)
#define SPI5_I2SCFGR_PCMSYNC_bb				SPIx_I2SCFGR_PCMSYNC_bb(SPI5)
#define SPI5_I2SCFGR_I2SSTD_0_bb			SPIx_I2SCFGR_I2SSTD_0_bb(SPI5)
#define SPI5_I2SCFGR_I2SSTD_1_bb			SPIx_I2SCFGR_I2SSTD_1_bb(SPI5)
#define SPI5_I2SCFGR_CKPOL_bb				SPIx_I2SCFGR_CKPOL_bb(SPI5)
#define SPI5_I2SCFGR_DATLEN_0_bb			SPIx_I2SCFGR_DATLEN_0_bb(SPI5)
#define SPI5_I2SCFGR_DATLEN_1_bb			SPIx_I2SCFGR_DATLEN_1_bb(SPI5)
#define SPI5_I2SCFGR_CHLEN_bb				SPIx_I2SCFGR_CHLEN_bb(SPI5)

#define SPI6_I2SCFGR_ASTREN_bb				SPIx_I2SCFGR_ASTREN_bb(SPI6)
#define SPI6_I2SCFGR_I2SMOD_bb				SPIx_I2SCFGR_I2SMOD_bb(SPI6)
#define SPI6_I2SCFGR_I2SE_bb				SPIx_I2SCFGR_I2SE_bb(SPI6)
#define SPI6_I2SCFGR_I2SCFG_0_bb			SPIx_I2SCFGR_I2SCFG_0_bb(SPI6)
#define SPI6_I2SCFGR_I2SCFG_1_bb			SPIx_I2SCFGR_I2SCFG_1_bb(SPI6)
#define SPI6_I2SCFGR_PCMSYNC_bb				SPIx_I2SCFGR_PCMSYNC_bb(SPI6)
#define SPI6_I2SCFGR_I2SSTD_0_bb			SPIx_I2SCFGR_I2SSTD_0_bb(SPI6)
#define SPI6_I2SCFGR_I2SSTD_1_bb			SPIx_I2SCFGR_I2SSTD_1_bb(SPI6)
#define SPI6_I2SCFGR_CKPOL_bb				SPIx_I2SCFGR_CKPOL_bb(SPI6)
#define SPI6_I2SCFGR_DATLEN_0_bb			SPIx_I2SCFGR_DATLEN_0_bb(SPI6)
#define SPI6_I2SCFGR_DATLEN_1_bb			SPIx_I2SCFGR_DATLEN_1_bb(SPI6)
#define SPI6_I2SCFGR_CHLEN_bb				SPIx_I2SCFGR_CHLEN_bb(SPI6)

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

#define SPIx_I2SPR_MCKOE_bb(base)			BITBAND(&(base)->I2SPR, SPI_I2SPR_MCKOE_bit)
#define SPIx_I2SPR_ODD_bb(base)				BITBAND(&(base)->I2SPR, SPI_I2SPR_ODD_bit)
#define SPIx_I2SPR_I2SDIV_0_bb(base)		BITBAND(&(base)->I2SPR, SPI_I2SPR_I2SDIV_0_bit)
#define SPIx_I2SPR_I2SDIV_1_bb(base)		BITBAND(&(base)->I2SPR, SPI_I2SPR_I2SDIV_1_bit)
#define SPIx_I2SPR_I2SDIV_2_bb(base)		BITBAND(&(base)->I2SPR, SPI_I2SPR_I2SDIV_2_bit)
#define SPIx_I2SPR_I2SDIV_3_bb(base)		BITBAND(&(base)->I2SPR, SPI_I2SPR_I2SDIV_3_bit)
#define SPIx_I2SPR_I2SDIV_4_bb(base)		BITBAND(&(base)->I2SPR, SPI_I2SPR_I2SDIV_4_bit)
#define SPIx_I2SPR_I2SDIV_5_bb(base)		BITBAND(&(base)->I2SPR, SPI_I2SPR_I2SDIV_5_bit)
#define SPIx_I2SPR_I2SDIV_6_bb(base)		BITBAND(&(base)->I2SPR, SPI_I2SPR_I2SDIV_6_bit)
#define SPIx_I2SPR_I2SDIV_7_bb(base)		BITBAND(&(base)->I2SPR, SPI_I2SPR_I2SDIV_7_bit)

#define SPI1_I2SPR_MCKOE_bb					SPIx_I2SPR_MCKOE_bb(SPI1)
#define SPI1_I2SPR_ODD_bb					SPIx_I2SPR_ODD_bb(SPI1)
#define SPI1_I2SPR_I2SDIV_0_bb				SPIx_I2SPR_I2SDIV_0_bb(SPI1)
#define SPI1_I2SPR_I2SDIV_1_bb				SPIx_I2SPR_I2SDIV_1_bb(SPI1)
#define SPI1_I2SPR_I2SDIV_2_bb				SPIx_I2SPR_I2SDIV_2_bb(SPI1)
#define SPI1_I2SPR_I2SDIV_3_bb				SPIx_I2SPR_I2SDIV_3_bb(SPI1)
#define SPI1_I2SPR_I2SDIV_4_bb				SPIx_I2SPR_I2SDIV_4_bb(SPI1)
#define SPI1_I2SPR_I2SDIV_5_bb				SPIx_I2SPR_I2SDIV_5_bb(SPI1)
#define SPI1_I2SPR_I2SDIV_6_bb				SPIx_I2SPR_I2SDIV_6_bb(SPI1)
#define SPI1_I2SPR_I2SDIV_7_bb				SPIx_I2SPR_I2SDIV_7_bb(SPI1)

#define SPI2_I2SPR_MCKOE_bb					SPIx_I2SPR_MCKOE_bb(SPI2)
#define SPI2_I2SPR_ODD_bb					SPIx_I2SPR_ODD_bb(SPI2)
#define SPI2_I2SPR_I2SDIV_0_bb				SPIx_I2SPR_I2SDIV_0_bb(SPI2)
#define SPI2_I2SPR_I2SDIV_1_bb				SPIx_I2SPR_I2SDIV_1_bb(SPI2)
#define SPI2_I2SPR_I2SDIV_2_bb				SPIx_I2SPR_I2SDIV_2_bb(SPI2)
#define SPI2_I2SPR_I2SDIV_3_bb				SPIx_I2SPR_I2SDIV_3_bb(SPI2)
#define SPI2_I2SPR_I2SDIV_4_bb				SPIx_I2SPR_I2SDIV_4_bb(SPI2)
#define SPI2_I2SPR_I2SDIV_5_bb				SPIx_I2SPR_I2SDIV_5_bb(SPI2)
#define SPI2_I2SPR_I2SDIV_6_bb				SPIx_I2SPR_I2SDIV_6_bb(SPI2)
#define SPI2_I2SPR_I2SDIV_7_bb				SPIx_I2SPR_I2SDIV_7_bb(SPI2)

#define SPI3_I2SPR_MCKOE_bb					SPIx_I2SPR_MCKOE_bb(SPI3)
#define SPI3_I2SPR_ODD_bb					SPIx_I2SPR_ODD_bb(SPI3)
#define SPI3_I2SPR_I2SDIV_0_bb				SPIx_I2SPR_I2SDIV_0_bb(SPI3)
#define SPI3_I2SPR_I2SDIV_1_bb				SPIx_I2SPR_I2SDIV_1_bb(SPI3)
#define SPI3_I2SPR_I2SDIV_2_bb				SPIx_I2SPR_I2SDIV_2_bb(SPI3)
#define SPI3_I2SPR_I2SDIV_3_bb				SPIx_I2SPR_I2SDIV_3_bb(SPI3)
#define SPI3_I2SPR_I2SDIV_4_bb				SPIx_I2SPR_I2SDIV_4_bb(SPI3)
#define SPI3_I2SPR_I2SDIV_5_bb				SPIx_I2SPR_I2SDIV_5_bb(SPI3)
#define SPI3_I2SPR_I2SDIV_6_bb				SPIx_I2SPR_I2SDIV_6_bb(SPI3)
#define SPI3_I2SPR_I2SDIV_7_bb				SPIx_I2SPR_I2SDIV_7_bb(SPI3)

#define SPI4_I2SPR_MCKOE_bb					SPIx_I2SPR_MCKOE_bb(SPI4)
#define SPI4_I2SPR_ODD_bb					SPIx_I2SPR_ODD_bb(SPI4)
#define SPI4_I2SPR_I2SDIV_0_bb				SPIx_I2SPR_I2SDIV_0_bb(SPI4)
#define SPI4_I2SPR_I2SDIV_1_bb				SPIx_I2SPR_I2SDIV_1_bb(SPI4)
#define SPI4_I2SPR_I2SDIV_2_bb				SPIx_I2SPR_I2SDIV_2_bb(SPI4)
#define SPI4_I2SPR_I2SDIV_3_bb				SPIx_I2SPR_I2SDIV_3_bb(SPI4)
#define SPI4_I2SPR_I2SDIV_4_bb				SPIx_I2SPR_I2SDIV_4_bb(SPI4)
#define SPI4_I2SPR_I2SDIV_5_bb				SPIx_I2SPR_I2SDIV_5_bb(SPI4)
#define SPI4_I2SPR_I2SDIV_6_bb				SPIx_I2SPR_I2SDIV_6_bb(SPI4)
#define SPI4_I2SPR_I2SDIV_7_bb				SPIx_I2SPR_I2SDIV_7_bb(SPI4)

#define SPI5_I2SPR_MCKOE_bb					SPIx_I2SPR_MCKOE_bb(SPI5)
#define SPI5_I2SPR_ODD_bb					SPIx_I2SPR_ODD_bb(SPI5)
#define SPI5_I2SPR_I2SDIV_0_bb				SPIx_I2SPR_I2SDIV_0_bb(SPI5)
#define SPI5_I2SPR_I2SDIV_1_bb				SPIx_I2SPR_I2SDIV_1_bb(SPI5)
#define SPI5_I2SPR_I2SDIV_2_bb				SPIx_I2SPR_I2SDIV_2_bb(SPI5)
#define SPI5_I2SPR_I2SDIV_3_bb				SPIx_I2SPR_I2SDIV_3_bb(SPI5)
#define SPI5_I2SPR_I2SDIV_4_bb				SPIx_I2SPR_I2SDIV_4_bb(SPI5)
#define SPI5_I2SPR_I2SDIV_5_bb				SPIx_I2SPR_I2SDIV_5_bb(SPI5)
#define SPI5_I2SPR_I2SDIV_6_bb				SPIx_I2SPR_I2SDIV_6_bb(SPI5)
#define SPI5_I2SPR_I2SDIV_7_bb				SPIx_I2SPR_I2SDIV_7_bb(SPI5)

#define SPI6_I2SPR_MCKOE_bb					SPIx_I2SPR_MCKOE_bb(SPI6)
#define SPI6_I2SPR_ODD_bb					SPIx_I2SPR_ODD_bb(SPI6)
#define SPI6_I2SPR_I2SDIV_0_bb				SPIx_I2SPR_I2SDIV_0_bb(SPI6)
#define SPI6_I2SPR_I2SDIV_1_bb				SPIx_I2SPR_I2SDIV_1_bb(SPI6)
#define SPI6_I2SPR_I2SDIV_2_bb				SPIx_I2SPR_I2SDIV_2_bb(SPI6)
#define SPI6_I2SPR_I2SDIV_3_bb				SPIx_I2SPR_I2SDIV_3_bb(SPI6)
#define SPI6_I2SPR_I2SDIV_4_bb				SPIx_I2SPR_I2SDIV_4_bb(SPI6)
#define SPI6_I2SPR_I2SDIV_5_bb				SPIx_I2SPR_I2SDIV_5_bb(SPI6)
#define SPI6_I2SPR_I2SDIV_6_bb				SPIx_I2SPR_I2SDIV_6_bb(SPI6)
#define SPI6_I2SPR_I2SDIV_7_bb				SPIx_I2SPR_I2SDIV_7_bb(SPI6)

#endif /* SOURCE_CHIP_STM32_PERIPHERALS_SPIV1_INCLUDE_DISTORTOS_CHIP_STM32_SPIV1_BITS_H_ */
