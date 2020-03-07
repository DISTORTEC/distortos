/**
 * \file
 * \brief Definitions of low-level SPI master drivers for SPIv1 in ST,NUCLEO-F401RE (ST,STM32F401RE chip)
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/chip/spis.hpp"

#include "distortos/chip/dmas.hpp"
#include "distortos/chip/PinInitializer.hpp"
#include "distortos/chip/SpiMasterLowLevelDmaBased.hpp"
#include "distortos/chip/SpiMasterLowLevelInterruptBased.hpp"
#include "distortos/chip/STM32-SPIv1-SpiPeripheral.hpp"

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

#ifdef DISTORTOS_CHIP_SPI2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SPI2
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

/**
 * \brief Low-level chip initializer for SPI2
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void spi2LowLevelInitializer()
{
#if defined(RCC_APB1ENR_SPI2EN)
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
#elif defined(RCC_APB2ENR_SPI2EN)
	RCC->APB2ENR |= RCC_APB2ENR_SPI2EN;
#else
	#error "Unsupported bus for SPI2!"
#endif
}

BIND_LOW_LEVEL_INITIALIZER(50, spi2LowLevelInitializer);

/// raw SPI2 peripheral
const SpiPeripheral spi2Peripheral {SPI2_BASE};

}	// namespace

#ifdef DISTORTOS_CHIP_SPI2_DMA_BASED

SpiMasterLowLevelDmaBased spi2
{
		spi2Peripheral,
		DMA_CHANNEL(DISTORTOS_CHIP_SPI2_RX_DMA, DISTORTOS_CHIP_SPI2_RX_DMA_CHANNEL),
		DISTORTOS_CHIP_SPI2_RX_DMA_REQUEST,
		DMA_CHANNEL(DISTORTOS_CHIP_SPI2_TX_DMA, DISTORTOS_CHIP_SPI2_TX_DMA_CHANNEL),
		DISTORTOS_CHIP_SPI2_TX_DMA_REQUEST
};

#else	// !def DISTORTOS_CHIP_SPI2_DMA_BASED

SpiMasterLowLevelInterruptBased spi2 {spi2Peripheral};

/**
 * \brief SPI2 interrupt handler
 */

extern "C" void SPI2_IRQHandler()
{
	spi2.interruptHandler();
}

#endif	// !def DISTORTOS_CHIP_SPI2_DMA_BASED

#endif	// def DISTORTOS_CHIP_SPI2_ENABLE

#ifdef DISTORTOS_CHIP_SPI3_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SPI3
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

/**
 * \brief Low-level chip initializer for SPI3
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void spi3LowLevelInitializer()
{
#if defined(RCC_APB1ENR_SPI3EN)
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
#elif defined(RCC_APB2ENR_SPI3EN)
	RCC->APB2ENR |= RCC_APB2ENR_SPI3EN;
#else
	#error "Unsupported bus for SPI3!"
#endif
}

BIND_LOW_LEVEL_INITIALIZER(50, spi3LowLevelInitializer);

/// raw SPI3 peripheral
const SpiPeripheral spi3Peripheral {SPI3_BASE};

}	// namespace

#ifdef DISTORTOS_CHIP_SPI3_DMA_BASED

SpiMasterLowLevelDmaBased spi3
{
		spi3Peripheral,
		DMA_CHANNEL(DISTORTOS_CHIP_SPI3_RX_DMA, DISTORTOS_CHIP_SPI3_RX_DMA_CHANNEL),
		DISTORTOS_CHIP_SPI3_RX_DMA_REQUEST,
		DMA_CHANNEL(DISTORTOS_CHIP_SPI3_TX_DMA, DISTORTOS_CHIP_SPI3_TX_DMA_CHANNEL),
		DISTORTOS_CHIP_SPI3_TX_DMA_REQUEST
};

#else	// !def DISTORTOS_CHIP_SPI3_DMA_BASED

SpiMasterLowLevelInterruptBased spi3 {spi3Peripheral};

/**
 * \brief SPI3 interrupt handler
 */

extern "C" void SPI3_IRQHandler()
{
	spi3.interruptHandler();
}

#endif	// !def DISTORTOS_CHIP_SPI3_DMA_BASED

#endif	// def DISTORTOS_CHIP_SPI3_ENABLE

}	// namespace chip

}	// namespace distortos
