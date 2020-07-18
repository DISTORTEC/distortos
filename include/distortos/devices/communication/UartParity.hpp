/**
 * \file
 * \brief UartParity enum class header
 *
 * \author Copyright (C) 2016 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_UARTPARITY_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_UARTPARITY_HPP_

#include <cstdint>

namespace distortos
{

namespace devices
{

/**
 * \brief UART parity
 *
 * \ingroup devices
 */

enum class UartParity : uint8_t
{
	/// parity control is disabled
	none,
	/// even parity control is enabled
	even,
	/// odd parity control is enabled
	odd,
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_UARTPARITY_HPP_
