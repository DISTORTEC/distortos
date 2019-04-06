/**
 * \file
 * \brief SpiMaster class header
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTER_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTER_HPP_

#include "distortos/devices/communication/SpiMasterTransfersRange.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace devices
{

class SpiDevice;
class SpiMasterLowLevel;
class SpiMasterProxy;

/**
 * SpiMaster class is a driver for SPI master
 *
 * \ingroup devices
 */

class SpiMaster
{
	friend class SpiMasterProxy;

public:

	/// import SpiMasterProxy as SpiMaster::Proxy
	using Proxy = SpiMasterProxy;

	/**
	 * \brief SpiMaster's constructor
	 *
	 * \param [in] spiMaster is a reference to low-level implementation of SpiMasterLowLevel interface
	 */

	constexpr explicit SpiMaster(SpiMasterLowLevel& spiMaster) :
			mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
			spiMaster_{spiMaster},
			openCount_{}
	{

	}

	/**
	 * \brief SpiMaster's destructor
	 *
	 * Does nothing if all users already closed this device. If they did not, performs forced close of device.
	 *
	 * \warning This function must not be called from interrupt context!
	 */

	~SpiMaster();

	/**
	 * \brief Closes SPI master.
	 *
	 * Does nothing if any user still has this device opened. Otherwise low-level driver is stopped.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the device is already completely closed;
	 * - error codes returned by SpiMasterLowLevel::stop();
	 */

	int close();

	/**
	 * \brief Executes series of transfers as a single atomic transaction.
	 *
	 * First SPI is configured to match parameters of SPI device (clock frequency, mode, format, ...). Then the device
	 * is selected and the transfers are executed. The transaction is finished when all transfers are complete or when
	 * any error is detected - in either case the device is unselected and this function returns.
	 *
	 * \deprecated scheduled to be removed after v0.7.0, use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] device is a reference to SPI device which is the target of the transaction
	 * \param [in] transfersRange is the range of transfers that will be executed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of successfully completed transfers
	 * from \a transfersRange; error codes:
	 * - EINVAL - \a transfersRange has no transfers;
	 * - error codes returned by SpiMasterProxy::configure();
	 * - error codes returned by SpiMasterProxy::executeTransaction();
	 */

	__attribute__ ((deprecated("Use SpiDeviceHandle, SpiMasterProxy and SpiDeviceSelectGuard")))
	std::pair<int, size_t> executeTransaction(SpiDevice& device, SpiMasterTransfersRange transfersRange);

	/**
	 * \brief Opens SPI master.
	 *
	 * Does nothing if any user already has this device opened. Otherwise low-level driver is started.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EMFILE - this device is already opened too many times;
	 * - error codes returned by SpiMasterLowLevel::start();
	 */

	int open();

private:

	/// mutex used to serialize access to this object
	Mutex mutex_;

	/// reference to low-level implementation of SpiMasterLowLevel interface
	SpiMasterLowLevel& spiMaster_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTER_HPP_
