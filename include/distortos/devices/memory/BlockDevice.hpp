/**
 * \file
 * \brief BlockDevice class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_BLOCKDEVICE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_BLOCKDEVICE_HPP_

#include <cstddef>
#include <cstdint>

namespace distortos
{

namespace devices
{

/**
 * BlockDevice class is an interface for a block device.
 *
 * \ingroup devices
 */

class BlockDevice
{
public:

	/**
	 * \brief BlockDevice's destructor
	 *
	 * \pre Device is closed.
	 */

	virtual ~BlockDevice() = default;

	/**
	 * \brief Closes device.
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int close() = 0;

	/**
	 * \brief Erases blocks on a device.
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a size are valid.
	 *
	 * \param [in] address is the address of range that will be erased, must be a multiple of block size
	 * \param [in] size is the size of erased range, bytes, must be a multiple of block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOSPC - selected range is greater than size of device;
	 */

	virtual int erase(uint64_t address, uint64_t size) = 0;

	/**
	 * \return block size, bytes
	 */

	virtual size_t getBlockSize() const = 0;

	/**
	 * \return size of block device, bytes
	 */

	virtual uint64_t getSize() const = 0;

	/**
	 * \brief Locks the device for exclusive use by current thread.
	 *
	 * When the object is locked, any call to any member function from other thread will be blocked until the object is
	 * unlocked. Locking is optional, but may be useful when more than one transaction must be done atomically.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The number of recursive locks of device is less than 65535.
	 *
	 * \post Device is locked.
	 */

	virtual void lock() = 0;

	/**
	 * \brief Opens device.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EMFILE - this device is already opened too many times;
	 */

	virtual int open() = 0;

	/**
	 * \brief Reads data from a device.
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a buffer and \a size are valid.
	 *
	 * \param [in] address is the address of data that will be read, must be a multiple of block size
	 * \param [out] buffer is the buffer into which the data will be read, must be valid
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOSPC - selected range is greater than size of device;
	 */

	virtual int read(uint64_t address, void* buffer, size_t size) = 0;

	/**
	 * \brief Synchronizes state of a device, ensuring all cached writes are finished.
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int synchronize() = 0;

	/**
	 * \brief Unlocks the device which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the device.
	 */

	virtual void unlock() = 0;

	/**
	 * \brief Writes data to a device.
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a buffer and \a size are valid.
	 *
	 * \param [in] address is the address of data that will be written, must be a multiple of block size
	 * \param [in] buffer is the buffer with data that will be written, must be valid
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOSPC - selected range is greater than size of device;
	 */

	virtual int write(uint64_t address, const void* buffer, size_t size) = 0;

	BlockDevice() = default;
	BlockDevice(const BlockDevice&) = delete;
	BlockDevice& operator=(const BlockDevice&) = delete;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_BLOCKDEVICE_HPP_
