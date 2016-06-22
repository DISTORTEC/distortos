/**
 * \file
 * \brief ChipUartLowLevel class implementation for STM32F4
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/ChipUartLowLevel.hpp"

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32F4-RCC-bits.h"
#include "distortos/chip/STM32F4-USART-bits.h"

#include "distortos/devices/communication/UartBase.hpp"

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
 * \brief Decode value of USART_SR register to devices::UartBase::ErrorSet
 *
 * \param [in] sr is the value of USART_SR register that will be decoded
 *
 * \return devices::UartBase::ErrorSet with errors decoded from \a sr
 */

devices::UartBase::ErrorSet decodeErrors(const uint32_t sr)
{
	devices::UartBase::ErrorSet errorSet {};
	errorSet[devices::UartBase::framingError] = (sr & USART_SR_FE) != 0;
	errorSet[devices::UartBase::noiseError] = (sr & USART_SR_NE) != 0;
	errorSet[devices::UartBase::overrunError] = (sr & USART_SR_ORE) != 0;
	errorSet[devices::UartBase::parityError] = (sr & USART_SR_PE) != 0;
	return errorSet;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public types
+---------------------------------------------------------------------------------------------------------------------*/

/// parameters for construction of UART low-level drivers
class ChipUartLowLevel::Parameters
{
public:

	static_assert(APB1PERIPH_BASE < APB2PERIPH_BASE && APB2PERIPH_BASE < AHB1PERIPH_BASE,
				"Invalid order of APB1, APB2 and AHB!");

	/**
	 * \brief Parameters's constructor
	 *
	 * \param [in] uart is a pointer to USART_TypeDef with registers
	 * \param [in] rxneieBb is a pointer to bitband alias of RXNEIE bit in USART_CR1 register
	 * \param [in] tcieBb is a pointer to bitband alias of TCIE bit in USART_CR1 register
	 * \param [in] txeieBb is a pointer to bitband alias of TXEIE bit in USART_CR1 register
	 * \param [in] rccEnBb is a pointer to bitband alias of apropriate U[S]ARTxEN bit in RCC register
	 * \param [in] rccRstBb is a pointer to bitband alias of apropriate U[S]ARTxRST bit in RCC register
	 * \param [in] irqNumber is the NVIC's IRQ number of associated U[S]ART
	 *
	 * \note Don't add "const" to values of pointers, don't use references - see
	 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=71113
	 */

	constexpr Parameters(USART_TypeDef* uart, volatile unsigned long* rxneieBb, volatile unsigned long* tcieBb,
			volatile unsigned long* txeieBb, volatile unsigned long* rccEnBb, volatile unsigned long* rccRstBb,
			const IRQn_Type irqNumber) :
					uart_{uart},
					peripheralFrequency_{reinterpret_cast<uint32_t>(uart) < APB2PERIPH_BASE ? apb1Frequency :
							reinterpret_cast<uint32_t>(uart) < AHB1PERIPH_BASE ? apb2Frequency : ahbFrequency},
					rxneieBb_{rxneieBb},
					tcieBb_{tcieBb},
					txeieBb_{txeieBb},
					rccEnBb_{rccEnBb},
					rccRstBb_{rccRstBb},
					irqNumber_{irqNumber}
	{

	}

	/**
	 * \brief Sets priority of interrupt to CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI.
	 */

	void configureInterruptPriority() const
	{
		NVIC_SetPriority(irqNumber_, CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI);
	}

	/**
	 * \brief Enables or disables interrupt in NVIC.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableInterrupt(const bool enable) const
	{
		enable == true ? NVIC_EnableIRQ(irqNumber_) : NVIC_DisableIRQ(irqNumber_);
	}

	/**
	 * \brief Enables or disables peripheral clock in RCC.
	 *
	 * \param [in] enable selects whether the clock will be enabled (true) or disabled (false)
	 */

	void enablePeripheralClock(const bool enable) const
	{
		*rccEnBb_ = enable;
	}

	/**
	 * \brief Enables or disables RXNE interrupt of UART.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableRxneInterrupt(const bool enable) const
	{
		*rxneieBb_ = enable;
	}

	/**
	 * \brief Enables or disables TC interrupt of UART.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableTcInterrupt(const bool enable) const
	{
		*tcieBb_ = enable;
	}

	/**
	 * \brief Enables or disables TXE interrupt of UART.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableTxeInterrupt(const bool enable) const
	{
		*txeieBb_ = enable;
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
		return *uart_;
	}

	/**
	 * \return true if real character length (including optional parity) is 9 bits, false otherwise
	 */

	bool is9BitFormatEnabled() const
	{
		return (getUart().CR1 & (USART_CR1_M | USART_CR1_PCE)) == USART_CR1_M;
	}

	/**
	 * \brief Resets all peripheral's registers via RCC
	 *
	 * \note Peripheral clock must be enabled in RCC for this operation to work.
	 */

	void resetPeripheral() const
	{
		*rccRstBb_ = 1;
		*rccRstBb_ = 0;
	}

private:

	/// pointer to USART_TypeDef with registers
	USART_TypeDef* uart_;

	/// peripheral clock frequency, Hz
	uint32_t peripheralFrequency_;

	/// pointer to bitband alias of RXNEIE bit in USART_CR1 register
	volatile unsigned long* rxneieBb_;

	/// pointer to bitband alias of TCIE bit in USART_CR1 register
	volatile unsigned long* tcieBb_;

	/// pointer to bitband alias of TXEIE bit in USART_CR1 register
	volatile unsigned long* txeieBb_;

	/// pointer to bitband alias of apropriate U[S]ARTxEN bit in RCC register
	volatile unsigned long* rccEnBb_;

	/// pointer to bitband alias of apropriate U[S]ARTxRST bit in RCC register
	volatile unsigned long* rccRstBb_;

	/// NVIC's IRQ number of associated U[S]ART
	IRQn_Type irqNumber_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| public static objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_HAS_USART1

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart1parameters {USART1, &USART1_CR1_RXNEIE_bb,
		&USART1_CR1_TCIE_bb, &USART1_CR1_TXEIE_bb, &RCC_APB2ENR_USART1EN_bb, &RCC_APB2RSTR_USART1RST_bb, USART1_IRQn};

#endif	// def CONFIG_CHIP_HAS_USART1

#ifdef CONFIG_CHIP_HAS_USART2

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart2parameters {USART2, &USART2_CR1_RXNEIE_bb,
		&USART2_CR1_TCIE_bb, &USART2_CR1_TXEIE_bb, &RCC_APB1ENR_USART2EN_bb, &RCC_APB1RSTR_USART2RST_bb, USART2_IRQn};

#endif	// def CONFIG_CHIP_HAS_USART2

#ifdef CONFIG_CHIP_HAS_USART3

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart3parameters {USART3, &USART3_CR1_RXNEIE_bb,
		&USART3_CR1_TCIE_bb, &USART3_CR1_TXEIE_bb, &RCC_APB1ENR_USART3EN_bb, &RCC_APB1RSTR_USART3RST_bb, USART3_IRQn};

#endif	// def CONFIG_CHIP_HAS_USART3

#ifdef CONFIG_CHIP_HAS_UART4

const ChipUartLowLevel::Parameters ChipUartLowLevel::uart4parameters {UART4, &UART4_CR1_RXNEIE_bb,
		&UART4_CR1_TCIE_bb, &UART4_CR1_TXEIE_bb, &RCC_APB1ENR_UART4EN_bb, &RCC_APB1RSTR_UART4RST_bb, UART4_IRQn};

#endif	// def CONFIG_CHIP_HAS_UART4

#ifdef CONFIG_CHIP_HAS_UART5

const ChipUartLowLevel::Parameters ChipUartLowLevel::uart5parameters {UART5, &UART5_CR1_RXNEIE_bb,
		&UART5_CR1_TCIE_bb, &UART5_CR1_TXEIE_bb, &RCC_APB1ENR_UART5EN_bb, &RCC_APB1RSTR_UART5RST_bb, UART5_IRQn};

#endif	// def CONFIG_CHIP_HAS_UART5

#ifdef CONFIG_CHIP_HAS_USART6

const ChipUartLowLevel::Parameters ChipUartLowLevel::usart6parameters {USART6, &USART6_CR1_RXNEIE_bb,
		&USART6_CR1_TCIE_bb, &USART6_CR1_TXEIE_bb, &RCC_APB2ENR_USART6EN_bb, &RCC_APB2RSTR_USART6RST_bb, USART6_IRQn};

#endif	// def CONFIG_CHIP_HAS_USART6

#ifdef CONFIG_CHIP_HAS_UART7

const ChipUartLowLevel::Parameters ChipUartLowLevel::uart7parameters {UART7, &UART7_CR1_RXNEIE_bb,
		&UART7_CR1_TCIE_bb, &UART7_CR1_TXEIE_bb, &RCC_APB1ENR_UART7EN_bb, &RCC_APB1RSTR_UART7RST_bb, UART7_IRQn};

#endif	// def CONFIG_CHIP_HAS_UART7

#ifdef CONFIG_CHIP_HAS_UART8

const ChipUartLowLevel::Parameters ChipUartLowLevel::uart8parameters {UART8, &UART8_CR1_RXNEIE_bb,
		&UART8_CR1_TCIE_bb, &UART8_CR1_TXEIE_bb, &RCC_APB1ENR_UART8EN_bb, &RCC_APB1RSTR_UART8RST_bb, UART8_IRQn};

#endif	// def CONFIG_CHIP_HAS_UART8

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ChipUartLowLevel::~ChipUartLowLevel()
{
	if (isStarted() == false)
		return;

	parameters_.enableInterrupt(false);
	parameters_.resetPeripheral();
	parameters_.enablePeripheralClock(false);
}

void ChipUartLowLevel::interruptHandler()
{
	auto& uart = parameters_.getUart();
	const auto _9BitFormat = parameters_.is9BitFormatEnabled();
	uint32_t sr;
	uint32_t maskedSr;
	// loop while there are enabled interrupt sources waiting to be served
	while (sr = uart.SR, (maskedSr = sr & uart.CR1 & (USART_SR_RXNE | USART_SR_TXE | USART_SR_TC)) != 0)
	{
		if ((maskedSr & USART_SR_RXNE) != 0)		// read & receive errors
		{
			const uint16_t character = uart.DR;
			const auto readBuffer = readBuffer_;
			auto readPosition = readPosition_;
			readBuffer[readPosition++] = character;
			if (_9BitFormat == true)
				readBuffer[readPosition++] = character >> 8;
			readPosition_ = readPosition;
			if ((sr & (USART_SR_FE | USART_SR_NE | USART_SR_ORE | USART_SR_PE)) != 0)
				uartBase_->receiveErrorEvent(decodeErrors(sr));
			if (readPosition == readSize_)
				uartBase_->readCompleteEvent(stopRead());
		}
		else if ((maskedSr & USART_SR_TXE) != 0)	// write
		{
			const auto writeBuffer = writeBuffer_;
			auto writePosition = writePosition_;
			const uint16_t characterLow = writeBuffer[writePosition++];
			const uint16_t characterHigh = _9BitFormat == true ? writeBuffer[writePosition++] : 0;
			writePosition_ = writePosition;
			uart.DR = characterLow | (characterHigh << 8);
			if (writePosition == writeSize_)
				uartBase_->writeCompleteEvent(stopWrite());
		}
		else if ((maskedSr & USART_SR_TC) != 0)		// transmit complete
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

	if (mantissa == 0 || mantissa > (USART_BRR_DIV_Mantissa >> USART_BRR_DIV_Mantissa_bit))
		return {EINVAL, {}};

	const auto realCharacterLength = characterLength + (parity != devices::UartParity::none);
	if (realCharacterLength < minUartCharacterLength + 1 || realCharacterLength > maxUartCharacterLength)
		return {EINVAL, {}};

	parameters_.enablePeripheralClock(true);
	parameters_.resetPeripheral();

	parameters_.configureInterruptPriority();
	uartBase_ = &uartBase;
	auto& uart = parameters_.getUart();
	uart.BRR = (mantissa << USART_BRR_DIV_Mantissa_bit) | (fraction << USART_BRR_DIV_Fraction_bit);
	uart.CR2 = _2StopBits << USART_CR2_STOP_1_bit;
	uart.CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE | (over8 << USART_CR1_OVER8_bit) |
			((realCharacterLength == maxUartCharacterLength) << USART_CR1_M_bit) |
			((parity != devices::UartParity::none) << USART_CR1_PCE_bit) |
			((parity == devices::UartParity::odd) << USART_CR1_PS_bit);
	parameters_.enableInterrupt(true);
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

	if (parameters_.is9BitFormatEnabled() == true && size % 2 != 0)
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

	if (parameters_.is9BitFormatEnabled() == true && size % 2 != 0)
		return EINVAL;

	writeBuffer_ = static_cast<const uint8_t*>(buffer);
	writeSize_ = size;
	writePosition_ = 0;
	parameters_.enableTcInterrupt(false);

	if ((parameters_.getUart().SR & USART_SR_TC) != 0)
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

	parameters_.enableInterrupt(false);
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
