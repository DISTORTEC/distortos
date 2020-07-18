/**
 * \file
 * \brief DmaChannelHandle class header for STM32
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_DMACHANNELHANDLE_HPP_
#define SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_DMACHANNELHANDLE_HPP_

#include "distortos/chip/DmaChannel.hpp"

namespace distortos
{

namespace chip
{

/**
 * \brief DmaChannelHandle class can be used to access DmaChannel's functionality.
 *
 * \ingroup devices
 */

class DmaChannelHandle
{
public:

	/// import DmaChannelFlags
	using Flags = DmaChannelFlags;

	/**
	 * \brief DmaChannelHandle's constructor
	 */

	constexpr DmaChannelHandle() :
			channel_{}
	{

	}

	/**
	 * \brief DmaChannelHandle's destructor
	 *
	 * \pre No driver is reserved with this handle.
	 */

	~DmaChannelHandle()
	{
		assert(channel_ == nullptr);
	}

	/**
	 * \pre Driver is reserved with this handle.
	 *
	 * \return number of transactions left
	 */

	size_t getTransactionsLeft() const
	{
		assert(channel_ != nullptr);
		return channel_->getTransactionsLeft();
	}

	/**
	 * \brief Releases any associated low-level DMA channel driver.
	 *
	 * \pre No transfer is in progress.
	 *
	 * \post No driver is reserved with this handle.
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
	 * \pre No driver is reserved with this handle.
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
		assert(channel_ == nullptr);

		const auto ret = channel.reserve(request, functor);
		if (ret != 0)
			return ret;

		channel_ = &channel;
		return {};
	}

	/**
	 * \brief Configures and starts asynchronous transfer.
	 *
	 * This function returns immediately. When the transfer is physically finished (either expected number of
	 * transactions were executed or an error was detected), one of DmaChannelFunctor functions will be executed.
	 *
	 * \pre Driver is reserved with this handle.
	 * \pre \a memoryAddress and \a peripheralAddress and \a transactions and \a flags are valid.
	 * \pre Memory data size multiplied by memory burst size is less than or equal to 16.
	 * \pre No transfer is in progress.
	 *
	 * \post Transfer is in progress.
	 *
	 * \param [in] memoryAddress is the memory address, must be divisible by configured memory data size multiplied
	 * by configured memory burst size
	 * \param [in] peripheralAddress is the peripheral address, must be divisible by peripheral data size multiplied
	 * by configured peripheral burst size
	 * \param [in] transactions is the number of transactions, [1; 65535]
	 * \param [in] flags are configuration flags
	 */

	void startTransfer(const uintptr_t memoryAddress, const uintptr_t peripheralAddress, const size_t transactions,
			const Flags flags) const
	{
		assert(channel_ != nullptr);
		channel_->startTransfer(memoryAddress, peripheralAddress, transactions, flags);
	}

	/**
	 * \brief Stops transfer.
	 *
	 * This function should be used after previous asynchronous transfer is finished to restore DMA channel to
	 * proper state. It may also be used to stop any ongoing asynchronous transfer.
	 *
	 * \pre Driver is reserved with this handle.
	 *
	 * \post No transfer is in progress.
	 */

	void stopTransfer() const
	{
		assert(channel_ != nullptr);
		channel_->stopTransfer();
	}

	DmaChannelHandle(const DmaChannelHandle&) = delete;
	DmaChannelHandle(DmaChannelHandle&&) = delete;
	const DmaChannelHandle& operator=(const DmaChannelHandle&) = delete;
	DmaChannelHandle& operator=(DmaChannelHandle&&) = delete;

private:

	/// pointer to low-level DMA channel driver associated with this handle
	DmaChannel* channel_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_DMACHANNELHANDLE_HPP_
