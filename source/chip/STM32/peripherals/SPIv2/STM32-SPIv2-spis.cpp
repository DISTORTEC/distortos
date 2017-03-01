/**
 * \file
 * \brief Definitions of low-level SPI master drivers for SPIv2 in STM32
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/spis.hpp"

#include "distortos/chip/ChipSpiMasterLowLevel.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| SPI1 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE

ChipSpiMasterLowLevel spi1 {ChipSpiMasterLowLevel::spi1Parameters};

/**
 * \brief SPI1 interrupt handler
 */

extern "C" void SPI1_IRQHandler()
{
	spi1.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SPI2 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI2_ENABLE

ChipSpiMasterLowLevel spi2 {ChipSpiMasterLowLevel::spi2Parameters};

/**
 * \brief SPI2 interrupt handler
 */

extern "C" void SPI2_IRQHandler()
{
	spi2.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SPI3 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI3_ENABLE

ChipSpiMasterLowLevel spi3 {ChipSpiMasterLowLevel::spi3Parameters};

/**
 * \brief SPI3 interrupt handler
 */

extern "C" void SPI3_IRQHandler()
{
	spi3.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI3_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SPI4 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI4_ENABLE

ChipSpiMasterLowLevel spi4 {ChipSpiMasterLowLevel::spi4Parameters};

/**
 * \brief SPI4 interrupt handler
 */

extern "C" void SPI4_IRQHandler()
{
	spi4.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI4_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SPI5 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI5_ENABLE

ChipSpiMasterLowLevel spi5 {ChipSpiMasterLowLevel::spi5Parameters};

/**
 * \brief SPI5 interrupt handler
 */

extern "C" void SPI5_IRQHandler()
{
	spi5.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI5_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SPI6 global objects and functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI6_ENABLE

ChipSpiMasterLowLevel spi6 {ChipSpiMasterLowLevel::spi6Parameters};

/**
 * \brief SPI6 interrupt handler
 */

extern "C" void SPI6_IRQHandler()
{
	spi6.interruptHandler();
}

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI6_ENABLE

}	// namespace chip

}	// namespace distortos
