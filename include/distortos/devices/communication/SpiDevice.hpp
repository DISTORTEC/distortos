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

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace devices
{

class OutputPin;
class SpiDeviceProxy;
class SpiMaster;

/**
 * SpiDevice class represents a single SPI slave device connected to SPI master
 *
 * \ingroup devices
 */

class SpiDevice
{
	friend class SpiDeviceProxy;

public:

	/// import SpiDeviceProxy as SpiDevice::Proxy
	using Proxy = SpiDeviceProxy;

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
