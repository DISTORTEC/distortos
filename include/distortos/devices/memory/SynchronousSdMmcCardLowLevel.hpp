/**
 * \file
 * \brief SynchronousSdMmcCardLowLevel class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SYNCHRONOUSSDMMCCARDLOWLEVEL_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SYNCHRONOUSSDMMCCARDLOWLEVEL_HPP_

#include "distortos/devices/memory/SdMmcCardBase.hpp"
#include "distortos/devices/memory/SdMmcCardLowLevel.hpp"

#include "distortos/Semaphore.hpp"

namespace distortos
{

namespace devices
{

class SdMmcCardLowLevel;

/// SynchronousSdMmcCardLowLevel is a synchronous wrapper for SdMmcCardLowLevel which implements SdMmcCardBase interface
class SynchronousSdMmcCardLowLevel : private SdMmcCardBase
{
public:

	/**
	 * \brief SynchronousSdMmcCardLowLevel's constructor
	 *
	 * \param [in] sdMmcCard is a reference to asynchronous low-level implementation of SdMmcCardLowLevel interface
	 */

	constexpr explicit SynchronousSdMmcCardLowLevel(SdMmcCardLowLevel& sdMmcCard) :
			semaphore_{0, 1},
			sdMmcCard_{sdMmcCard},
			result_{}
	{

	}

	/**
	 * \brief Configures parameters of low-level SD/MMC card driver.
	 *
	 * \pre Driver is started.
	 * \pre No transaction is in progress.
	 * \pre \a clockFrequency is valid.
	 *
	 * \param [in] busMode is the desired bus mode
	 * \param [in] clockFrequency is the desired clock frequency, Hz, must be greater than or equal to 400 kHz
	 */

	void configure(const SdMmcCardLowLevel::BusMode busMode, const uint32_t clockFrequency) const
	{
		sdMmcCard_.configure(busMode, clockFrequency);
	}

	/**
	 * \brief Executes transaction.
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
	 *
	 * \post No transaction is in progress.
	 *
	 * \param [in] command is the command associated with the transaction, [0; SdMmcCardLowLevel::maxCommand]
	 * \param [in] argument is the argument for \a command
	 * \param [out] response is the buffer into which the command response will be read, it's size determines what type
	 * of response is expected (none, short or long)
	 * \param [in,out] transfer is the transfer associated with transaction
	 *
	 * \return result of transaction
	 */

	Result executeTransaction(const uint8_t command, const uint32_t argument,
			const SdMmcCardLowLevel::Response response, const SdMmcCardLowLevel::Transfer transfer)
	{
		sdMmcCard_.startTransaction(*this, command, argument, response, transfer);
		while (semaphore_.wait() != 0);
		return result_;
	}

	/**
	 * \brief Starts low-level SD/MMC card driver.
	 *
	 * \pre Driver is stopped.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SdMmcCardLowLevel::start();
	 */

	int start() const
	{
		return sdMmcCard_.start();
	}

	/**
	 * \brief Stops low-level SD/MMC card driver.
	 *
	 * \pre Driver is started.
	 * \pre No transaction is in progress.
	 *
	 * \post Driver is stopped.
	 */

	void stop() const
	{
		sdMmcCard_.stop();
	}

private:

	/**
	 * \brief "Transaction complete" event
	 *
	 * Called by low-level SD/MMC card driver when the transaction is physically finished.
	 *
	 * \param [in] result is the result of transaction
	 */

	void transactionCompleteEvent(Result result) override;

	/// semaphore used to notify waiting thread about completion of transaction
	Semaphore semaphore_;

	/// reference to asynchronous low-level implementation of SdMmcCardLowLevel interface
	SdMmcCardLowLevel& sdMmcCard_;

	/// result of transaction passed to transactionCompleteEvent()
	Result result_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SYNCHRONOUSSDMMCCARDLOWLEVEL_HPP_
