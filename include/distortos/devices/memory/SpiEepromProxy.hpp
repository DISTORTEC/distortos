/**
 * \file
 * \brief SpiEepromProxy class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROMPROXY_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROMPROXY_HPP_

#include "distortos/devices/communication/SpiDeviceProxy.hpp"
#include "distortos/devices/communication/SpiMasterOperationsRange.hpp"

namespace distortos
{

namespace devices
{

class SpiEeprom;

/**
 * SpiEepromProxy is a [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)-like class for locking
 * SpiEeprom and for accessing its core functionality.
 *
 * \note Locks are recursive.
 *
 * \ingroup devices
 */

class SpiEepromProxy
{
public:

	/**
	 * \brief SpiEepromProxy's constructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] spiEeprom is a reference to SpiEeprom associated with this proxy
	 */

	explicit SpiEepromProxy(SpiEeprom& spiEeprom);

private:

	/**
	 * \brief Executes series of operations as a single atomic transaction.
	 *
	 * \param [in] operationsRange is the range of operations that will be executed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of successfully completed
	 * operations from \a operationsRange; error codes:
	 * - error codes returned by SpiMasterProxy::configure();
	 * - error codes returned by SpiMasterProxy::executeTransaction();
	 */

	std::pair<int, size_t> executeTransaction(SpiMasterOperationsRange operationsRange) const;

	/// internal proxy for SPI device
	SpiDeviceProxy spiDeviceProxy_;

	/// reference to SpiEeprom associated with this proxy
	SpiEeprom& spiEeprom_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROMPROXY_HPP_
