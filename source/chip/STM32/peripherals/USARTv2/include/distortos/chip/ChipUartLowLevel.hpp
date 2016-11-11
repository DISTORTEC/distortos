/**
 * \file
 * \brief ChipUartLowLevel class header for USARTv2 in STM32
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_INCLUDE_DISTORTOS_CHIP_CHIPUARTLOWLEVEL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_INCLUDE_DISTORTOS_CHIP_CHIPUARTLOWLEVEL_HPP_

#include "distortos/devices/communication/UartLowLevel.hpp"

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

/// ChipUartLowLevel class is a low-level UART driver for USARTv2 in STM32
class ChipUartLowLevel : public devices::UartLowLevel
{
public:

	/// minimum allowed value for UART character length
#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT
	constexpr static uint8_t minUartCharacterLength {6};
#else	// !def CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT
	constexpr static uint8_t minUartCharacterLength {7};
#endif	// !def CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT

	/// maximum allowed value for UART character length
	constexpr static uint8_t maxUartCharacterLength {9};

	class Parameters;

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART1

	/// parameters for construction of UART low-level driver for USART1
	static const Parameters usart1Parameters;

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART1

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART2

	/// parameters for construction of UART low-level driver for USART2
	static const Parameters usart2Parameters;

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART2

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART3

	/// parameters for construction of UART low-level driver for USART3
	static const Parameters usart3Parameters;

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART3

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART4

	/// parameters for construction of UART low-level driver for USART4
	static const Parameters usart4Parameters;

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART4

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART5

	/// parameters for construction of UART low-level driver for USART5
	static const Parameters usart5Parameters;

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART5

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART6

	/// parameters for construction of UART low-level driver for USART6
	static const Parameters usart6Parameters;

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART6

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART7

	/// parameters for construction of UART low-level driver for USART7
	static const Parameters usart7Parameters;

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART7

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART8

	/// parameters for construction of UART low-level driver for USART8
	static const Parameters usart8Parameters;

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART8

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
	 * - if parity control is disabled, character length must not be \a minUartCharacterLength,
	 * - if parity control is enabled, character length must not be 9.
	 *
	 * \param [in] uartBase is a reference to UartBase object that will be associated with this one
	 * \param [in] baudRate is the desired baud rate, bps
	 * \param [in] characterLength selects character length, bits, [minUartCharacterLength; maxUartCharacterLength]
	 * \param [in] parity selects parity
	 * \param [in] _2StopBits selects whether 1 (false) or 2 (true) stop bits are used
	 *
	 * \return pair with return code (0 on success, error code otherwise) and real baud rate;
	 * error codes:
	 * - EBADF - the driver is not stopped;
	 * - EINVAL - selected baud rate and/or format are invalid;
	 */

	std::pair<int, uint32_t> start(devices::UartBase& uartBase, uint32_t baudRate, uint8_t characterLength,
			devices::UartParity parity, bool _2StopBits) override;

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

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_USARTV2_INCLUDE_DISTORTOS_CHIP_CHIPUARTLOWLEVEL_HPP_
