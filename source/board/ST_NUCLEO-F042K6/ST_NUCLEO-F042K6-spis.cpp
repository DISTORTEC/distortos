/**
 * \file
 * \brief Definitions of low-level SPI master drivers for SPIv2 in ST,NUCLEO-F042K6 (ST,STM32F042K6 chip)
 *
 * \author Copyright (C) 2016-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/chip/spis.hpp"

#include "distortos/chip/dmas.hpp"
#include "distortos/chip/PinInitializer.hpp"
#include "distortos/chip/SpiMasterLowLevelDmaBased.hpp"
#include "distortos/chip/SpiMasterLowLevelInterruptBased.hpp"
#include "distortos/chip/STM32-SPIv2-SpiPeripheral.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

/**
 * \brief Generates name of DMA channel object in the form `dma<dmaId>Channel<channelId>`.
 *
 * \param [in] dmaId is a DMA identifier
 * \param [in] channelId is a DMA channel identifier
 */

#define DMA_CHANNEL(dmaId, channelId)	CONCATENATE4(dma, dmaId, Channel, channelId)

namespace distortos
{

namespace chip
{

#ifdef DISTORTOS_CHIP_SPI1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SPI1
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

/**
 * \brief Low-level chip initializer for SPI1
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void spi1LowLevelInitializer()
{
#if defined(RCC_APB1ENR_SPI1EN)
	RCC->APB1ENR |= RCC_APB1ENR_SPI1EN;
#elif defined(RCC_APB1ENR1_SPI1EN)
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI1EN;
#elif defined(RCC_APB2ENR_SPI1EN)
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
#else
	#error "Unsupported bus for SPI1!"
#endif
}

BIND_LOW_LEVEL_INITIALIZER(50, spi1LowLevelInitializer);

/// raw SPI1 peripheral
const SpiPeripheral spi1Peripheral {SPI1_BASE};

}	// namespace

#ifdef DISTORTOS_CHIP_SPI1_DMA_BASED

SpiMasterLowLevelDmaBased spi1
{
		spi1Peripheral,
		DMA_CHANNEL(DISTORTOS_CHIP_SPI1_RX_DMA, DISTORTOS_CHIP_SPI1_RX_DMA_CHANNEL),
		DISTORTOS_CHIP_SPI1_RX_DMA_REQUEST,
		DMA_CHANNEL(DISTORTOS_CHIP_SPI1_TX_DMA, DISTORTOS_CHIP_SPI1_TX_DMA_CHANNEL),
		DISTORTOS_CHIP_SPI1_TX_DMA_REQUEST
};

#else	// !def DISTORTOS_CHIP_SPI1_DMA_BASED

SpiMasterLowLevelInterruptBased spi1 {spi1Peripheral};

/**
 * \brief SPI1 interrupt handler
 */

extern "C" void SPI1_IRQHandler()
{
	spi1.interruptHandler();
}

#endif	// !def DISTORTOS_CHIP_SPI1_DMA_BASED

#endif	// def DISTORTOS_CHIP_SPI1_ENABLE

}	// namespace chip

}	// namespace distortos
