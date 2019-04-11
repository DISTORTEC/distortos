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

#include "distortos/devices/communication/SpiMasterBase.hpp"
#include "distortos/devices/communication/SpiMasterTransfersRange.hpp"
#include "distortos/devices/communication/SpiMode.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

class Semaphore;

namespace devices
{

class SpiMasterLowLevel;

/**
 * SpiMaster class is a driver for SPI master
 *
 * \ingroup devices
 */

class SpiMaster : private SpiMasterBase
{
	friend class SpiMasterHandle;

public:

	/**
	 * \brief SpiMaster's constructor
	 *
	 * \param [in] spiMaster is a reference to low-level implementation of SpiMasterLowLevel interface
	 */

	constexpr explicit SpiMaster(SpiMasterLowLevel& spiMaster) :
			mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
			transfersRange_{},
			ret_{},
			semaphore_{},
			spiMaster_{spiMaster},
			openCount_{}
	{

	}

	/**
	 * \brief SpiMaster's destructor
	 *
	 * \pre Device is closed.
	 */

	~SpiMaster() override;

private:

	/**
	 * \brief Closes SPI master.
	 *
	 * Does nothing if any user still has this device opened. Otherwise low-level driver is stopped.
	 *
	 * \pre Device is opened.
	 */

	void close();

	/**
	 * \brief Configures parameters of SPI master.
	 *
	 * \pre Device is opened.
	 * \pre \a clockFrequency and \a wordLength are valid for associated low-level implementation of SpiMasterLowLevel
	 * interface.
	 *
	 * \param [in] mode is the desired SPI mode
	 * \param [in] clockFrequency is the desired clock frequency, Hz
	 * \param [in] wordLength selects word length, bits
	 * \param [in] lsbFirst selects whether MSB (false) or LSB (true) is transmitted first
	 * \param [in] dummyData is the dummy data that will be sent if write buffer of transfer is nullptr
	 *
	 * \return real clock frequency, Hz
	 */

	uint32_t configure(SpiMode mode, uint32_t clockFrequency, uint8_t wordLength, bool lsbFirst,
			uint32_t dummyData) const;

	/**
	 * \brief Executes series of transfers as a single atomic transaction.
	 *
	 * The transaction is finished when all transfers are complete or when any error is detected.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 * \pre \a transfersRange has at least one transfer.
	 *
	 * \param [in] transfersRange is the range of transfers that will be executed
	 *
	 * \return 0 on success, error code otherwise:
	 * - EIO - failure detected by low-level SPI master driver;
	 */

	int executeTransaction(SpiMasterTransfersRange transfersRange);

	/**
	 * \brief Locks SPI master for exclusive use by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The number of recursive locks of device is less than 65535.
	 *
	 * \post Device is locked.
	 */

	void lock();

	/**
	 * \brief Notifies waiting thread about completion of transaction.
	 *
	 * \param [in] ret is the last error code returned by transaction handling code
	 */

	void notifyWaiter(int ret);

	/**
	 * \brief Opens SPI master.
	 *
	 * Does nothing if any user already has this device opened. Otherwise low-level driver is started.
	 *
	 * \pre The number of times the device is opened is less than 255.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SpiMasterLowLevel::start();
	 */

	int open();

	/**
	 * \brief "Transfer complete" event
	 *
	 * Called by low-level SPI master driver when the transfer is physically finished.
	 *
	 * Handles the next transfer from the currently handled transaction. If there are no more transfers, waiting thread
	 * is notified about completion of transaction.
	 *
	 * \param [in] bytesTransfered is the number of bytes transferred by low-level SPI master driver (read from write
	 * buffer and/or written to read buffer), may be unreliable if transfer error was detected (\a bytesTransfered is
	 * not equal to size of transfer)
	 */

	void transferCompleteEvent(size_t bytesTransfered) override;

	/**
	 * \brief Unlocks SPI master which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the device.
	 */

	void unlock();

	/// mutex used to serialize access to this object
	Mutex mutex_;

	/// range of transfers that are part of currently handled transaction
	SpiMasterTransfersRange transfersRange_;

	/// error codes detected in transferCompleteEvent()
	volatile int ret_;

	/// pointer to semaphore used to notify waiting thread about completion of transaction
	Semaphore* volatile semaphore_;

	/// reference to low-level implementation of SpiMasterLowLevel interface
	SpiMasterLowLevel& spiMaster_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTER_HPP_
