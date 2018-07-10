/**
 * \file
 * \brief SpiDeviceSelectGuard class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICESELECTGUARD_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICESELECTGUARD_HPP_

namespace distortos
{

namespace devices
{

class SpiDevice;
class SpiMasterProxy;

/**
 * SpiDeviceSelectGuard is a RAII-style class for selecting SpiDevice.
 *
 * \ingroup devices
 */

class SpiDeviceSelectGuard
{
public:

	/**
	 * \brief SpiDeviceSelectGuard's constructor
	 *
	 * \param [in] spiMasterProxy is a reference to SpiMasterProxy associated with this select guard
	 */

	explicit SpiDeviceSelectGuard(SpiMasterProxy& spiMasterProxy);

	/**
	 * \brief SpiDeviceSelectGuard's destructor
	 */

	~SpiDeviceSelectGuard();

	SpiDeviceSelectGuard(const SpiDeviceSelectGuard&) = delete;
	SpiDeviceSelectGuard& operator=(const SpiDeviceSelectGuard&) = delete;

protected:

	/**
	 * \return reference to SpiMasterProxy associated with this select guard
	 */

	SpiMasterProxy& getSpiMasterProxy() const
	{
		return spiMasterProxy_;
	}

private:

	/**
	 * \return reference to SpiDevice associated with this select guard
	 */

	SpiDevice& getSpiDevice() const;

	/// reference to SpiMasterProxy associated with this select guard
	SpiMasterProxy& spiMasterProxy_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICESELECTGUARD_HPP_
