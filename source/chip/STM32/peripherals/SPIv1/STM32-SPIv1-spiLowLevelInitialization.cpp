/**
 * \file
 * \brief chip::spiLowLevelInitialization() definition for SPIv1 in STM32
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "STM32-SPIv1-spiLowLevelInitialization.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// priority of SPI interrupts
#if defined(CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI)
constexpr uint8_t interruptPriority {CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI};
#else	// !defined(CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI)
constexpr uint8_t interruptPriority {};
#endif	// !defined(CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI)

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void spiLowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32_SPIV1_SPI1_ENABLE
	NVIC_SetPriority(SPI1_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI1_IRQn);
#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI1_ENABLE
#ifdef CONFIG_CHIP_STM32_SPIV1_SPI2_ENABLE
	NVIC_SetPriority(SPI2_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI2_IRQn);
#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI2_ENABLE
#ifdef CONFIG_CHIP_STM32_SPIV1_SPI3_ENABLE
	NVIC_SetPriority(SPI3_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI3_IRQn);
#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI3_ENABLE
#ifdef CONFIG_CHIP_STM32_SPIV1_SPI4_ENABLE
	NVIC_SetPriority(SPI4_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI4_IRQn);
#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI4_ENABLE
#ifdef CONFIG_CHIP_STM32_SPIV1_SPI5_ENABLE
	NVIC_SetPriority(SPI5_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI5_IRQn);
#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI5_ENABLE
#ifdef CONFIG_CHIP_STM32_SPIV1_SPI6_ENABLE
	NVIC_SetPriority(SPI6_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI6_IRQn);
#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI6_ENABLE
}

}	// namespace chip

}	// namespace distortos
