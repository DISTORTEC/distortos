/**
 * \file
 * \brief SpiMasterProxy class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERHANDLE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERHANDLE_HPP_

#include "distortos/devices/communication/SpiMasterBase.hpp"
#include "distortos/devices/communication/SpiMasterTransfersRange.hpp"
#include "distortos/devices/communication/SpiMode.hpp"

namespace distortos
{

class Semaphore;

namespace devices
{

class SpiDevice;
class SpiDeviceHandle;
class SpiMaster;

/**
 * SpiMasterProxy is a [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)-like class for locking
 * SpiMaster and for accessing its core functionality.
 *
 * \note Locks are recursive.
 *
 * \ingroup devices
 */

class SpiMasterProxy : private SpiMasterBase
{
	friend class SpiDeviceSelectGuard;

public:

	/**
	 * \brief SpiMasterProxy's constructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] spiDeviceHandle is a reference to SpiDeviceHandle associated with this proxy
	 */

	explicit SpiMasterProxy(const SpiDeviceHandle& spiDeviceHandle);

	/**
	 * \brief SpiMasterProxy's destructor
	 *
	 * \warning This function must not be called from interrupt context!
	 */

	~SpiMasterProxy() override;

	/**
	 * \brief Configures parameters of associated SPI master.
	 *
	 * \param [in] mode is the desired SPI mode
	 * \param [in] clockFrequency is the desired clock frequency, Hz
	 * \param [in] wordLength selects word length, bits, [1; 32]
	 * \param [in] lsbFirst selects whether MSB (false) or LSB (true) is transmitted first
	 * \param [in] dummyData is the dummy data that will be sent if write buffer of transfer is nullptr
	 *
	 * \return pair with return code (0 on success, error code otherwise) and real clock frequency; error codes:
	 * - EBADF - associated SPI device or associated SPI master are not opened;
	 * - error codes returned by SpiMasterLowLevel::configure();
	 */

	std::pair<int, uint32_t> configure(SpiMode mode, uint32_t clockFrequency, uint8_t wordLength, bool lsbFirst,
			uint32_t dummyData) const;

	/**
	 * \brief Executes series of transfers as a single atomic transaction.
	 *
	 * The transaction is finished when all transfers are complete or when any error is detected.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] transfersRange is the range of transfers that will be executed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of successfully completed transfers
	 * from \a transfersRange; error codes:
	 * - EBADF - associated SPI device or associated SPI master are not opened;
	 * - EINVAL - \a transfersRange has no transfers;
	 * - EIO - failure detected by low-level SPI master driver;
	 * - error codes returned by SpiMasterLowLevel::startTransfer();
	 */

	std::pair<int, size_t> executeTransaction(SpiMasterTransfersRange transfersRange);

	SpiMasterProxy(const SpiMasterProxy&) = delete;
	SpiMasterProxy& operator=(const SpiMasterProxy&) = delete;

private:

	/**
	 * \return reference to SpiDevice associated with this proxy
	 */

	SpiDevice& getSpiDevice() const;

	/**
	 * \return reference to SpiMaster associated with this proxy
	 */

	SpiMaster& getSpiMaster() const;

	/**
	 * \brief Notifies waiting thread about completion of transaction.
	 *
	 * \param [in] ret is the last error code returned by transaction handling code, default - 0
	 */

	void notifyWaiter(int ret = {});

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

	/// range of transfers that are part of currently handled transaction
	SpiMasterTransfersRange transfersRange_;

	/// reference to SpiDeviceHandle associated with this proxy
	const SpiDeviceHandle& spiDeviceHandle_;

	/// error codes detected in transferCompleteEvent()
	volatile int ret_;

	/// pointer to semaphore used to notify waiting thread about completion of transaction
	Semaphore* volatile semaphore_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERHANDLE_HPP_
