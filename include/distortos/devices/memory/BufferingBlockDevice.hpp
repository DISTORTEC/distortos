/**
 * \file
 * \brief BufferingBlockDevice class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_BUFFERINGBLOCKDEVICE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_BUFFERINGBLOCKDEVICE_HPP_

#include "distortos/devices/memory/BlockDevice.hpp"

namespace distortos
{

namespace devices
{

class BlockDevice;

/**
 * \brief BufferingBlockDevice class is a buffering wrapper for BlockDevice.
 *
 * This class tries to minimize amount of BlockDevice operations by buffering both reads and writes. Such buffering can
 * give signigicant gain in case of devices like SD cards, where each operation may cause the device to become "busy"
 * for a noticeable amount of time. With this class several adjacent reads or writes can be combined into a single
 * larger operation, which is much faster overall when you also include the waits for the SD card to become "idle".
 *
 * Another use for this class is as a proxy between a file system and a block device which requires specific alignment.
 *
 * \ingroup devices
 */

class BufferingBlockDevice : public BlockDevice
{
public:

	/**
	 * \brief BufferingBlockDevice's constructor
	 *
	 * \param [in] blockDevice is a reference to associated block device
	 * \param [in] readBuffer is a pointer to buffer for reads, its address must be aligned to
	 * `DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT` bytes
	 * \param [in] readBufferSize is the size of \a readBuffer, bytes, must be a multiple of \a blockDevice block size
	 * \param [in] writeBuffer is a pointer to buffer for writes, its address must be aligned to
	 * `DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT` bytes
	 * \param [in] writeBufferSize is the size of \a writeBuffer, bytes, must be a multiple of \a blockDevice block size
	 */

	constexpr explicit BufferingBlockDevice(BlockDevice& blockDevice, void* const readBuffer,
			const size_t readBufferSize, void* const writeBuffer, const size_t writeBufferSize) :
					readBufferAddress_{},
					writeBufferAddress_{},
					blockDevice_{blockDevice},
					readBuffer_{readBuffer},
					readBufferSize_{readBufferSize},
					writeBuffer_{writeBuffer},
					writeBufferSize_{writeBufferSize},
					writeBufferValidSize_{},
					openCount_{},
					readBufferValid_{}
	{

	}

	/**
	 * \brief BufferingBlockDevice's destructor
	 *
	 * \pre Device is closed.
	 */

	~BufferingBlockDevice() override;

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
	 * - error codes returned by flushWriteBuffer();
	 * - error codes returned by BlockDevice::close();
	 */

	int close() override;

	/**
	 * \brief Erases blocks on a device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a size are valid.
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of range that will be erased, must be a multiple of block size
	 * \param [in] size is the size of erased range, bytes, must be a multiple of block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by flushWriteBuffer(size_t);
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
	 * \pre Addresses of associated read and write buffers are aligned to `DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT`
	 * bytes.
	 * \pre Sizes of associated read and write buffers are non-zero multiples of associated block device's block size.
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
	 * - error codes returned by readImplementation();
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
	 * - error codes returned by flushWriteBuffer();
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
	 * - error codes returned by flushWriteBuffer();
	 */

	int write(uint64_t address, const void* buffer, size_t size) override;

private:

	/**
	 * \brief Flushes whole write buffer to the associated block device.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by flushWriteBuffer(size_t);
	 */

	int flushWriteBuffer()
	{
		return flushWriteBuffer(writeBufferValidSize_);
	}

	/**
	 * \brief Flushes write buffer to the associated block device.
	 *
	 * \param [in] size is the max amount of data that will be flushed, bytes, must be a multiple of block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::write();
	 */

	int flushWriteBuffer(size_t size);

	/**
	 * \brief Implementation of read()
	 *
	 * \pre Selected range must either not intersect with read buffer or the address of intersection must be equal to
	 * the address of read buffer.
	 *
	 * \param [in] address is the address of data that will be read, must be a multiple of block size
	 * \param [out] buffer is the buffer into which the data will be read, must be valid
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of block size
	 * \param [in] deviceSize is the size of block device, bytes
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by BlockDevice::read();
	 */

	int readImplementation(uint64_t address, void* buffer, size_t size, uint64_t deviceSize);

	/// address of data in read buffer
	uint64_t readBufferAddress_;

	/// address of data in write buffer
	uint64_t writeBufferAddress_;

	/// reference to associated block device
	BlockDevice& blockDevice_;

	/// pointer to buffer for reads
	void* readBuffer_;

	/// size of \a readBuffer_, bytes
	size_t readBufferSize_;

	/// pointer to buffer for writes
	void* writeBuffer_;

	/// size of \a writeBuffer_, bytes
	size_t writeBufferSize_;

	/// amount of data pending to be written in write buffer, bytes
	size_t writeBufferValidSize_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;

	/// true if read buffer holds valid data, false otherwise
	bool readBufferValid_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_BUFFERINGBLOCKDEVICE_HPP_
