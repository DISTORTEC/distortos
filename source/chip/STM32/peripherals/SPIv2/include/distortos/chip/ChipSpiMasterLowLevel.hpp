/**
 * \file
 * \brief ChipSpiMasterLowLevel class header for SPIv2 in STM32
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_CHIPSPIMASTERLOWLEVEL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_CHIPSPIMASTERLOWLEVEL_HPP_

#include "distortos/chip/SpiMasterLowLevelInterruptBased.hpp"

namespace distortos
{

namespace chip
{

/**
 * ChipSpiMasterLowLevel is an alias for SpiMasterLowLevelInterruptBased.
 */

class ChipSpiMasterLowLevel : public SpiMasterLowLevelInterruptBased
{
public:

	using SpiMasterLowLevelInterruptBased::SpiMasterLowLevelInterruptBased;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_CHIPSPIMASTERLOWLEVEL_HPP_
