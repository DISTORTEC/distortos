/**
 * \file
 * \brief Header with definition of bits in STM32 USARTv2 control registers
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_INCLUDE_DISTORTOS_CHIP_STM32_USARTV2_BITS_H_
#define SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_INCLUDE_DISTORTOS_CHIP_STM32_USARTV2_BITS_H_

/*---------------------------------------------------------------------------------------------------------------------+
| USART_CR1 - Control register 1
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_CR1_M1_bit					28
#define USART_CR1_EOBIE_bit					27
#define USART_CR1_RTOIE_bit					26
#define USART_CR1_DEAT_bit					21
#define USART_CR1_DEAT_0_bit				21
#define USART_CR1_DEAT_1_bit				22
#define USART_CR1_DEAT_2_bit				23
#define USART_CR1_DEAT_3_bit				24
#define USART_CR1_DEAT_4_bit				25
#define USART_CR1_DEDT_bit					16
#define USART_CR1_DEDT_0_bit				16
#define USART_CR1_DEDT_1_bit				17
#define USART_CR1_DEDT_2_bit				18
#define USART_CR1_DEDT_3_bit				19
#define USART_CR1_DEDT_4_bit				20
#define USART_CR1_OVER8_bit					15
#define USART_CR1_CMIE_bit					14
#define USART_CR1_MME_bit					13
#define USART_CR1_M0_bit					12
#define USART_CR1_WAKE_bit					11
#define USART_CR1_PCE_bit					10
#define USART_CR1_PS_bit					9
#define USART_CR1_PEIE_bit					8
#define USART_CR1_TXEIE_bit					7
#define USART_CR1_TCIE_bit					6
#define USART_CR1_RXNEIE_bit				5
#define USART_CR1_IDLEIE_bit				4
#define USART_CR1_TE_bit					3
#define USART_CR1_RE_bit					2
#define USART_CR1_UESM_bit					1
#define USART_CR1_UE_bit					0

/*---------------------------------------------------------------------------------------------------------------------+
| USART_CR2 - Control register 2
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_CR2_ADD_bit					24
#define USART_CR2_ADD_0_bit					24
#define USART_CR2_ADD_1_bit					25
#define USART_CR2_ADD_2_bit					26
#define USART_CR2_ADD_3_bit					27
#define USART_CR2_ADD_4_bit					28
#define USART_CR2_ADD_5_bit					29
#define USART_CR2_ADD_6_bit					30
#define USART_CR2_ADD_7_bit					31
#define USART_CR2_RTOEN_bit					23
#define USART_CR2_ABRMOD_bit				21
#define USART_CR2_ABRMOD_0_bit				21
#define USART_CR2_ABRMOD_1_bit				22
#define USART_CR2_ABREN_bit					20
#define USART_CR2_MSBFIRST_bit				19
#define USART_CR2_DATAINV_bit				18
#define USART_CR2_TXINV_bit					17
#define USART_CR2_RXINV_bit					16
#define USART_CR2_SWAP_bit					15
#define USART_CR2_LINEN_bit					14
#define USART_CR2_STOP_bit					12
#define USART_CR2_STOP_0_bit				12
#define USART_CR2_STOP_1_bit				13
#define USART_CR2_CLKEN_bit					11
#define USART_CR2_CPOL_bit					10
#define USART_CR2_CPHA_bit					9
#define USART_CR2_LBCL_bit					8
#define USART_CR2_LBDIE_bit					6
#define USART_CR2_LBDL_bit					5
#define USART_CR2_ADDM7_bit					4

#define USART_CR2_ABRMOD_START_value		0
#define USART_CR2_ABRMOD_FALLING_EDGE_value	1
#define USART_CR2_ABRMOD_0x7F_value			2
#define USART_CR2_ABRMOD_0x55_value			3
#define USART_CR2_ABRMOD_mask				3

#define USART_CR2_STOP_1BIT_value			0
#define USART_CR2_STOP_05BIT_value			1
#define USART_CR2_STOP_2BIT_value			2
#define USART_CR2_STOP_15BIT_value			3
#define USART_CR2_STOP_mask					3

#define USART_CR2_ABRMOD_START				(USART_CR2_ABRMOD_START_value << USART_CR2_ABRMOD_bit)
#define USART_CR2_ABRMOD_FALLING_EDGE		(USART_CR2_ABRMOD_FALLING_EDGE_value << USART_CR2_ABRMOD_bit)
#define USART_CR2_ABRMOD_0x7F				(USART_CR2_ABRMOD_0x7F_value << USART_CR2_ABRMOD_bit)
#define USART_CR2_ABRMOD_0x55				(USART_CR2_ABRMOD_0x55_value << USART_CR2_ABRMOD_bit)

#define USART_CR2_STOP_1BIT					(USART_CR2_STOP_1BIT_value << USART_CR2_STOP_bit)
#define USART_CR2_STOP_05BIT				(USART_CR2_STOP_05BIT_value << USART_CR2_STOP_bit)
#define USART_CR2_STOP_2BIT					(USART_CR2_STOP_2BIT_value << USART_CR2_STOP_bit)
#define USART_CR2_STOP_15BIT				(USART_CR2_STOP_15BIT_value << USART_CR2_STOP_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| USART_CR3 - Control register 3
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_CR3_WUFIE_bit					22
#define USART_CR3_WUS_bit					20
#define USART_CR3_WUS_0_bit					20
#define USART_CR3_WUS_1_bit					21
#define USART_CR3_SCARCNT_bit				17
#define USART_CR3_SCARCNT_0_bit				17
#define USART_CR3_SCARCNT_1_bit				18
#define USART_CR3_SCARCNT_2_bit				19
#define USART_CR3_DEP_bit					15
#define USART_CR3_DEM_bit					14
#define USART_CR3_DDRE_bit					13
#define USART_CR3_OVRDIS_bit				12
#define USART_CR3_ONEBIT_bit				11
#define USART_CR3_CTSIE_bit					10
#define USART_CR3_CTSE_bit					9
#define USART_CR3_RTSE_bit					8
#define USART_CR3_DMAT_bit					7
#define USART_CR3_DMAR_bit					6
#define USART_CR3_SCEN_bit					5
#define USART_CR3_NACK_bit					4
#define USART_CR3_HDSEL_bit					3
#define USART_CR3_IRLP_bit					2
#define USART_CR3_IREN_bit					1
#define USART_CR3_EIE_bit					0

#define USART_CR3_WUS_ADDRESS_value			0
#define USART_CR3_WUS_START_value			2
#define USART_CR3_WUS_RXNE_value			3
#define USART_CR3_WUS_mask					3

#define USART_CR3_WUS_ADDRESS				(USART_CR3_WUS_ADDRESS_value << USART_CR3_WUS_bit)
#define USART_CR3_WUS_START					(USART_CR3_WUS_START_value << USART_CR3_WUS_bit)
#define USART_CR3_WUS_RXNE					(USART_CR3_WUS_RXNE_value << USART_CR3_WUS_bit)

/*---------------------------------------------------------------------------------------------------------------------+
| USART_BRR - Baud rate register
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_BRR_DIV_MANTISSA_bit			4
#define USART_BRR_DIV_FRACTION_bit			0

/*---------------------------------------------------------------------------------------------------------------------+
| USART_GTPR - Guard time and prescaler register
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_GTPR_GT_bit					8
#define USART_GTPR_PSC_bit					0

/*---------------------------------------------------------------------------------------------------------------------+
| USART_RTOR - Receiver timeout register
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_RTOR_BLEN_bit					24
#define USART_RTOR_RTO_bit					0

/*---------------------------------------------------------------------------------------------------------------------+
| USART_RQR - Request register
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_RQR_TXFRQ_bit					4
#define USART_RQR_RXFRQ_bit					3
#define USART_RQR_MMRQ_bit					2
#define USART_RQR_SBKRQ_bit					1
#define USART_RQR_ABRRQ_bit					0

/*---------------------------------------------------------------------------------------------------------------------+
| USART_ISR - Interrupt & status register
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_ISR_REACK_bit					22
#define USART_ISR_TEACK_bit					21
#define USART_ISR_WUF_bit					20
#define USART_ISR_RWU_bit					19
#define USART_ISR_SBKF_bit					18
#define USART_ISR_CMF_bit					17
#define USART_ISR_BUSY_bit					16
#define USART_ISR_ABRF_bit					15
#define USART_ISR_ABRE_bit					14
#define USART_ISR_EOBF_bit					12
#define USART_ISR_RTOF_bit					11
#define USART_ISR_CTS_bit					10
#define USART_ISR_CTSIF_bit					9
#define USART_ISR_LBDF_bit					8
#define USART_ISR_TXE_bit					7
#define USART_ISR_TC_bit					6
#define USART_ISR_RXNE_bit					5
#define USART_ISR_IDLE_bit					4
#define USART_ISR_ORE_bit					3
#define USART_ISR_NF_bit					2
#define USART_ISR_FE_bit					1
#define USART_ISR_PE_bit					0

/*---------------------------------------------------------------------------------------------------------------------+
| USART_ICR - Interrupt flag clear register
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_ICR_WUCF_bit					20
#define USART_ICR_CMCF_bit					17
#define USART_ICR_EOBCF_bit					12
#define USART_ICR_RTOCF_bit					11
#define USART_ICR_CTSCF_bit					9
#define USART_ICR_LBDCF_bit					8
#define USART_ICR_TCCF_bit					6
#define USART_ICR_IDLECF_bit				4
#define USART_ICR_ORECF_bit					3
#define USART_ICR_NCF_bit					2
#define USART_ICR_FECF_bit					1
#define USART_ICR_PECF_bit					0

#endif /* SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_INCLUDE_DISTORTOS_CHIP_STM32_USARTV2_BITS_H_ */
