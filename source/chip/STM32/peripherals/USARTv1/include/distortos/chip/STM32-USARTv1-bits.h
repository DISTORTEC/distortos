/**
 * \file
 * \brief Header with definition of bits in STM32 USARTv1 control registers
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_USARTV1_INCLUDE_DISTORTOS_CHIP_STM32_USARTV1_BITS_H_
#define SOURCE_CHIP_STM32_PERIPHERALS_USARTV1_INCLUDE_DISTORTOS_CHIP_STM32_USARTV1_BITS_H_

#include "distortos/architecture/ARMv7-M-bit-banding.h"

/*---------------------------------------------------------------------------------------------------------------------+
| USART_SR - Status register
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_SR_CTS_bit					9
#define USART_SR_LBD_bit					8
#define USART_SR_TXE_bit					7
#define USART_SR_TC_bit						6
#define USART_SR_RXNE_bit					5
#define USART_SR_IDLE_bit					4
#define USART_SR_ORE_bit					3
#define USART_SR_NF_bit						2
#define USART_SR_FE_bit						1
#define USART_SR_PE_bit						0

#define USARTx_SR_CTS_bb(base)				BITBAND(&(base)->SR, USART_SR_CTS_bit)
#define USARTx_SR_LBD_bb(base)				BITBAND(&(base)->SR, USART_SR_LBD_bit)
#define USARTx_SR_TXE_bb(base)				BITBAND(&(base)->SR, USART_SR_TXE_bit)
#define USARTx_SR_TC_bb(base)				BITBAND(&(base)->SR, USART_SR_TC_bit)
#define USARTx_SR_RXNE_bb(base)				BITBAND(&(base)->SR, USART_SR_RXNE_bit)
#define USARTx_SR_IDLE_bb(base)				BITBAND(&(base)->SR, USART_SR_IDLE_bit)
#define USARTx_SR_ORE_bb(base)				BITBAND(&(base)->SR, USART_SR_ORE_bit)
#define USARTx_SR_NF_bb(base)				BITBAND(&(base)->SR, USART_SR_NF_bit)
#define USARTx_SR_FE_bb(base)				BITBAND(&(base)->SR, USART_SR_FE_bit)
#define USARTx_SR_PE_bb(base)				BITBAND(&(base)->SR, USART_SR_PE_bit)

#define USART1_SR_CTS_bb					USARTx_SR_CTS_bb(USART1)
#define USART1_SR_LBD_bb					USARTx_SR_LBD_bb(USART1)
#define USART1_SR_TXE_bb					USARTx_SR_TXE_bb(USART1)
#define USART1_SR_TC_bb						USARTx_SR_TC_bb(USART1)
#define USART1_SR_RXNE_bb					USARTx_SR_RXNE_bb(USART1)
#define USART1_SR_IDLE_bb					USARTx_SR_IDLE_bb(USART1)
#define USART1_SR_ORE_bb					USARTx_SR_ORE_bb(USART1)
#define USART1_SR_NF_bb						USARTx_SR_NF_bb(USART1)
#define USART1_SR_FE_bb						USARTx_SR_FE_bb(USART1)
#define USART1_SR_PE_bb						USARTx_SR_PE_bb(USART1)

#define USART2_SR_CTS_bb					USARTx_SR_CTS_bb(USART2)
#define USART2_SR_LBD_bb					USARTx_SR_LBD_bb(USART2)
#define USART2_SR_TXE_bb					USARTx_SR_TXE_bb(USART2)
#define USART2_SR_TC_bb						USARTx_SR_TC_bb(USART2)
#define USART2_SR_RXNE_bb					USARTx_SR_RXNE_bb(USART2)
#define USART2_SR_IDLE_bb					USARTx_SR_IDLE_bb(USART2)
#define USART2_SR_ORE_bb					USARTx_SR_ORE_bb(USART2)
#define USART2_SR_NF_bb						USARTx_SR_NF_bb(USART2)
#define USART2_SR_FE_bb						USARTx_SR_FE_bb(USART2)
#define USART2_SR_PE_bb						USARTx_SR_PE_bb(USART2)

#define USART3_SR_CTS_bb					USARTx_SR_CTS_bb(USART3)
#define USART3_SR_LBD_bb					USARTx_SR_LBD_bb(USART3)
#define USART3_SR_TXE_bb					USARTx_SR_TXE_bb(USART3)
#define USART3_SR_TC_bb						USARTx_SR_TC_bb(USART3)
#define USART3_SR_RXNE_bb					USARTx_SR_RXNE_bb(USART3)
#define USART3_SR_IDLE_bb					USARTx_SR_IDLE_bb(USART3)
#define USART3_SR_ORE_bb					USARTx_SR_ORE_bb(USART3)
#define USART3_SR_NF_bb						USARTx_SR_NF_bb(USART3)
#define USART3_SR_FE_bb						USARTx_SR_FE_bb(USART3)
#define USART3_SR_PE_bb						USARTx_SR_PE_bb(USART3)

#define UART4_SR_LBD_bb						USARTx_SR_LBD_bb(UART4)
#define UART4_SR_TXE_bb						USARTx_SR_TXE_bb(UART4)
#define UART4_SR_TC_bb						USARTx_SR_TC_bb(UART4)
#define UART4_SR_RXNE_bb					USARTx_SR_RXNE_bb(UART4)
#define UART4_SR_IDLE_bb					USARTx_SR_IDLE_bb(UART4)
#define UART4_SR_ORE_bb						USARTx_SR_ORE_bb(UART4)
#define UART4_SR_NF_bb						USARTx_SR_NF_bb(UART4)
#define UART4_SR_FE_bb						USARTx_SR_FE_bb(UART4)
#define UART4_SR_PE_bb						USARTx_SR_PE_bb(UART4)

#define UART5_SR_LBD_bb						USARTx_SR_LBD_bb(UART5)
#define UART5_SR_TXE_bb						USARTx_SR_TXE_bb(UART5)
#define UART5_SR_TC_bb						USARTx_SR_TC_bb(UART5)
#define UART5_SR_RXNE_bb					USARTx_SR_RXNE_bb(UART5)
#define UART5_SR_IDLE_bb					USARTx_SR_IDLE_bb(UART5)
#define UART5_SR_ORE_bb						USARTx_SR_ORE_bb(UART5)
#define UART5_SR_NF_bb						USARTx_SR_NF_bb(UART5)
#define UART5_SR_FE_bb						USARTx_SR_FE_bb(UART5)
#define UART5_SR_PE_bb						USARTx_SR_PE_bb(UART5)

#define USART6_SR_CTS_bb					USARTx_SR_CTS_bb(USART6)
#define USART6_SR_LBD_bb					USARTx_SR_LBD_bb(USART6)
#define USART6_SR_TXE_bb					USARTx_SR_TXE_bb(USART6)
#define USART6_SR_TC_bb						USARTx_SR_TC_bb(USART6)
#define USART6_SR_RXNE_bb					USARTx_SR_RXNE_bb(USART6)
#define USART6_SR_IDLE_bb					USARTx_SR_IDLE_bb(USART6)
#define USART6_SR_ORE_bb					USARTx_SR_ORE_bb(USART6)
#define USART6_SR_NF_bb						USARTx_SR_NF_bb(USART6)
#define USART6_SR_FE_bb						USARTx_SR_FE_bb(USART6)
#define USART6_SR_PE_bb						USARTx_SR_PE_bb(USART6)

#define UART7_SR_LBD_bb						USARTx_SR_LBD_bb(UART7)
#define UART7_SR_TXE_bb						USARTx_SR_TXE_bb(UART7)
#define UART7_SR_TC_bb						USARTx_SR_TC_bb(UART7)
#define UART7_SR_RXNE_bb					USARTx_SR_RXNE_bb(UART7)
#define UART7_SR_IDLE_bb					USARTx_SR_IDLE_bb(UART7)
#define UART7_SR_ORE_bb						USARTx_SR_ORE_bb(UART7)
#define UART7_SR_NF_bb						USARTx_SR_NF_bb(UART7)
#define UART7_SR_FE_bb						USARTx_SR_FE_bb(UART7)
#define UART7_SR_PE_bb						USARTx_SR_PE_bb(UART7)

#define UART8_SR_LBD_bb						USARTx_SR_LBD_bb(UART8)
#define UART8_SR_TXE_bb						USARTx_SR_TXE_bb(UART8)
#define UART8_SR_TC_bb						USARTx_SR_TC_bb(UART8)
#define UART8_SR_RXNE_bb					USARTx_SR_RXNE_bb(UART8)
#define UART8_SR_IDLE_bb					USARTx_SR_IDLE_bb(UART8)
#define UART8_SR_ORE_bb						USARTx_SR_ORE_bb(UART8)
#define UART8_SR_NF_bb						USARTx_SR_NF_bb(UART8)
#define UART8_SR_FE_bb						USARTx_SR_FE_bb(UART8)
#define UART8_SR_PE_bb						USARTx_SR_PE_bb(UART8)

#define UART9_SR_LBD_bb						USARTx_SR_LBD_bb(UART9)
#define UART9_SR_TXE_bb						USARTx_SR_TXE_bb(UART9)
#define UART9_SR_TC_bb						USARTx_SR_TC_bb(UART9)
#define UART9_SR_RXNE_bb					USARTx_SR_RXNE_bb(UART9)
#define UART9_SR_IDLE_bb					USARTx_SR_IDLE_bb(UART9)
#define UART9_SR_ORE_bb						USARTx_SR_ORE_bb(UART9)
#define UART9_SR_NF_bb						USARTx_SR_NF_bb(UART9)
#define UART9_SR_FE_bb						USARTx_SR_FE_bb(UART9)
#define UART9_SR_PE_bb						USARTx_SR_PE_bb(UART9)

#define UART10_SR_LBD_bb					USARTx_SR_LBD_bb(UART10)
#define UART10_SR_TXE_bb					USARTx_SR_TXE_bb(UART10)
#define UART10_SR_TC_bb						USARTx_SR_TC_bb(UART10)
#define UART10_SR_RXNE_bb					USARTx_SR_RXNE_bb(UART10)
#define UART10_SR_IDLE_bb					USARTx_SR_IDLE_bb(UART10)
#define UART10_SR_ORE_bb					USARTx_SR_ORE_bb(UART10)
#define UART10_SR_NF_bb						USARTx_SR_NF_bb(UART10)
#define UART10_SR_FE_bb						USARTx_SR_FE_bb(UART10)
#define UART10_SR_PE_bb						USARTx_SR_PE_bb(UART10)

/*---------------------------------------------------------------------------------------------------------------------+
| USART_BRR - Baud rate register
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_BRR_DIV_Mantissa_bit			4
#define USART_BRR_DIV_Fraction_bit			0

/*---------------------------------------------------------------------------------------------------------------------+
| USART_CR1 - Control register 1
+---------------------------------------------------------------------------------------------------------------------*/

#define USART_CR1_OVER8_bit					15
#define USART_CR1_UE_bit					13
#define USART_CR1_M_bit						12
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
#define USART_CR1_RWU_bit					1
#define USART_CR1_SBK_bit					0

#define USARTx_CR1_OVER8_bb(base)			BITBAND(&(base)->CR1, USART_CR1_OVER8_bit)
#define USARTx_CR1_UE_bb(base)				BITBAND(&(base)->CR1, USART_CR1_UE_bit)
#define USARTx_CR1_M_bb(base)				BITBAND(&(base)->CR1, USART_CR1_M_bit)
#define USARTx_CR1_WAKE_bb(base)			BITBAND(&(base)->CR1, USART_CR1_WAKE_bit)
#define USARTx_CR1_PCE_bb(base)				BITBAND(&(base)->CR1, USART_CR1_PCE_bit)
#define USARTx_CR1_PS_bb(base)				BITBAND(&(base)->CR1, USART_CR1_PS_bit)
#define USARTx_CR1_PEIE_bb(base)			BITBAND(&(base)->CR1, USART_CR1_PEIE_bit)
#define USARTx_CR1_TXEIE_bb(base)			BITBAND(&(base)->CR1, USART_CR1_TXEIE_bit)
#define USARTx_CR1_TCIE_bb(base)			BITBAND(&(base)->CR1, USART_CR1_TCIE_bit)
#define USARTx_CR1_RXNEIE_bb(base)			BITBAND(&(base)->CR1, USART_CR1_RXNEIE_bit)
#define USARTx_CR1_IDLEIE_bb(base)			BITBAND(&(base)->CR1, USART_CR1_IDLEIE_bit)
#define USARTx_CR1_TE_bb(base)				BITBAND(&(base)->CR1, USART_CR1_TE_bit)
#define USARTx_CR1_RE_bb(base)				BITBAND(&(base)->CR1, USART_CR1_RE_bit)
#define USARTx_CR1_RWU_bb(base)				BITBAND(&(base)->CR1, USART_CR1_RWU_bit)
#define USARTx_CR1_SBK_bb(base)				BITBAND(&(base)->CR1, USART_CR1_SBK_bit)

#define USART1_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(USART1)
#define USART1_CR1_UE_bb					USARTx_CR1_UE_bb(USART1)
#define USART1_CR1_M_bb						USARTx_CR1_M_bb(USART1)
#define USART1_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(USART1)
#define USART1_CR1_PCE_bb					USARTx_CR1_PCE_bb(USART1)
#define USART1_CR1_PS_bb					USARTx_CR1_PS_bb(USART1)
#define USART1_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(USART1)
#define USART1_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(USART1)
#define USART1_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(USART1)
#define USART1_CR1_RXNEIE_bb				USARTx_CR1_RXNEIE_bb(USART1)
#define USART1_CR1_IDLEIE_bb				USARTx_CR1_IDLEIE_bb(USART1)
#define USART1_CR1_TE_bb					USARTx_CR1_TE_bb(USART1)
#define USART1_CR1_RE_bb					USARTx_CR1_RE_bb(USART1)
#define USART1_CR1_RWU_bb					USARTx_CR1_RWU_bb(USART1)
#define USART1_CR1_SBK_bb					USARTx_CR1_SBK_bb(USART1)

#define USART2_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(USART2)
#define USART2_CR1_UE_bb					USARTx_CR1_UE_bb(USART2)
#define USART2_CR1_M_bb						USARTx_CR1_M_bb(USART2)
#define USART2_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(USART2)
#define USART2_CR1_PCE_bb					USARTx_CR1_PCE_bb(USART2)
#define USART2_CR1_PS_bb					USARTx_CR1_PS_bb(USART2)
#define USART2_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(USART2)
#define USART2_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(USART2)
#define USART2_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(USART2)
#define USART2_CR1_RXNEIE_bb				USARTx_CR1_RXNEIE_bb(USART2)
#define USART2_CR1_IDLEIE_bb				USARTx_CR1_IDLEIE_bb(USART2)
#define USART2_CR1_TE_bb					USARTx_CR1_TE_bb(USART2)
#define USART2_CR1_RE_bb					USARTx_CR1_RE_bb(USART2)
#define USART2_CR1_RWU_bb					USARTx_CR1_RWU_bb(USART2)
#define USART2_CR1_SBK_bb					USARTx_CR1_SBK_bb(USART2)

#define USART3_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(USART3)
#define USART3_CR1_UE_bb					USARTx_CR1_UE_bb(USART3)
#define USART3_CR1_M_bb						USARTx_CR1_M_bb(USART3)
#define USART3_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(USART3)
#define USART3_CR1_PCE_bb					USARTx_CR1_PCE_bb(USART3)
#define USART3_CR1_PS_bb					USARTx_CR1_PS_bb(USART3)
#define USART3_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(USART3)
#define USART3_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(USART3)
#define USART3_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(USART3)
#define USART3_CR1_RXNEIE_bb				USARTx_CR1_RXNEIE_bb(USART3)
#define USART3_CR1_IDLEIE_bb				USARTx_CR1_IDLEIE_bb(USART3)
#define USART3_CR1_TE_bb					USARTx_CR1_TE_bb(USART3)
#define USART3_CR1_RE_bb					USARTx_CR1_RE_bb(USART3)
#define USART3_CR1_RWU_bb					USARTx_CR1_RWU_bb(USART3)
#define USART3_CR1_SBK_bb					USARTx_CR1_SBK_bb(USART3)

#define UART4_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(UART4)
#define UART4_CR1_UE_bb						USARTx_CR1_UE_bb(UART4)
#define UART4_CR1_M_bb						USARTx_CR1_M_bb(UART4)
#define UART4_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(UART4)
#define UART4_CR1_PCE_bb					USARTx_CR1_PCE_bb(UART4)
#define UART4_CR1_PS_bb						USARTx_CR1_PS_bb(UART4)
#define UART4_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(UART4)
#define UART4_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(UART4)
#define UART4_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(UART4)
#define UART4_CR1_RXNEIE_bb					USARTx_CR1_RXNEIE_bb(UART4)
#define UART4_CR1_IDLEIE_bb					USARTx_CR1_IDLEIE_bb(UART4)
#define UART4_CR1_TE_bb						USARTx_CR1_TE_bb(UART4)
#define UART4_CR1_RE_bb						USARTx_CR1_RE_bb(UART4)
#define UART4_CR1_RWU_bb					USARTx_CR1_RWU_bb(UART4)
#define UART4_CR1_SBK_bb					USARTx_CR1_SBK_bb(UART4)

#define UART5_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(UART5)
#define UART5_CR1_UE_bb						USARTx_CR1_UE_bb(UART5)
#define UART5_CR1_M_bb						USARTx_CR1_M_bb(UART5)
#define UART5_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(UART5)
#define UART5_CR1_PCE_bb					USARTx_CR1_PCE_bb(UART5)
#define UART5_CR1_PS_bb						USARTx_CR1_PS_bb(UART5)
#define UART5_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(UART5)
#define UART5_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(UART5)
#define UART5_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(UART5)
#define UART5_CR1_RXNEIE_bb					USARTx_CR1_RXNEIE_bb(UART5)
#define UART5_CR1_IDLEIE_bb					USARTx_CR1_IDLEIE_bb(UART5)
#define UART5_CR1_TE_bb						USARTx_CR1_TE_bb(UART5)
#define UART5_CR1_RE_bb						USARTx_CR1_RE_bb(UART5)
#define UART5_CR1_RWU_bb					USARTx_CR1_RWU_bb(UART5)
#define UART5_CR1_SBK_bb					USARTx_CR1_SBK_bb(UART5)

#define USART6_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(USART6)
#define USART6_CR1_UE_bb					USARTx_CR1_UE_bb(USART6)
#define USART6_CR1_M_bb						USARTx_CR1_M_bb(USART6)
#define USART6_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(USART6)
#define USART6_CR1_PCE_bb					USARTx_CR1_PCE_bb(USART6)
#define USART6_CR1_PS_bb					USARTx_CR1_PS_bb(USART6)
#define USART6_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(USART6)
#define USART6_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(USART6)
#define USART6_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(USART6)
#define USART6_CR1_RXNEIE_bb				USARTx_CR1_RXNEIE_bb(USART6)
#define USART6_CR1_IDLEIE_bb				USARTx_CR1_IDLEIE_bb(USART6)
#define USART6_CR1_TE_bb					USARTx_CR1_TE_bb(USART6)
#define USART6_CR1_RE_bb					USARTx_CR1_RE_bb(USART6)
#define USART6_CR1_RWU_bb					USARTx_CR1_RWU_bb(USART6)
#define USART6_CR1_SBK_bb					USARTx_CR1_SBK_bb(USART6)

#define UART7_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(UART7)
#define UART7_CR1_UE_bb						USARTx_CR1_UE_bb(UART7)
#define UART7_CR1_M_bb						USARTx_CR1_M_bb(UART7)
#define UART7_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(UART7)
#define UART7_CR1_PCE_bb					USARTx_CR1_PCE_bb(UART7)
#define UART7_CR1_PS_bb						USARTx_CR1_PS_bb(UART7)
#define UART7_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(UART7)
#define UART7_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(UART7)
#define UART7_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(UART7)
#define UART7_CR1_RXNEIE_bb					USARTx_CR1_RXNEIE_bb(UART7)
#define UART7_CR1_IDLEIE_bb					USARTx_CR1_IDLEIE_bb(UART7)
#define UART7_CR1_TE_bb						USARTx_CR1_TE_bb(UART7)
#define UART7_CR1_RE_bb						USARTx_CR1_RE_bb(UART7)
#define UART7_CR1_RWU_bb					USARTx_CR1_RWU_bb(UART7)
#define UART7_CR1_SBK_bb					USARTx_CR1_SBK_bb(UART7)

#define UART8_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(UART8)
#define UART8_CR1_UE_bb						USARTx_CR1_UE_bb(UART8)
#define UART8_CR1_M_bb						USARTx_CR1_M_bb(UART8)
#define UART8_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(UART8)
#define UART8_CR1_PCE_bb					USARTx_CR1_PCE_bb(UART8)
#define UART8_CR1_PS_bb						USARTx_CR1_PS_bb(UART8)
#define UART8_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(UART8)
#define UART8_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(UART8)
#define UART8_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(UART8)
#define UART8_CR1_RXNEIE_bb					USARTx_CR1_RXNEIE_bb(UART8)
#define UART8_CR1_IDLEIE_bb					USARTx_CR1_IDLEIE_bb(UART8)
#define UART8_CR1_TE_bb						USARTx_CR1_TE_bb(UART8)
#define UART8_CR1_RE_bb						USARTx_CR1_RE_bb(UART8)
#define UART8_CR1_RWU_bb					USARTx_CR1_RWU_bb(UART8)
#define UART8_CR1_SBK_bb					USARTx_CR1_SBK_bb(UART8)

#define UART9_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(UART9)
#define UART9_CR1_UE_bb						USARTx_CR1_UE_bb(UART9)
#define UART9_CR1_M_bb						USARTx_CR1_M_bb(UART9)
#define UART9_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(UART9)
#define UART9_CR1_PCE_bb					USARTx_CR1_PCE_bb(UART9)
#define UART9_CR1_PS_bb						USARTx_CR1_PS_bb(UART9)
#define UART9_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(UART9)
#define UART9_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(UART9)
#define UART9_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(UART9)
#define UART9_CR1_RXNEIE_bb					USARTx_CR1_RXNEIE_bb(UART9)
#define UART9_CR1_IDLEIE_bb					USARTx_CR1_IDLEIE_bb(UART9)
#define UART9_CR1_TE_bb						USARTx_CR1_TE_bb(UART9)
#define UART9_CR1_RE_bb						USARTx_CR1_RE_bb(UART9)
#define UART9_CR1_RWU_bb					USARTx_CR1_RWU_bb(UART9)
#define UART9_CR1_SBK_bb					USARTx_CR1_SBK_bb(UART9)

#define UART10_CR1_OVER8_bb					USARTx_CR1_OVER8_bb(UART10)
#define UART10_CR1_UE_bb					USARTx_CR1_UE_bb(UART10)
#define UART10_CR1_M_bb						USARTx_CR1_M_bb(UART10)
#define UART10_CR1_WAKE_bb					USARTx_CR1_WAKE_bb(UART10)
#define UART10_CR1_PCE_bb					USARTx_CR1_PCE_bb(UART10)
#define UART10_CR1_PS_bb					USARTx_CR1_PS_bb(UART10)
#define UART10_CR1_PEIE_bb					USARTx_CR1_PEIE_bb(UART10)
#define UART10_CR1_TXEIE_bb					USARTx_CR1_TXEIE_bb(UART10)
#define UART10_CR1_TCIE_bb					USARTx_CR1_TCIE_bb(UART10)
#define UART10_CR1_RXNEIE_bb				USARTx_CR1_RXNEIE_bb(UART10)
#define UART10_CR1_IDLEIE_bb				USARTx_CR1_IDLEIE_bb(UART10)
#define UART10_CR1_TE_bb					USARTx_CR1_TE_bb(UART10)
#define UART10_CR1_RE_bb					USARTx_CR1_RE_bb(UART10)
#define UART10_CR1_RWU_bb					USARTx_CR1_RWU_bb(UART10)
#define UART10_CR1_SBK_bb					USARTx_CR1_SBK_bb(UART10)

/*---------------------------------------------------------------------------------------------------------------------+
| USART_CR2 - Control register 2
+---------------------------------------------------------------------------------------------------------------------*/

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
#define USART_CR2_ADD_bit					0
#define USART_CR2_ADD_0_bit					0
#define USART_CR2_ADD_1_bit					1
#define USART_CR2_ADD_2_bit					2
#define USART_CR2_ADD_3_bit					3

#define USART_CR2_STOP_1BIT_value			0
#define USART_CR2_STOP_05BIT_value			1
#define USART_CR2_STOP_2BIT_value			2
#define USART_CR2_STOP_15BIT_value			3
#define USART_CR2_STOP_mask					3

#define USART_CR2_STOP_1BIT					(USART_CR2_STOP_1BIT_value << USART_CR2_STOP_bit)
#define USART_CR2_STOP_05BIT				(USART_CR2_STOP_05BIT_value << USART_CR2_STOP_bit)
#define USART_CR2_STOP_2BIT					(USART_CR2_STOP_2BIT_value << USART_CR2_STOP_bit)
#define USART_CR2_STOP_15BIT				(USART_CR2_STOP_15BIT_value << USART_CR2_STOP_bit)

#define USARTx_CR2_LINEN_bb(base)			BITBAND(&(base)->CR2, USART_CR2_LINEN_bit)
#define USARTx_CR2_STOP_0_bb(base)			BITBAND(&(base)->CR2, USART_CR2_STOP_0_bit)
#define USARTx_CR2_STOP_1_bb(base)			BITBAND(&(base)->CR2, USART_CR2_STOP_1_bit)
#define USARTx_CR2_CLKEN_bb(base)			BITBAND(&(base)->CR2, USART_CR2_CLKEN_bit)
#define USARTx_CR2_CPOL_bb(base)			BITBAND(&(base)->CR2, USART_CR2_CPOL_bit)
#define USARTx_CR2_CPHA_bb(base)			BITBAND(&(base)->CR2, USART_CR2_CPHA_bit)
#define USARTx_CR2_LBCL_bb(base)			BITBAND(&(base)->CR2, USART_CR2_LBCL_bit)
#define USARTx_CR2_LBDIE_bb(base)			BITBAND(&(base)->CR2, USART_CR2_LBDIE_bit)
#define USARTx_CR2_LBDL_bb(base)			BITBAND(&(base)->CR2, USART_CR2_LBDL_bit)
#define USARTx_CR2_ADD_0_bb(base)			BITBAND(&(base)->CR2, USART_CR2_ADD_0_bit)
#define USARTx_CR2_ADD_1_bb(base)			BITBAND(&(base)->CR2, USART_CR2_ADD_1_bit)
#define USARTx_CR2_ADD_2_bb(base)			BITBAND(&(base)->CR2, USART_CR2_ADD_2_bit)
#define USARTx_CR2_ADD_3_bb(base)			BITBAND(&(base)->CR2, USART_CR2_ADD_3_bit)

#define USART1_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(USART1)
#define USART1_CR2_STOP_0_bb				USARTx_CR2_STOP_0_bb(USART1)
#define USART1_CR2_STOP_1_bb				USARTx_CR2_STOP_1_bb(USART1)
#define USART1_CR2_CLKEN_bb					USARTx_CR2_CLKEN_bb(USART1)
#define USART1_CR2_CPOL_bb					USARTx_CR2_CPOL_bb(USART1)
#define USART1_CR2_CPHA_bb					USARTx_CR2_CPHA_bb(USART1)
#define USART1_CR2_LBCL_bb					USARTx_CR2_LBCL_bb(USART1)
#define USART1_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(USART1)
#define USART1_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(USART1)
#define USART1_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(USART1)
#define USART1_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(USART1)
#define USART1_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(USART1)
#define USART1_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(USART1)

#define USART2_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(USART2)
#define USART2_CR2_STOP_0_bb				USARTx_CR2_STOP_0_bb(USART2)
#define USART2_CR2_STOP_1_bb				USARTx_CR2_STOP_1_bb(USART2)
#define USART2_CR2_CLKEN_bb					USARTx_CR2_CLKEN_bb(USART2)
#define USART2_CR2_CPOL_bb					USARTx_CR2_CPOL_bb(USART2)
#define USART2_CR2_CPHA_bb					USARTx_CR2_CPHA_bb(USART2)
#define USART2_CR2_LBCL_bb					USARTx_CR2_LBCL_bb(USART2)
#define USART2_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(USART2)
#define USART2_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(USART2)
#define USART2_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(USART2)
#define USART2_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(USART2)
#define USART2_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(USART2)
#define USART2_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(USART2)

#define USART3_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(USART3)
#define USART3_CR2_STOP_0_bb				USARTx_CR2_STOP_0_bb(USART3)
#define USART3_CR2_STOP_1_bb				USARTx_CR2_STOP_1_bb(USART3)
#define USART3_CR2_CLKEN_bb					USARTx_CR2_CLKEN_bb(USART3)
#define USART3_CR2_CPOL_bb					USARTx_CR2_CPOL_bb(USART3)
#define USART3_CR2_CPHA_bb					USARTx_CR2_CPHA_bb(USART3)
#define USART3_CR2_LBCL_bb					USARTx_CR2_LBCL_bb(USART3)
#define USART3_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(USART3)
#define USART3_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(USART3)
#define USART3_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(USART3)
#define USART3_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(USART3)
#define USART3_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(USART3)
#define USART3_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(USART3)

#define UART4_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(UART4)
#define UART4_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(UART4)
#define UART4_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(UART4)
#define UART4_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(UART4)
#define UART4_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(UART4)
#define UART4_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(UART4)
#define UART4_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(UART4)

#define UART5_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(UART5)
#define UART5_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(UART5)
#define UART5_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(UART5)
#define UART5_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(UART5)
#define UART5_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(UART5)
#define UART5_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(UART5)
#define UART5_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(UART5)

#define USART6_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(USART6)
#define USART6_CR2_STOP_0_bb				USARTx_CR2_STOP_0_bb(USART6)
#define USART6_CR2_STOP_1_bb				USARTx_CR2_STOP_1_bb(USART6)
#define USART6_CR2_CLKEN_bb					USARTx_CR2_CLKEN_bb(USART6)
#define USART6_CR2_CPOL_bb					USARTx_CR2_CPOL_bb(USART6)
#define USART6_CR2_CPHA_bb					USARTx_CR2_CPHA_bb(USART6)
#define USART6_CR2_LBCL_bb					USARTx_CR2_LBCL_bb(USART6)
#define USART6_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(USART6)
#define USART6_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(USART6)
#define USART6_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(USART6)
#define USART6_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(USART6)
#define USART6_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(USART6)
#define USART6_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(USART6)

#define UART7_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(UART7)
#define UART7_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(UART7)
#define UART7_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(UART7)
#define UART7_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(UART7)
#define UART7_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(UART7)
#define UART7_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(UART7)
#define UART7_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(UART7)

#define UART8_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(UART8)
#define UART8_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(UART8)
#define UART8_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(UART8)
#define UART8_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(UART8)
#define UART8_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(UART8)
#define UART8_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(UART8)
#define UART8_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(UART8)

#define UART9_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(UART9)
#define UART9_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(UART9)
#define UART9_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(UART9)
#define UART9_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(UART9)
#define UART9_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(UART9)
#define UART9_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(UART9)
#define UART9_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(UART9)

#define UART10_CR2_LINEN_bb					USARTx_CR2_LINEN_bb(UART10)
#define UART10_CR2_LBDIE_bb					USARTx_CR2_LBDIE_bb(UART10)
#define UART10_CR2_LBDL_bb					USARTx_CR2_LBDL_bb(UART10)
#define UART10_CR2_ADD_0_bb					USARTx_CR2_ADD_0_bb(UART10)
#define UART10_CR2_ADD_1_bb					USARTx_CR2_ADD_1_bb(UART10)
#define UART10_CR2_ADD_2_bb					USARTx_CR2_ADD_2_bb(UART10)
#define UART10_CR2_ADD_3_bb					USARTx_CR2_ADD_3_bb(UART10)

/*---------------------------------------------------------------------------------------------------------------------+
| USART_CR3 - Control register 3
+---------------------------------------------------------------------------------------------------------------------*/

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

#define USARTx_CR3_ONEBIT_bb(base)			BITBAND(&(base)->CR3, USART_CR3_ONEBIT_bit)
#define USARTx_CR3_CTSIE_bb(base)			BITBAND(&(base)->CR3, USART_CR3_CTSIE_bit)
#define USARTx_CR3_CTSE_bb(base)			BITBAND(&(base)->CR3, USART_CR3_CTSE_bit)
#define USARTx_CR3_RTSE_bb(base)			BITBAND(&(base)->CR3, USART_CR3_RTSE_bit)
#define USARTx_CR3_DMAT_bb(base)			BITBAND(&(base)->CR3, USART_CR3_DMAT_bit)
#define USARTx_CR3_DMAR_bb(base)			BITBAND(&(base)->CR3, USART_CR3_DMAR_bit)
#define USARTx_CR3_SCEN_bb(base)			BITBAND(&(base)->CR3, USART_CR3_SCEN_bit)
#define USARTx_CR3_NACK_bb(base)			BITBAND(&(base)->CR3, USART_CR3_NACK_bit)
#define USARTx_CR3_HDSEL_bb(base)			BITBAND(&(base)->CR3, USART_CR3_HDSEL_bit)
#define USARTx_CR3_IRLP_bb(base)			BITBAND(&(base)->CR3, USART_CR3_IRLP_bit)
#define USARTx_CR3_IREN_bb(base)			BITBAND(&(base)->CR3, USART_CR3_IREN_bit)
#define USARTx_CR3_EIE_bb(base)				BITBAND(&(base)->CR3, USART_CR3_EIE_bit)

#define USART1_CR3_ONEBIT_bb				USARTx_CR3_ONEBIT_bb(USART1)
#define USART1_CR3_CTSIE_bb					USARTx_CR3_CTSIE_bb(USART1)
#define USART1_CR3_CTSE_bb					USARTx_CR3_CTSE_bb(USART1)
#define USART1_CR3_RTSE_bb					USARTx_CR3_RTSE_bb(USART1)
#define USART1_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(USART1)
#define USART1_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(USART1)
#define USART1_CR3_SCEN_bb					USARTx_CR3_SCEN_bb(USART1)
#define USART1_CR3_NACK_bb					USARTx_CR3_NACK_bb(USART1)
#define USART1_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(USART1)
#define USART1_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(USART1)
#define USART1_CR3_IREN_bb					USARTx_CR3_IREN_bb(USART1)
#define USART1_CR3_EIE_bb					USARTx_CR3_EIE_bb(USART1)

#define USART2_CR3_ONEBIT_bb				USARTx_CR3_ONEBIT_bb(USART2)
#define USART2_CR3_CTSIE_bb					USARTx_CR3_CTSIE_bb(USART2)
#define USART2_CR3_CTSE_bb					USARTx_CR3_CTSE_bb(USART2)
#define USART2_CR3_RTSE_bb					USARTx_CR3_RTSE_bb(USART2)
#define USART2_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(USART2)
#define USART2_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(USART2)
#define USART2_CR3_SCEN_bb					USARTx_CR3_SCEN_bb(USART2)
#define USART2_CR3_NACK_bb					USARTx_CR3_NACK_bb(USART2)
#define USART2_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(USART2)
#define USART2_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(USART2)
#define USART2_CR3_IREN_bb					USARTx_CR3_IREN_bb(USART2)
#define USART2_CR3_EIE_bb					USARTx_CR3_EIE_bb(USART2)

#define USART3_CR3_ONEBIT_bb				USARTx_CR3_ONEBIT_bb(USART3)
#define USART3_CR3_CTSIE_bb					USARTx_CR3_CTSIE_bb(USART3)
#define USART3_CR3_CTSE_bb					USARTx_CR3_CTSE_bb(USART3)
#define USART3_CR3_RTSE_bb					USARTx_CR3_RTSE_bb(USART3)
#define USART3_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(USART3)
#define USART3_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(USART3)
#define USART3_CR3_SCEN_bb					USARTx_CR3_SCEN_bb(USART3)
#define USART3_CR3_NACK_bb					USARTx_CR3_NACK_bb(USART3)
#define USART3_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(USART3)
#define USART3_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(USART3)
#define USART3_CR3_IREN_bb					USARTx_CR3_IREN_bb(USART3)
#define USART3_CR3_EIE_bb					USARTx_CR3_EIE_bb(USART3)

#define UART4_CR3_ONEBIT_bb					USARTx_CR3_ONEBIT_bb(UART4)
#define UART4_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(UART4)
#define UART4_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(UART4)
#define UART4_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(UART4)
#define UART4_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(UART4)
#define UART4_CR3_IREN_bb					USARTx_CR3_IREN_bb(UART4)
#define UART4_CR3_EIE_bb					USARTx_CR3_EIE_bb(UART4)

#define UART5_CR3_ONEBIT_bb					USARTx_CR3_ONEBIT_bb(UART5)
#define UART5_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(UART5)
#define UART5_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(UART5)
#define UART5_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(UART5)
#define UART5_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(UART5)
#define UART5_CR3_IREN_bb					USARTx_CR3_IREN_bb(UART5)
#define UART5_CR3_EIE_bb					USARTx_CR3_EIE_bb(UART5)

#define USART6_CR3_ONEBIT_bb				USARTx_CR3_ONEBIT_bb(USART6)
#define USART6_CR3_CTSIE_bb					USARTx_CR3_CTSIE_bb(USART6)
#define USART6_CR3_CTSE_bb					USARTx_CR3_CTSE_bb(USART6)
#define USART6_CR3_RTSE_bb					USARTx_CR3_RTSE_bb(USART6)
#define USART6_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(USART6)
#define USART6_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(USART6)
#define USART6_CR3_SCEN_bb					USARTx_CR3_SCEN_bb(USART6)
#define USART6_CR3_NACK_bb					USARTx_CR3_NACK_bb(USART6)
#define USART6_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(USART6)
#define USART6_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(USART6)
#define USART6_CR3_IREN_bb					USARTx_CR3_IREN_bb(USART6)
#define USART6_CR3_EIE_bb					USARTx_CR3_EIE_bb(USART6)

#define UART7_CR3_ONEBIT_bb					USARTx_CR3_ONEBIT_bb(UART7)
#define UART7_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(UART7)
#define UART7_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(UART7)
#define UART7_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(UART7)
#define UART7_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(UART7)
#define UART7_CR3_IREN_bb					USARTx_CR3_IREN_bb(UART7)
#define UART7_CR3_EIE_bb					USARTx_CR3_EIE_bb(UART7)

#define UART8_CR3_ONEBIT_bb					USARTx_CR3_ONEBIT_bb(UART8)
#define UART8_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(UART8)
#define UART8_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(UART8)
#define UART8_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(UART8)
#define UART8_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(UART8)
#define UART8_CR3_IREN_bb					USARTx_CR3_IREN_bb(UART8)
#define UART8_CR3_EIE_bb					USARTx_CR3_EIE_bb(UART8)

#define UART9_CR3_ONEBIT_bb					USARTx_CR3_ONEBIT_bb(UART9)
#define UART9_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(UART9)
#define UART9_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(UART9)
#define UART9_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(UART9)
#define UART9_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(UART9)
#define UART9_CR3_IREN_bb					USARTx_CR3_IREN_bb(UART9)
#define UART9_CR3_EIE_bb					USARTx_CR3_EIE_bb(UART9)

#define UART10_CR3_ONEBIT_bb				USARTx_CR3_ONEBIT_bb(UART10)
#define UART10_CR3_DMAT_bb					USARTx_CR3_DMAT_bb(UART10)
#define UART10_CR3_DMAR_bb					USARTx_CR3_DMAR_bb(UART10)
#define UART10_CR3_HDSEL_bb					USARTx_CR3_HDSEL_bb(UART10)
#define UART10_CR3_IRLP_bb					USARTx_CR3_IRLP_bb(UART10)
#define UART10_CR3_IREN_bb					USARTx_CR3_IREN_bb(UART10)
#define UART10_CR3_EIE_bb					USARTx_CR3_EIE_bb(UART10)

#endif /* SOURCE_CHIP_STM32_PERIPHERALS_USARTV1_INCLUDE_DISTORTOS_CHIP_STM32_USARTV1_BITS_H_ */
