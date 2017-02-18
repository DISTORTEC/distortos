/**
 * \file
 * \brief ChipSpiMasterLowLevel class implementation for SPIv1 in STM32
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/ChipSpiMasterLowLevel.hpp"

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32-bit-banding.h"

#include "distortos/devices/communication/SpiMasterBase.hpp"

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
	/// base address of APB1 peripherals
	constexpr static uintptr_t apb1PeripheralsBaseAddress {APB1PERIPH_BASE};
	/// base address of APB2 peripherals
	constexpr static uintptr_t apb2PeripheralsBaseAddress {APB2PERIPH_BASE};
	/// base address of AHB peripherals
#if defined(AHBPERIPH_BASE)
	constexpr static uintptr_t ahbPeripheralsBaseAddress {AHBPERIPH_BASE};
#elif defined(AHB1PERIPH_BASE)
	constexpr static uintptr_t ahbPeripheralsBaseAddress {AHB1PERIPH_BASE};
#else
	#error "Unknown base address of AHB peripherals!"
#endif

	static_assert(apb1PeripheralsBaseAddress < apb2PeripheralsBaseAddress &&
			apb2PeripheralsBaseAddress < ahbPeripheralsBaseAddress, "Invalid order of APB1, APB2 and AHB!");

public:

	/**
	 * \brief Parameters's constructor
	 *
	 * \param [in] spiBase is a base address of SPI peripheral
	 * \param [in] rccEnBbAddress is an address of bitband alias of appropriate SPIxEN bit in RCC register
	 * \param [in] rccRstBbAddress is an address of bitband alias of appropriate SPIxRST bit in RCC register
	 */

	constexpr Parameters(const uintptr_t spiBase, const uintptr_t rccEnBbAddress, const uintptr_t rccRstBbAddress) :
			spiBase_{spiBase},
			peripheralFrequency_{spiBase < apb2PeripheralsBaseAddress ? apb1Frequency :
					spiBase < ahbPeripheralsBaseAddress ? apb2Frequency : ahbFrequency},
			speBbAddress_{STM32_BITBAND_IMPLEMENTATION(spiBase, SPI_TypeDef, CR1, SPI_CR1_SPE)},
			errieBbAddress_{STM32_BITBAND_IMPLEMENTATION(spiBase, SPI_TypeDef, CR2, SPI_CR2_ERRIE)},
			rxneieBbAddress_{STM32_BITBAND_IMPLEMENTATION(spiBase, SPI_TypeDef, CR2, SPI_CR2_RXNEIE)},
			txeieBbAddress_{STM32_BITBAND_IMPLEMENTATION(spiBase, SPI_TypeDef, CR2, SPI_CR2_TXEIE)},
			rccEnBbAddress_{rccEnBbAddress},
			rccRstBbAddress_{rccRstBbAddress}
	{

	}

	/**
	 * \brief Enables or disables ERR interrupt of SPI.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableErrInterrupt(const bool enable) const
	{
		*reinterpret_cast<volatile unsigned long*>(errieBbAddress_) = enable;
	}

	/**
	 * \brief Enables or disables peripheral in SPI_CR1.
	 *
	 * \param [in] enable selects whether the peripheral will be enabled (true) or disabled (false)
	 */

	void enablePeripheral(const bool enable) const
	{
		*reinterpret_cast<volatile unsigned long*>(speBbAddress_) = enable;
	}

	/**
	 * \brief Enables or disables peripheral clock in RCC.
	 *
	 * \param [in] enable selects whether the clock will be enabled (true) or disabled (false)
	 */

	void enablePeripheralClock(const bool enable) const
	{
		*reinterpret_cast<volatile unsigned long*>(rccEnBbAddress_) = enable;
	}

	/**
	 * \brief Enables or disables RXNE interrupt of SPI.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableRxneInterrupt(const bool enable) const
	{
		*reinterpret_cast<volatile unsigned long*>(rxneieBbAddress_) = enable;
	}

	/**
	 * \brief Enables or disables TXE interrupt of SPI.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableTxeInterrupt(const bool enable) const
	{
		*reinterpret_cast<volatile unsigned long*>(txeieBbAddress_) = enable;
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
	 * \return current word length, bits, {8, 16}
	 */

	uint8_t getWordLength() const
	{
		return (getSpi().CR1 & SPI_CR1_DFF) == 0 ? 8 : 16;
	}

	/**
	 * \brief Resets all peripheral's registers via RCC
	 *
	 * \note Peripheral clock must be enabled in RCC for this operation to work.
	 */

	void resetPeripheral() const
	{
		*reinterpret_cast<volatile unsigned long*>(rccRstBbAddress_) = 1;
		*reinterpret_cast<volatile unsigned long*>(rccRstBbAddress_) = 0;
	}

private:

	/// base address of SPI peripheral
	uintptr_t spiBase_;

	/// peripheral clock frequency, Hz
	uint32_t peripheralFrequency_;

	/// address of bitband alias of SPE bit in SPI_CR1 register
	uintptr_t speBbAddress_;

	/// address of bitband alias of ERRIE bit in SPI_CR2 register
	uintptr_t errieBbAddress_;

	/// address of bitband alias of RXNEIE bit in SPI_CR2 register
	uintptr_t rxneieBbAddress_;

	/// address of bitband alias of TXEIE bit in SPI_CR2 register
	uintptr_t txeieBbAddress_;

	/// address of bitband alias of appropriate SPIxEN bit in RCC register
	uintptr_t rccEnBbAddress_;

	/// address of bitband alias of appropriate SPIxRST bit in RCC register
	uintptr_t rccRstBbAddress_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| public static objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI1_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi1Parameters {SPI1_BASE,
		STM32_BITBAND_ADDRESS(RCC, APB2ENR, SPI1EN), STM32_BITBAND_ADDRESS(RCC, APB2RSTR, SPI1RST)};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI1_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI2_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi2Parameters {SPI2_BASE,
		STM32_BITBAND_ADDRESS(RCC, APB1ENR, SPI2EN), STM32_BITBAND_ADDRESS(RCC, APB1RSTR, SPI2RST)};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI2_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI3_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi3Parameters {SPI3_BASE,
		STM32_BITBAND_ADDRESS(RCC, APB1ENR, SPI3EN), STM32_BITBAND_ADDRESS(RCC, APB1RSTR, SPI3RST)};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI3_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI4_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi4Parameters {SPI4_BASE,
		STM32_BITBAND_ADDRESS(RCC, APB2ENR, SPI4EN), STM32_BITBAND_ADDRESS(RCC, APB2RSTR, SPI4RST)};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI4_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI5_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi5Parameters {SPI5_BASE,
		STM32_BITBAND_ADDRESS(RCC, APB2ENR, SPI5EN), STM32_BITBAND_ADDRESS(RCC, APB2RSTR, SPI5RST)};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI5_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI6_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi6Parameters {SPI6_BASE,
		STM32_BITBAND_ADDRESS(RCC, APB2ENR, SPI6EN), STM32_BITBAND_ADDRESS(RCC, APB2RSTR, SPI6RST)};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI6_ENABLE

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
	if (wordLength != 8 && wordLength != 16)
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

	// value of DFF bit (which determines word length) must be changed only when SPI peripheral is disabled
	const auto disablePeripheral = wordLength != parameters_.getWordLength();
	if (disablePeripheral == true)
		parameters_.enablePeripheral(false);

	spi.CR1 = (spi.CR1 & ~(SPI_CR1_DFF | SPI_CR1_LSBFIRST | SPI_CR1_BR | SPI_CR1_CPOL | SPI_CR1_CPHA)) |
			(wordLength == 16) << SPI_CR1_DFF_Pos | lsbFirst << SPI_CR1_LSBFIRST_Pos | br << SPI_CR1_BR_Pos |
			(mode == devices::SpiMode::cpol1cpha0 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPOL_Pos |
			(mode == devices::SpiMode::cpol0cpha1 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPHA_Pos;

	if (disablePeripheral == true)
		parameters_.enablePeripheral(true);

	return {{}, peripheralFrequency / (1 << (br + 1))};
}

void ChipSpiMasterLowLevel::interruptHandler()
{
	bool done {};
	auto& spi = parameters_.getSpi();
	const auto sr = spi.SR;
	const auto cr2 = spi.CR2;
	const auto wordLength = parameters_.getWordLength();

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
		const uint16_t word = spi.DR;
		const auto readBuffer = readBuffer_;
		auto readPosition = readPosition_;
		if (readBuffer != nullptr)
		{
			readBuffer[readPosition++] = word;
			if (wordLength == 16)
				readBuffer[readPosition++] = word >> 8;
		}
		else
			readPosition += wordLength / 8;
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
			const uint16_t characterHigh = wordLength == 16 ? writeBuffer[writePosition++] : 0;
			word = characterLow | characterHigh << 8;
		}
		else
		{
			writePosition += wordLength / 8;
			word = 0xffff;
		}
		writePosition_ = writePosition;
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
	parameters_.getSpi().CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR;

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

	if (size % (parameters_.getWordLength() / 8) != 0)
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
