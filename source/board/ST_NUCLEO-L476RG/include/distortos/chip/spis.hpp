/**
 * \file
 * \brief Declarations of low-level SPI master drivers for SPIv2 in ST,NUCLEO-L476RG (ST,STM32L476RG chip)
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

#ifndef SOURCE_BOARD_ST_NUCLEO_L476RG_INCLUDE_DISTORTOS_CHIP_SPIS_HPP_
#define SOURCE_BOARD_ST_NUCLEO_L476RG_INCLUDE_DISTORTOS_CHIP_SPIS_HPP_

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

class SpiMasterLowLevelInterruptBased;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE

/// SPI master low-level driver for SPI1
extern SpiMasterLowLevelInterruptBased spi1;

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI2_ENABLE

/// SPI master low-level driver for SPI2
extern SpiMasterLowLevelInterruptBased spi2;

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI2_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI3_ENABLE

/// SPI master low-level driver for SPI3
extern SpiMasterLowLevelInterruptBased spi3;

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI3_ENABLE

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_BOARD_ST_NUCLEO_L476RG_INCLUDE_DISTORTOS_CHIP_SPIS_HPP_
