/**
 * \file
 * \brief SpiDevice class header
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICE_HPP_

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
public:

	/// parameters required for correct communication with SPI slave device
	class Parameters
	{
	public:

		/**
		 * \brief Parameters's constructor
		 *
		 * \param [in] mode is the SPI mode used by SPI slave device
		 * \param [in] maxClockFrequency is the max clock frequency supported by SPI slave device, Hz
		 * \param [in] wordLength is the word length used by SPI slave device, bits
		 * \param [in] lsbFirst selects whether data should be transmitted/received to/from the SPI slave device with
		 * MSB (false) or LSB (true) first
		 */

		constexpr Parameters(const SpiMode mode, const uint32_t maxClockFrequency, const uint8_t wordLength,
				const bool lsbFirst) :
						maxClockFrequency_{maxClockFrequency},
						lsbFirst_{lsbFirst},
						mode_{mode},
						wordLength_{wordLength}
		{

		}

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
		 * \return word length used by SPI slave device, bits
		 */

		uint8_t getWordLength() const
		{
			return wordLength_;
		}

	private:

		/// max clock frequency supported by SPI slave device, Hz
		uint32_t maxClockFrequency_;

		/// selects whether data should be transmitted/received to/from the SPI slave device with MSB (false) or LSB
		/// (true) first
		bool lsbFirst_;

		/// SPI mode used by SPI slave device
		SpiMode mode_;

		/// word length used by SPI slave device, bits
		uint8_t wordLength_;
	};

	/**
	 * \brief SpiDevice's constructor
	 *
	 * \param [in] parameters is a reference to parameters required for correct communication with this SPI slave device
	 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
	 * \param [in] spiMaster is a reference to SPI master to which this SPI slave device is connected
	 */

	constexpr SpiDevice(const Parameters& parameters, OutputPin& slaveSelectPin, SpiMaster& spiMaster) :
			mutex_{Mutex::Type::normal, Mutex::Protocol::priorityInheritance},
			parameters_{parameters},
			slaveSelectPin_{slaveSelectPin},
			spiMaster_{spiMaster},
			openCount_{}
	{

	}

	/**
	 * \brief Closes SPI master.
	 *
	 * Does nothing if any user still has this device opened. Otherwise low-level driver is stopped.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the device is already completely closed;
	 * - error codes returned by SpiMasterLowLevel::stop();
	 */

	int close();

	/**
	 * \return reference to parameters required for correct communication with this SPI slave device
	 */

	const Parameters& getParameters() const
	{
		return parameters_;
	}

	/**
	 * \return reference to slave select pin of this SPI slave device
	 */

	OutputPin& getSlaveSelectPin() const
	{
		return slaveSelectPin_;
	}

	/**
	 * \brief Opens SPI device.
	 *
	 * Does nothing if any user already has this device opened. Otherwise associated SPI master is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EMFILE - this device is already opened too many times;
	 * - error codes returned by SpiMaster::open();
	 */

	int open();

private:

	/// mutex used to serialize access to this object
	Mutex mutex_;

	/// reference to parameters required for correct communication with this SPI slave device
	const Parameters& parameters_;

	/// reference to slave select pin of this SPI slave device
	OutputPin& slaveSelectPin_;

	/// reference to SPI master to which this SPI slave device is connected
	SpiMaster& spiMaster_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIDEVICE_HPP_
