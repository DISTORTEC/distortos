/**
 * \file
 * \brief SdMmcCardLowLevel class implementation for SDMMCv1 in STM32
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/SdMmcCardLowLevel.hpp"

#include "distortos/chip/STM32-SDMMCv1-SdmmcPeripheral.hpp"

#include "distortos/devices/memory/SdMmcCardBase.hpp"

#include "estd/log2u.hpp"

#include <cassert>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SdMmcCardLowLevel::~SdMmcCardLowLevel()
{
	assert(isStarted() == false);
}

void SdMmcCardLowLevel::configure(const BusMode busMode, const uint32_t clockFrequency)
{
	assert(isStarted() == true);
	assert(isTransactionInProgress() == false);

	const auto adapterFrequency = sdmmcPeripheral_.getAdapterFrequency();
	const auto divider = (adapterFrequency + clockFrequency - 1) / clockFrequency;
	assert(divider <= (SDMMC_CLKCR_CLKDIV >> SDMMC_CLKCR_CLKDIV_Pos) + 2);

	const auto widbus = busMode == BusMode::_1Bit ? 0 :
			busMode == BusMode::_4Bit ? SDMMC_CLKCR_WIDBUS_0 : SDMMC_CLKCR_WIDBUS_1;
	const auto bypass = (divider <= 1) << SDMMC_CLKCR_BYPASS_Pos;
	const auto clkdiv = (bypass == false ? divider - 2 : 0) << SDMMC_CLKCR_CLKDIV_Pos;

	const auto clkcr = sdmmcPeripheral_.readClkcr();
	sdmmcPeripheral_.writeClkcr((clkcr & ~(SDMMC_CLKCR_WIDBUS | SDMMC_CLKCR_BYPASS | SDMMC_CLKCR_CLKDIV)) |
			widbus |
			bypass |
			clkdiv);
	clockFrequency_ = bypass == false ? adapterFrequency / divider : adapterFrequency;
}

void SdMmcCardLowLevel::interruptHandler()
{
	assert(isStarted() == true);
	assert(isTransactionInProgress() == true);

	const auto sta = sdmmcPeripheral_.readSta();
	sdmmcPeripheral_.writeIcr(SDMMC_ICR_SDIOITC | SDMMC_ICR_DBCKENDC | SDMMC_ICR_DATAENDC | SDMMC_ICR_CMDSENTC |
			SDMMC_ICR_CMDRENDC | SDMMC_ICR_RXOVERRC | SDMMC_ICR_TXUNDERRC | SDMMC_ICR_DTIMEOUTC | SDMMC_ICR_CTIMEOUTC |
			SDMMC_ICR_DCRCFAILC | SDMMC_ICR_CCRCFAILC);

	const auto dctrl = sdmmcPeripheral_.readDctrl();
	const auto receiveOverrun = (sta & SDMMC_STA_RXOVERR) != 0 ||
			(dmaError_ == true && (dctrl & SDMMC_DCTRL_DTDIR) != 0);
	const auto transmitUnderrun = (sta & SDMMC_STA_TXUNDERR) != 0 ||
			(dmaError_ == true && (dctrl & SDMMC_DCTRL_DTDIR) == 0);
	const auto result = receiveOverrun == true ? devices::SdMmcCardBase::Result::receiveOverrun :
			transmitUnderrun == true ? devices::SdMmcCardBase::Result::transmitUnderrun :
			(sta & SDMMC_STA_DCRCFAIL) != 0 ? devices::SdMmcCardBase::Result::dataCrcMismatch :
			(sta & SDMMC_STA_DTIMEOUT) != 0 ? devices::SdMmcCardBase::Result::dataTimeout :
			(sta & SDMMC_STA_CCRCFAIL) != 0 ? devices::SdMmcCardBase::Result::responseCrcMismatch :
			(sta & SDMMC_STA_CTIMEOUT) != 0 ? devices::SdMmcCardBase::Result::responseTimeout :
			devices::SdMmcCardBase::Result::success;

	const auto writeTransferPending = dctrl != 0 && (dctrl & SDMMC_DCTRL_DTEN) == 0;

	// transfer finished? write transfer pending but transaction failed?
	if (dctrl != 0 && (writeTransferPending == false || result != devices::SdMmcCardBase::Result::success))
	{
		sdmmcPeripheral_.writeDctrl({});
		dmaChannelHandle_.stopTransfer();
	}

	// write transfer pending and transaction successful so far?
	if (writeTransferPending == true && result == devices::SdMmcCardBase::Result::success)
	{
		sdmmcPeripheral_.writeDctrl(dctrl | SDMMC_DCTRL_DTEN);
		sdmmcPeripheral_.writeMask(SDMMC_MASK_DATAENDIE | SDMMC_MASK_TXUNDERRIE | SDMMC_MASK_DTIMEOUTIE |
				SDMMC_MASK_DCRCFAILIE);
		return;
	}

	sdmmcPeripheral_.writeMask({});

	if (response_.size() != 0)
	{
		if (response_.size() == 1)
			response_[0] = sdmmcPeripheral_.readResp1();
		else	// if (response_.size() == 4)
		{
			response_[0] = sdmmcPeripheral_.readResp4();
			response_[1] = sdmmcPeripheral_.readResp3();
			response_[2] = sdmmcPeripheral_.readResp2();
			response_[3] = sdmmcPeripheral_.readResp1();
		}

		response_ = {};
	}

	const auto sdMmcCardBase = sdMmcCardBase_;
	sdMmcCardBase_ = {};
	sdMmcCardBase->transactionCompleteEvent(result);
}

int SdMmcCardLowLevel::start()
{
	assert(isStarted() == false);

	{
		const auto ret = dmaChannelHandle_.reserve(dmaChannel_, dmaRequest_, dmaChannelFunctor_);
		if (ret != 0)
			return ret;
	}

	clockFrequency_ = sdmmcPeripheral_.getAdapterFrequency() / ((SDMMC_CLKCR_CLKDIV >> SDMMC_CLKCR_CLKDIV_Pos) + 2);
	sdmmcPeripheral_.writeClkcr(SDMMC_CLKCR_CLKEN | SDMMC_CLKCR_CLKDIV);
	sdmmcPeripheral_.writePower(SDMMC_POWER_PWRCTRL);

	return {};
}

void SdMmcCardLowLevel::startTransaction(devices::SdMmcCardBase& sdMmcCardBase, const uint8_t command,
		const uint32_t argument, const Response response, const Transfer transfer)
{
	assert(isStarted() == true);
	assert(isTransactionInProgress() == false);
	assert(command <= maxCommand);

	if (transfer.getSize() != 0)
	{
		assert(response.size() != 0);

		assert((transfer.isWriteTransfer() == false ? transfer.getReadBuffer() : transfer.getWriteBuffer()) != nullptr);

		const auto dblocksize = estd::log2u(transfer.getBlockSize());
		assert(transfer.getBlockSize() >= 4);
		assert(transfer.getBlockSize() == 1u << dblocksize);
		assert(dblocksize <= 14);

		const auto dlen = transfer.getSize();
		assert(transfer.getSize() % transfer.getBlockSize() == 0);
		assert(dlen <= SDMMC_DLEN_DATALENGTH >> SDMMC_DLEN_DATALENGTH_Pos);

		const auto dtimer = static_cast<uint64_t>((clockFrequency_ + 1000 - 1) / 1000) * transfer.getTimeoutMs();
		assert(dtimer <= SDMMC_DTIMER_DATATIME >> SDMMC_DTIMER_DATATIME_Pos);

		dmaError_ = {};

		{
			const auto memoryAddress = reinterpret_cast<uintptr_t>(transfer.isWriteTransfer() == false ?
					transfer.getReadBuffer() : transfer.getWriteBuffer());
			const auto directionFlags = transfer.isWriteTransfer() == false ? DmaChannel::Flags::peripheralToMemory :
					DmaChannel::Flags::memoryToPeripheral;
			dmaChannelHandle_.startTransfer(memoryAddress,
					sdmmcPeripheral_.getFifoAddress(), transfer.getSize() / 4,
					DmaChannel::Flags::transferCompleteInterruptDisable |
					DmaChannel::Flags::peripheralFlowController |
					directionFlags |
					DmaChannel::Flags::peripheralFixed |
					DmaChannel::Flags::memoryIncrement |
					DmaChannel::Flags::veryHighPriority |
					DmaChannel::Flags::dataSize4 |
					DmaChannel::Flags::burstSize4);
		}

		sdmmcPeripheral_.writeDtimer(dtimer);
		sdmmcPeripheral_.writeDlen(dlen);
		sdmmcPeripheral_.writeDctrl(dblocksize << SDMMC_DCTRL_DBLOCKSIZE_Pos |
				SDMMC_DCTRL_DMAEN |
				(transfer.isWriteTransfer() == false) << SDMMC_DCTRL_DTDIR_Pos |
				(transfer.isWriteTransfer() == false) << SDMMC_DCTRL_DTEN_Pos);
	}

	sdmmcPeripheral_.writeArg(argument);
	const uint32_t waitresp = response.size() == 0 ? 0 :
			response.size() == 1 ? SDMMC_CMD_WAITRESP_0 :
			SDMMC_CMD_WAITRESP;
	sdmmcPeripheral_.writeCmd(SDMMC_CMD_CPSMEN | waitresp | command << SDMMC_CMD_CMDINDEX_Pos);

	constexpr uint32_t noResponseMask {SDMMC_MASK_CMDSENTIE};
	constexpr uint32_t noTransferMask {SDMMC_MASK_CMDRENDIE | SDMMC_MASK_CTIMEOUTIE | SDMMC_MASK_CCRCFAILIE};
	constexpr uint32_t readTransferMask {SDMMC_MASK_DATAENDIE | SDMMC_MASK_RXOVERRIE | SDMMC_MASK_DTIMEOUTIE |
			SDMMC_MASK_CTIMEOUTIE | SDMMC_MASK_DCRCFAILIE | SDMMC_MASK_CCRCFAILIE};
	const auto mask = transfer.getSize() != 0 && transfer.isWriteTransfer() == false ? readTransferMask :
			response.size() != 0 ? noTransferMask :
			noResponseMask;

	sdMmcCardBase_ = &sdMmcCardBase;
	response_ = response;
	sdmmcPeripheral_.writeMask(mask);
}

void SdMmcCardLowLevel::stop()
{
	assert(isStarted() == true);
	assert(isTransactionInProgress() == false);

	dmaChannelHandle_.release();

	// reset peripheral
	sdmmcPeripheral_.writeMask({});
	sdmmcPeripheral_.writeDctrl({});
	sdmmcPeripheral_.writeCmd({});
	sdmmcPeripheral_.writeClkcr({});
	sdmmcPeripheral_.writePower({});

	clockFrequency_ = {};
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SdMmcCardLowLevel::transferErrorEventHandler(size_t)
{
	dmaError_ = true;
}

/*---------------------------------------------------------------------------------------------------------------------+
| SdMmcCardLowLevel::DmaChannelFunctor public functions
+---------------------------------------------------------------------------------------------------------------------*/

void SdMmcCardLowLevel::DmaChannelFunctor::transferErrorEvent(const size_t transactionsLeft)
{
	owner_.transferErrorEventHandler(transactionsLeft);
}

}	// namespace chip

}	// namespace distortos
