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

/// DMA transfer configuration flags
enum class DmaChannelFlags : uint16_t
{
	transferCompleteInterruptDisable = 0 << 0,
	transferCompleteInterruptEnable = 1 << 0,

	peripheralToMemory = 0 << 1,
	memoryToPeripheral = 1 << 1,

	peripheralFixed = 0 << 2,
	peripheralIncrement = 1 << 2,

	memoryFixed = 0 << 3,
	memoryIncrement = 1 << 3,

	peripheralDataSize1 = 0 << 4,
	peripheralDataSize2 = 1 << 4,
	peripheralDataSize4 = 2 << 4,

	memoryDataSize1 = 0 << 6,
	memoryDataSize2 = 1 << 6,
	memoryDataSize4 = 2 << 6,

	lowPriority = 0 << 8,
	mediumPriority = 1 << 8,
	highPriority = 2 << 8,
	veryHighPriority = 3 << 8,

	dataSize1 = peripheralDataSize1 | memoryDataSize1,
	dataSize2 = peripheralDataSize2 | memoryDataSize2,
	dataSize4 = peripheralDataSize4 | memoryDataSize4,
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
			release();
		}

		int configureTransfer(const uintptr_t memoryAddress, const uintptr_t peripheralAddress,
				const size_t transactions, const Flags flags) const
		{
			if (channel_ == nullptr)
				return EBADF;

			return channel_->configureTransfer(memoryAddress, peripheralAddress, transactions, flags);
		}

		std::pair<int, size_t> getTransactionsLeft() const
		{
			if (channel_ == nullptr)
				return {EBADF, {}};

			return {{}, channel_->getTransactionsLeft()};
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
			release();

			const auto ret = channel.reserve(request, functor);
			if (ret != 0)
				return ret;

			channel_ = &channel;
			return {};
		}

		int startTransfer() const
		{
			if (channel_ == nullptr)
				return EBADF;

			return channel_->startTransfer();
		}

		int stopTransfer() const
		{
			if (channel_ == nullptr)
				return EBADF;

			channel_->stopTransfer();
			return {};
		}

		UniqueHandle(const UniqueHandle&) = delete;
		UniqueHandle(UniqueHandle&&) = delete;
		const UniqueHandle& operator=(const UniqueHandle&) = delete;
		UniqueHandle& operator=(UniqueHandle&&) = delete;

	private:

		DmaChannel* channel_;
	};

	MAKE_CONST_MOCK4(configureTransfer, int(uintptr_t, uintptr_t, size_t, Flags));
	MAKE_CONST_MOCK0(getTransactionsLeft, size_t());
	MAKE_MOCK0(release, void());
	MAKE_MOCK2(reserve, int(uint8_t, DmaChannelFunctor&));
	MAKE_CONST_MOCK0(startTransfer, int());
	MAKE_CONST_MOCK0(stopTransfer, void());
};

}	// namespace chip

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_DMAV1_DMAV2_DMACHANNEL_HPP_DISTORTOS_CHIP_DMACHANNEL_HPP_
