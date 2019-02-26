/**
 * \file
 * \brief SdMmcCardLowLevel class header for SDMMCv1 in STM32
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SDMMCV1_INCLUDE_DISTORTOS_CHIP_SDMMCCARDLOWLEVEL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_SDMMCV1_INCLUDE_DISTORTOS_CHIP_SDMMCCARDLOWLEVEL_HPP_

#include "distortos/chip/DmaChannel.hpp"
#include "distortos/chip/DmaChannelFunctorCommon.hpp"

#include "distortos/devices/memory/SdMmcCardLowLevel.hpp"

namespace distortos
{

namespace chip
{

class SdmmcPeripheral;

/**
 * SdMmcCardLowLevel class is a low-level SD/MMC card driver for SDMMCv1 in STM32.
 *
 * \ingroup devices
 */

class SdMmcCardLowLevel : public devices::SdMmcCardLowLevel
{
public:

	/**
	 * \brief SdMmcCardLowLevel's constructor
	 *
	 * \param [in] sdmmcPeripheral is a reference to raw SDMMC peripheral
	 * \param [in] dmaChannel is a reference to DMA channel used for transfers
	 * \param [in] dmaRequest is the request identifier for DMA channel used for transfers
	 */

	constexpr SdMmcCardLowLevel(const SdmmcPeripheral& sdmmcPeripheral, DmaChannel& dmaChannel,
			const uint8_t dmaRequest) :
					clockFrequency_{},
					dmaChannel_{dmaChannel},
					dmaChannelFunctor_{*this},
					dmaChannelUniqueHandle_{},
					response_{},
					sdMmcCardBase_{},
					sdmmcPeripheral_{sdmmcPeripheral},
					dmaError_{},
					dmaRequest_{dmaRequest}
	{

	}

	/**
	 * \brief SdMmcCardLowLevel's destructor
	 *
	 * \pre Driver is stopped.
	 */

	~SdMmcCardLowLevel() override;

	/**
	 * \brief Configures parameters of low-level SD/MMC card driver.
	 *
	 * \pre Driver is started.
	 * \pre No transaction is in progress.
	 *
	 * \param [in] busMode is the desired bus mode
	 * \param [in] clockFrequency is the desired clock frequency, Hz
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - selected clock frequency is invalid;
	 */

	int configure(BusMode busMode, uint32_t clockFrequency) override;

	/**
	 * \brief Interrupt handler
	 *
	 * \note this must not be called by user code
	 */

	void interruptHandler();

	/**
	 * \brief Starts low-level SD/MMC card driver.
	 *
	 * \pre Driver is stopped.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by DmaChannelUniqueHandle::reserve();
	 */

	int start() override;

	/**
	 * \brief Starts asynchronous transaction.
	 *
	 * This function returns immediately. When the transaction is physically finished (either command, its argument,
	 * response and associated transfer were sent/received or an error was detected),
	 * SdMmcCardBase::transactionCompleteEvent() will be executed.
	 *
	 * \pre Driver is started.
	 * \pre No transaction is in progress.
	 * \pre \a command is valid.
	 * \pre When there is an associated transfer:
	 * - either short or long response is expected;
	 * - transfer's read buffer (for read transfers) or write buffer (for write transfers) is valid and its address is
	 * aligned to DISTORTOS_SDMMCCARD_BUFFER_ALIGNMENT;
	 * - transfer's block size is a power of two, greater than or equal to 4 and less than or equal to 2^14;
	 * - transfer's size is an integer multiple of block size and less than or equal to 2^25 - 1;
	 * - transfer's timeout converted to clock cycles must be less than or equal to 2^32 - 1;
	 * \post Transaction is in progress.
	 *
	 * \param [in] sdMmcCardBase is a reference to SdMmcCardBase object that will be notified about completed
	 * transaction
	 * \param [in] command is the command associated with the transaction, [0; maxCommand]
	 * \param [in] argument is the argument for \a command
	 * \param [out] response is the buffer into which the command response will be read, it's size determines what type
	 * of response is expected (none, short or long)
	 * \param [in,out] transfer is the transfer associated with transaction
	 */

	void startTransaction(devices::SdMmcCardBase& sdMmcCardBase, uint8_t command, uint32_t argument, Response response,
			Transfer transfer) override;

	/**
	 * \brief Stops low-level SD/MMC card driver.
	 *
	 * \pre Driver is started.
	 * \pre No transaction is in progress.
	 * \post Driver is stopped.
	 */

	void stop() override;

private:

	/// DmaChannelFunctor class is a DmaChannelFunctorCommon for DMA channel used for transfers
	class DmaChannelFunctor : public DmaChannelFunctorCommon
	{
	public:

		/**
		 * \brief DmaChannelFunctor's constructor
		 *
		 * \param [in] owner is a reference to owner SdMmcCardLowLevel object
		 */

		constexpr explicit DmaChannelFunctor(SdMmcCardLowLevel& owner) :
				owner_{owner}
		{

		}

		/**
		 * \brief "Transfer error" event
		 *
		 * Called by low-level DMA channel driver when transfer error is detected.
		 *
		 * \param [in] transactionsLeft is the number of transactions left
		 */

		void transferErrorEvent(size_t transactionsLeft) override;

	private:

		/// reference to owner SdMmcCardLowLevel object
		SdMmcCardLowLevel& owner_;
	};

	/**
	 * \return true if driver is started, false otherwise
	 */

	bool isStarted() const
	{
		return clockFrequency_ != 0;
	}

	/**
	 * \return true if transaction is in progress, false otherwise
	 */

	bool isTransactionInProgress() const
	{
		return sdMmcCardBase_ != nullptr;
	}

	/**
	 * \brief "Transfer error" event handler
	 *
	 * \param [in] transactionsLeft is the number of transactions left
	 */

	void transferErrorEventHandler(size_t transactionsLeft);

	/// clock frequency, Hz
	uint32_t clockFrequency_;

	/// reference to DMA channel used for transfers
	DmaChannel& dmaChannel_;

	/// functor for DMA channel used for transfers
	DmaChannelFunctor dmaChannelFunctor_;

	/// unique handle of DMA channel used for transfers
	DmaChannel::UniqueHandle dmaChannelUniqueHandle_;

	/// buffer into which the command response will be read
	Response response_;

	/// pointer to SdMmcCardBase object associated with this one
	devices::SdMmcCardBase* volatile sdMmcCardBase_;

	/// reference to raw SDMMC peripheral
	const SdmmcPeripheral& sdmmcPeripheral_;

	/// true if DMA error was detected, false otherwise
	volatile bool dmaError_;

	/// request identifier for DMA channel used for transfers
	uint8_t dmaRequest_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_SDMMCV1_INCLUDE_DISTORTOS_CHIP_SDMMCCARDLOWLEVEL_HPP_
