/**
 * \file
 * \brief SpiDeviceSelectGuard class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICESELECTGUARD_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICESELECTGUARD_HPP_

#include "distortos/devices/io/OutputPin.hpp"

namespace distortos
{

namespace devices
{

/**
 * SpiDeviceSelectGuard is a RAII-style class for selecting SPI device.
 *
 * \ingroup devices
 */

class SpiDeviceSelectGuard
{
public:

	/**
	 * \brief SpiDeviceSelectGuard's constructor
	 *
	 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
	 */

	explicit SpiDeviceSelectGuard(OutputPin& slaveSelectPin) :
			slaveSelectPin_{slaveSelectPin}
	{
		slaveSelectPin_.set(false);
	}

	/**
	 * \brief SpiDeviceSelectGuard's destructor
	 */

	~SpiDeviceSelectGuard()
	{
		slaveSelectPin_.set(true);
	}

	SpiDeviceSelectGuard(const SpiDeviceSelectGuard&) = delete;
	SpiDeviceSelectGuard& operator=(const SpiDeviceSelectGuard&) = delete;

private:

	/// reference to slave select pin of this SPI slave device
	OutputPin& slaveSelectPin_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICESELECTGUARD_HPP_
