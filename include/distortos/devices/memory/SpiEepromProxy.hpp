/**
 * \file
 * \brief SpiEepromProxy class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROMPROXY_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROMPROXY_HPP_

#include "distortos/devices/communication/SpiDeviceProxy.hpp"

namespace distortos
{

namespace devices
{

class SpiEeprom;

/**
 * SpiEepromProxy is a [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)-like class for locking
 * SpiEeprom and for accessing its core functionality.
 *
 * \note Locks are recursive.
 *
 * \ingroup devices
 */

class SpiEepromProxy
{
public:

	/**
	 * \brief SpiEepromProxy's constructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] spiEeprom is a reference to SpiEeprom associated with this proxy
	 */

	explicit SpiEepromProxy(SpiEeprom& spiEeprom);

private:

	/// internal proxy for SPI device
	SpiDeviceProxy spiDeviceProxy_;

	/// reference to SpiEeprom associated with this proxy
	SpiEeprom& spiEeprom_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROMPROXY_HPP_
