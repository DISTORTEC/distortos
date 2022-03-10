/**
 * \file
 * \brief DmaChannel class implementation for DMAv2 in STM32
 *
 * \author Copyright (C) 2018-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
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

static_assert(static_cast<uint32_t>(DmaChannel::Flags::transferCompleteInterruptDisable) == 0,
		"DmaChannel::Flags::transferCompleteInterruptDisable doesn't match expected value of DMA_SxCR_TCIE field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::transferCompleteInterruptEnable) == DMA_SxCR_TCIE,
		"DmaChannel::Flags::transferCompleteInterruptEnable doesn't match expected value of DMA_SxCR_TCIE field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::dmaFlowController) == 0,
		"DmaChannel::Flags::dmaFlowController doesn't match expected value of DMA_SxCR_PFCTRL field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralFlowController) == DMA_SxCR_PFCTRL,
		"DmaChannel::Flags::peripheralFlowController doesn't match expected value of DMA_SxCR_PFCTRL field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralToMemory) == 0,
		"DmaChannel::Flags::peripheralToMemory doesn't match expected value of DMA_SxCR_DIR field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryToPeripheral) == DMA_SxCR_DIR_0,
		"DmaChannel::Flags::memoryToPeripheral doesn't match expected value of DMA_SxCR_DIR field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::circularModeDisable) == 0,
		"DmaChannel::Flags::circularModeDisable doesn't match expected value of DMA_SxCR_CIRC field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::circularModeEnable) == DMA_SxCR_CIRC,
		"DmaChannel::Flags::circularModeEnable doesn't match expected value of DMA_SxCR_CIRC field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralFixed) == 0,
		"DmaChannel::Flags::peripheralFixed doesn't match expected value of DMA_SxCR_PINC field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralIncrement) == DMA_SxCR_PINC,
		"DmaChannel::Flags::peripheralIncrement doesn't match expected value of DMA_SxCR_PINC field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryFixed) == 0,
		"DmaChannel::Flags::memoryFixed doesn't match expected value of DMA_SxCR_MINC field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryIncrement) == DMA_SxCR_MINC,
		"DmaChannel::Flags::memoryIncrement doesn't match expected value of DMA_SxCR_MINC field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralDataSize1) == 0,
		"DmaChannel::Flags::peripheralDataSize1 doesn't match expected value of DMA_SxCR_PSIZE field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralDataSize2) == DMA_SxCR_PSIZE_0,
		"DmaChannel::Flags::peripheralDataSize2 doesn't match expected value of DMA_SxCR_PSIZE field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralDataSize4) == DMA_SxCR_PSIZE_1,
		"DmaChannel::Flags::peripheralDataSize4 doesn't match expected value of DMA_SxCR_PSIZE field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryDataSize1) == 0,
		"DmaChannel::Flags::memoryDataSize1 doesn't match expected value of DMA_SxCR_MSIZE field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryDataSize2) == DMA_SxCR_MSIZE_0,
		"DmaChannel::Flags::memoryDataSize2 doesn't match expected value of DMA_SxCR_MSIZE field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryDataSize4) == DMA_SxCR_MSIZE_1,
		"DmaChannel::Flags::memoryDataSize4 doesn't match expected value of DMA_SxCR_MSIZE field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::lowPriority) == 0,
		"DmaChannel::Flags::lowPriority doesn't match expected value of DMA_SxCR_PL field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::mediumPriority) == DMA_SxCR_PL_0,
		"DmaChannel::Flags::mediumPriority doesn't match expected value of DMA_SxCR_PL field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::highPriority) == DMA_SxCR_PL_1,
		"DmaChannel::Flags::highPriority doesn't match expected value of DMA_SxCR_PL field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::veryHighPriority) == DMA_SxCR_PL,
		"DmaChannel::Flags::veryHighPriority doesn't match expected value of DMA_SxCR_PL field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralBurstSize1) == 0,
		"DmaChannel::Flags::peripheralBurstSize1 doesn't match expected value of DMA_SxCR_PBURST field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralBurstSize4) == DMA_SxCR_PBURST_0,
		"DmaChannel::Flags::peripheralBurstSize4 doesn't match expected value of DMA_SxCR_PBURST field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralBurstSize8) == DMA_SxCR_PBURST_1,
		"DmaChannel::Flags::peripheralBurstSize8 doesn't match expected value of DMA_SxCR_PBURST field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::peripheralBurstSize16) == DMA_SxCR_PBURST,
		"DmaChannel::Flags::peripheralBurstSize16 doesn't match expected value of DMA_SxCR_PBURST field!");

static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryBurstSize1) == 0,
		"DmaChannel::Flags::memoryBurstSize1 doesn't match expected value of DMA_SxCR_MBURST field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryBurstSize4) == DMA_SxCR_MBURST_0,
		"DmaChannel::Flags::memoryBurstSize4 doesn't match expected value of DMA_SxCR_MBURST field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryBurstSize8) == DMA_SxCR_MBURST_1,
		"DmaChannel::Flags::memoryBurstSize8 doesn't match expected value of DMA_SxCR_MBURST field!");
static_assert(static_cast<uint32_t>(DmaChannel::Flags::memoryBurstSize16) == DMA_SxCR_MBURST,
		"DmaChannel::Flags::memoryBurstSize16 doesn't match expected value of DMA_SxCR_MBURST field!");

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
/// TCIE, HTIE, TEIE, DMEIE and EN flags of CR register
constexpr uint32_t tcieHtieTeieDmeieEnFlags {DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE |
		DMA_SxCR_EN};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void DmaChannel::interruptHandler()
{
	assert(functor_ != nullptr);

	const auto channelId = dmaChannelPeripheral_.getChannelId();
	const auto channelShift = getChannelShift(channelId);
	const auto tcFlag = DMA_LISR_TCIF0 << channelShift;
	const auto htFlag = DMA_LISR_HTIF0 << channelShift;
	const auto teFlag = DMA_LISR_TEIF0 << channelShift;
	const auto flags = readIsr(dmaPeripheral_, channelId) & (tcFlag | htFlag | teFlag);
	if (flags == 0)
		return;

	const auto enabledFlags = flags & (dmaChannelPeripheral_.readCr() << (channelShift + 1));
	if (enabledFlags == 0)
		return;

	writeIfcr(dmaPeripheral_, channelId, enabledFlags);

	if ((enabledFlags & tcFlag) != 0)
		functor_->transferCompleteEvent();
	if ((enabledFlags & htFlag) != 0)
		functor_->halfTransferEvent();
	if ((enabledFlags & teFlag) != 0)
		functor_->transferErrorEvent(getTransactionsLeft());
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
	assert((dmaChannelPeripheral_.readCr() & tcieHtieTeieDmeieEnFlags) == 0);
	functor_ = {};
}

int DmaChannel::reserve(const uint8_t request, DmaChannelFunctor& functor)
{
	assert(request <= maxRequest);

	const InterruptMaskingLock interruptMaskingLock;

	if (functor_ != nullptr)
		return EBUSY;

	functor_ = &functor;
	request_ = request;

	return {};
}

void DmaChannel::startTransfer(const uintptr_t memoryAddress, const uintptr_t peripheralAddress,
		const size_t transactions, const Flags flags) const
{
	assert(functor_ != nullptr);

	assert((flags & Flags::peripheralFlowController) == Flags::dmaFlowController ||
			(flags & Flags::circularModeEnable) == Flags::circularModeDisable);

	constexpr auto memoryDataSizeMask = Flags::memoryDataSize1 | Flags::memoryDataSize2 | Flags::memoryDataSize4;
	const auto memoryDataSizeFlags = flags & memoryDataSizeMask;
	const auto memoryDataSize = memoryDataSizeFlags == Flags::memoryDataSize1 ? 1 :
			memoryDataSizeFlags == Flags::memoryDataSize2 ? 2 :
			memoryDataSizeFlags == Flags::memoryDataSize4 ? 4 : 0;

	constexpr auto peripheralDataSizeMask =
			Flags::peripheralDataSize1 | Flags::peripheralDataSize2 | Flags::peripheralDataSize4;
	const auto peripheralDataSizeFlags = flags & peripheralDataSizeMask;
	const auto peripheralDataSize = peripheralDataSizeFlags == Flags::peripheralDataSize1 ? 1 :
			peripheralDataSizeFlags == Flags::peripheralDataSize2 ? 2 :
			peripheralDataSizeFlags == Flags::peripheralDataSize4 ? 4 : 0;

	assert(memoryDataSize != 0 && peripheralDataSize != 0);

	constexpr auto memoryBurstSizeMask = Flags::memoryBurstSize1 | Flags::memoryBurstSize4 | Flags::memoryBurstSize8 |
			Flags::memoryBurstSize16;
	const auto memoryBurstSizeFlags = flags & memoryBurstSizeMask;
	const auto memoryBurstSize = memoryBurstSizeFlags == Flags::memoryBurstSize1 ? 1 :
			memoryBurstSizeFlags == Flags::memoryBurstSize4 ? 4 :
			memoryBurstSizeFlags == Flags::memoryBurstSize8 ? 8 : 16;

	assert(memoryDataSize * memoryBurstSize <= 16);

	constexpr auto peripheralBurstSizeMask = Flags::peripheralBurstSize1 | Flags::peripheralBurstSize4 |
			Flags::peripheralBurstSize8 | Flags::peripheralBurstSize16;
	const auto peripheralBurstSizeFlags = flags & peripheralBurstSizeMask;
	const auto peripheralBurstSize = peripheralBurstSizeFlags == Flags::peripheralBurstSize1 ? 1 :
			peripheralBurstSizeFlags == Flags::peripheralBurstSize4 ? 4 :
			peripheralBurstSizeFlags == Flags::peripheralBurstSize8 ? 8 : 16;

	assert(memoryAddress % (memoryDataSize * memoryBurstSize) == 0 &&
			peripheralAddress % (peripheralDataSize * peripheralBurstSize) == 0);

	assert(transactions != 0 && transactions <= UINT16_MAX);
	assert((dmaChannelPeripheral_.readCr() & tcieHtieTeieDmeieEnFlags) == 0);

	dmaChannelPeripheral_.writeNdtr(transactions);
	dmaChannelPeripheral_.writePar(peripheralAddress);
	dmaChannelPeripheral_.writeM0ar(memoryAddress);
	dmaChannelPeripheral_.writeFcr(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);
	dmaChannelPeripheral_.writeCr(request_ << DMA_SxCR_CHSEL_Pos |
			static_cast<uint32_t>(flags) |
			DMA_SxCR_TEIE |
			DMA_SxCR_EN);
}

void DmaChannel::stopTransfer() const
{
	assert(functor_ != nullptr);

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
