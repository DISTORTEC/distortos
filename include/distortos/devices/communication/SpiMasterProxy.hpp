/**
 * \file
 * \brief SpiMasterProxy class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERPROXY_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERPROXY_HPP_

#include "distortos/devices/communication/SpiMasterBase.hpp"
#include "distortos/devices/communication/SpiMasterOperationsRange.hpp"
#include "distortos/devices/communication/SpiMode.hpp"

namespace distortos
{

class Semaphore;

namespace devices
{

class SpiDevice;
class SpiDeviceProxy;
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
	 * \param [in] spiDeviceProxy is a reference to SpiDeviceProxy associated with this proxy
	 */

	explicit SpiMasterProxy(const SpiDeviceProxy& spiDeviceProxy);

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
	 *
	 * \return pair with return code (0 on success, error code otherwise) and real clock frequency; error codes:
	 * - EBADF - associated SPI device or associated SPI master are not opened;
	 * - error codes returned by SpiMasterLowLevel::configure();
	 */

	std::pair<int, uint32_t> configure(SpiMode mode, uint32_t clockFrequency, uint8_t wordLength, bool lsbFirst) const;

	/**
	 * \brief Executes series of operations as a single atomic transaction.
	 *
	 * The transaction is finished when all operations are complete or when any error is detected.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] operationsRange is the range of operations that will be executed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of successfully completed
	 * operations from \a operationsRange; error codes:
	 * - EBADF - associated SPI device or associated SPI master are not opened;
	 * - EINVAL - \a operationsRange has no operations;
	 * - EIO - failure detected by low-level SPI master driver;
	 * - error codes returned by SpiMasterLowLevel::startTransfer();
	 */

	std::pair<int, size_t> executeTransaction(SpiMasterOperationsRange operationsRange);

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
	 * Handles the next operation from the currently handled transaction. If there are no more operations, waiting
	 * thread is notified about completion of transaction.
	 *
	 * \param [in] errorSet is the set of error bits
	 * \param [in] bytesTransfered is the number of bytes transfered by low-level SPI master driver (read from write
	 * buffer and/or written to read buffer), may be unreliable if \a errorSet is not empty (i.e. transfer error was
	 * detected)
	 */

	void transferCompleteEvent(SpiMasterErrorSet errorSet, size_t bytesTransfered) override;

	/// range of operations that are part of currently handled transaction
	SpiMasterOperationsRange operationsRange_;

	/// reference to SpiDeviceProxy associated with this proxy
	const SpiDeviceProxy& spiDeviceProxy_;

	/// error codes detected in transferCompleteEvent()
	volatile int ret_;

	/// pointer to semaphore used to notify waiting thread about completion of transaction
	Semaphore* volatile semaphore_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERPROXY_HPP_
