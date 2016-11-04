/**
 * \file
 * \brief ChipSpiMasterLowLevel class implementation for SPIv1 in STM32
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
#include "distortos/chip/STM32-SPIv1-bits.h"

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
 * \param [in] cr1 is the current value of CR1 register in SPI module
 *
 * \return current word length, bits, {8, 16}
 */

uint8_t getWordLength(const uint32_t cr1)
{
	return (cr1 & SPI_CR1_DFF) == 0 ? 8 : 16;
}

}	// namespace

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
	 * \param [in] spi is a base address of SPI peripheral
	 * \param [in] rccEnBbAddress is an address of bitband alias of appropriate SPIxEN bit in RCC register
	 * \param [in] rccRstBbAddress is an address of bitband alias of appropriate SPIxRST bit in RCC register
	 * \param [in] irqNumber is the NVIC's IRQ number of associated SPI
	 */

	constexpr Parameters(const uintptr_t spiBase, const uintptr_t rccEnBbAddress, const uintptr_t rccRstBbAddress,
			const IRQn_Type irqNumber) :
					spiBase_{spiBase},
					peripheralFrequency_{spiBase < apb2PeripheralsBaseAddress ? apb1Frequency :
							spiBase < ahbPeripheralsBaseAddress ? apb2Frequency : ahbFrequency},
					speBbAddress_{BITBAND_ADDRESS(spiBase + offsetof(SPI_TypeDef, CR1), SPI_CR1_SPE_bit)},
					errieBbAddress_{BITBAND_ADDRESS(spiBase + offsetof(SPI_TypeDef, CR2), SPI_CR2_ERRIE_bit)},
					rxneieBbAddress_{BITBAND_ADDRESS(spiBase + offsetof(SPI_TypeDef, CR2), SPI_CR2_RXNEIE_bit)},
					txeieBbAddress_{BITBAND_ADDRESS(spiBase + offsetof(SPI_TypeDef, CR2), SPI_CR2_TXEIE_bit)},
					rccEnBbAddress_{rccEnBbAddress},
					rccRstBbAddress_{rccRstBbAddress},
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
	 * \brief Enables or disables ERR interrupt of SPI.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableErrInterrupt(const bool enable) const
	{
		*reinterpret_cast<volatile unsigned long*>(errieBbAddress_) = enable;
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

	/// NVIC's IRQ number of associated SPI
	IRQn_Type irqNumber_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| public static objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI1_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi1Parameters {SPI1_BASE,
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB2ENR), __builtin_ctzl(RCC_APB2ENR_SPI1EN)),
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR), __builtin_ctzl(RCC_APB2RSTR_SPI1RST)),
		SPI1_IRQn};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI1_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI2_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi2Parameters {SPI2_BASE,
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB1ENR), __builtin_ctzl(RCC_APB1ENR_SPI2EN)),
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB1RSTR), __builtin_ctzl(RCC_APB1RSTR_SPI2RST)),
		SPI2_IRQn};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI2_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI3_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi3Parameters {SPI3_BASE,
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB1ENR), __builtin_ctzl(RCC_APB1ENR_SPI3EN)),
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB1RSTR), __builtin_ctzl(RCC_APB1RSTR_SPI3RST)),
		SPI3_IRQn};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI3_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI4_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi4Parameters {SPI4_BASE,
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB2ENR), __builtin_ctzl(RCC_APB2ENR_SPI4EN)),
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR), __builtin_ctzl(RCC_APB2RSTR_SPI4RST)),
		SPI4_IRQn};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI4_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI5_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi5Parameters {SPI5_BASE,
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB2ENR), __builtin_ctzl(RCC_APB2ENR_SPI5EN)),
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR), __builtin_ctzl(RCC_APB2RSTR_SPI5RST)),
		SPI5_IRQn};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI5_ENABLE

#ifdef CONFIG_CHIP_STM32_SPIV1_SPI6_ENABLE

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi6Parameters {SPI6_BASE,
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB2ENR), __builtin_ctzl(RCC_APB2ENR_SPI6EN)),
		BITBAND_ADDRESS(RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR), __builtin_ctzl(RCC_APB2RSTR_SPI6RST)),
		SPI6_IRQn};

#endif	// def CONFIG_CHIP_STM32_SPIV1_SPI6_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ChipSpiMasterLowLevel::~ChipSpiMasterLowLevel()
{
	if (isStarted() == false)
		return;

	parameters_.enableInterrupt(false);
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
	const auto disablePeripheral = wordLength != getWordLength(spi.CR1);
	if (disablePeripheral == true)
		parameters_.enablePeripheral(false);

	spi.CR1 = (spi.CR1 & ~(SPI_CR1_DFF | SPI_CR1_LSBFIRST | SPI_CR1_BR | SPI_CR1_CPOL | SPI_CR1_CPHA)) |
			((wordLength == 16) << SPI_CR1_DFF_bit) | (lsbFirst << SPI_CR1_LSBFIRST_bit) | (br << SPI_CR1_BR_bit) |
			((mode == devices::SpiMode::cpol1cpha0 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPOL_bit) |
			((mode == devices::SpiMode::cpol0cpha1 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPHA_bit);

	if (disablePeripheral == true)
		parameters_.enablePeripheral(true);

	return {{}, peripheralFrequency / (1 << (br + 1))};
}

void ChipSpiMasterLowLevel::interruptHandler()
{
	bool done {};
	auto& spi = parameters_.getSpi();
	auto wordLength = getWordLength(spi.CR1);
	const auto sr = spi.SR;
	const auto cr2 = spi.CR2;

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
			word = characterLow | (characterHigh << 8);
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
		wordLength = getWordLength(spi.CR1);
	}
}

int ChipSpiMasterLowLevel::start(devices::SpiMasterBase& spiMasterBase)
{
	if (isStarted() == true)
		return EBADF;

	parameters_.enablePeripheralClock(true);
	parameters_.resetPeripheral();
	parameters_.configureInterruptPriority();
	spiMasterBase_ = &spiMasterBase;
	parameters_.getSpi().CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR;
	parameters_.enableInterrupt(true);

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

	if (size % (getWordLength(parameters_.getSpi().CR1) / 8) != 0)
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

	parameters_.enableInterrupt(false);
	parameters_.resetPeripheral();
	parameters_.enablePeripheralClock(false);
	spiMasterBase_ = nullptr;
	return 0;
}

}	// namespace chip

}	// namespace distortos
