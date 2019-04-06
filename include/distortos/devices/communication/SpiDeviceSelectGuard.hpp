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
class SpiMasterHandle;

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
	 * \param [in] spiMasterHandle is a reference to SpiMasterHandle associated with this select guard
	 */

	explicit SpiDeviceSelectGuard(SpiMasterHandle& spiMasterHandle);

	/**
	 * \brief SpiDeviceSelectGuard's destructor
	 */

	~SpiDeviceSelectGuard();

	SpiDeviceSelectGuard(const SpiDeviceSelectGuard&) = delete;
	SpiDeviceSelectGuard& operator=(const SpiDeviceSelectGuard&) = delete;

protected:

	/**
	 * \return reference to SpiMasterHandle associated with this select guard
	 */

	SpiMasterHandle& getSpiMasterHandle() const
	{
		return spiMasterHandle_;
	}

private:

	/**
	 * \return reference to SpiDevice associated with this select guard
	 */

	SpiDevice& getSpiDevice() const;

	/// reference to SpiMasterHandle associated with this select guard
	SpiMasterHandle& spiMasterHandle_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICESELECTGUARD_HPP_
