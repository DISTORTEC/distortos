/**
 * \file
 * \brief SpiDeviceProxy class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICEPROXY_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICEPROXY_HPP_

namespace distortos
{

namespace devices
{

class SpiDevice;
class SpiMaster;

/**
 * SpiDeviceProxy is a [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)-like class for locking
 * SpiDevice and for accessing its core functionality.
 *
 * \note Locks are recursive.
 *
 * \ingroup devices
 */

class SpiDeviceProxy
{
	friend class SpiMasterProxy;

public:

	/**
	 * \brief SpiDeviceProxy's constructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] spiDevice is a reference to SpiDevice associated with this proxy
	 */

	explicit SpiDeviceProxy(SpiDevice& spiDevice);

	/**
	 * \brief SpiDeviceProxy's destructor
	 *
	 * \warning This function must not be called from interrupt context!
	 */

	~SpiDeviceProxy();

	/**
	 * \return true if associated SPI device is opened, false otherwise
	 */

	bool isOpened() const;

	SpiDeviceProxy(const SpiDeviceProxy&) = delete;
	SpiDeviceProxy& operator=(const SpiDeviceProxy&) = delete;

private:

	/**
	 * \return reference to SpiMaster associated with this proxy
	 */

	SpiMaster& getSpiMaster() const;

	/// reference to SpiDevice associated with this proxy
	SpiDevice& spiDevice_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICEPROXY_HPP_
