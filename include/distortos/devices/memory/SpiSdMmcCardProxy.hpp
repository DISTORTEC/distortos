/**
 * \file
 * \brief SpiSdMmcCardProxy class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SPISDMMCCARDPROXY_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SPISDMMCCARDPROXY_HPP_

#include "distortos/devices/communication/SpiDeviceProxy.hpp"

#include <utility>

#include <cstddef>
#include <cstdint>

namespace distortos
{

namespace devices
{

class SpiSdMmcCard;

/**
 * SpiSdMmcCardProxy is a [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)-like class for locking
 * SpiSdMmcCard and for accessing its core functionality.
 *
 * \note Locks are recursive.
 *
 * \ingroup devices
 */

class SpiSdMmcCardProxy
{
	friend class SpiSdMmcCard;

public:

	/**
	 * \brief SpiSdMmcCardProxy's constructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] spiSdMmcCard is a reference to SpiSdMmcCard associated with this proxy
	 */

	explicit SpiSdMmcCardProxy(SpiSdMmcCard& spiSdMmcCard);

	/**
	 * \brief Erases blocks on SD or MMC card connected via SPI..
	 *
	 * \param [in] address is the address of range that will be erased, must be a multiple of erase block size
	 * \param [in] size is the size of erased range, bytes, must be a multiple of erase block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the device is not opened;
	 * - EINVAL - \a address and/or \a size are not valid;
	 * - ENOSPC - selected range is greater than size of device;
	 * - error codes returned by executeCmd32();
	 * - error codes returned by executeCmd33();
	 * - error codes returned by executeCmd38();
	 */

	int erase(uint64_t address, uint64_t size) const;

	/**
	 * \return true if associated SPI device is opened, false otherwise
	 */

	bool isOpened() const
	{
		return spiDeviceProxy_.isOpened();
	}

	/**
	 * \brief Reads data from SD or MMC card connected via SPI.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be read, must be a multiple of block size
	 * \param [out] buffer is the buffer to which the data will be written
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of block size
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - associated SD or MMC card is not opened;
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - EIO - error during communication with SD or MMC card;
	 * - ENOSPC - selected range is invalid;
	 * - error codes returned by executeCmd12();
	 * - error codes returned by executeCmd17();
	 * - error codes returned by executeCmd18();
	 * - error codes returned by readDataBlock();
	 */

	std::pair<int, size_t> read(uint64_t address, void* buffer, size_t size) const;

	/**
	 * \brief Writes data to SD or MMC card connected via SPI.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be written, must be a multiple of block size
	 * \param [in] buffer is the buffer with data that will be written
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of block size
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - associated SD or MMC card is not opened;
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - EIO - error during communication with SD or MMC card;
	 * - ENOSPC - selected range is invalid;
	 * - error codes returned by executeAcmd23();
	 * - error codes returned by executeCmd24();
	 * - error codes returned by executeCmd25();
	 * - error codes returned by waitWhileBusy();
	 * - error codes returned by writeDataBlock();
	 * - error codes returned by SpiMasterProxy::executeTransaction();
	 */

	std::pair<int, size_t> write(uint64_t address, const void* buffer, size_t size) const;

private:

	/**
	 * \brief Deinitializes SD or MMC card connected via SPI.
	 */

	void deinitialize() const;

	/**
	 * \brief Initializes SD or MMC card connected via SPI.
	 *
	 * Algorithm is based on ChaN's
	 * [How to Use MMC/SDC: Initialization Procedure for SPI Mode](http://elm-chan.org/docs/mmc/mmc_e.html#spiinit).
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - associated SD or MMC card is already initialized;
	 * - EIO - error during communication with SD or MMC card;
	 * - ETIMEDOUT - timed-out while waiting for SD or MMC card to respond;
	 * - error codes returned by executeAcmd41();
	 * - error codes returned by executeCmd0();
	 * - error codes returned by executeCmd1();
	 * - error codes returned by executeCmd8();
	 * - error codes returned by executeCmd9();
	 * - error codes returned by executeCmd16();
	 * - error codes returned by executeCmd58();
	 * - error codes returned by SpiMasterProxy::configure();
	 * - error codes returned by SpiMasterProxy::executeTransaction();
	 */

	int initialize() const;

	/// internal proxy for SPI device
	SpiDeviceProxy spiDeviceProxy_;

	/// reference to SpiSdMmcCard associated with this proxy
	SpiSdMmcCard& spiSdMmcCard_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SPISDMMCCARDPROXY_HPP_
