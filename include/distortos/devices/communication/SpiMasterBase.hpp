/**
 * \file
 * \brief SpiMasterBase class header
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERBASE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERBASE_HPP_

namespace distortos
{

namespace devices
{

/**
 * SpiMasterBase class is an interface with callbacks for low-level SPI master driver, which can serve as a base for
 * high-level SPI master drivers
 *
 * \ingroup devices
 */

class SpiMasterBase
{
public:

	/**
	 * \brief SpiMasterBase's destructor
	 */

	virtual ~SpiMasterBase() = default;

	/**
	 * \brief "Transfer complete" event
	 *
	 * Called by low-level SPI master driver when the transfer is physically finished.
	 *
	 * \param [in] success tells whether the transfer was successful (true) or not (false)
	 */

	virtual void transferCompleteEvent(bool success) = 0;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERBASE_HPP_
