/**
 * \file
 * \brief DmaChannel class implementation for DMAv1 in STM32
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/DmaChannel.hpp"

#include "distortos/chip/DmaChannelFunctor.hpp"
#include "distortos/chip/STM32-DMAv1-DmaChannelPeripheral.hpp"
#include "distortos/chip/STM32-DMAv1-DmaPeripheral.hpp"

#include "distortos/InterruptMaskingLock.hpp"

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
 * \brief Gets channel shift for shared DMA registers (ISR, IFCR and CSELR).
 *
 * \param [in] channelId is a DMA channel identifier
 *
 * \return channel shift
 */

uint8_t getChannelShift(const uint8_t channelId)
{
	return (channelId - 1) * 4;
}

/**
 * \brief Modifies current value of CCR register.
 *
 * \param [in] ccr is the current value of CCR register
 * \param [in] dmaChannelPeripheral is a reference to raw DMA channel peripheral
 * \param [in] clear is the bitmask of bits that should be cleared in CCR register
 * \param [in] set is the bitmask of bits that should be set in CCR register
 *
 * \return value written to CCR register
 */

uint32_t modifyCcr(const uint32_t ccr, const DmaChannelPeripheral& dmaChannelPeripheral, const uint32_t clear,
		const uint32_t set)
{
	const auto newCcr = (ccr & ~clear) | set;
	dmaChannelPeripheral.writeCcr(newCcr);
	return newCcr;
}

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

/**
 * \brief Modifies current value of CSELR register.
 *
 * \param [in] cselr is the current value of CSELR register
 * \param [in] dmaPeripheral is a reference to raw DMA peripheral
 * \param [in] clear is the bitmask of bits that should be cleared in CSELR register
 * \param [in] set is the bitmask of bits that should be set in CSELR register
 *
 * \return value written to CSELR register
 */

uint32_t modifyCselr(const uint32_t cselr, const DmaPeripheral& dmaPeripheral, const uint32_t clear, const uint32_t set)
{
	const auto newCselr = (cselr & ~clear) | set;
	dmaPeripheral.writeCselr(newCselr);
	return newCselr;
}

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// maximum allowed value for request identifier
constexpr uint8_t maxRequest {(1 << DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS) - 1};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void DmaChannel::interruptHandler()
{
	const auto channelShift = getChannelShift(dmaChannelPeripheral_.getChannelId());
	const auto teFlag = DMA_ISR_TEIF1 << channelShift;
	const auto tcFlag = DMA_ISR_TCIF1 << channelShift;
	const auto flags = dmaPeripheral_.readIsr() & (teFlag | tcFlag);
	if (flags == 0)
		return;

	const auto enabledFlags = flags & (dmaChannelPeripheral_.readCcr() << channelShift);
	if (enabledFlags == 0)
		return;

	dmaPeripheral_.writeIfcr(enabledFlags);

	if ((enabledFlags & teFlag) != 0)
		functor_->transferError(dmaChannelPeripheral_.readCndtr());
	if ((enabledFlags & tcFlag) != 0)
		functor_->transferComplete();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

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
		return EBADF;

	functor_ = &functor;

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

	const auto channelShift = getChannelShift(dmaChannelPeripheral_.getChannelId());
	modifyCselr(dmaPeripheral_.readCselr(), dmaPeripheral_, maxRequest << channelShift, request << channelShift);

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

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

	if ((dmaChannelPeripheral_.readCcr() & DMA_CCR_EN) != 0)
		return EBUSY;

	dmaChannelPeripheral_.writeCcr(memoryDataSize / 2 << DMA_CCR_MSIZE_Pos |
			peripheralDataSize / 2 << DMA_CCR_PSIZE_Pos |
			memoryIncrement << DMA_CCR_MINC_Pos |
			peripheralIncrement << DMA_CCR_PINC_Pos |
			memoryToPeripheral << DMA_CCR_DIR_Pos |
			DMA_CCR_TEIE |
			DMA_CCR_TCIE);
	dmaChannelPeripheral_.writeCndtr(transactions);
	dmaChannelPeripheral_.writeCpar(peripheralAddress);
	dmaChannelPeripheral_.writeCmar(memoryAddress);
	return {};
}

int DmaChannel::startTransfer() const
{
	const auto ccr = dmaChannelPeripheral_.readCcr();
	if ((ccr & DMA_CCR_EN) != 0)
		return EBUSY;

	modifyCcr(ccr, dmaChannelPeripheral_, {}, DMA_CCR_EN);
	return {};
}

void DmaChannel::stopTransfer() const
{
	modifyCcr(dmaChannelPeripheral_.readCcr(), dmaChannelPeripheral_,
			DMA_CCR_TEIE | DMA_CCR_HTIE | DMA_CCR_TCIE | DMA_CCR_EN, {});
	dmaPeripheral_.writeIfcr(DMA_IFCR_CGIF1 << getChannelShift(dmaChannelPeripheral_.getChannelId()));
}

}	// namespace chip

}	// namespace distortos
