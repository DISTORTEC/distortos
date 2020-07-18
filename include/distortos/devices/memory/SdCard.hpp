/**
 * \file
 * \brief SdCard class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SDCARD_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SDCARD_HPP_

#include "distortos/devices/memory/BlockDevice.hpp"
#include "distortos/devices/memory/SynchronousSdMmcCardLowLevel.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace devices
{

class SdMmcCardLowLevel;

/**
 * \brief SdCard class is a SD card.
 *
 * This class supports SD version 2.0 cards only.
 *
 * \ingroup devices
 */

class SdCard : public BlockDevice
{
public:

	/// size of block, bytes
	constexpr static size_t blockSize {512};

	/**
	 * \brief SdCard's constructor
	 *
	 * \param [in] sdMmcCard is a reference to low-level implementation of SdMmcCardLowLevel interface
	 * \param [in] _4BitBusMode selects whether 1-bit (false) or 4-bit (true) bus mode will be used, default - true
	 * \param [in] maxClockFrequency is the max allowed clock frequency of SD card, Hz, default - 25 MHz
	 */

	constexpr explicit SdCard(SdMmcCardLowLevel& sdMmcCard, const bool _4BitBusMode = true,
			const uint32_t maxClockFrequency = 25000000) :
					busyDeadline_{},
					mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
					sdCard_{sdMmcCard},
					auSize_{},
					blocksCount_{},
					maxClockFrequency_{maxClockFrequency},
					eraseTimeoutMs_{},
					rca_{},
					readTimeoutMs_{},
					writeTimeoutMs_{},
					_4BitBusMode_{_4BitBusMode},
					blockAddressing_{},
					openCount_{}
	{

	}

	/**
	 * \brief SdCard's destructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is closed.
	 */

	~SdCard() override;

	/**
	 * \brief Closes SD card.
	 *
	 * \note Even if error code is returned, the device must not be used from the context which opened it (until it is
	 * successfully opened again).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by waitForTransferState();
	 */

	int close() override;

	/**
	 * \brief Erases blocks on a SD card.
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
	 * - EIO - error during communication with SD card;
	 * - error codes returned by executeCmd32();
	 * - error codes returned by executeCmd33();
	 * - error codes returned by executeCmd38();
	 * - error codes returned by waitForTransferState();
	 */

	int erase(uint64_t address, uint64_t size) override;

	/**
	 * \return block size, bytes
	 */

	size_t getBlockSize() const override;

	/**
	 * \return size of SD card, bytes
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
	 * \brief Opens SD card.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The number of times the device is opened is less than 255.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by initialize();
	 * - error codes returned by SynchronousSdMmcCardLowLevel::start();
	 */

	int open() override;

	/**
	 * \brief Reads data from SD card.
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
	 * - EIO - error during communication with SD card;
	 * - error codes returned by executeCmd12();
	 * - error codes returned by executeCmd17();
	 * - error codes returned by executeCmd18();
	 * - error codes returned by waitForTransferState();
	 */

	int read(uint64_t address, void* buffer, size_t size) override;

	/**
	 * \brief Synchronizes state of SD card, ensuring all cached writes are finished.
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by waitForTransferState();
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
	 * \brief Writes data to SD card.
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
	 * - EIO - error during communication with SD card;
	 * - error codes returned by executeAcmd23();
	 * - error codes returned by executeCmd12();
	 * - error codes returned by executeCmd24();
	 * - error codes returned by executeCmd25();
	 */

	int write(uint64_t address, const void* buffer, size_t size) override;

private:

	/**
	 * \brief Deinitializes SD card.
	 */

	void deinitialize();

	/**
	 * \brief Initializes SD card.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EIO - error during communication with SD card;
	 * - ENOTSUP - card is not supported;
	 * - ETIMEDOUT - timed-out while waiting for SD card to respond;
	 * - error codes returned by executeAcmd6();
	 * - error codes returned by executeAcmd13();
	 * - error codes returned by executeAcmd41();
	 * - error codes returned by executeCmd2();
	 * - error codes returned by executeCmd3();
	 * - error codes returned by executeCmd7();
	 * - error codes returned by executeCmd8();
	 * - error codes returned by executeCmd9();
	 * - error codes returned by executeCmd16();
	 */

	int initialize();

	/// current deadline of waiting while card is busy
	TickClock::time_point busyDeadline_;

	/// mutex used to serialize access to this object
	Mutex mutex_;

	/// synchronous wrapper for low-level implementation of SdMmcCardLowLevel interface
	SynchronousSdMmcCardLowLevel sdCard_;

	/// size of AU, bytes
	uint32_t auSize_;

	/// number of blocks available on SD card
	size_t blocksCount_;

	/// max allowed clock frequency of SD card, Hz
	uint32_t maxClockFrequency_;

	/// timeout of erase operation of single AU, milliseconds
	uint16_t eraseTimeoutMs_;

	/// relative card address
	uint16_t rca_;

	/// timeout of read operation, milliseconds
	uint16_t readTimeoutMs_;

	/// timeout of write operation, milliseconds
	uint16_t writeTimeoutMs_;

	/// selects whether 1-bit (false) or 4-bit (true) bus mode will be used
	bool _4BitBusMode_;

	/// selects whether card uses byte (false) or block (true) addressing
	bool blockAddressing_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SDCARD_HPP_
