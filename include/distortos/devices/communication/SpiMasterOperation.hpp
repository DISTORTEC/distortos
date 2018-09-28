/**
 * \file
 * \brief SpiMasterOperation class header
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATION_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATION_HPP_

#include "distortos/devices/communication/SpiMasterTransfer.hpp"

namespace distortos
{

namespace devices
{

/**
 * SpiMasterOperation class is a single operation that can be executed by SPI master as part of a transaction.
 *
 * \ingroup devices
 */

class SpiMasterOperation
{
public:

	/// import SpiMasterTransfer as Transfer
	using Transfer = SpiMasterTransfer;

	/**
	 * \brief SpiMasterOperation's constructor with transfer operation
	 *
	 * \param [in] transfer is the transfer operation that will be executed
	 *
	 * \note non-explicit on purpose
	 */

	constexpr SpiMasterOperation(const Transfer transfer) :
			transfer_{transfer}
	{

	}

	/**
	 * \return pointer to contained transfer operation, nullptr if this object contains different type of operation
	 */

	Transfer* getTransfer()
	{
		return &transfer_;
	}

private:

	/// single transfer operation
	Transfer transfer_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATION_HPP_
