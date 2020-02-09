/**
 * \file
 * \brief ChipOutputPin class header for GPIOv1 in STM32
 * \deprecated scheduled to be removed after v0.8.0, use distortos/chip/OutputPin.hpp
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_CHIPOUTPUTPIN_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_CHIPOUTPUTPIN_HPP_

#warning "'distortos/chip/ChipOutputPin.hpp' is deprecated: Use OutputPin"

#include "distortos/chip/OutputPin.hpp"

namespace distortos
{

namespace chip
{

/**
 * \brief ChipOutputPin is an alias for OutputPin.
 *
 * \deprecated scheduled to be removed after v0.8.0, use distortos::chip::OutputPin
 */

class __attribute__ ((deprecated("Use OutputPin"))) ChipOutputPin : public OutputPin
{
public:

	using OutputPin::OutputPin;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_CHIPOUTPUTPIN_HPP_
