/**
 * \file
 * \brief SpiSdMmcCard class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SPISDMMCCARD_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SPISDMMCCARD_HPP_

#include "distortos/devices/communication/SpiDevice.hpp"

#include "distortos/devices/memory/MemoryTechnologyDevice.hpp"

namespace distortos
{

namespace devices
{

/**
 * SpiSdMmcCard class is a SD or MMC card connected via SPI.
 *
 * \ingroup devices
 */

class SpiSdMmcCard : public MemoryTechnologyDevice
{
public:

	/// type of card connected via SPI
	enum class Type : uint8_t
	{
		/// unknown type
		unknown,

		/// MMC card
		mmc,
		/// SD version 1.0 card
		sdVersion1,
		/// SD version 2.0 card
		sdVersion2,
	};

	/// size of block, bytes
	constexpr static size_t blockSize {512};

	/**
	 * \brief SpiSdMmcCard's constructor
	 *
	 * \param [in] spiMaster is a reference to SPI master to which this SD or MMC card is connected
	 * \param [in] slaveSelectPin is a reference to slave select pin of this SD or MMC card
	 * \param [in] clockFrequency is the desired clock frequency of SD or MMC card, Hz, default - 5 MHz
	 */

	constexpr SpiSdMmcCard(SpiMaster& spiMaster, OutputPin& slaveSelectPin, const uint32_t clockFrequency = 5000000) :
			spiDevice_{spiMaster, slaveSelectPin},
			blocksCount_{},
			auSize_{},
			clockFrequency_{clockFrequency},
			eraseTimeoutMs_{},
			readTimeoutMs_{},
			writeTimeoutMs_{},
			blockAddressing_{},
			type_{}
	{

	}

	/**
	 * \brief SpiSdMmcCard's destructor
	 */

	~SpiSdMmcCard() override;

	/**
	 * \brief Closes SD or MMC card connected via SPI.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SpiDevice::close();
	 */

	int close() override;

	/**
	 * \brief Erases blocks on a SD or MMC card connected via SPI.
	 *
	 * \warning This function must not be called from interrupt context!
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
	 * - error codes returned by SpiMasterProxy::configure();
	 */

	int erase(uint64_t address, uint64_t size) override;

	/**
	 * \return erase block size, bytes
	 */

	size_t getEraseBlockSize() const override;

	/**
	 * \return pair with bool telling whether erased value is defined (true) or not (false) and value of erased bytes
	 * (valid only if defined);
	 */

	std::pair<bool, uint8_t> getErasedValue() const override;

	/**
	 * \return program block size, bytes
	 */

	size_t getProgramBlockSize() const override;

	/**
	 * \return read block size, bytes
	 */

	size_t getReadBlockSize() const override;

	/**
	 * \return size of SD or MMC card connected via SPI, bytes
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
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SpiDevice::lock();
	 */

	int lock() override;

	/**
	 * \brief Opens SD or MMC card connected via SPI.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by initialize();
	 * - error codes returned by SpiDevice::open();
	 */

	int open() override;

	/**
	 * \brief Programs data to SD or MMC card connected via SPI.
	 *
	 * Selected range of blocks must have been erased prior to being programmed.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be programmed, must be a multiple of program block size
	 * \param [in] buffer is the buffer with data that will be programmed
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of program block size
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of programmed bytes (valid even
	 * when error code is returned); error codes:
	 * - EBADF - the device is not opened;
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - EIO - error during communication with SD or MMC card;
	 * - ENOSPC - selected range is greater than size of device;
	 * - error codes returned by executeCmd24();
	 * - error codes returned by executeCmd25();
	 * - error codes returned by waitWhileBusy();
	 * - error codes returned by writeDataBlock();
	 * - error codes returned by SpiMasterProxy::configure();
	 * - error codes returned by SpiMasterProxy::executeTransaction();
	 */

	std::pair<int, size_t> program(uint64_t address, const void* buffer, size_t size) override;

	/**
	 * \brief Reads data from SD or MMC card connected via SPI.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be read, must be a multiple of read block size
	 * \param [out] buffer is the buffer into which the data will be read
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of read block size
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - the device is not opened;
	 * - EINVAL - \a address and/or \a buffer and/or \a size are not valid;
	 * - EIO - error during communication with SD or MMC card;
	 * - ENOSPC - selected range is greater than size of device;
	 * - error codes returned by executeCmd12();
	 * - error codes returned by executeCmd17();
	 * - error codes returned by executeCmd18();
	 * - error codes returned by readDataBlock();
	 * - error codes returned by SpiMasterProxy::configure();
	 */

	std::pair<int, size_t> read(uint64_t address, void* buffer, size_t size) override;

	/**
	 * \brief Synchronizes state of SD or MMC card connected via SPI, ensuring all cached writes are finished.
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
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SpiDevice::unlock();
	 */

	int unlock() override;

private:

	/**
	 * \brief Deinitializes SD or MMC card connected via SPI.
	 */

	void deinitialize();

	/**
	 * \brief Initializes SD or MMC card connected via SPI.
	 *
	 * Algorithm is based on ChaN's
	 * [How to Use MMC/SDC: Initialization Procedure for SPI Mode](http://elm-chan.org/docs/mmc/mmc_e.html#spiinit).
	 *
	 * \param [in] spiDeviceProxy is a reference to SpiDeviceProxy associated with this object
	 *
	 * \return 0 on success, error code otherwise:
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

	int initialize(const SpiDeviceProxy& spiDeviceProxy);

	/// internal SPI slave device
	SpiDevice spiDevice_;

	/// number of blocks available on SD or MMC card
	size_t blocksCount_;

	/// size of AU, bytes
	uint32_t auSize_;

	/// desired clock frequency of SD or MMC card, Hz
	uint32_t clockFrequency_;

	/// timeout of erase operation of single AU, milliseconds
	uint16_t eraseTimeoutMs_;

	/// timeout of read operation, milliseconds
	uint16_t readTimeoutMs_;

	/// timeout of write operation, milliseconds
	uint16_t writeTimeoutMs_;

	/// selects whether card uses byte (false) or block (true) addressing
	bool blockAddressing_;

	/// type of card connected via SPI
	Type type_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SPISDMMCCARD_HPP_
