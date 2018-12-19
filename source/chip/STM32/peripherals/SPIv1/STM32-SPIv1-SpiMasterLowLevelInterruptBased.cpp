/**
 * \file
 * \brief ChipSpiMasterLowLevel class implementation for SPIv1 in STM32
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/SpiMasterLowLevelInterruptBased.hpp"

#include "distortos/chip/STM32-SPIv1-SpiPeripheral.hpp"

#include "distortos/devices/communication/SpiMasterBase.hpp"

#include "distortos/assert.h"

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
 * \brief Gets current word length of SPI peripheral.
 *
 * \param [in] cr1 is the current value of CR1 register in SPI module
 *
 * \return current word length, bits, {8, 16}
 */

constexpr uint8_t getWordLength(const uint16_t cr1)
{
	return (cr1 & SPI_CR1_DFF) == 0 ? 8 : 16;
}

/**
 * \brief Modifies current value of CR1 register.
 *
 * \param [in] cr1 is the current value of CR1 register
 * \param [in] spiPeripheral is a reference to raw SPI peripheral
 * \param [in] clear is the bitmask of bits that should be cleared in CR1 register
 * \param [in] set is the bitmask of bits that should be set in CR1 register
 *
 * \return value written to CR1 register
 */

uint32_t modifyCr1(const uint32_t cr1, const SpiPeripheral& spiPeripheral, const uint32_t clear, const uint32_t set)
{
	const auto newCr1 = (cr1 & ~clear) | set;
	spiPeripheral.writeCr1(newCr1);
	return newCr1;
}

/**
 * \brief Modifies current value of CR2 register.
 *
 * \param [in] cr2 is the current value of CR2 register
 * \param [in] spiPeripheral is a reference to raw SPI peripheral
 * \param [in] clear is the bitmask of bits that should be cleared in CR2 register
 * \param [in] set is the bitmask of bits that should be set in CR2 register
 *
 * \return value written to CR2 register
 */

uint32_t modifyCr2(const uint32_t cr2, const SpiPeripheral& spiPeripheral, const uint32_t clear, const uint32_t set)
{
	const auto newCr2 = (cr2 & ~clear) | set;
	spiPeripheral.writeCr2(newCr2);
	return newCr2;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ChipSpiMasterLowLevel::~ChipSpiMasterLowLevel()
{
	if (isStarted() == false)
		return;

	// reset peripheral
	spiPeripheral_.writeCr1({});
	spiPeripheral_.writeCr2({});
}

std::pair<int, uint32_t> ChipSpiMasterLowLevel::configure(const devices::SpiMode mode, const uint32_t clockFrequency,
		const uint8_t wordLength, const bool lsbFirst, const uint32_t dummyData)
{
	if (wordLength != 8 && wordLength != 16)
		return {EINVAL, {}};

	if (isStarted() == false)
		return {EBADF, {}};

	if (isTransferInProgress() == true)
		return {EBUSY, {}};

	const auto peripheralFrequency = spiPeripheral_.getPeripheralFrequency();
	const auto divider = (peripheralFrequency + clockFrequency - 1) / clockFrequency;
	if (divider > 256)
		return {EINVAL, {}};

	const uint32_t br = divider <= 2 ? 0 : 31 - __builtin_clz(divider - 1);

	auto cr1 = spiPeripheral_.readCr1();
	// value of DFF bit (which determines word length) must be changed only when SPI peripheral is disabled
	const auto disablePeripheral = wordLength != getWordLength(cr1);
	if (disablePeripheral == true)
		cr1 = modifyCr1(cr1, spiPeripheral_, SPI_CR1_SPE, {});	// disable peripheral

	cr1 = modifyCr1(cr1, spiPeripheral_, SPI_CR1_DFF | SPI_CR1_LSBFIRST | SPI_CR1_BR | SPI_CR1_CPOL | SPI_CR1_CPHA,
			(wordLength == 16) << SPI_CR1_DFF_Pos |
			lsbFirst << SPI_CR1_LSBFIRST_Pos |
			SPI_CR1_SPE |
			br << SPI_CR1_BR_Pos |
			(mode == devices::SpiMode::cpol1cpha0 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPOL_Pos |
			(mode == devices::SpiMode::cpol0cpha1 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPHA_Pos);

	dummyData_ = dummyData;

	return {{}, peripheralFrequency / (1 << (br + 1))};
}

void ChipSpiMasterLowLevel::interruptHandler()
{
	const uint16_t word = spiPeripheral_.readDr();
	const auto wordLength = getWordLength(spiPeripheral_.readCr1());
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

	if (readPosition < size_)	// transfer not finished yet?
	{
		writeNextItem(wordLength);
		return;
	}

	// transfer finished
	modifyCr2(spiPeripheral_.readCr2(), spiPeripheral_, SPI_CR2_RXNEIE, {});	// disable RXNE interrupt

	const auto spiMasterBase = spiMasterBase_;

	spiMasterBase_ = {};
	readBuffer_ = {};
	writeBuffer_ = {};
	size_ = {};
	readPosition_ = {};
	writePosition_ = {};

	assert(spiMasterBase != nullptr);
	spiMasterBase->transferCompleteEvent(readPosition);
}

int ChipSpiMasterLowLevel::start()
{
	if (isStarted() == true)
		return EBADF;

	spiPeripheral_.writeCr1(SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR);
	spiPeripheral_.writeCr2({});
	started_ = true;

	return 0;
}

int ChipSpiMasterLowLevel::startTransfer(devices::SpiMasterBase& spiMasterBase, const void* const writeBuffer,
		void* const readBuffer, const size_t size)
{
	if (size == 0)
		return EINVAL;

	if (isStarted() == false)
		return EBADF;

	if (isTransferInProgress() == true)
		return EBUSY;

	const auto wordLength = getWordLength(spiPeripheral_.readCr1());
	if (size % (wordLength / 8) != 0)
		return EINVAL;

	spiMasterBase_ = &spiMasterBase;
	readBuffer_ = static_cast<uint8_t*>(readBuffer);
	writeBuffer_ = static_cast<const uint8_t*>(writeBuffer);
	size_ = size;
	readPosition_ = 0;
	writePosition_ = 0;

	modifyCr2(spiPeripheral_.readCr2(), spiPeripheral_, {}, SPI_CR2_RXNEIE);	// enable RXNE interrupt
	writeNextItem(wordLength);	// write first item to start the transfer

	return 0;
}

int ChipSpiMasterLowLevel::stop()
{
	if (isStarted() == false)
		return EBADF;

	if (isTransferInProgress() == true)
		return EBUSY;

	// reset peripheral
	spiPeripheral_.writeCr1({});
	spiPeripheral_.writeCr2({});
	started_ = false;
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void ChipSpiMasterLowLevel::writeNextItem(const uint8_t wordLength)
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
		word = dummyData_;
	}
	writePosition_ = writePosition;
	spiPeripheral_.writeDr(word);
}

}	// namespace chip

}	// namespace distortos
