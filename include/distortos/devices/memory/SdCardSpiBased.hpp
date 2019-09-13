/**
 * \file
 * \brief SdCardSpiBased class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SDCARDSPIBASED_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SDCARDSPIBASED_HPP_

#include "distortos/devices/memory/BlockDevice.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace devices
{

class OutputPin;
class SpiMaster;

/**
 * \brief SdCardSpiBased class is a SD card connected via SPI.
 *
 * This class supports SD version 2.0 cards only.
 *
 * \ingroup devices
 */

class SdCardSpiBased : public BlockDevice
{
public:

	/// size of block, bytes
	constexpr static size_t blockSize {512};

	/**
	 * \brief SdCardSpiBased's constructor
	 *
	 * \param [in] spiMaster is a reference to SPI master to which this SD card is connected
	 * \param [in] slaveSelectPin is a reference to slave select pin of this SD card
	 * \param [in] clockFrequency is the desired clock frequency of SD card, Hz, default - 25 MHz
	 */

	constexpr SdCardSpiBased(SpiMaster& spiMaster, OutputPin& slaveSelectPin,
			const uint32_t clockFrequency = 25000000) :
					mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
					blocksCount_{},
					auSize_{},
					clockFrequency_{clockFrequency},
					slaveSelectPin_{slaveSelectPin},
					spiMaster_{spiMaster},
					eraseTimeoutMs_{},
					readTimeoutMs_{},
					writeTimeoutMs_{},
					blockAddressing_{},
					openCount_{}
	{

	}

	/**
	 * \brief SdCardSpiBased's destructor
	 *
	 * \pre Device is closed.
	 */

	~SdCardSpiBased() override;

	/**
	 * \brief Closes SD card connected via SPI.
	 *
	 * \note Even if error code is returned, the device must not be used from the context which opened it (until it is
	 * successfully opened again).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise
	 */

	int close() override;

	/**
	 * \brief Erases blocks on a SD card connected via SPI.
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
	 * - error codes returned by executeCmd32();
	 * - error codes returned by executeCmd33();
	 * - error codes returned by executeCmd38();
	 */

	int erase(uint64_t address, uint64_t size) override;

	/**
	 * \return block size, bytes
	 */

	size_t getBlockSize() const override;

	/**
	 * \return size of SD card connected via SPI, bytes
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
	 * \brief Opens SD card connected via SPI.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The number of times the device is opened is less than 255.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by initialize();
	 * - error codes returned by SpiMasterHandle::open();
	 */

	int open() override;

	/**
	 * \brief Reads data from SD card connected via SPI.
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
	 * - error codes returned by readDataBlock();
	 */

	int read(uint64_t address, void* buffer, size_t size) override;

	/**
	 * \brief Synchronizes state of SD card connected via SPI, ensuring all cached writes are finished.
	 *
	 * \pre Device is opened.
	 *
	 * \return always 0
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
	 * \brief Writes data to SD card connected via SPI.
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
	 * - error codes returned by executeCmd24();
	 * - error codes returned by executeCmd25();
	 * - error codes returned by waitWhileBusy();
	 * - error codes returned by writeDataBlock();
	 * - error codes returned by SpiMasterHandle::executeTransaction();
	 */

	int write(uint64_t address, const void* buffer, size_t size) override;

private:

	/**
	 * \brief Deinitializes SD card connected via SPI.
	 */

	void deinitialize();

	/**
	 * \brief Initializes SD card connected via SPI.
	 *
	 * Algorithm is based on ChaN's
	 * [How to Use MMC/SDC: Initialization Procedure for SPI Mode](http://elm-chan.org/docs/mmc/mmc_e.html#spiinit).
	 *
	 * \return 0 on success, error code otherwise:
	 * - EIO - error during communication with SD card;
	 * - ETIMEDOUT - timed-out while waiting for SD card to respond;
	 * - error codes returned by executeAcmd41();
	 * - error codes returned by executeCmd0();
	 * - error codes returned by executeCmd8();
	 * - error codes returned by executeCmd9();
	 * - error codes returned by executeCmd16();
	 * - error codes returned by executeCmd58();
	 * - error codes returned by SpiMasterHandle::executeTransaction();
	 */

	int initialize();

	/// mutex used to serialize access to this object
	Mutex mutex_;

	/// number of blocks available on SD card
	size_t blocksCount_;

	/// size of AU, bytes
	uint32_t auSize_;

	/// desired clock frequency of SD card, Hz
	uint32_t clockFrequency_;

	/// reference to slave select pin of this SD card
	OutputPin& slaveSelectPin_;

	/// reference to SPI master to which this SD card is connected
	SpiMaster& spiMaster_;

	/// timeout of erase operation of single AU, milliseconds
	uint16_t eraseTimeoutMs_;

	/// timeout of read operation, milliseconds
	uint16_t readTimeoutMs_;

	/// timeout of write operation, milliseconds
	uint16_t writeTimeoutMs_;

	/// selects whether card uses byte (false) or block (true) addressing
	bool blockAddressing_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SDCARDSPIBASED_HPP_
