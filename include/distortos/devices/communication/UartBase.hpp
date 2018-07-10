/**
 * \file
 * \brief UartBase class header
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_UARTBASE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_UARTBASE_HPP_

#include <bitset>

namespace distortos
{

namespace devices
{

/**
 * UartBase class is an interface with callbacks for low-level UART driver, which can serve as a base for high-level
 * UART drivers
 *
 * \ingroup devices
 */

class UartBase
{
public:

	/// indexes of error bits in ErrorSet
	enum ErrorBits
	{
		/// framing error
		framingError,
		/// noise error
		noiseError,
		/// overrun error
		overrunError,
		/// parity error
		parityError,

		/// number of supported error bits - must be last!
		errorBitsMax
	};

	/// set of error bits
	using ErrorSet = std::bitset<errorBitsMax>;

	/**
	 * \brief UartBase's destructor
	 */

	virtual ~UartBase() = 0;

	/**
	 * \brief "Read complete" event
	 *
	 * Called by low-level UART driver when whole read buffer is filled.
	 *
	 * \param [in] bytesRead is the number of bytes read by low-level UART driver (and written to read buffer)
	 */

	virtual void readCompleteEvent(size_t bytesRead) = 0;

	/**
	 * \brief "Receive error" event
	 *
	 * Called by low-level UART driver when the last character was received with an error. This character is written to
	 * the read buffer before this function is called.
	 *
	 * \param [in] errorSet is the set of error bits
	 */

	virtual void receiveErrorEvent(ErrorSet errorSet) = 0;

	/**
	 * \brief "Transmit complete" event
	 *
	 * Called by low-level UART driver when the transmission is physically finished.
	 */

	virtual void transmitCompleteEvent() = 0;

	/**
	 * \brief "Transmit start" event
	 *
	 * Called by low-level UART driver when new transmission starts.
	 */

	virtual void transmitStartEvent() = 0;

	/**
	 * \brief "Write complete" event
	 *
	 * Called by low-level UART driver when whole write buffer was transferred - the transmission may still be in
	 * progress.
	 *
	 * \param [in] bytesWritten is the number of bytes written by low-level UART driver (and read from write buffer)
	 */

	virtual void writeCompleteEvent(size_t bytesWritten) = 0;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_UARTBASE_HPP_
