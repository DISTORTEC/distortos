/**
 * \file
 * \brief SpiMode enum class header
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMODE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMODE_HPP_

#include <cstdint>

namespace distortos
{

namespace devices
{

/**
 * \brief SPI mode
 *
 * \ingroup devices
 */

enum class SpiMode : uint8_t
{
	/// mode 0 - CPOL == 0, CPHA == 0
	_0 = 0 << 0 | 0 << 1,
	/// mode 1 - CPOL == 0, CPHA == 1
	_1 = 0 << 0 | 1 << 1,
	/// mode 2 - CPOL == 1, CPHA == 0
	_2 = 1 << 0 | 0 << 1,
	/// mode 3 - CPOL == 1, CPHA == 1
	_3 = 1 << 0 | 1 << 1,

	/// alias of mode 0 - CPOL == 0, CPHA == 0
	cpol0cpha0 = _0,
	/// alias of mode 1 - CPOL == 0, CPHA == 1
	cpol0cpha1 = _1,
	/// alias of mode 2 - CPOL == 1, CPHA == 0
	cpol1cpha0 = _2,
	/// alias of mode 3 - CPOL == 1, CPHA == 1
	cpol1cpha1 = _3,
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMODE_HPP_
