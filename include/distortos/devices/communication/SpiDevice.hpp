/**
 * \file
 * \brief SpiDevice class header
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICE_HPP_

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace devices
{

class SpiMaster;

/**
 * SpiDevice class represents a single SPI slave device connected to SPI master
 *
 * \ingroup devices
 */

class SpiDevice
{
	friend class SpiDeviceHandle;

public:

	/**
	 * \brief SpiDevice's constructor
	 *
	 * \param [in] spiMaster is a reference to SPI master to which this SPI slave device is connected
	 */

	constexpr explicit SpiDevice(SpiMaster& spiMaster) :
			mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
			spiMaster_{spiMaster},
			openCount_{}
	{

	}

	/**
	 * \brief SpiDevice's destructor
	 *
	 * Does nothing if all users already closed this device. If they did not, performs forced close of device.
	 *
	 * \warning This function must not be called from interrupt context!
	 */

	~SpiDevice();

	/**
	 * \brief Closes SPI device.
	 *
	 * Does nothing if any user still has this device opened. Otherwise low-level driver is stopped.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the device is already completely closed;
	 * - error codes returned by SpiMasterLowLevel::stop();
	 */

	int close();

	/**
	 * \brief Locks the device for exclusive use by current thread.
	 *
	 * When the object is locked, any call to any member function from other thread will be blocked until the object is
	 * unlocked. Locking is optional, but may be useful when more than one transaction must be done atomically.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Mutex::lock();
	 */

	int lock()
	{
		return mutex_.lock();
	}

	/**
	 * \brief Opens SPI device.
	 *
	 * Does nothing if any user already has this device opened. Otherwise associated SPI master is opened.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EMFILE - this device is already opened too many times;
	 * - error codes returned by SpiMaster::open();
	 */

	int open();

	/**
	 * \brief Unlocks the device which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Mutex::unlock();
	 */

	int unlock()
	{
		return mutex_.unlock();
	}

private:

	/// mutex used to serialize access to this object
	Mutex mutex_;

	/// reference to SPI master to which this SPI slave device is connected
	SpiMaster& spiMaster_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICE_HPP_
