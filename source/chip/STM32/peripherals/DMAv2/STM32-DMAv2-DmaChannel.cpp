/**
 * \file
 * \brief DmaChannel class implementation for DMAv2 in STM32
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/DmaChannel.hpp"

#include "distortos/chip/DmaChannelFunctor.hpp"
#include "distortos/chip/STM32-DMAv2-DmaChannelPeripheral.hpp"
#include "distortos/chip/STM32-DMAv2-DmaPeripheral.hpp"

#include "distortos/InterruptMaskingLock.hpp"

#include <array>

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
 * \brief Gets channel shift for shared DMA registers (LISR, LISR, LIFCR and HIFCR).
 *
 * \param [in] channelId is a DMA channel identifier
 *
 * \return channel shift
 */

uint8_t getChannelShift(const uint8_t channelId)
{
	static const std::array<uint8_t, 4> channelShifts
	{{
			0,
			6,
			16,
			22,
	}};
	return channelShifts[channelId % channelShifts.size()];
}

/**
 * \brief Modifies current value of CR register.
 *
 * \param [in] cr is the current value of CR register
 * \param [in] dmaChannelPeripheral is a reference to raw DMA channel peripheral
 * \param [in] clear is the bitmask of bits that should be cleared in CR register
 * \param [in] set is the bitmask of bits that should be set in CR register
 *
 * \return value written to CR register
 */

uint32_t modifyCr(const uint32_t cr, const DmaChannelPeripheral& dmaChannelPeripheral, const uint32_t clear,
		const uint32_t set)
{
	const auto newCr = (cr & ~clear) | set;
	dmaChannelPeripheral.writeCr(newCr);
	return newCr;
}

/**
 * \brief Reads current value of LISR or HISR register.
 *
 * \param [in] dmaPeripheral is a reference to raw DMA peripheral
 * \param [in] channelId is a DMA channel identifier
 *
 * \return current value of LISR or HISR register
 */

uint32_t readIsr(const DmaPeripheral& dmaPeripheral, const uint8_t channelId)
{
	if (channelId <= 3)
		return dmaPeripheral.readLisr();
	else
		return dmaPeripheral.readHisr();
}

/**
 * \brief Writes value to LIFCR or HIFCR register.
 *
 * \param [in] dmaPeripheral is a reference to raw DMA peripheral
 * \param [in] channelId is a DMA channel identifier
 * \param [in] ifcr is the value that will be written to LIFCR or HIFCR register
 */

void writeIfcr(const DmaPeripheral& dmaPeripheral, const uint8_t channelId, const uint32_t ifcr)
{
	if (channelId <= 3)
		dmaPeripheral.writeLifcr(ifcr);
	else
		dmaPeripheral.writeHifcr(ifcr);
}

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// maximum allowed value for request identifier
constexpr uint8_t maxRequest {DMA_SxCR_CHSEL_Msk >> DMA_SxCR_CHSEL_Pos};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void DmaChannel::interruptHandler()
{
	const auto channelId = dmaChannelPeripheral_.getChannelId();
	const auto channelShift = getChannelShift(channelId);
	const auto tcFlag = DMA_LISR_TCIF0 << channelShift;
	const auto teFlag = DMA_LISR_TEIF0 << channelShift;
	const auto flags = readIsr(dmaPeripheral_, channelId) & (tcFlag | teFlag);
	if (flags == 0)
		return;

	const auto enabledFlags = flags & (dmaChannelPeripheral_.readCr() << (channelShift + 1));
	if (enabledFlags == 0)
		return;

	writeIfcr(dmaPeripheral_, channelId, enabledFlags);

	if ((enabledFlags & tcFlag) != 0)
		functor_->transferCompleteEvent();
	if ((enabledFlags & teFlag) != 0)
		functor_->transferErrorEvent(dmaChannelPeripheral_.readNdtr());
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

size_t DmaChannel::getTransactionsLeft() const
{
	return dmaChannelPeripheral_.readNdtr();
}

void DmaChannel::release()
{
	stopTransfer();
	functor_ = {};
}

int DmaChannel::reserve(const uint8_t request, DmaChannelFunctor& functor)
{
	if (request > maxRequest)
		return EINVAL;

	const InterruptMaskingLock interruptMaskingLock;

	if (functor_ != nullptr)
		return EBUSY;

	functor_ = &functor;
	request_ = request;

	return {};
}

int DmaChannel::configureTransfer(const uintptr_t memoryAddress, const size_t memoryDataSize,
		const bool memoryIncrement, const uintptr_t peripheralAddress, const size_t peripheralDataSize,
		const bool peripheralIncrement, const size_t transactions, const bool memoryToPeripheral) const
{
	if ((memoryDataSize != 1 && memoryDataSize != 2 && memoryDataSize != 4) ||
			(peripheralDataSize != 1 && peripheralDataSize != 2 && peripheralDataSize != 4))
		return EINVAL;

	if (memoryAddress % memoryDataSize != 0 || peripheralAddress % peripheralDataSize != 0)
		return EINVAL;

	if (transactions == 0)
		return EINVAL;

	if (transactions > UINT16_MAX)	/// \todo add support for very high number of transactions
		return ENOTSUP;

	if ((dmaChannelPeripheral_.readCr() & DMA_SxCR_EN) != 0)
		return EBUSY;

	dmaChannelPeripheral_.writeCr(request_ << DMA_SxCR_CHSEL_Pos |
			memoryDataSize / 2 << DMA_SxCR_MSIZE_Pos |
			peripheralDataSize / 2 << DMA_SxCR_PSIZE_Pos |
			memoryIncrement << DMA_SxCR_MINC_Pos |
			peripheralIncrement << DMA_SxCR_PINC_Pos |
			memoryToPeripheral << DMA_SxCR_DIR_Pos |
			DMA_SxCR_TEIE |
			DMA_SxCR_TCIE);
	dmaChannelPeripheral_.writeNdtr(transactions);
	dmaChannelPeripheral_.writePar(peripheralAddress);
	dmaChannelPeripheral_.writeM0ar(memoryAddress);
	dmaChannelPeripheral_.writeFcr(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);
	return {};
}

int DmaChannel::startTransfer() const
{
	const auto cr = dmaChannelPeripheral_.readCr();
	if ((cr & DMA_SxCR_EN) != 0)
		return EBUSY;

	modifyCr(cr, dmaChannelPeripheral_, {}, DMA_SxCR_EN);
	return {};
}

void DmaChannel::stopTransfer() const
{
	modifyCr(dmaChannelPeripheral_.readCr(), dmaChannelPeripheral_,
			DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | DMA_SxCR_EN, {});
	while ((dmaChannelPeripheral_.readCr() & DMA_SxCR_EN) != 0);
	const auto channelId = dmaChannelPeripheral_.getChannelId();
	constexpr uint32_t allFlags {DMA_LIFCR_CTCIF0 | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CDMEIF0 |
			DMA_LIFCR_CFEIF0};
	writeIfcr(dmaPeripheral_, channelId, allFlags << getChannelShift(channelId));
}

}	// namespace chip

}	// namespace distortos
