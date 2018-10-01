/**
 * \file
 * \brief Definitions of low-level SPI master drivers for SPIv2 in ST,NUCLEO-L432KC (ST,STM32L432KC chip)
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/chip/spis.hpp"

#include "distortos/chip/ChipSpiMasterLowLevel.hpp"
#include "distortos/chip/STM32-SPIv2-SpiPeripheral.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

namespace distortos
{

namespace chip
{

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE

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

ChipSpiMasterLowLevel spi1 {spi1Peripheral};

/**
 * \brief SPI1 interrupt handler
 */

extern "C" void SPI1_IRQHandler()
{
	spi1.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI3_ENABLE

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
#elif defined(RCC_APB1ENR1_SPI3EN)
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
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

ChipSpiMasterLowLevel spi3 {spi3Peripheral};

/**
 * \brief SPI3 interrupt handler
 */

extern "C" void SPI3_IRQHandler()
{
	spi3.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI3_ENABLE

}	// namespace chip

}	// namespace distortos
