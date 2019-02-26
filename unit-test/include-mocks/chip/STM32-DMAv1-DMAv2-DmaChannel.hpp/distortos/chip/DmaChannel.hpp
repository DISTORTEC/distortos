/**
 * \file
 * \brief Mock of DmaChannel class for DMAv1 & DMAv2 in STM32
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_DMAV1_DMAV2_DMACHANNEL_HPP_DISTORTOS_CHIP_DMACHANNEL_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_DMAV1_DMAV2_DMACHANNEL_HPP_DISTORTOS_CHIP_DMACHANNEL_HPP_

#include "unit-test-common.hpp"

#include "distortos/chip/DmaChannelFunctor.hpp"

#include "estd/EnumClassFlags.hpp"

namespace distortos
{

namespace chip
{

enum class DmaChannelFlags : uint32_t
{
	transferCompleteInterruptDisable = 0 << 4,
	transferCompleteInterruptEnable = 1 << 4,

#ifdef DISTORTOS_CHIP_STM32_DMAV2

	dmaFlowController = 0 << 5,
	peripheralFlowController = 1 << 5,

#endif	// def DISTORTOS_CHIP_STM32_DMAV2

	peripheralToMemory = 0 << 6,
	memoryToPeripheral = 1 << 6,

	peripheralFixed = 0 << 9,
	peripheralIncrement = 1 << 9,

	memoryFixed = 0 << 10,
	memoryIncrement = 1 << 10,

	peripheralDataSize1 = 0 << 11,
	peripheralDataSize2 = 1 << 11,
	peripheralDataSize4 = 2 << 11,

	memoryDataSize1 = 0 << 13,
	memoryDataSize2 = 1 << 13,
	memoryDataSize4 = 2 << 13,

	lowPriority = 0 << 16,
	mediumPriority = 1 << 16,
	highPriority = 2 << 16,
	veryHighPriority = 3 << 16,

#ifdef DISTORTOS_CHIP_STM32_DMAV2

	peripheralBurstSize1 = 0 << 21,
	peripheralBurstSize4 = 1 << 21,
	peripheralBurstSize8 = 2 << 21,
	peripheralBurstSize16 = 3 << 21,

	memoryBurstSize1 = 0 << 23,
	memoryBurstSize4 = 1 << 23,
	memoryBurstSize8 = 2 << 23,
	memoryBurstSize16 = 3 << 23,

#endif	// def DISTORTOS_CHIP_STM32_DMAV2

	dataSize1 = peripheralDataSize1 | memoryDataSize1,
	dataSize2 = peripheralDataSize2 | memoryDataSize2,
	dataSize4 = peripheralDataSize4 | memoryDataSize4,

#ifdef DISTORTOS_CHIP_STM32_DMAV2

	burstSize1 = peripheralBurstSize1 | memoryBurstSize1,
	burstSize4 = peripheralBurstSize4 | memoryBurstSize4,
	burstSize8 = peripheralBurstSize8 | memoryBurstSize8,
	burstSize16 = peripheralBurstSize16 | memoryBurstSize16,

#endif	// def DISTORTOS_CHIP_STM32_DMAV2
};

}	// namespace chip

}	// namespace distortos

/// \brief Enable bitwise operators for distortos::chip::DmaChannelFlags
template<>
struct estd::isEnumClassFlags<distortos::chip::DmaChannelFlags> : std::true_type
{

};

namespace distortos
{

namespace chip
{

class DmaChannel
{
public:

	using Flags = DmaChannelFlags;

	class UniqueHandle
	{
	public:

		constexpr UniqueHandle() :
				channel_{}
		{

		}

		~UniqueHandle()
		{
			assert(channel_ == nullptr);
		}

		size_t getTransactionsLeft() const
		{
			assert(channel_ != nullptr);
			return channel_->getTransactionsLeft();
		}

		void release()
		{
			if (channel_ == nullptr)
				return;

			channel_->release();
			channel_ = {};
		}

		int reserve(DmaChannel& channel, const uint8_t request, DmaChannelFunctor& functor)
		{
			assert(channel_ == nullptr);

			const auto ret = channel.reserve(request, functor);
			if (ret != 0)
				return ret;

			channel_ = &channel;
			return {};
		}

		void startTransfer(const uintptr_t memoryAddress, const uintptr_t peripheralAddress, const size_t transactions,
				const Flags flags) const
		{
			assert(channel_ != nullptr);
			channel_->startTransfer(memoryAddress, peripheralAddress, transactions, flags);
		}

		void stopTransfer() const
		{
			assert(channel_ != nullptr);
			channel_->stopTransfer();
		}

		UniqueHandle(const UniqueHandle&) = delete;
		UniqueHandle(UniqueHandle&&) = delete;
		const UniqueHandle& operator=(const UniqueHandle&) = delete;
		UniqueHandle& operator=(UniqueHandle&&) = delete;

	private:

		DmaChannel* channel_;
	};

	MAKE_CONST_MOCK0(getTransactionsLeft, size_t());
	MAKE_MOCK0(release, void());
	MAKE_MOCK2(reserve, int(uint8_t, DmaChannelFunctor&));
	MAKE_CONST_MOCK4(startTransfer, void(uintptr_t, uintptr_t, size_t, Flags));
	MAKE_CONST_MOCK0(stopTransfer, void());
};

}	// namespace chip

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_DMAV1_DMAV2_DMACHANNEL_HPP_DISTORTOS_CHIP_DMACHANNEL_HPP_
