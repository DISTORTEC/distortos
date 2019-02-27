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
	 *
	 * \pre Device is closed.
	 */

	virtual ~MemoryTechnologyDevice() = default;

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
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of range that will be erased, must be a multiple of erase block size
	 * \param [in] size is the size of erased range, bytes, must be a multiple of erase block size
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int erase(uint64_t address, uint64_t size) = 0;

	/**
	 * \return erase block size, bytes
	 */

	virtual size_t getEraseBlockSize() const = 0;

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
	 * \pre The number of recursive locks of device is less than 65535.
	 *
	 * \post Device is locked.
	 */

	virtual void lock() = 0;

	/**
	 * \brief Opens device.
	 *
	 * \pre The number of times the device is opened is less than 255.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int open() = 0;

	/**
	 * \brief Programs data to a device.
	 *
	 * Selected range of blocks must have been erased prior to being programmed.
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a buffer and \a size are valid.
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of data that will be programmed, must be a multiple of program block size
	 * \param [in] buffer is the buffer with data that will be programmed, must be valid
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of program block size
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int program(uint64_t address, const void* buffer, size_t size) = 0;

	/**
	 * \brief Reads data from a device.
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a buffer and \a size are valid.
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of data that will be read, must be a multiple of read block size
	 * \param [out] buffer is the buffer into which the data will be read, must be valid
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of read block size
	 *
	 * \return 0 on success, error code otherwise
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

	MemoryTechnologyDevice() = default;
	MemoryTechnologyDevice(const MemoryTechnologyDevice&) = delete;
	MemoryTechnologyDevice& operator=(const MemoryTechnologyDevice&) = delete;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_MEMORYTECHNOLOGYDEVICE_HPP_
