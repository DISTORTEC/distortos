/**
 * \file
 * \brief SdMmcCardBase class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SDMMCCARDBASE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SDMMCCARDBASE_HPP_

#include <cstdint>

namespace distortos
{

namespace devices
{

/**
 * \brief SdMmcCardBase class is an interface with callbacks for low-level SD/MMC card driver, which can serve as a base
 * for high-level SD/MMC card drivers.
 *
 * \ingroup devices
 */

class SdMmcCardBase
{
public:

	/// result of transaction
	enum class Result : uint8_t
	{
		/// transaction completed successfully
		success,
		/// transaction failed - response timeout
		responseTimeout,
		/// transaction failed - response CRC mismatch
		responseCrcMismatch,
		/// transaction failed - data timeout
		dataTimeout,
		/// transaction failed - data CRC mismatch
		dataCrcMismatch,
		/// transaction failed - transmit underrun
		transmitUnderrun,
		/// transaction failed - receive overrun
		receiveOverrun,
	};

	/**
	 * \brief SdMmcCardBase's destructor
	 */

	virtual ~SdMmcCardBase() = default;

	/**
	 * \brief "Transaction complete" event
	 *
	 * Called by low-level SD/MMC card driver when the transaction is physically finished.
	 *
	 * \param [in] result is the result of transaction
	 */

	virtual void transactionCompleteEvent(Result result) = 0;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SDMMCCARDBASE_HPP_
