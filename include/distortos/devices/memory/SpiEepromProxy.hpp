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

	/**
	 * \brief Checks whether any write operation is currently in progress.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current status of device: false - device
	 * is idle, true - write operation is in progress; error codes:
	 * - error codes returned by readStatusRegister();
	 */

	std::pair<int, bool> isWriteInProgress() const;

	/**
	 * \brief Reads data from SPI EEPROM.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be read
	 * \param [out] buffer is the buffer to which the data will be written
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - error codes returned by executeTransaction();
	 * - error codes returned by waitWhileWriteInProgress();
	 */

	std::pair<int, size_t> read(uint32_t address, void* buffer, size_t size) const;

	/**
	 * \brief Waits while any write operation is currently in progress.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by isWriteInProgress();
	 * - error codes returned by ThisThread::sleepFor();
	 */

	int waitWhileWriteInProgress() const;

	/**
	 * \brief Writes data to SPI EEPROM.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be written
	 * \param [in] buffer is the buffer with data that will be written
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned); error codes:
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - error codes returned by writePage();
	 */

	std::pair<int, size_t> write(uint32_t address, const void* buffer, size_t size) const;

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

	/**
	 * \brief Reads value of status register of SPI EEPROM.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and value of status register of SPI EEPROM;
	 * error codes:
	 * - error codes returned by executeTransaction();
	 */

	std::pair<int, uint8_t> readStatusRegister() const;

	/**
	 * \brief Enables writes in SPI EEPROM.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by executeTransaction();
	 */

	int writeEnable() const;

	/**
	 * \brief Writes single page.
	 *
	 * \param [in] address is the address of data that will be written, must be valid!
	 * \param [in] buffer is the buffer with data that will be written
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned); error codes:
	 * - error codes returned by waitWhileWriteInProgress();
	 * - error codes returned by writeEnable();
	 * - error codes returned by SpiDevice::executeTransaction();
	 */

	std::pair<int, size_t> writePage(uint32_t address, const void* buffer, size_t size) const;

	/// internal proxy for SPI device
	SpiDeviceProxy spiDeviceProxy_;

	/// reference to SpiEeprom associated with this proxy
	SpiEeprom& spiEeprom_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROMPROXY_HPP_
