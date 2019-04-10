/**
 * \file
 * \brief SpiMasterLowLevelDmaBased class implementation for SPIv2 in STM32
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/SpiMasterLowLevelDmaBased.hpp"

#include "distortos/chip/STM32-SPIv2.hpp"
#include "distortos/chip/STM32-SPIv2-SpiPeripheral.hpp"

#include "distortos/devices/communication/SpiMasterBase.hpp"

#include "distortos/assert.h"

#include "estd/ScopeGuard.hpp"

#include <cerrno>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiMasterLowLevelDmaBased::~SpiMasterLowLevelDmaBased()
{
	assert(isStarted() == false);
}

uint32_t SpiMasterLowLevelDmaBased::configure(const devices::SpiMode mode, const uint32_t clockFrequency,
		const uint8_t wordLength, const bool lsbFirst, const uint32_t dummyData)
{
	assert(isStarted() == true);
	assert(isTransferInProgress() == false);

	const auto realClockFrequency = configureSpi(spiPeripheral_, mode, clockFrequency, wordLength, lsbFirst);
	txDummyData_ = dummyData;
	wordLength_ = wordLength;
	return realClockFrequency;
}

int SpiMasterLowLevelDmaBased::start()
{
	assert(isStarted() == false);

	{
		const auto ret = rxDmaChannelHandle_.reserve(rxDmaChannel_, rxDmaRequest_, rxDmaChannelFunctor_);
		if (ret != 0)
			return ret;
	}

	auto rxDmaChannelHandleScopeGuard = estd::makeScopeGuard([this]()
			{
				rxDmaChannelHandle_.release();
			});

	{
		const auto ret = txDmaChannelHandle_.reserve(txDmaChannel_, txDmaRequest_, txDmaChannelFunctor_);
		if (ret != 0)
			return ret;
	}

	rxDmaChannelHandleScopeGuard.release();

	wordLength_ = 8;
	spiPeripheral_.writeCr1(SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR);
	spiPeripheral_.writeCr2(SPI_CR2_FRXTH | (wordLength_ - 1) << SPI_CR2_DS_Pos | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
	started_ = true;

	return {};
}

int SpiMasterLowLevelDmaBased::startTransfer(devices::SpiMasterBase& spiMasterBase, const void* const writeBuffer,
		void* const readBuffer, const size_t size)
{
	assert(isStarted() == true);

	if (size == 0)
		return EINVAL;

	if (isTransferInProgress() == true)
		return EBUSY;

	const auto dataSize = (wordLength_ + 8 - 1) / 8;
	if (size % dataSize != 0)
		return EINVAL;

	spiMasterBase_ = &spiMasterBase;
	size_ = size;

	const auto transactions = size / dataSize;
	const auto commonDmaFlags = DmaChannel::Flags::peripheralFixed |
			(dataSize == 1 ? DmaChannel::Flags::dataSize1 : DmaChannel::Flags::dataSize2);

	{
		const auto memoryAddress = reinterpret_cast<uintptr_t>(readBuffer != nullptr ? readBuffer : &rxDummyData_);
		const auto rxDmaFlags = DmaChannel::Flags::transferCompleteInterruptEnable |
				DmaChannel::Flags::peripheralToMemory |
				(readBuffer != nullptr ? DmaChannel::Flags::memoryIncrement : DmaChannel::Flags::memoryFixed) |
				DmaChannel::Flags::veryHighPriority;
		rxDmaChannelHandle_.startTransfer(memoryAddress, spiPeripheral_.getDrAddress(), transactions,
				commonDmaFlags | rxDmaFlags);
	}
	{
		const auto memoryAddress = reinterpret_cast<uintptr_t>(writeBuffer != nullptr ? writeBuffer : &txDummyData_);
		const auto txDmaFlags = DmaChannel::Flags::transferCompleteInterruptDisable |
				DmaChannel::Flags::memoryToPeripheral |
				(writeBuffer != nullptr ? DmaChannel::Flags::memoryIncrement : DmaChannel::Flags{}) |
				DmaChannel::Flags::lowPriority;
		txDmaChannelHandle_.startTransfer(memoryAddress, spiPeripheral_.getDrAddress(), transactions,
				commonDmaFlags | txDmaFlags);
	}

	return 0;
}

void SpiMasterLowLevelDmaBased::stop()
{
	assert(isStarted() == true);
	assert(isTransferInProgress() == false);

	rxDmaChannelHandle_.release();
	txDmaChannelHandle_.release();

	// reset peripheral
	spiPeripheral_.writeCr1({});
	spiPeripheral_.writeCr2({});
	started_ = false;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SpiMasterLowLevelDmaBased::eventHandler(const size_t transactionsLeft)
{
	txDmaChannelHandle_.stopTransfer();
	rxDmaChannelHandle_.stopTransfer();

	const auto bytesTransfered = size_ - transactionsLeft * ((wordLength_ + 8 - 1) / 8);
	size_ = {};

	const auto spiMasterBase = spiMasterBase_;
	spiMasterBase_ = {};
	assert(spiMasterBase != nullptr);
	spiMasterBase->transferCompleteEvent(bytesTransfered);
}

/*---------------------------------------------------------------------------------------------------------------------+
| SpiMasterLowLevelDmaBased::RxDmaChannelFunctor public functions
+---------------------------------------------------------------------------------------------------------------------*/

void SpiMasterLowLevelDmaBased::RxDmaChannelFunctor::transferCompleteEvent()
{
	owner_.eventHandler(0);
}

void SpiMasterLowLevelDmaBased::RxDmaChannelFunctor::transferErrorEvent(const size_t transactionsLeft)
{
	owner_.eventHandler(transactionsLeft);
}

/*---------------------------------------------------------------------------------------------------------------------+
| SpiMasterLowLevelDmaBased::TxDmaChannelFunctor public functions
+---------------------------------------------------------------------------------------------------------------------*/

void SpiMasterLowLevelDmaBased::TxDmaChannelFunctor::transferErrorEvent(const size_t transactionsLeft)
{
	owner_.eventHandler(transactionsLeft);
}

}	// namespace chip

}	// namespace distortos
