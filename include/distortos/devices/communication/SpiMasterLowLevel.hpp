/**
 * \file
 * \brief SpiMasterLowLevel class header
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERLOWLEVEL_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERLOWLEVEL_HPP_

#include "distortos/devices/communication/SpiMode.hpp"

#include <cstddef>

namespace distortos
{

namespace devices
{

class SpiMasterBase;

/**
 * SpiMasterLowLevel class is an interface for low-level SPI master driver
 *
 * \ingroup devices
 */

class SpiMasterLowLevel
{
public:

	/**
	 * \brief SpiMasterLowLevel's destructor
	 *
	 * \pre Driver is stopped.
	 */

	virtual ~SpiMasterLowLevel() = default;

	/**
	 * \brief Configures parameters of low-level SPI master driver.
	 *
	 * \pre Driver is started.
	 * \pre No transfer is in progress.
	 * \pre \a mode, \a clockFrequency, \a wordLength and \a lsbFirst are valid for implementation of this interface.
	 *
	 * \param [in] mode is the desired SPI mode
	 * \param [in] clockFrequency is the desired clock frequency, Hz
	 * \param [in] wordLength selects word length, bits, [1; 32]
	 * \param [in] lsbFirst selects whether MSB (false) or LSB (true) is transmitted first
	 * \param [in] dummyData is the dummy data that will be sent if write buffer of transfer is nullptr
	 *
	 * \return real clock frequency, Hz
	 */

	virtual uint32_t configure(SpiMode mode, uint32_t clockFrequency, uint8_t wordLength, bool lsbFirst,
			uint32_t dummyData) = 0;

	/**
	 * \brief Starts low-level SPI master driver.
	 *
	 * \pre Driver is stopped.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int start() = 0;

	/**
	 * \brief Starts asynchronous transfer.
	 *
	 * This function returns immediately. When the transfer is physically finished (either expected number of bytes were
	 * written and read or an error was detected), SpiMasterBase::transferCompleteEvent() will be executed.
	 *
	 * \param [in] spiMasterBase is a reference to SpiMasterBase object that will be notified about completed transfer
	 * \param [in] writeBuffer is the buffer with data that will be written, nullptr to send dummy data
	 * \param [out] readBuffer is the buffer with data that will be read, nullptr to ignore received data
	 * \param [in] size is the size of transfer (size of \a writeBuffer and/or \a readBuffer), bytes, must be even if
	 * number of data bits is in range (8; 16], divisible by 3 if number of data bits is in range (16; 24] or divisible
	 * by 4 if number of data bits is in range (24; 32]
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the driver is not started;
	 * - EBUSY - transfer is in progress;
	 * - EINVAL - \a size is invalid;
	 */

	virtual int startTransfer(SpiMasterBase& spiMasterBase, const void* writeBuffer, void* readBuffer, size_t size) = 0;

	/**
	 * \brief Stops low-level SPI master driver.
	 *
	 * \pre Driver is started.
	 * \pre No transfer is in progress.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int stop() = 0;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERLOWLEVEL_HPP_
