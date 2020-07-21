/**
 * \file
 * \brief ChipUartLowLevel class header for USARTv1 in STM32
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_USARTV1_INCLUDE_DISTORTOS_CHIP_CHIPUARTLOWLEVEL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_USARTV1_INCLUDE_DISTORTOS_CHIP_CHIPUARTLOWLEVEL_HPP_

#include "distortos/devices/communication/UartLowLevel.hpp"

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

/**
 * \brief ChipUartLowLevel class is a low-level UART driver for USARTv1 in STM32.
 *
 * \ingroup devices
 */

class ChipUartLowLevel : public devices::UartLowLevel
{
public:

	/// minimum allowed value for UART character length
	constexpr static uint8_t minCharacterLength {7};

	/// maximum allowed value for UART character length
	constexpr static uint8_t maxCharacterLength {9};

	class Parameters;

#ifdef DISTORTOS_CHIP_USART1_ENABLE

	/// parameters for construction of UART low-level driver for USART1
	static const Parameters usart1Parameters;

#endif	// def DISTORTOS_CHIP_USART1_ENABLE

#ifdef DISTORTOS_CHIP_USART2_ENABLE

	/// parameters for construction of UART low-level driver for USART2
	static const Parameters usart2Parameters;

#endif	// def DISTORTOS_CHIP_USART2_ENABLE

#ifdef DISTORTOS_CHIP_USART3_ENABLE

	/// parameters for construction of UART low-level driver for USART3
	static const Parameters usart3Parameters;

#endif	// def DISTORTOS_CHIP_USART3_ENABLE

#ifdef DISTORTOS_CHIP_UART4_ENABLE

	/// parameters for construction of UART low-level driver for UART4
	static const Parameters uart4Parameters;

#endif	// def DISTORTOS_CHIP_UART4_ENABLE

#ifdef DISTORTOS_CHIP_UART5_ENABLE

	/// parameters for construction of UART low-level driver for UART5
	static const Parameters uart5Parameters;

#endif	// def DISTORTOS_CHIP_UART5_ENABLE

#ifdef DISTORTOS_CHIP_USART6_ENABLE

	/// parameters for construction of UART low-level driver for USART6
	static const Parameters usart6Parameters;

#endif	// def DISTORTOS_CHIP_USART6_ENABLE

#ifdef DISTORTOS_CHIP_UART7_ENABLE

	/// parameters for construction of UART low-level driver for UART7
	static const Parameters uart7Parameters;

#endif	// def DISTORTOS_CHIP_UART7_ENABLE

#ifdef DISTORTOS_CHIP_UART8_ENABLE

	/// parameters for construction of UART low-level driver for UART8
	static const Parameters uart8Parameters;

#endif	// def DISTORTOS_CHIP_UART8_ENABLE

#ifdef DISTORTOS_CHIP_UART9_ENABLE

	/// parameters for construction of UART low-level driver for UART9
	static const Parameters uart9Parameters;

#endif	// def DISTORTOS_CHIP_UART9_ENABLE

#ifdef DISTORTOS_CHIP_UART10_ENABLE

	/// parameters for construction of UART low-level driver for UART10
	static const Parameters uart10Parameters;

#endif	// def DISTORTOS_CHIP_UART10_ENABLE

	/**
	 * \brief ChipUartLowLevel's constructor
	 *
	 * \param [in] parameters is a reference to object with peripheral parameters
	 */

	constexpr explicit ChipUartLowLevel(const Parameters& parameters) :
			parameters_{parameters},
			uartBase_{},
			readBuffer_{},
			readSize_{},
			readPosition_{},
			writeBuffer_{},
			writeSize_{},
			writePosition_{}
	{

	}

	/**
	 * \brief ChipUartLowLevel's destructor
	 *
	 * Does nothing if driver is already stopped. If it's not, performs forced stop of operation.
	 */

	~ChipUartLowLevel() override;

	/**
	 * \brief Interrupt handler
	 *
	 * \note this must not be called by user code
	 */

	void interruptHandler();

	/**
	 * \brief Starts low-level UART driver.
	 *
	 * Not all combinations of data format are supported. The general rules are:
	 * - if parity control is disabled, character length must not be 7,
	 * - if parity control is enabled, character length must not be 9.
	 *
	 * \param [in] uartBase is a reference to UartBase object that will be associated with this one
	 * \param [in] baudRate is the desired baud rate, bps
	 * \param [in] characterLength selects character length, bits, [7; 9] or [minCharacterLength; maxCharacterLength]
	 * \param [in] parity selects parity
	 * \param [in] _2StopBits selects whether 1 (false) or 2 (true) stop bits are used
	 * \param [in] hardwareFlowControl selects whether hardware flow control is disabled (false) or enabled (true)
	 *
	 * \return pair with return code (0 on success, error code otherwise) and real baud rate; error codes:
	 * - EBADF - the driver is not stopped;
	 * - EINVAL - selected baud rate and/or format are invalid;
	 */

	std::pair<int, uint32_t> start(devices::UartBase& uartBase, uint32_t baudRate, uint8_t characterLength,
			devices::UartParity parity, bool _2StopBits, bool hardwareFlowControl) override;

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

	int startRead(void* buffer, size_t size) override;

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

	int startWrite(const void* buffer, size_t size) override;

	/**
	 * \brief Stops low-level UART driver.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the driver is not started;
	 * - EBUSY - read and/or write are in progress;
	 */

	int stop() override;

	/**
	 * \brief Stops asynchronous read operation.
	 *
	 * This function returns immediately. After this call UartBase::readCompleteEvent() will not be executed.
	 *
	 * \return number of bytes already read by low-level UART driver (and written to read buffer)
	 */

	size_t stopRead() override;

	/**
	 * \brief Stops asynchronous write operation.
	 *
	 * This function returns immediately. After this call UartBase::writeCompleteEvent() will not be executed.
	 * UartBase::transmitCompleteEvent() will not be suppressed.
	 *
	 * \return number of bytes already written by low-level UART driver (and read from write buffer)
	 */

	size_t stopWrite() override;

private:

	/**
	 * \return true if driver is started, false otherwise
	 */

	bool isStarted() const
	{
		return uartBase_ != nullptr;
	}

	/**
	 * \return true if read operation is in progress, false otherwise
	 */

	bool isReadInProgress() const
	{
		return readBuffer_ != nullptr;
	}

	/**
	 * \return true if write operation is in progress, false otherwise
	 */

	bool isWriteInProgress() const
	{
		return writeBuffer_ != nullptr;
	}

	/// reference to configuration parameters
	const Parameters& parameters_;

	/// pointer to UartBase object associated with this one
	devices::UartBase* uartBase_;

	/// buffer to which the data is being written
	uint8_t* volatile readBuffer_;

	/// size of \a readBuffer_, bytes
	volatile size_t readSize_;

	/// current position in \a readBuffer_
	volatile size_t readPosition_;

	/// buffer with data that is being transmitted
	const uint8_t* volatile writeBuffer_;

	/// size of \a writeBuffer_, bytes
	volatile size_t writeSize_;

	/// current position in \a writeBuffer_
	volatile size_t writePosition_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_USARTV1_INCLUDE_DISTORTOS_CHIP_CHIPUARTLOWLEVEL_HPP_
