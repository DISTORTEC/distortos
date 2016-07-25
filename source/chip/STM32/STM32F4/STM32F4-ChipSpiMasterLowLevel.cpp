/**
 * \file
 * \brief ChipSpiMasterLowLevel class implementation for STM32F4
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
#include "distortos/chip/STM32F4-RCC-bits.h"
#include "distortos/chip/STM32F4-SPI-bits.h"

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
public:

	static_assert(APB1PERIPH_BASE < APB2PERIPH_BASE && APB2PERIPH_BASE < AHB1PERIPH_BASE,
				"Invalid order of APB1, APB2 and AHB!");

	/**
	 * \brief Parameters's constructor
	 *
	 * \param [in] spi is a pointer to SPI_TypeDef with registers
	 * \param [in] speBb is a pointer to bitband alias of SPE bit in SPI_CR1 register
	 * \param [in] errieBb is a pointer to bitband alias of ERRIE bit in SPI_CR2 register
	 * \param [in] rxneieBb is a pointer to bitband alias of RXNEIE bit in SPI_CR2 register
	 * \param [in] txeieBb is a pointer to bitband alias of TXEIE bit in SPI_CR2 register
	 * \param [in] rccEnBb is a pointer to bitband alias of appropriate SPIxEN bit in RCC register
	 * \param [in] rccRstBb is a pointer to bitband alias of appropriate SPIxRST bit in RCC register
	 * \param [in] irqNumber is the NVIC's IRQ number of associated SPI
	 *
	 * \note Don't add "const" to values of pointers, don't use references - see
	 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=71113
	 */

	constexpr Parameters(SPI_TypeDef* spi, volatile unsigned long* speBb, volatile unsigned long* errieBb,
			volatile unsigned long* rxneieBb, volatile unsigned long* txeieBb, volatile unsigned long* rccEnBb,
			volatile unsigned long* rccRstBb, const IRQn_Type irqNumber) :
					spi_{spi},
					peripheralFrequency_{reinterpret_cast<uint32_t>(spi) < APB2PERIPH_BASE ? apb1Frequency :
							reinterpret_cast<uint32_t>(spi) < AHB1PERIPH_BASE ? apb2Frequency : ahbFrequency},
					speBb_{speBb},
					errieBb_{errieBb},
					rxneieBb_{rxneieBb},
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
	 * \brief Enables or disables ERR interrupt of SPI.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableErrInterrupt(const bool enable) const
	{
		*errieBb_ = enable;
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
		*speBb_ = enable;
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
	 * \brief Enables or disables RXNE interrupt of SPI.
	 *
	 * \param [in] enable selects whether the interrupt will be enabled (true) or disabled (false)
	 */

	void enableRxneInterrupt(const bool enable) const
	{
		*rxneieBb_ = enable;
	}

	/**
	 * \brief Enables or disables TXE interrupt of SPI.
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
	 * \return reference to SPI_TypeDef object
	 */

	SPI_TypeDef& getSpi() const
	{
		return *spi_;
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

	/// pointer to SPI_TypeDef with registers
	SPI_TypeDef* spi_;

	/// peripheral clock frequency, Hz
	uint32_t peripheralFrequency_;

	/// pointer to bitband alias of SPE bit in SPI_CR1 register
	volatile unsigned long* speBb_;

	/// pointer to bitband alias of ERRIE bit in SPI_CR2 register
	volatile unsigned long* errieBb_;

	/// pointer to bitband alias of RXNEIE bit in SPI_CR2 register
	volatile unsigned long* rxneieBb_;

	/// pointer to bitband alias of TXEIE bit in SPI_CR2 register
	volatile unsigned long* txeieBb_;

	/// pointer to bitband alias of appropriate SPIxEN bit in RCC register
	volatile unsigned long* rccEnBb_;

	/// pointer to bitband alias of appropriate SPIxRST bit in RCC register
	volatile unsigned long* rccRstBb_;

	/// NVIC's IRQ number of associated SPI
	IRQn_Type irqNumber_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| public static objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_HAS_SPI1

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi1parameters {SPI1, &SPI1_CR1_SPE_bb,
		&SPI1_CR2_ERRIE_bb, &SPI1_CR2_RXNEIE_bb, &SPI1_CR2_TXEIE_bb, &RCC_APB2ENR_SPI1EN_bb, &RCC_APB2RSTR_SPI1RST_bb,
		SPI1_IRQn};

#endif	// def CONFIG_CHIP_HAS_SPI1

#ifdef CONFIG_CHIP_HAS_SPI2

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi2parameters {SPI2, &SPI2_CR1_SPE_bb,
		&SPI2_CR2_ERRIE_bb, &SPI2_CR2_RXNEIE_bb, &SPI2_CR2_TXEIE_bb, &RCC_APB1ENR_SPI2EN_bb, &RCC_APB1RSTR_SPI2RST_bb,
		SPI2_IRQn};

#endif	// def CONFIG_CHIP_HAS_SPI2

#ifdef CONFIG_CHIP_HAS_SPI3

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi3parameters {SPI3, &SPI3_CR1_SPE_bb,
		&SPI3_CR2_ERRIE_bb, &SPI3_CR2_RXNEIE_bb, &SPI3_CR2_TXEIE_bb, &RCC_APB1ENR_SPI3EN_bb, &RCC_APB1RSTR_SPI3RST_bb,
		SPI3_IRQn};

#endif	// def CONFIG_CHIP_HAS_SPI3

#ifdef CONFIG_CHIP_HAS_SPI4

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi4parameters {SPI4, &SPI4_CR1_SPE_bb,
		&SPI4_CR2_ERRIE_bb, &SPI4_CR2_RXNEIE_bb, &SPI4_CR2_TXEIE_bb, &RCC_APB2ENR_SPI4EN_bb, &RCC_APB2RSTR_SPI4RST_bb,
		SPI4_IRQn};

#endif	// def CONFIG_CHIP_HAS_SPI4

#ifdef CONFIG_CHIP_HAS_SPI5

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi5parameters {SPI5, &SPI5_CR1_SPE_bb,
		&SPI5_CR2_ERRIE_bb, &SPI5_CR2_RXNEIE_bb, &SPI5_CR2_TXEIE_bb, &RCC_APB2ENR_SPI5EN_bb, &RCC_APB2RSTR_SPI5RST_bb,
		SPI5_IRQn};

#endif	// def CONFIG_CHIP_HAS_SPI5

#ifdef CONFIG_CHIP_HAS_SPI6

const ChipSpiMasterLowLevel::Parameters ChipSpiMasterLowLevel::spi6parameters {SPI6, &SPI6_CR1_SPE_bb,
		&SPI6_CR2_ERRIE_bb, &SPI6_CR2_RXNEIE_bb, &SPI6_CR2_TXEIE_bb, &RCC_APB2ENR_SPI6EN_bb, &RCC_APB2RSTR_SPI6RST_bb,
		SPI6_IRQn};

#endif	// def CONFIG_CHIP_HAS_SPI6

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
