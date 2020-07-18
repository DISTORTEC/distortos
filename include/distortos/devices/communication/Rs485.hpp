/**
 * \file
 * \brief Rs485 class header
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_RS485_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_RS485_HPP_

#include "distortos/devices/communication/SerialPort.hpp"

namespace distortos
{

namespace devices
{

class OutputPin;

/**
 * \brief Rs485 class is a RS-485 variant of serial port with an interface similar to standard files.
 *
 * \ingroup devices
 */

class Rs485 : public SerialPort
{
public:

	/**
	 * \brief Rs485's constructor
	 *
	 * \param [in] uart is a reference to low-level implementation of UartLowLevel interface
	 * \param [in] readBuffer is a buffer for read operations
	 * \param [in] readBufferSize is the size of \a readBuffer, bytes, should be even, must be greater than or equal to
	 * 2
	 * \param [in] writeBuffer is a buffer to write operations
	 * \param [in] writeBufferSize is the size of \a writeBuffer, bytes, should be even, must be greater than or equal
	 * to 2
	 * \param [in] driverEnablePin is a reference to output pin used to control the state of RS-485 driver
	 * \param [in] driverEnabledState is the state of \a driverEnablePin in which RS-485 driver is enabled
	 */

	constexpr Rs485(UartLowLevel& uart, void* const readBuffer, const size_t readBufferSize, void* const writeBuffer,
			const size_t writeBufferSize, OutputPin& driverEnablePin, const bool driverEnabledState) :
					SerialPort{uart, readBuffer, readBufferSize, writeBuffer, writeBufferSize},
					driverEnablePin_{driverEnablePin},
					driverEnabledState_{driverEnabledState}
	{

	}

	/**
	 * \brief Rs485's destructor
	 *
	 * Disables RS-485 driver.
	 *
	 * \warning This function must not be called from interrupt context!
	 */

	~Rs485();

protected:

	/**
	 * \brief "Transmit complete" event
	 *
	 * Called by low-level UART driver when the transmission is physically finished.
	 *
	 * Disables RS-485 driver and calls SerialPort::transmitCompleteEvent().
	 */

	void transmitCompleteEvent() override;

	/**
	 * \brief "Transmit start" event
	 *
	 * Called by low-level UART driver when new transmission starts.
	 *
	 * Enables RS-485 driver and calls SerialPort::transmitStartEvent().
	 */

	void transmitStartEvent() override;

private:

	/**
	 * \brief Enables or disables RS-485 driver.
	 *
	 * \param [in] enable selects whether RS-485 driver will be enabled (true) or disabled (false)
	 */

	void enableDriver(bool enable) const;

	/// reference to output pin used to control the state of RS-485 driver
	OutputPin& driverEnablePin_;

	/// state of \a driverEnablePin_ in which RS-485 driver is enabled
	bool driverEnabledState_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_RS485_HPP_
