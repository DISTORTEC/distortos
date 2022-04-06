/**
 * \file
 * \brief SpiMasterLowLevelDmaBased class implementation for SPIv1 in STM32
 *
 * \author Copyright (C) 2018-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/SpiMasterLowLevelDmaBased.hpp"

#include "distortos/chip/STM32-SPIv1.hpp"
#include "distortos/chip/STM32-SPIv1-SpiPeripheral.hpp"

#include "distortos/devices/communication/SpiMasterBase.hpp"

#include "estd/ScopeGuard.hpp"

#include <cassert>

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

void SpiMasterLowLevelDmaBased::configure(const devices::SpiMode mode, const uint32_t clockFrequency,
		const uint8_t wordLength, const bool lsbFirst, const uint32_t dummyData)
{
	assert(isStarted() == true);
	assert(isTransferInProgress() == false);

	txDummyData_ = dummyData;
	wordLength_ = wordLength;
	configureSpi(spiPeripheral_, mode, clockFrequency, wordLength, lsbFirst);
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
	spiPeripheral_.writeCr2(SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
	started_ = true;

	return {};
}

void SpiMasterLowLevelDmaBased::startTransfer(devices::SpiMasterBase& spiMasterBase, const void* const writeBuffer,
		void* const readBuffer, const size_t size)
{
	assert(isStarted() == true);
	assert(isTransferInProgress() == false);
	const auto dataSize = wordLength_ / 8;
	assert(size != 0 && size % dataSize == 0);

	spiMasterBase_ = &spiMasterBase;

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

void SpiMasterLowLevelDmaBased::eventHandler(const bool success)
{
	txDmaChannelHandle_.stopTransfer();
	rxDmaChannelHandle_.stopTransfer();

	const auto spiMasterBase = spiMasterBase_;
	spiMasterBase_ = {};
	assert(spiMasterBase != nullptr);
	spiMasterBase->transferCompleteEvent(success);
}

/*---------------------------------------------------------------------------------------------------------------------+
| SpiMasterLowLevelDmaBased::RxDmaChannelFunctor public functions
+---------------------------------------------------------------------------------------------------------------------*/

void SpiMasterLowLevelDmaBased::RxDmaChannelFunctor::transferCompleteEvent()
{
	owner_.eventHandler(true);
}

void SpiMasterLowLevelDmaBased::RxDmaChannelFunctor::transferErrorEvent(size_t)
{
	owner_.eventHandler(false);
}

/*---------------------------------------------------------------------------------------------------------------------+
| SpiMasterLowLevelDmaBased::TxDmaChannelFunctor public functions
+---------------------------------------------------------------------------------------------------------------------*/

void SpiMasterLowLevelDmaBased::TxDmaChannelFunctor::transferErrorEvent(size_t)
{
	owner_.eventHandler(false);
}

}	// namespace chip

}	// namespace distortos
