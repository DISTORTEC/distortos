/**
 * \file
 * \brief SpiDevice class header
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICE_HPP_

#include "distortos/devices/communication/SpiMasterOperationsRange.hpp"
#include "distortos/devices/communication/SpiMode.hpp"

#include "distortos/ConditionVariable.hpp"
#include "distortos/Mutex.hpp"

namespace distortos
{

class Thread;

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
public:

	/**
	 * \brief SpiDevice's constructor
	 *
	 * \param [in] spiMaster is a reference to SPI master to which this SPI slave device is connected
	 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
	 * \param [in] mode is the SPI mode used by SPI slave device
	 * \param [in] maxClockFrequency is the max clock frequency supported by SPI slave device, Hz
	 * \param [in] wordLength is the word length used by SPI slave device, bits
	 * \param [in] lsbFirst selects whether data should be transmitted/received to/from the SPI slave device with
	 * MSB (false) or LSB (true) first
	 */

	constexpr SpiDevice(SpiMaster& spiMaster, OutputPin& slaveSelectPin, const SpiMode mode,
			const uint32_t maxClockFrequency, const uint8_t wordLength, const bool lsbFirst) :
					conditionVariable_{},
					mutex_{Mutex::Protocol::priorityInheritance},
					maxClockFrequency_{maxClockFrequency},
					owner_{},
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
	 * \brief Executes series of operations as a single atomic transaction.
	 *
	 * Wrapper for SpiMaster::executeTransaction().
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] operationsRange is the range of operations that will be executed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of successfully completed
	 * operations from \a operationsRange; error codes:
	 * - EBADF - the device is not opened;
	 * - EINVAL - \a operationsRange has no operations;
	 * - error codes returned by SpiMaster::executeTransaction();
	 */

	std::pair<int, size_t> executeTransaction(SpiMasterOperationsRange operationsRange);

	/**
	 * \return false if data should be transmitted/received to/from the SPI slave device with
	 * MSB first, true if data should be transmitted/received to/from the SPI slave device with LSB first
	 */

	bool getLsbFirst() const
	{
		return lsbFirst_;
	}

	/**
	 * \return max clock frequency supported by SPI slave device, Hz
	 */

	uint32_t getMaxClockFrequency() const
	{
		return maxClockFrequency_;
	}

	/**
	 * \return SPI mode used by SPI slave device
	 */

	SpiMode getMode() const
	{
		return mode_;
	}

	/**
	 * \return reference to slave select pin of this SPI slave device
	 */

	OutputPin& getSlaveSelectPin() const
	{
		return slaveSelectPin_;
	}

	/**
	 * \return word length used by SPI slave device, bits
	 */

	uint8_t getWordLength() const
	{
		return wordLength_;
	}

	/**
	 * \brief Locks the object for exclusive use by current thread using condition variable and mutex.
	 *
	 * When the object is locked, any call to any member function from other thread will be blocked until the object is
	 * unlocked. Locking is optional, but may be useful when more than one transaction must be done atomically.
	 *
	 * \note Locks may be nested.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return previous state of lock: false if this SPI device was unlocked before this call, true if it was already
	 * locked by current thread
	 */

	bool lock();

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
	 * \brief Unlocks the object that was previously locked by current thread.
	 *
	 * Does nothing if SPI device is not locked by current thread.
	 *
	 * \note Locks may be nested.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param previousLockState is the value returned by matching call to lock()
	 */

	void unlock(bool previousLockState);

private:

	/**
	 * \brief Internal version of lock() - without locking the mutex.
	 *
	 * \return previous state of lock: false if this SPI device was unlocked before this call, true if it was already
	 * locked by current thread
	 */

	bool lockInternal();

	/**
	 * \brief Internal version of unlock() - without locking the mutex.
	 *
	 * Does nothing if SPI device is not locked by current thread.
	 *
	 * \param previousLockState is the value returned by matching call to lockInternal()
	 */

	void unlockInternal(bool previousLockState);

	/// condition variable used for locking access to this object
	ConditionVariable conditionVariable_;

	/// mutex used to serialize access to this object
	Mutex mutex_;

	/// max clock frequency supported by SPI slave device, Hz
	uint32_t maxClockFrequency_;

	/// pointer to thread that locked this object
	const Thread* owner_;

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
