/**
 * \file
 * \brief MemoryTechnologyDevice class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_MEMORYTECHNOLOGYDEVICE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_MEMORYTECHNOLOGYDEVICE_HPP_

#include <utility>

#include <cstddef>
#include <cstdint>

namespace distortos
{

namespace devices
{

/**
 * MemoryTechnologyDevice class is an interface for a memory technology device (flash memory).
 *
 * \ingroup devices
 */

class MemoryTechnologyDevice
{
public:

	/**
	 * \brief MemoryTechnologyDevice's destructor
	 */

	virtual ~MemoryTechnologyDevice() = default;

	/**
	 * \brief Closes device.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the device is already completely closed;
	 */

	virtual int close() = 0;

	/**
	 * \brief Erases blocks on a device.
	 *
	 * \param [in] address is the address of range that will be erased, must be a multiple of erase block size
	 * \param [in] size is the size of erased range, bytes, must be a multiple of erase block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the device is not opened;
	 * - EINVAL - \a address and/or \a size are not valid;
	 * - ENOSPC - selected range is greater than size of device;
	 */

	virtual int erase(uint64_t address, uint64_t size) = 0;

	/**
	 * \return erase block size, bytes
	 */

	virtual size_t getEraseBlockSize() const = 0;

	/**
	 * \return pair with bool telling whether erased value is defined (true) or not (false) and value of erased bytes
	 * (valid only if defined);
	 */

	virtual std::pair<bool, uint8_t> getErasedValue() const = 0;

	/**
	 * \return program block size, bytes
	 */

	virtual size_t getProgramBlockSize() const = 0;

	/**
	 * \return read block size, bytes
	 */

	virtual size_t getReadBlockSize() const = 0;

	/**
	 * \return size of device, bytes
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
	 * \return 0 on success, error code otherwise:
	 * - EAGAIN - the lock could not be acquired because the maximum number of recursive locks for device has been
	 * exceeded;
	 */

	virtual int lock() = 0;

	/**
	 * \brief Opens device.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EMFILE - this device is already opened too many times;
	 */

	virtual int open() = 0;

	/**
	 * \brief Programs data to a device.
	 *
	 * Selected range of blocks must have been erased prior to being programmed.
	 *
	 * \param [in] address is the address of data that will be programmed, must be a multiple of program block size
	 * \param [in] buffer is the buffer with data that will be programmed
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of program block size
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of programmed bytes (valid even
	 * when error code is returned); error codes:
	 * - EBADF - the device is not opened;
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - ENOSPC - selected range is greater than size of device;
	 */

	virtual std::pair<int, size_t> program(uint64_t address, const void* buffer, size_t size) = 0;

	/**
	 * \brief Reads data from a device.
	 *
	 * \param [in] address is the address of data that will be read, must be a multiple of read block size
	 * \param [out] buffer is the buffer into which the data will be read
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of read block size
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - the device is not opened;
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - ENOSPC - selected range is greater than size of device;
	 */

	virtual std::pair<int, size_t> read(uint64_t address, void* buffer, size_t size) = 0;

	/**
	 * \brief Synchronizes state of a device, ensuring all cached writes are finished.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the device is not opened;
	 */

	virtual int synchronize() = 0;

	/**
	 * \brief Unlocks the device which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EPERM - current thread did not lock the device;
	 */

	virtual int unlock() = 0;

	MemoryTechnologyDevice() = default;
	MemoryTechnologyDevice(const MemoryTechnologyDevice&) = delete;
	MemoryTechnologyDevice& operator=(const MemoryTechnologyDevice&) = delete;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_MEMORYTECHNOLOGYDEVICE_HPP_
