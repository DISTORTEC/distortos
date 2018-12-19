/**
 * \file
 * \brief UartLowLevel class header
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_UARTLOWLEVEL_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_UARTLOWLEVEL_HPP_

#include "distortos/devices/communication/UartParity.hpp"

#include <utility>

#include <cstddef>

namespace distortos
{

namespace devices
{

class UartBase;

/**
 * UartLowLevel class is an interface for low-level UART driver
 *
 * \ingroup devices
 */

class UartLowLevel
{
public:

	/**
	 * \brief UartLowLevel's destructor
	 */

	virtual ~UartLowLevel() = default;

	/**
	 * \brief Starts low-level UART driver.
	 *
	 * \param [in] uartBase is a reference to UartBase object that will be associated with this one
	 * \param [in] baudRate is the desired baud rate, bps
	 * \param [in] characterLength selects character length, bits
	 * \param [in] parity selects parity
	 * \param [in] _2StopBits selects whether 1 (false) or 2 (true) stop bits are used
	 *
	 * \return pair with return code (0 on success, error code otherwise) and real baud rate; error codes:
	 * - EBADF - the driver is not stopped;
	 * - EINVAL - selected baud rate and/or format are invalid;
	 */

	virtual std::pair<int, uint32_t> start(UartBase& uartBase, uint32_t baudRate, uint8_t characterLength,
			UartParity parity, bool _2StopBits) = 0;

	/**
	 * \brief Starts asynchronous read operation.
	 *
	 * This function returns immediately. When the operation is finished (expected number of bytes were read),
	 * UartBase::readCompleteEvent() will be executed. For any detected error during reception,
	 * UartBase::receiveErrorEvent() will be executed. Note that overrun error may be reported even if it happened when
	 * no read operation was in progress.
	 *
	 * \param [out] buffer is the buffer to which the data will be written
	 * \param [in] size is the size of \a buffer, bytes, must be even if selected character length is greater than 8
	 * bits
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the driver is not started;
	 * - EBUSY - read is in progress;
	 * - EINVAL - \a buffer and/or \a size are invalid;
	 */

	virtual int startRead(void* buffer, size_t size) = 0;

	/**
	 * \brief Starts asynchronous write operation.
	 *
	 * This function returns immediately. If no transmission is active, UartBase::transmitStartEvent() will be executed.
	 * When the operation is finished (expected number of bytes were written), UartBase::writeCompleteEvent() will be
	 * executed. When the transmission physically ends, UartBase::transmitCompleteEvent() will be executed.
	 *
	 * \param [in] buffer is the buffer with data that will be transmitted
	 * \param [in] size is the size of \a buffer, bytes, must be even if selected character length is greater than 8
	 * bits
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the driver is not started;
	 * - EBUSY - write is in progress;
	 * - EINVAL - \a buffer and/or \a size are invalid;
	 */

	virtual int startWrite(const void* buffer, size_t size) = 0;

	/**
	 * \brief Stops low-level UART driver.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the driver is not started;
	 * - EBUSY - read and/or write are in progress;
	 */

	virtual int stop() = 0;

	/**
	 * \brief Stops asynchronous read operation.
	 *
	 * This function returns immediately. After this call UartBase::readCompleteEvent() will not be executed.
	 *
	 * \return number of bytes already read by low-level UART driver (and written to read buffer)
	 */

	virtual size_t stopRead() = 0;

	/**
	 * \brief Stops asynchronous write operation.
	 *
	 * This function returns immediately. After this call UartBase::writeCompleteEvent() will not be executed.
	 * UartBase::transmitCompleteEvent() will not be suppressed.
	 *
	 * \return number of bytes already written by low-level UART driver (and read from write buffer)
	 */

	virtual size_t stopWrite() = 0;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_UARTLOWLEVEL_HPP_
