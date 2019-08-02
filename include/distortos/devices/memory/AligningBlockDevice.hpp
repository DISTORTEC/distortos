/**
 * \file
 * \brief AligningBlockDevice class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_ALIGNINGBLOCKDEVICE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_ALIGNINGBLOCKDEVICE_HPP_

#include "distortos/devices/memory/BlockDevice.hpp"

namespace distortos
{

namespace devices
{

class BlockDevice;

/**
 * AligningBlockDevice class is a wrapper for BlockDevice which allows to use unaligned buffers with `read()` and
 * `write()`.
 *
 * Common use for this class is as a proxy between a file system and a block device which requires specific alignment.
 *
 * \ingroup devices
 */

class AligningBlockDevice : public BlockDevice
{
public:

	/**
	 * \brief AligningBlockDevice's constructor
	 *
	 * \param [in] blockDevice is a reference to associated block device
	 * \param [in] buffer is a pointer to buffer, its address must be aligned to
	 * `DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT` bytes
	 * \param [in] bufferSize is the size of \a buffer, must be a multiple of \a blockDevice block size
	 */

	constexpr explicit AligningBlockDevice(BlockDevice& blockDevice, void* const buffer, const size_t bufferSize) :
			blockDevice_{blockDevice},
			buffer_{buffer},
			bufferSize_{bufferSize},
			openCount_{}
	{

	}

	/**
	 * \brief AligningBlockDevice's destructor
	 *
	 * \pre Device is closed.
	 */

	~AligningBlockDevice() override;

	/**
	 * \brief Closes device.
	 *
	 * \note Even if error code is returned, the device must not be used from the context which opened it (until it is
	 * successfully opened again).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::close();
	 */

	int close() override;

	/**
	 * \brief Erases blocks on a device.
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a size are valid.
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of range that will be erased, must be a multiple of block size
	 * \param [in] size is the size of erased range, bytes, must be a multiple of block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::erase();
	 */

	int erase(uint64_t address, uint64_t size) override;

	/**
	 * \return block size, bytes
	 */

	size_t getBlockSize() const override;

	/**
	 * \return size of block device, bytes
	 */

	uint64_t getSize() const override;

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

	void lock() override;

	/**
	 * \brief Opens device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The number of times the device is opened is less than 255.
	 * \pre Address of associated buffer is aligned to `DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT` bytes.
	 * \pre Size of associated buffer is a non-zero multiple of associated block device's block size.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::open();
	 */

	int open() override;

	/**
	 * \brief Reads data from a device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a buffer and \a size are valid.
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of data that will be read, must be a multiple of block size
	 * \param [out] buffer is the buffer into which the data will be read, must be valid
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::read();
	 */

	int read(uint64_t address, void* buffer, size_t size) override;

	/**
	 * \brief Synchronizes state of a device, ensuring all cached writes are finished.
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::synchronize();
	 */

	int synchronize() override;

	/**
	 * \brief Unlocks the device which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the device.
	 */

	void unlock() override;

	/**
	 * \brief Writes data to a device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a buffer and \a size are valid.
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of data that will be written, must be a multiple of block size
	 * \param [in] buffer is the buffer with data that will be written, must be valid
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::write();
	 */

	int write(uint64_t address, const void* buffer, size_t size) override;

private:

	/// reference to associated block device
	BlockDevice& blockDevice_;

	/// pointer to buffer, its address must be aligned to `DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT` bytes
	void* buffer_;

	/// size of \a buffer_, must be a multiple of \a blockDevice_ block size
	size_t bufferSize_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_ALIGNINGBLOCKDEVICE_HPP_
