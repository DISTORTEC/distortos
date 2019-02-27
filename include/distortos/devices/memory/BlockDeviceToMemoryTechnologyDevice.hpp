/**
 * \file
 * \brief BlockDeviceToMemoryTechnologyDevice class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_BLOCKDEVICETOMEMORYTECHNOLOGYDEVICE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_BLOCKDEVICETOMEMORYTECHNOLOGYDEVICE_HPP_

#include "distortos/devices/memory/MemoryTechnologyDevice.hpp"

namespace distortos
{

namespace devices
{

class BlockDevice;

/**
 * BlockDeviceToMemoryTechnologyDevice class is a wrapper for BlockDevice which implements MemoryTechnologyDevice
 * interface by buffering erase operations.
 *
 * \ingroup devices
 */

class BlockDeviceToMemoryTechnologyDevice : public MemoryTechnologyDevice
{
public:

	/**
	 * \brief BlockDeviceToMemoryTechnologyDevice's constructor
	 *
	 * \param [in] blockDevice is a reference to associated block device
	 */

	constexpr explicit BlockDeviceToMemoryTechnologyDevice(BlockDevice& blockDevice) :
			pendingEraseAddress_{},
			pendingEraseSize_{},
			blockDevice_{blockDevice},
			openCount_{}
	{

	}

	/**
	 * \brief BlockDeviceToMemoryTechnologyDevice's destructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is closed.
	 */

	~BlockDeviceToMemoryTechnologyDevice() override;

	/**
	 * \brief Closes device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::close();
	 * - error codes returned by BlockDevice::erase();
	 */

	int close() override;

	/**
	 * \brief Erases blocks on a device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a size are valid.
	 *
	 * \param [in] address is the address of range that will be erased, must be a multiple of erase block size
	 * \param [in] size is the size of erased range, bytes, must be a multiple of erase block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOSPC - selected range is greater than size of device;
	 * - error codes returned by BlockDevice::erase();
	 */

	int erase(uint64_t address, uint64_t size) override;

	/**
	 * \return erase block size, bytes
	 */

	size_t getEraseBlockSize() const override;

	/**
	 * \return program block size, bytes
	 */

	size_t getProgramBlockSize() const override;

	/**
	 * \return read block size, bytes
	 */

	size_t getReadBlockSize() const override;

	/**
	 * \return size of device, bytes
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
	 * \return 0 on success, error code otherwise:
	 * - EMFILE - this device is already opened too many times;
	 * - error codes returned by BlockDevice::open();
	 */

	int open() override;

	/**
	 * \brief Programs data to a device.
	 *
	 * Selected range of blocks must have been erased prior to being programmed.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 *
	 * \param [in] address is the address of data that will be programmed, must be a multiple of program block size
	 * \param [in] buffer is the buffer with data that will be programmed
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of program block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - ENOSPC - selected range is greater than size of device;
	 * - error codes returned by BlockDevice::erase();
	 * - error codes returned by BlockDevice::write();
	 */

	int program(uint64_t address, const void* buffer, size_t size) override;

	/**
	 * \brief Reads data from a device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 *
	 * \param [in] address is the address of data that will be read, must be a multiple of read block size
	 * \param [out] buffer is the buffer into which the data will be read
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of read block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - ENOSPC - selected range is greater than size of device;
	 * - error codes returned by BlockDevice::erase();
	 * - error codes returned by BlockDevice::read();
	 */

	int read(uint64_t address, void* buffer, size_t size) override;

	/**
	 * \brief Synchronizes state of a device, ensuring all cached writes are finished.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::erase();
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

private:

	/// address of pending erase range, valid only if `pendingEraseSize_ != 0`
	uint64_t pendingEraseAddress_;

	/// size of pending erase range
	uint64_t pendingEraseSize_;

	/// reference to associated block device
	BlockDevice& blockDevice_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_BLOCKDEVICETOMEMORYTECHNOLOGYDEVICE_HPP_
