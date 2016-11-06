/**
 * \file
 * \brief chip::usartLowLevelInitialization() declaration for USARTv2 in STM32
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_STM32_USARTV2_USARTLOWLEVELINITIALIZATION_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_STM32_USARTV2_USARTLOWLEVELINITIALIZATION_HPP_

namespace distortos
{

namespace chip
{

/**
 * \brief Low level USART initialization for USARTv2 in STM32
 *
 * This function is called before constructors for global and static objects via peripheralsLowLevelInitialization().
 */

void usartLowLevelInitialization();

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_STM32_USARTV2_USARTLOWLEVELINITIALIZATION_HPP_
