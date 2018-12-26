/**
 * \file
 * \brief DmaChannel class header for DMAv2 in STM32
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_DMAV2_INCLUDE_DISTORTOS_CHIP_DMACHANNEL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_DMAV2_INCLUDE_DISTORTOS_CHIP_DMACHANNEL_HPP_

#include <utility>

#include <cerrno>
#include <cstddef>
#include <cstdint>

namespace distortos
{

namespace chip
{

class DmaChannelFunctor;
class DmaChannelPeripheral;
class DmaPeripheral;

/**
 * DmaChannel class is a low-level DMA channel driver for DMAv2 in STM32
 *
 * \ingroup devices
 */

class DmaChannel
{
public:

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
		 * \brief Configures parameters of transfer.
		 *
		 * \param [in] memoryAddress is the memory address, must be divisible by \a memoryDataSize
		 * \param [in] memoryDataSize is the memory data size, {1, 2, 4}
		 * \param [in] memoryIncrement selects whether memory address is incremented after each transaction (true) or not
		 * (false)
		 * \param [in] peripheralAddress is the peripheral address, must be divisible by \a peripheralDataSize
		 * \param [in] peripheralDataSize is the peripheral data size, {1, 2, 4}
		 * \param [in] peripheralIncrement selects whether peripheral address is incremented after each transaction (true)
		 * or not (false)
		 * \param [in] transactions is the number of transactions
		 * \param [in] memoryToPeripheral selects whether the transfer is from memory to peripheral (true) or from
		 * peripheral to memory (false)
		 *
		 * \return 0 on success, error code otherwise:
		 * - EBADF - no low-level DMA channel driver is associated with this handle;
		 * - error codes returned by DmaChannel::configureTransfer();
		 */

		int configureTransfer(const uintptr_t memoryAddress, const size_t memoryDataSize, const bool memoryIncrement,
				const uintptr_t peripheralAddress, const size_t peripheralDataSize, const bool peripheralIncrement,
				const size_t transactions, const bool memoryToPeripheral) const
		{
			if (channel_ == nullptr)
				return EBADF;

			return channel_->configureTransfer(memoryAddress, memoryDataSize, memoryIncrement, peripheralAddress,
					peripheralDataSize, peripheralIncrement, transactions, memoryToPeripheral);
		}

		/**
		 * \brief Starts asynchronous transfer.
		 *
		 * This function returns immediately. When the transfer is physically finished (either expected number of
		 * transactions were executed or an error was detected), one of DmaChannelFunctor functions will be executed.
		 *
		 * \return 0 on success, error code otherwise:
		 * - EBADF - no low-level DMA channel driver is associated with this handle;
		 * - error codes returned by DmaChannel::startTransfer();
		 */

		int startTransfer() const
		{
			if (channel_ == nullptr)
				return EBADF;

			return channel_->startTransfer();
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
			functor_{},
			request_{}
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
	 * \return number of transactions left
	 */

	size_t getTransactionsLeft() const;

	/**
	 * \brief Releases low-level DMA channel driver.
	 */

	void release();

	/**
	 * \brief Reserves low-level DMA channel driver for exclusive use.
	 *
	 * \param [in] request is the request identifier with which this object will be associated
	 * \param [in] functor is a reference to DmaChannelFunctor object that will be notified about transfer-related
	 * events
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBUSY - the driver is not released;
	 * - EINVAL - \a request is invalid;
	 */

	int reserve(uint8_t request, DmaChannelFunctor& functor);

	/**
	 * \brief Configures parameters of transfer.
	 *
	 * \param [in] memoryAddress is the memory address, must be divisible by \a memoryDataSize
	 * \param [in] memoryDataSize is the memory data size, {1, 2, 4}
	 * \param [in] memoryIncrement selects whether memory address is incremented after each transaction (true) or not
	 * (false)
	 * \param [in] peripheralAddress is the peripheral address, must be divisible by \a peripheralDataSize
	 * \param [in] peripheralDataSize is the peripheral data size, {1, 2, 4}
	 * \param [in] peripheralIncrement selects whether peripheral address is incremented after each transaction (true)
	 * or not (false)
	 * \param [in] transactions is the number of transactions, [1; 65535]
	 * \param [in] memoryToPeripheral selects whether the transfer is from memory to peripheral (true) or from
	 * peripheral to memory (false)
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBUSY - transfer is in progress;
	 * - EINVAL - \a memoryAddress and/or \a memoryDataSize and/or \a peripheralAddress and/or \a peripheralDataSize
	 * and/or \a transactions are invalid;
	 * - ENOTSUP - more than 65535 \a transactions are not supported;
	 */

	int configureTransfer(uintptr_t memoryAddress, size_t memoryDataSize, bool memoryIncrement,
			uintptr_t peripheralAddress, size_t peripheralDataSize, bool peripheralIncrement, size_t transactions,
			bool memoryToPeripheral) const;

	/**
	 * \brief Starts asynchronous transfer.
	 *
	 * This function returns immediately. When the transfer is physically finished (either expected number of
	 * transactions were executed or an error was detected), one of DmaChannelFunctor functions will be executed.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBUSY - transfer is in progress;
	 */

	int startTransfer() const;

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

	/// request identifier with which this object is associated
	uint8_t request_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_DMAV2_INCLUDE_DISTORTOS_CHIP_DMACHANNEL_HPP_
