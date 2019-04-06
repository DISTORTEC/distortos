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

#include "distortos/devices/communication/SpiMasterTransfersRange.hpp"
#include "distortos/devices/communication/SpiMode.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace devices
{

class OutputPin;
class SpiMaster;

/**
 * SpiDevice class represents a single SPI slave device connected to SPI master
 *
 * \ingroup devices
 */

class SpiDevice
{
	friend class SpiDeviceHandle;
	friend class SpiDeviceSelectGuard;

public:

	/**
	 * \brief SpiDevice's constructor
	 *
	 * \param [in] spiMaster is a reference to SPI master to which this SPI slave device is connected
	 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
	 */

	constexpr SpiDevice(SpiMaster& spiMaster, OutputPin& slaveSelectPin) :
			mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
			maxClockFrequency_{},
			slaveSelectPin_{slaveSelectPin},
			spiMaster_{spiMaster},
			lsbFirst_{},
			mode_{},
			openCount_{},
			wordLength_{}
	{

	}

	/**
	 * \brief SpiDevice's constructor
	 *
	 * \deprecated scheduled to be removed after v0.7.0, use SpiDevice::SpiDevice(SpiMaster&, OutputPin&),
	 * SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard
	 *
	 * \param [in] spiMaster is a reference to SPI master to which this SPI slave device is connected
	 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
	 * \param [in] mode is the SPI mode used by SPI slave device
	 * \param [in] maxClockFrequency is the max clock frequency supported by SPI slave device, Hz
	 * \param [in] wordLength is the word length used by SPI slave device, bits
	 * \param [in] lsbFirst selects whether data should be transmitted/received to/from the SPI slave device with
	 * MSB (false) or LSB (true) first
	 */

	__attribute__ ((deprecated("Use SpiDevice::SpiDevice(SpiMaster&, OutputPin&), SpiDeviceHandle, SpiMasterProxy and "
			"SpiDeviceSelectGuard")))
	constexpr SpiDevice(SpiMaster& spiMaster, OutputPin& slaveSelectPin, const SpiMode mode,
			const uint32_t maxClockFrequency, const uint8_t wordLength, const bool lsbFirst) :
					mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
					maxClockFrequency_{maxClockFrequency},
					slaveSelectPin_{slaveSelectPin},
					spiMaster_{spiMaster},
					lsbFirst_{lsbFirst},
					mode_{mode},
					openCount_{},
					wordLength_{wordLength}
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
	 * \brief Executes series of transfers as a single atomic transaction.
	 *
	 * \deprecated scheduled to be removed after v0.7.0, use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] transfersRange is the range of transfers that will be executed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of successfully completed transfers
	 * from \a transfersRange; error codes:
	 * - error codes returned by SpiMasterProxy::configure();
	 * - error codes returned by SpiMasterProxy::executeTransaction();
	 */

	__attribute__ ((deprecated("Use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard")))
	std::pair<int, size_t> executeTransaction(SpiMasterTransfersRange transfersRange);

	/**
	 * \deprecated scheduled to be removed after v0.7.0, use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard
	 *
	 * \return false if data should be transmitted/received to/from the SPI slave device with
	 * MSB first, true if data should be transmitted/received to/from the SPI slave device with LSB first
	 */

	__attribute__ ((deprecated("Use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard")))
	bool getLsbFirst() const
	{
		return lsbFirst_;
	}

	/**
	 * \deprecated scheduled to be removed after v0.7.0, use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard
	 *
	 * \return max clock frequency supported by SPI slave device, Hz
	 */

	__attribute__ ((deprecated("Use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard")))
	uint32_t getMaxClockFrequency() const
	{
		return maxClockFrequency_;
	}

	/**
	 * \deprecated scheduled to be removed after v0.7.0, use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard
	 *
	 * \return SPI mode used by SPI slave device
	 */

	__attribute__ ((deprecated("Use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard")))
	SpiMode getMode() const
	{
		return mode_;
	}

	/**
	 * \deprecated scheduled to be removed after v0.7.0, use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard
	 *
	 * \return reference to slave select pin of this SPI slave device
	 */

	__attribute__ ((deprecated("Use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard")))
	OutputPin& getSlaveSelectPin() const
	{
		return slaveSelectPin_;
	}

	/**
	 * \deprecated scheduled to be removed after v0.7.0, use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard
	 *
	 * \return word length used by SPI slave device, bits
	 */

	__attribute__ ((deprecated("Use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard")))
	uint8_t getWordLength() const
	{
		return wordLength_;
	}

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

	/// max clock frequency supported by SPI slave device, Hz
	uint32_t maxClockFrequency_;

	/// reference to slave select pin of this SPI slave device
	OutputPin& slaveSelectPin_;

	/// reference to SPI master to which this SPI slave device is connected
	SpiMaster& spiMaster_;

	/// selects whether data should be transmitted/received to/from the SPI slave device with MSB (false) or LSB
	/// (true) first
	bool lsbFirst_;

	/// SPI mode used by SPI slave device
	SpiMode mode_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;

	/// word length used by SPI slave device, bits
	uint8_t wordLength_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICE_HPP_
