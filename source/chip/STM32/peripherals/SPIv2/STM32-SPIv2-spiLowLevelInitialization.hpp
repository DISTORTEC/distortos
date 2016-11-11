/**
 * \file
 * \brief chip::spiLowLevelInitialization() declaration for SPIv2 in STM32
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_STM32_SPIV2_SPILOWLEVELINITIALIZATION_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_STM32_SPIV2_SPILOWLEVELINITIALIZATION_HPP_

namespace distortos
{

namespace chip
{

/**
 * \brief Low level SPI initialization for SPIv2 in STM32
 *
 * This function is called before constructors for global and static objects via peripheralsLowLevelInitialization().
 */

void spiLowLevelInitialization();

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_STM32_SPIV2_SPILOWLEVELINITIALIZATION_HPP_
