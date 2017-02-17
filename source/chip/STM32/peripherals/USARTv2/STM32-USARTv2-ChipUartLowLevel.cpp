/**
 * \file
 * \brief ChipUartLowLevel class implementation for USARTv2 in STM32
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/ChipUartLowLevel.hpp"

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"

#include "distortos/devices/communication/UartBase.hpp"

#include "distortos/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

namespace chip
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Decode value of USART_ISR register to devices::UartBase::ErrorSet
 *
 * \param [in] isr is the value of USART_ISR register that will be decoded
 *
 * \return devices::UartBase::ErrorSet with errors decoded from \a isr
 */

devices::UartBase::ErrorSet decodeErrors(const uint32_t isr)
{
	devices::UartBase::ErrorSet errorSet {};
	errorSet[devices::UartBase::framingError] = (isr & USART_ISR_FE) != 0;
	errorSet[devices::UartBase::noiseError] = (isr & USART_ISR_NE) != 0;
	errorSet[devices::UartBase::overrunError] = (isr & USART_ISR_ORE) != 0;
	errorSet[devices::UartBase::parityError] = (isr & USART_ISR_PE) != 0;
	return errorSet;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public types
+---------------------------------------------------------------------------------------------------------------------*/

/// parameters for construction of UART low-level drivers
class ChipUartLowLevel::Parameters
{
	/// base address of APB peripherals
	constexpr static uintptr_t apbPeripheralsBaseAddress {APBPERIPH_BASE};
	/// base address of AHB peripherals
	constexpr static uintptr_t ahbPeripheralsBaseAddress {AHBPERIPH_BASE};

	static_assert(apbPeripheralsBaseAddress < ahbPeripheralsBaseAddress, "Invalid order of APB and AHB!");

public:

	/**
	 * \brief Parameters's constructor
	 *
	 * \param [in] uartBase is a base address of UART peripheral
	 * \param [in] rccEnOffset is the offset of RCC register with appropriate U[S]ARTxEN bit, bytes
	 * \param [in] rccEnBitmask is the bitmask of appropriate U[S]ARTxEN bit in RCC register at \a rccEnOffset offset
	 * \param [in] rccRstOffset is the offset of RCC register with appropriate U[S]ARTxRST bit, bytes
	 * \param [in] rccRstBitmask is the bitmask of appropriate U[S]ARTxRST bit in RCC register at \a rccRstOffset offset
	 */

	constexpr Parameters(const uintptr_t uartBase, const size_t rccEnOffset, const uint32_t rccEnBitmask,
			const size_t rccRstOffset, const uint32_t rccRstBitmask) :
					uartBase_{uartBase},
					peripheralFrequency_{uartBase < ahbPeripheralsBaseAddress ? apbFrequency : ahbFrequency},
					rccEnBitmask_{rccEnBitmask},
					rccEnOffset_{rccEnOffset},
					rccRstBitmask_{rccRstBitmask},
					rccRstOffset_{rccRstOffset}
	{

	}

	/**
	 * \brief Enables or disables peripheral clock in RCC.
	 *
	 * \param [in] enable selects whether the clock will be enabled (true) or disabled (false)
	 */

	void enablePeripheralClock(const bool enable) const
	{
		auto& rccEn = *reinterpret_cast<volatile uint32_t*>(RCC_BASE + rccEnOffset_);
		const InterruptMaskingLock interruptMaskingLock;
		rccEn = (rccEn & ~rccEnBitmask_) | (enable == true ? rccEnBitmask_ : 0);
	}

	/**
	 * \brief Enables or disables RXNE interrupt of UART.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableRxneInterrupt(const bool enable) const
	{
		auto& uart = getUart();
		const InterruptMaskingLock interruptMaskingLock;
		uart.CR1 = (uart.CR1 & ~USART_CR1_RXNEIE) | (enable == true ? USART_CR1_RXNEIE : 0);
	}

	/**
	 * \brief Enables or disables TC interrupt of UART.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableTcInterrupt(const bool enable) const
	{
		auto& uart = getUart();
		const InterruptMaskingLock interruptMaskingLock;
		uart.CR1 = (uart.CR1 & ~USART_CR1_TCIE) | (enable == true ? USART_CR1_TCIE : 0);
	}

	/**
	 * \brief Enables or disables TXE interrupt of UART.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableTxeInterrupt(const bool enable) const
	{
		auto& uart = getUart();
		const InterruptMaskingLock interruptMaskingLock;
		uart.CR1 = (uart.CR1 & ~USART_CR1_TXEIE) | (enable == true ? USART_CR1_TXEIE : 0);
	}

	/**
	 * \return character length, bits
	 */

	uint8_t getCharacterLength() const
	{
		const auto cr1 = getUart().CR1;
		const auto realCharacterLength =
#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT
				(cr1 & USART_CR1_M1) != 0 ? 7 :
#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT
				(cr1 & USART_CR1_M0) != 0 ? 9 : 8;
		const auto parityControlEnabled = (cr1 & USART_CR1_PCE) != 0;
		return realCharacterLength - parityControlEnabled;
	}

	/**
	 * \return peripheral clock frequency, Hz
	 */

	uint32_t getPeripheralFrequency() const
	{
		return peripheralFrequency_;
	}

	/**
	 * \return reference to USART_TypeDef object
	 */

	USART_TypeDef& getUart() const
	{
		return *reinterpret_cast<USART_TypeDef*>(uartBase_);
	}

	/**
	 * \brief Resets all peripheral's registers via RCC
	 *
	 * \note Peripheral clock must be enabled in RCC for this operation to work.
	 */

	void resetPeripheral() const
	{
		auto& rccRst = *reinterpret_cast<volatile uint32_t*>(RCC_BASE + rccRstOffset_);
		const InterruptMaskingLock interruptMaskingLock;
		rccRst |= rccRstBitmask_;
		rccRst &= ~rccRstBitmask_;
	}

private:

	/// base address of UART peripheral
	uintptr_t uartBase_;

	/// peripheral clock frequency, Hz
	uint32_t peripheralFrequency_;

	/// bitmask of appropriate U[S]ARTxEN bit in RCC register at \a rccEnOffset_ offset
	uint32_t rccEnBitmask_;

	/// offset of RCC register with appropriate U[S]ARTxEN bit, bytes
	size_t rccEnOffset_;

	/// bitmask of appropriate U[S]ARTxRST bit in RCC register at \a rccRstOffset_ offset
	uint32_t rccRstBitmask_;

	/// offset of RCC register with appropriate U[S]ARTxRST bit, bytes
	size_t rccRstOffset_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| public static objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART1

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart1Parameters {USART1_BASE, offsetof(RCC_TypeDef, APB2ENR),
		RCC_APB2ENR_USART1EN, offsetof(RCC_TypeDef, APB2RSTR), RCC_APB2RSTR_USART1RST};

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART1

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART2

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart2Parameters {USART2_BASE, offsetof(RCC_TypeDef, APB1ENR),
		RCC_APB1ENR_USART2EN, offsetof(RCC_TypeDef, APB1RSTR), RCC_APB1RSTR_USART2RST};

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART2

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART3

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart3Parameters {USART3_BASE, offsetof(RCC_TypeDef, APB1ENR),
		RCC_APB1ENR_USART3EN, offsetof(RCC_TypeDef, APB1RSTR), RCC_APB1RSTR_USART3RST};

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART3

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART4

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart4Parameters {USART4_BASE, offsetof(RCC_TypeDef, APB1ENR),
		RCC_APB1ENR_USART4EN, offsetof(RCC_TypeDef, APB1RSTR), RCC_APB1RSTR_USART4RST};

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART4

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART5

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart5Parameters {USART5_BASE, offsetof(RCC_TypeDef, APB1ENR),
		RCC_APB1ENR_USART5EN, offsetof(RCC_TypeDef, APB1RSTR), RCC_APB1RSTR_USART5RST};

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART5

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART6

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart6Parameters {USART6_BASE, offsetof(RCC_TypeDef, APB2ENR),
		RCC_APB2ENR_USART6EN, offsetof(RCC_TypeDef, APB2RSTR), RCC_APB2RSTR_USART6RST};

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART6

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART7

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart7Parameters {USART7_BASE, offsetof(RCC_TypeDef, APB2ENR),
		RCC_APB2ENR_USART7EN, offsetof(RCC_TypeDef, APB2RSTR), RCC_APB2RSTR_USART7RST};

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART7

#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_USART8

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart8Parameters {USART8_BASE, offsetof(RCC_TypeDef, APB2ENR),
		RCC_APB2ENR_USART8EN, offsetof(RCC_TypeDef, APB2RSTR), RCC_APB2RSTR_USART8RST};

#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_USART8

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel::~ChipUartLowLevel()
{
	if (isStarted() == false)
		return;

	parameters_.resetPeripheral();
	parameters_.enablePeripheralClock(false);
}

void ChipUartLowLevel::interruptHandler()
{
	auto& uart = parameters_.getUart();
	const auto characterLength = parameters_.getCharacterLength();
	uint32_t isr;
	uint32_t maskedIsr;
	// loop while there are enabled interrupt sources waiting to be served
	while (isr = uart.ISR, (maskedIsr = isr & uart.CR1 & (USART_ISR_RXNE | USART_ISR_TXE | USART_ISR_TC)) != 0)
	{
		if ((maskedIsr & USART_ISR_RXNE) != 0)		// read & receive errors
		{
			const uint16_t characterMask = (1 << characterLength) - 1;
			const uint16_t character = uart.RDR & characterMask;
			const auto readBuffer = readBuffer_;
			auto readPosition = readPosition_;
			readBuffer[readPosition++] = character;
			if (characterLength > 8)
				readBuffer[readPosition++] = character >> 8;
			readPosition_ = readPosition;
			const auto isrErrorFlags = isr & (USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE | USART_ISR_PE);
			if (isrErrorFlags != 0)
			{
				uart.ICR = isrErrorFlags;	// clear served error flags
				uartBase_->receiveErrorEvent(decodeErrors(isr));
			}
			if (readPosition == readSize_)
				uartBase_->readCompleteEvent(stopRead());
		}
		else if ((maskedIsr & USART_ISR_TXE) != 0)	// write
		{
			const auto writeBuffer = writeBuffer_;
			auto writePosition = writePosition_;
			const uint16_t characterLow = writeBuffer[writePosition++];
			const uint16_t characterHigh = characterLength > 8 ? writeBuffer[writePosition++] : 0;
			writePosition_ = writePosition;
			uart.TDR = characterLow | (characterHigh << 8);
			if (writePosition == writeSize_)
				uartBase_->writeCompleteEvent(stopWrite());
		}
		else if ((maskedIsr & USART_ISR_TC) != 0)		// transmit complete
		{
			parameters_.enableTcInterrupt(false);
			uartBase_->transmitCompleteEvent();
		}
	}
}

std::pair<int, uint32_t> ChipUartLowLevel::start(devices::UartBase& uartBase, const uint32_t baudRate,
		const uint8_t characterLength, const devices::UartParity parity, const bool _2StopBits)
{
	if (isStarted() == true)
		return {EBADF, {}};

	const auto peripheralFrequency = parameters_.getPeripheralFrequency();
	const auto divider = (peripheralFrequency + baudRate / 2) / baudRate;
	const auto over8 = divider < 16;
	const auto mantissa = divider / (over8 == false ? 16 : 8);
	const auto fraction = divider % (over8 == false ? 16 : 8);

	if (mantissa == 0 || mantissa > (USART_BRR_DIV_MANTISSA >> USART_BRR_DIV_MANTISSA_Pos))
		return {EINVAL, {}};

	const auto realCharacterLength = characterLength + (parity != devices::UartParity::none);
	if (realCharacterLength < minCharacterLength + 1 || realCharacterLength > maxCharacterLength)
		return {EINVAL, {}};

	parameters_.enablePeripheralClock(true);
	parameters_.resetPeripheral();

	uartBase_ = &uartBase;
	auto& uart = parameters_.getUart();
	uart.BRR = (mantissa << USART_BRR_DIV_MANTISSA_Pos) | (fraction << USART_BRR_DIV_FRACTION_Pos);
	uart.CR2 = _2StopBits << (USART_CR2_STOP_Pos + 1);
	uart.CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE | (over8 << USART_CR1_OVER8_Pos) |
			((realCharacterLength == maxCharacterLength) << USART_CR1_M0_Pos) |
#ifdef CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT
			((realCharacterLength == minCharacterLength + 1) << USART_CR1_M1_Pos) |
#endif	// def CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT
			((parity != devices::UartParity::none) << USART_CR1_PCE_Pos) |
			((parity == devices::UartParity::odd) << USART_CR1_PS_Pos);
	return {{}, peripheralFrequency / divider};
}

int ChipUartLowLevel::startRead(void* const buffer, const size_t size)
{
	if (buffer == nullptr || size == 0)
		return EINVAL;

	if (isStarted() == false)
		return EBADF;

	if (isReadInProgress() == true)
		return EBUSY;

	if (parameters_.getCharacterLength() > 8 && size % 2 != 0)
		return EINVAL;

	readBuffer_ = static_cast<uint8_t*>(buffer);
	readSize_ = size;
	readPosition_ = 0;
	parameters_.enableRxneInterrupt(true);
	return 0;
}

int ChipUartLowLevel::startWrite(const void* const buffer, const size_t size)
{
	if (buffer == nullptr || size == 0)
		return EINVAL;

	if (isStarted() == false)
		return EBADF;

	if (isWriteInProgress() == true)
		return EBUSY;

	if (parameters_.getCharacterLength() > 8 && size % 2 != 0)
		return EINVAL;

	writeBuffer_ = static_cast<const uint8_t*>(buffer);
	writeSize_ = size;
	writePosition_ = 0;
	parameters_.enableTcInterrupt(false);

	if ((parameters_.getUart().ISR & USART_ISR_TC) != 0)
		uartBase_->transmitStartEvent();

	parameters_.enableTxeInterrupt(true);
	return 0;
}

int ChipUartLowLevel::stop()
{
	if (isStarted() == false)
		return EBADF;

	if (isReadInProgress() == true || isWriteInProgress() == true)
		return EBUSY;

	parameters_.resetPeripheral();
	parameters_.enablePeripheralClock(false);
	uartBase_ = nullptr;
	return 0;
}

size_t ChipUartLowLevel::stopRead()
{
	if (isReadInProgress() == false)
		return 0;

	parameters_.enableRxneInterrupt(false);
	const auto bytesRead = readPosition_;
	readPosition_ = {};
	readSize_ = {};
	readBuffer_ = {};
	return bytesRead;
}

size_t ChipUartLowLevel::stopWrite()
{
	if (isWriteInProgress() == false)
		return 0;

	parameters_.enableTxeInterrupt(false);
	parameters_.enableTcInterrupt(true);
	const auto bytesWritten = writePosition_;
	writePosition_ = {};
	writeSize_ = {};
	writeBuffer_ = {};
	return bytesWritten;
}

}	// namespace chip

}	// namespace distortos
