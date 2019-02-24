/**
 * \file
 * \brief DmaChannel class header for DMAv1 in STM32
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_DMAV1_INCLUDE_DISTORTOS_CHIP_DMACHANNEL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_DMAV1_INCLUDE_DISTORTOS_CHIP_DMACHANNEL_HPP_

#include "estd/EnumClassFlags.hpp"

#include <utility>

#include <cerrno>
#include <cstddef>
#include <cstdint>

namespace distortos
{

namespace chip
{

/// DMA transfer configuration flags
enum class DmaChannelFlags : uint16_t
{
	/// "transfer complete" interrupt is disabled
	transferCompleteInterruptDisable = 0 << 1,
	/// "transfer complete" interrupt is enabled
	transferCompleteInterruptEnable = 1 << 1,

	/// transfer from peripheral to memory
	peripheralToMemory = 0 << 4,
	/// transfer from memory to peripheral
	memoryToPeripheral = 1 << 4,

	/// peripheral address is fixed
	peripheralFixed = 0 << 6,
	/// peripheral address is incremented after each transaction
	peripheralIncrement = 1 << 6,

	/// memory address is fixed
	memoryFixed = 0 << 7,
	/// memory address is incremented after each transaction
	memoryIncrement = 1 << 7,

	/// peripheral data size - 1 byte
	peripheralDataSize1 = 0 << 8,
	/// peripheral data size - 2 bytes
	peripheralDataSize2 = 1 << 8,
	/// peripheral data size - 4 bytes
	peripheralDataSize4 = 2 << 8,

	/// memory data size - 1 byte
	memoryDataSize1 = 0 << 10,
	/// memory data size - 2 bytes
	memoryDataSize2 = 1 << 10,
	/// memory data size - 4 bytes
	memoryDataSize4 = 2 << 10,

	/// low priority
	lowPriority = 0 << 12,
	/// medium priority
	mediumPriority = 1 << 12,
	/// high priority
	highPriority = 2 << 12,
	/// very high priority
	veryHighPriority = 3 << 12,

	/// memory and peripheral data size - 1 byte
	dataSize1 = peripheralDataSize1 | memoryDataSize1,
	/// memory and peripheral data size - 2 bytes
	dataSize2 = peripheralDataSize2 | memoryDataSize2,
	/// memory and peripheral data size - 4 bytes
	dataSize4 = peripheralDataSize4 | memoryDataSize4,
};

}	// namespace chip

}	// namespace distortos

namespace estd
{

/// \brief Enable bitwise operators for distortos::chip::DmaChannelFlags
template<>
struct isEnumClassFlags<distortos::chip::DmaChannelFlags> : std::true_type
{

};

}	// namespace estd

namespace distortos
{

namespace chip
{

class DmaChannelFunctor;
class DmaChannelPeripheral;
class DmaPeripheral;

/**
 * DmaChannel class is a low-level DMA channel driver for DMAv1 in STM32
 *
 * \ingroup devices
 */

class DmaChannel
{
public:

	/// import DmaChannelFlags
	using Flags = DmaChannelFlags;

	/// UniqueHandle class can be used to access DmaChannel's functionality
	class UniqueHandle
	{
	public:

		/**
		 * \brief UniqueHandle's constructor
		 */

		constexpr UniqueHandle() :
				channel_{}
		{

		}

		/**
		 * \brief UniqueHandle's destructor
		 */

		~UniqueHandle()
		{
			release();
		}

		/**
		 * \brief Configures parameters of transfer.
		 *
		 * \param [in] memoryAddress is the memory address, must be divisible by configured memory data size
		 * \param [in] peripheralAddress is the peripheral address, must be divisible by peripheral data size
		 * \param [in] transactions is the number of transactions
		 * \param [in] flags are configuration flags
		 *
		 * \return 0 on success, error code otherwise:
		 * - EBADF - no low-level DMA channel driver is associated with this handle;
		 * - error codes returned by DmaChannel::configureTransfer();
		 */

		int configureTransfer(const uintptr_t memoryAddress, const uintptr_t peripheralAddress,
				const size_t transactions, const Flags flags) const
		{
			if (channel_ == nullptr)
				return EBADF;

			return channel_->configureTransfer(memoryAddress, peripheralAddress, transactions, flags);
		}

		/**
		 * \return pair with return code (0 on success, error code otherwise) and number of transactions left; error
		 * codes:
		 * - EBADF - no low-level DMA channel driver is associated with this handle;
		 */

		std::pair<int, size_t> getTransactionsLeft() const
		{
			if (channel_ == nullptr)
				return {EBADF, {}};

			return {{}, channel_->getTransactionsLeft()};
		}

		/**
		 * \brief Releases any associated low-level DMA channel driver.
		 *
		 * \pre No transfer is in progress.
		 */

		void release()
		{
			if (channel_ == nullptr)
				return;

			channel_->release();
			channel_ = {};
		}

		/**
		 * \brief Reserves low-level DMA channel driver for exclusive use via this handle.
		 *
		 * \pre \a request is valid.
		 *
		 * \param [in] channel is a reference to low-level DMA channel driver which will be associated with this handle
		 * \param [in] request is the request identifier with which low-level DMA channel driver will be associated
		 * \param [in] functor is a reference to DmaChannelFunctor object that will be notified about transfer-related
		 * events
		 *
		 * \return 0 on success, error code otherwise:
		 * - error codes returned by DmaChannel::reserve();
		 */

		int reserve(DmaChannel& channel, const uint8_t request, DmaChannelFunctor& functor)
		{
			release();

			const auto ret = channel.reserve(request, functor);
			if (ret != 0)
				return ret;

			channel_ = &channel;
			return {};
		}

		/**
		 * \brief Starts asynchronous transfer.
		 *
		 * This function returns immediately. When the transfer is physically finished (either expected number of
		 * transactions were executed or an error was detected), one of DmaChannelFunctor functions will be executed.
		 *
		 * \pre No transfer is in progress.
		 *
		 * \return 0 on success, error code otherwise:
		 * - EBADF - no low-level DMA channel driver is associated with this handle;
		 */

		int startTransfer() const
		{
			if (channel_ == nullptr)
				return EBADF;

			channel_->startTransfer();
			return {};
		}

		/**
		 * \brief Stops transfer.
		 *
		 * This function should be used after previous asynchronous transfer is finished to restore DMA channel to
		 * proper state. It may also be used to stop any ongoing asynchronous transfer.
		 *
		 * \return 0 on success, error code otherwise:
		 * - EBADF - no low-level DMA channel driver is associated with this handle;
		 */

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

		/// pointer to low-level DMA channel driver associated with this handle
		DmaChannel* channel_;
	};

	/**
	 * \brief DmaChannel's constructor
	 *
	 * \param [in] dmaPeripheral is a reference to raw DMA peripheral
	 * \param [in] dmaChannelPeripheral is a reference to raw DMA channel peripheral
	 */

	constexpr DmaChannel(const DmaPeripheral& dmaPeripheral, const DmaChannelPeripheral& dmaChannelPeripheral) :
			dmaPeripheral_{dmaPeripheral},
			dmaChannelPeripheral_{dmaChannelPeripheral},
			functor_{}
	{

	}

	/**
	 * \brief Interrupt handler
	 *
	 * \note this must not be called by user code
	 */

	void interruptHandler();

private:

	/**
	 * \brief Configures parameters of transfer.
	 *
	 * \param [in] memoryAddress is the memory address, must be divisible by configured memory data size
	 * \param [in] peripheralAddress is the peripheral address, must be divisible by peripheral data size
	 * \param [in] transactions is the number of transactions
	 * \param [in] flags are configuration flags
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBUSY - transfer is in progress;
	 * - EINVAL - \a memoryAddress and/or \a peripheralAddress and/or \a transactions and/or \a flags are invalid;
	 * - ENOTSUP - more than 65535 \a transactions are not supported;
	 */

	int configureTransfer(uintptr_t memoryAddress, uintptr_t peripheralAddress, size_t transactions, Flags flags) const;

	/**
	 * \return number of transactions left
	 */

	size_t getTransactionsLeft() const;

	/**
	 * \brief Releases low-level DMA channel driver.
	 *
	 * \pre No transfer is in progress.
	 */

	void release();

	/**
	 * \brief Reserves low-level DMA channel driver for exclusive use.
	 *
	 * \pre \a request is valid.
	 *
	 * \param [in] request is the request identifier with which this object will be associated
	 * \param [in] functor is a reference to DmaChannelFunctor object that will be notified about transfer-related
	 * events
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBUSY - the driver is not released;
	 */

	int reserve(uint8_t request, DmaChannelFunctor& functor);

	/**
	 * \brief Starts asynchronous transfer.
	 *
	 * This function returns immediately. When the transfer is physically finished (either expected number of
	 * transactions were executed or an error was detected), one of DmaChannelFunctor functions will be executed.
	 *
	 * \pre No transfer is in progress.
	 * \post Transfer is in progress.
	 */

	void startTransfer() const;

	/**
	 * \brief Stops transfer.
	 *
	 * This function should be used after previous asynchronous transfer is finished to restore DMA channel to proper
	 * state. It may also be used to stop any ongoing asynchronous transfer.
	 */

	void stopTransfer() const;

	/// reference to raw DMA peripheral
	const DmaPeripheral& dmaPeripheral_;

	/// reference to raw DMA channel peripheral
	const DmaChannelPeripheral& dmaChannelPeripheral_;

	/// pointer to DmaChannelFunctor object associated with this one
	DmaChannelFunctor* functor_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_DMAV1_INCLUDE_DISTORTOS_CHIP_DMACHANNEL_HPP_
