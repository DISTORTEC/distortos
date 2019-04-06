/**
 * \file
 * \brief SpiDeviceHandle class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICEHANDLE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICEHANDLE_HPP_

namespace distortos
{

namespace devices
{

class SpiDevice;
class SpiMaster;

/**
 * SpiDeviceHandle is a [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)-like class for locking
 * SpiDevice and for accessing its core functionality.
 *
 * \note Locks are recursive.
 *
 * \ingroup devices
 */

class SpiDeviceHandle
{
	friend class SpiMasterHandle;

public:

	/**
	 * \brief SpiDeviceHandle's constructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] spiDevice is a reference to SpiDevice associated with this handle
	 */

	explicit SpiDeviceHandle(SpiDevice& spiDevice);

	/**
	 * \brief SpiDeviceHandle's destructor
	 *
	 * \warning This function must not be called from interrupt context!
	 */

	~SpiDeviceHandle();

	/**
	 * \return true if associated SPI device is opened, false otherwise
	 */

	bool isOpened() const;

	SpiDeviceHandle(const SpiDeviceHandle&) = delete;
	SpiDeviceHandle& operator=(const SpiDeviceHandle&) = delete;

private:

	/**
	 * \return reference to SpiMaster associated with this handle
	 */

	SpiMaster& getSpiMaster() const;

	/// reference to SpiDevice associated with this handle
	SpiDevice& spiDevice_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICEHANDLE_HPP_
