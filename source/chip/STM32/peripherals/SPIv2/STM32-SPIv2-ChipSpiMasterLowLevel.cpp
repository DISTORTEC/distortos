/**
 * \file
 * \brief ChipSpiMasterLowLevel class implementation for SPIv2 in STM32
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/ChipSpiMasterLowLevel.hpp"

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32-SPIv2-bits.h"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| public types
+---------------------------------------------------------------------------------------------------------------------*/

/// parameters for construction of SPI master low-level drivers
class ChipSpiMasterLowLevel::Parameters
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
	 * \param [in] spiBase is a base address of SPI peripheral
	 * \param [in] rccEnOffset is the offset of RCC register with appropriate SPIxEN bit, bytes
	 * \param [in] rccEnBitmask is the bitmask of appropriate SPIxEN bit in RCC register at \a rccEnOffset offset
	 * \param [in] rccRstOffset is the offset of RCC register with appropriate SPIxRST bit, bytes
	 * \param [in] rccRstBitmask is the bitmask of appropriate SPIxRST bit in RCC register at \a rccRstOffset offset
	 */

	constexpr Parameters(const uintptr_t spiBase, const size_t rccEnOffset, const uint32_t rccEnBitmask,
			const size_t rccRstOffset, const uint32_t rccRstBitmask) :
					spiBase_{spiBase},
					peripheralFrequency_{spiBase < ahbPeripheralsBaseAddress ? apbFrequency : ahbFrequency},
					rccEnBitmask_{rccEnBitmask},
					rccEnOffset_{rccEnOffset},
					rccRstBitmask_{rccRstBitmask},
					rccRstOffset_{rccRstOffset}
	{

	}

	/**
	 * \brief Enables or disables ERR interrupt of SPI.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableErrInterrupt(const bool enable) const
	{
		auto& spi = getSpi();
		const architecture::InterruptMaskingLock interruptMaskingLock;
		spi.CR2 = (spi.CR2 & ~SPI_CR2_ERRIE) | (enable == true ? SPI_CR2_ERRIE : 0);
	}

	/**
	 * \brief Enables or disables peripheral in SPI_CR1.
	 *
	 * \param [in] enable selects whether the peripheral will be enabled (true) or disabled (false)
	 */

	void enablePeripheral(const bool enable) const
	{
		auto& spi = getSpi();
		const architecture::InterruptMaskingLock interruptMaskingLock;
		spi.CR1 = (spi.CR1 & ~SPI_CR1_SPE) | (enable == true ? SPI_CR1_SPE : 0);
	}

	/**
	 * \brief Enables or disables peripheral clock in RCC.
	 *
	 * \param [in] enable selects whether the clock will be enabled (true) or disabled (false)
	 */

	void enablePeripheralClock(const bool enable) const
	{
		auto& rccEn = *reinterpret_cast<volatile uint32_t*>(RCC_BASE + rccEnOffset_);
		const architecture::InterruptMaskingLock interruptMaskingLock;
		rccEn = (rccEn & ~rccEnBitmask_) | (enable == true ? rccEnBitmask_ : 0);
	}

	/**
	 * \brief Enables or disables RXNE interrupt of SPI.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableRxneInterrupt(const bool enable) const
	{
		auto& spi = getSpi();
		const architecture::InterruptMaskingLock interruptMaskingLock;
		spi.CR2 = (spi.CR2 & ~SPI_CR2_RXNEIE) | (enable == true ? SPI_CR2_RXNEIE : 0);
	}

	/**
	 * \brief Enables or disables TXE interrupt of SPI.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableTxeInterrupt(const bool enable) const
	{
		auto& spi = getSpi();
		const architecture::InterruptMaskingLock interruptMaskingLock;
		spi.CR2 = (spi.CR2 & ~SPI_CR2_TXEIE) | (enable == true ? SPI_CR2_TXEIE : 0);
	}

	/**
	 * \return peripheral clock frequency, Hz
	 */

	uint32_t getPeripheralFrequency() const
	{
		return peripheralFrequency_;
	}

	/**
	 * \return reference to SPI_TypeDef object
	 */

	SPI_TypeDef& getSpi() const
	{
		return *reinterpret_cast<SPI_TypeDef*>(spiBase_);
	}

	/**
	 * \brief Resets all peripheral's registers via RCC
	 *
	 * \note Peripheral clock must be enabled in RCC for this operation to work.
	 */

	void resetPeripheral() const
	{
		auto& rccRst = *reinterpret_cast<volatile uint32_t*>(RCC_BASE + rccRstOffset_);
		const architecture::InterruptMaskingLock interruptMaskingLock;
		rccRst |= rccRstBitmask_;
		rccRst &= ~rccRstBitmask_;
	}

	/**
	 * \param [in] cr2 is the current value of CR2 register in SPI module
	 *
	 * \return current word length, bits, [4; 16] or
	 * [ChipSpiMasterLowLevel::minWordLength; ChipSpiMasterLowLevel::maxWordLength]
	 */

	static uint8_t getWordLength(const uint32_t cr2)
	{
		return ((cr2 & SPI_CR2_DS) >> SPI_CR2_DS_bit) + 1;
	}

private:

	/// base address of SPI peripheral
	uintptr_t spiBase_;

	/// peripheral clock frequency, Hz
	uint32_t peripheralFrequency_;

	/// bitmask of appropriate SPIxEN bit in RCC register at \a rccEnOffset_ offset
	uint32_t rccEnBitmask_;

	/// offset of RCC register with appropriate SPIxEN bit, bytes
	size_t rccEnOffset_;

	/// bitmask of appropriate SPIxRST bit in RCC register at \a rccRstOffset_ offset
	uint32_t rccRstBitmask_;

	/// offset of RCC register with appropriate SPIxRST bit, bytes
	size_t rccRstOffset_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| public static objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi1Parameters {SPI1_BASE,
		offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_SPI1EN, offsetof(RCC_TypeDef, APB2RSTR), RCC_APB2RSTR_SPI1RST};

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV2_SPI2_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi2Parameters {SPI2_BASE,
		offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_SPI2EN, offsetof(RCC_TypeDef, APB1RSTR), RCC_APB1RSTR_SPI2RST};

#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ChipSpiMasterLowLevel::~ChipSpiMasterLowLevel()
{
	if (isStarted() == false)
		return;

	parameters_.resetPeripheral();
	parameters_.enablePeripheralClock(false);
}

std::pair<int, uint32_t> ChipSpiMasterLowLevel::configure(const devices::SpiMode mode, const uint32_t clockFrequency,
		const uint8_t wordLength, const bool lsbFirst)
{
	if (wordLength < minWordLength || wordLength > maxWordLength)
		return {EINVAL, {}};

	if (isStarted() == false)
		return {EBADF, {}};

	if (isTransferInProgress() == true)
		return {EBUSY, {}};

	const auto peripheralFrequency = parameters_.getPeripheralFrequency();
	const auto divider = (peripheralFrequency + clockFrequency - 1) / clockFrequency;
	if (divider > 256)
		return {EINVAL, {}};

	const uint32_t br = divider <= 2 ? 0 : 31 - __CLZ(divider - 1);
	auto& spi = parameters_.getSpi();
	spi.CR1 = (spi.CR1 & ~(SPI_CR1_LSBFIRST | SPI_CR1_BR | SPI_CR1_CPOL | SPI_CR1_CPHA)) |
			(lsbFirst << SPI_CR1_LSBFIRST_bit) | (br << SPI_CR1_BR_bit) |
			((mode == devices::SpiMode::cpol1cpha0 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPOL_bit) |
			((mode == devices::SpiMode::cpol0cpha1 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPHA_bit);
	spi.CR2 = (spi.CR2 & ~(SPI_CR2_FRXTH | SPI_CR2_DS)) | ((wordLength <= 8) << SPI_CR2_FRXTH_bit) |
			((wordLength - 1) << SPI_CR2_DS_bit);

	return {{}, peripheralFrequency / (1 << (br + 1))};
}

void ChipSpiMasterLowLevel::interruptHandler()
{
	bool done {};
	auto& spi = parameters_.getSpi();
	const auto sr = spi.SR;
	const auto cr2 = spi.CR2;
	const auto wordLength = Parameters::getWordLength(cr2);

	if ((sr & (SPI_SR_MODF | SPI_SR_OVR | SPI_SR_CRCERR)) != 0 && (cr2 & SPI_CR2_ERRIE) != 0)	// error?
	{
		if ((sr & SPI_SR_MODF) != 0)	// master mode fault?
		{
			parameters_.enablePeripheral(false);	// clears MODF flag
			errorSet_[devices::SpiMasterErrorSet::masterModeFault] = true;
			done = true;
		}
		if ((sr & SPI_SR_OVR) != 0)	// overrun error?
		{
			spi.DR;
			spi.SR;	// clears OVR flag
			errorSet_[devices::SpiMasterErrorSet::overrunError] = true;
		}
		if ((sr & SPI_SR_CRCERR) != 0)	// CRC error?
		{
			spi.SR = 0;	// clears CRCERR flag
			errorSet_[devices::SpiMasterErrorSet::crcError] = true;
		}

		parameters_.enableTxeInterrupt(false);

		if ((sr & SPI_SR_BSY) == 0)
			done = true;
	}
	else if ((sr & SPI_SR_RXNE) != 0 && (cr2 & SPI_CR2_RXNEIE) != 0)	// read?
	{
		const uint16_t word = wordLength <= 8 ? *reinterpret_cast<volatile uint8_t*>(&spi.DR) : spi.DR;
		const auto readBuffer = readBuffer_;
		auto readPosition = readPosition_;
		if (readBuffer != nullptr)
		{
			readBuffer[readPosition++] = word;
			if (wordLength > 8)
				readBuffer[readPosition++] = word >> 8;
		}
		else
			readPosition += (wordLength + 8 - 1) / 8;
		readPosition_ = readPosition;
		if (readPosition == size_)
		{
			parameters_.enableRxneInterrupt(false);
			done = true;
		}
	}
	else if ((sr & SPI_SR_TXE) != 0 && (cr2 & SPI_CR2_TXEIE) != 0)	// write?
	{
		const auto writeBuffer = writeBuffer_;
		auto writePosition = writePosition_;
		uint16_t word;
		if (writeBuffer != nullptr)
		{
			const uint16_t characterLow = writeBuffer[writePosition++];
			const uint16_t characterHigh = wordLength > 8 ? writeBuffer[writePosition++] : 0;
			word = characterLow | (characterHigh << 8);
		}
		else
		{
			writePosition += (wordLength + 8 - 1) / 8;
			word = 0xffff;
		}
		writePosition_ = writePosition;
		if (wordLength <= 8)
			*reinterpret_cast<volatile uint8_t*>(&spi.DR) = word;
		else
			spi.DR = word;
		if (writePosition == size_)
			parameters_.enableTxeInterrupt(false);
	}

	if (done == true)	// transfer finished of failed?
	{
		parameters_.enableTxeInterrupt(false);
		parameters_.enableRxneInterrupt(false);
		parameters_.enableErrInterrupt(false);
		const auto errorSet = errorSet_;
		errorSet_.reset();
		writePosition_ = {};
		const auto bytesTransfered = readPosition_;
		readPosition_ = {};
		size_ = {};
		writeBuffer_ = {};
		readBuffer_ = {};

		spiMasterBase_->transferCompleteEvent(errorSet, bytesTransfered);
	}
}

int ChipSpiMasterLowLevel::start(devices::SpiMasterBase& spiMasterBase)
{
	if (isStarted() == true)
		return EBADF;

	parameters_.enablePeripheralClock(true);
	parameters_.resetPeripheral();
	spiMasterBase_ = &spiMasterBase;
	auto& spi = parameters_.getSpi();
	spi.CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR;
	spi.CR2 = SPI_CR2_FRXTH | SPI_CR2_DS_8BIT;

	return 0;
}

int ChipSpiMasterLowLevel::startTransfer(const void* const writeBuffer, void* const readBuffer, const size_t size)
{
	if (size == 0)
		return EINVAL;

	if (isStarted() == false)
		return EBADF;

	if (isTransferInProgress() == true)
		return EBUSY;

	if (size % ((Parameters::getWordLength(parameters_.getSpi().CR2) + 8 - 1) / 8) != 0)
		return EINVAL;

	readBuffer_ = static_cast<uint8_t*>(readBuffer);
	writeBuffer_ = static_cast<const uint8_t*>(writeBuffer);
	size_ = size;
	readPosition_ = 0;
	writePosition_ = 0;

	parameters_.enableErrInterrupt(true);
	parameters_.enableRxneInterrupt(true);
	parameters_.enableTxeInterrupt(true);
	return 0;
}

int ChipSpiMasterLowLevel::stop()
{
	if (isStarted() == false)
		return EBADF;

	if (isTransferInProgress() == true)
		return EBUSY;

	parameters_.resetPeripheral();
	parameters_.enablePeripheralClock(false);
	spiMasterBase_ = nullptr;
	return 0;
}

}	// namespace chip

}	// namespace distortos
