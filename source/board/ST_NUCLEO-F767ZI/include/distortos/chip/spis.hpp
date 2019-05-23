/**
 * \file
 * \brief Declarations of low-level SPI master drivers for SPIv2 in ST,NUCLEO-F767ZI (ST,STM32F767ZI chip)
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#ifndef SOURCE_BOARD_ST_NUCLEO_F767ZI_INCLUDE_DISTORTOS_CHIP_SPIS_HPP_
#define SOURCE_BOARD_ST_NUCLEO_F767ZI_INCLUDE_DISTORTOS_CHIP_SPIS_HPP_

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

class SpiMasterLowLevelDmaBased;
class SpiMasterLowLevelInterruptBased;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_CHIP_SPI1_ENABLE

/// SPI master low-level driver for SPI1
#ifdef DISTORTOS_CHIP_SPI1_DMA_BASED
extern SpiMasterLowLevelDmaBased spi1;
#else	// !def DISTORTOS_CHIP_SPI1_DMA_BASED
extern SpiMasterLowLevelInterruptBased spi1;
#endif	// !def DISTORTOS_CHIP_SPI1_DMA_BASED

#endif	// def DISTORTOS_CHIP_SPI1_ENABLE

#ifdef DISTORTOS_CHIP_SPI2_ENABLE

/// SPI master low-level driver for SPI2
#ifdef DISTORTOS_CHIP_SPI2_DMA_BASED
extern SpiMasterLowLevelDmaBased spi2;
#else	// !def DISTORTOS_CHIP_SPI2_DMA_BASED
extern SpiMasterLowLevelInterruptBased spi2;
#endif	// !def DISTORTOS_CHIP_SPI2_DMA_BASED

#endif	// def DISTORTOS_CHIP_SPI2_ENABLE

#ifdef DISTORTOS_CHIP_SPI3_ENABLE

/// SPI master low-level driver for SPI3
#ifdef DISTORTOS_CHIP_SPI3_DMA_BASED
extern SpiMasterLowLevelDmaBased spi3;
#else	// !def DISTORTOS_CHIP_SPI3_DMA_BASED
extern SpiMasterLowLevelInterruptBased spi3;
#endif	// !def DISTORTOS_CHIP_SPI3_DMA_BASED

#endif	// def DISTORTOS_CHIP_SPI3_ENABLE

#ifdef DISTORTOS_CHIP_SPI4_ENABLE

/// SPI master low-level driver for SPI4
#ifdef DISTORTOS_CHIP_SPI4_DMA_BASED
extern SpiMasterLowLevelDmaBased spi4;
#else	// !def DISTORTOS_CHIP_SPI4_DMA_BASED
extern SpiMasterLowLevelInterruptBased spi4;
#endif	// !def DISTORTOS_CHIP_SPI4_DMA_BASED

#endif	// def DISTORTOS_CHIP_SPI4_ENABLE

#ifdef DISTORTOS_CHIP_SPI5_ENABLE

/// SPI master low-level driver for SPI5
#ifdef DISTORTOS_CHIP_SPI5_DMA_BASED
extern SpiMasterLowLevelDmaBased spi5;
#else	// !def DISTORTOS_CHIP_SPI5_DMA_BASED
extern SpiMasterLowLevelInterruptBased spi5;
#endif	// !def DISTORTOS_CHIP_SPI5_DMA_BASED

#endif	// def DISTORTOS_CHIP_SPI5_ENABLE

#ifdef DISTORTOS_CHIP_SPI6_ENABLE

/// SPI master low-level driver for SPI6
#ifdef DISTORTOS_CHIP_SPI6_DMA_BASED
extern SpiMasterLowLevelDmaBased spi6;
#else	// !def DISTORTOS_CHIP_SPI6_DMA_BASED
extern SpiMasterLowLevelInterruptBased spi6;
#endif	// !def DISTORTOS_CHIP_SPI6_DMA_BASED

#endif	// def DISTORTOS_CHIP_SPI6_ENABLE

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_BOARD_ST_NUCLEO_F767ZI_INCLUDE_DISTORTOS_CHIP_SPIS_HPP_
