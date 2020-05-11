/**
 * \file
 * \brief QspiNorFlashSpiBased class header
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_QSPINORFLASHSPIBASED_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_QSPINORFLASHSPIBASED_HPP_

#include "distortos/devices/communication/SpiMode.hpp"

#include "distortos/devices/memory/MemoryTechnologyDevice.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace devices
{

class OutputPin;
class SpiMaster;

/**
 * \brief QspiNorFlashSpiBased class is a QSPI NOR flash connected via SPI.
 *
 * This class supports chips which implement SFDP from JESD216 standard.
 *
 * \ingroup devices
 */

class QspiNorFlashSpiBased : public MemoryTechnologyDevice
{
public:

	/// flags with supported addressing modes
	enum class AddressFlags : uint8_t
	{
		/// 3-byte addressing
		_3 = 1 << 0,
		/// 4-byte addressing
		_4 = 1 << 1,
	};

	/// flags with supported software reset sequences
	enum class SoftwareResetFlags : uint8_t
	{
		// drive 0xf on all 4 data wires for 8 clocks
		highState8Clocks = 1 << 0,
		// drive 0xf on all 4 data wires for 10 clocks if device is operating in 4-byte addressing mode
		highState10Clocks4ByteAddressingMode = 1 << 1,
		// drive 0xf on all 4 data wires for 16 clocks
		highState16Clocks = 1 << 2,
		// issue instruction 0xf0
		_0xf0 = 1 << 3,
		// issue instruction 0x66 ("reset enable"), then issue instruction 0x99 ("reset"), the sequence may be issued on
		/// 1, 2, 4 or 8 wires, depending on the device operating mode
		_0x66_0x99 = 1 << 4,
		// exit from 0-4-4 mode is required prior to other reset sequences above if the device may be operating in this
		/// mode
		exit_0_4_4_Required = 1 << 5,
	};

	/// basic flash parameters
	struct BasicFlashParameters
	{
		constexpr static uint8_t maxEraseTypes {4};

		/// size of flash, bytes
		uint64_t size;
		/// sizes of each erase type, bytes
		size_t eraseSizes[maxEraseTypes];
		/// maximum erase time of each erase type, milliseconds
		uint32_t maximumEraseTimesMs[maxEraseTypes];
		/// size of page, bytes
		uint16_t pageSize;
		/// flags with supported addressing modes
		AddressFlags addressFlags;
		/// instructions of each erase type
		uint8_t eraseInstructions[maxEraseTypes];
		/// flags with supported software reset sequences
		SoftwareResetFlags softwareResetFlags;
	};

	/// currently selected sector map
	struct SectorMap
	{
		/// max number of supported regions in sector map
		constexpr static uint8_t maxRegionCount {2};

		/// size of each region, bytes
		uint64_t sizes[maxRegionCount];
		/// erase types supported by each region
		uint8_t eraseTypes[maxRegionCount];
		/// number of regions
		uint8_t regionCount;
	};

	/**
	 * \brief QspiNorFlashSpiBased's constructor
	 *
	 * \param [in] spiMaster is a reference to SPI master to which this QSPI NOR flash is connected
	 * \param [in] slaveSelectPin is a reference to slave select pin of this QSPI NOR flash
	 * \param [in] mode3 selects whether SPI mode 0 - CPOL == 0, CPHA == 0 - (false) or SPI mode 3 - CPOL == 1,
	 * CPHA == 1 - (true) will be used, default - SPI mode 0 (false)
	 * \param [in] clockFrequency is the desired clock frequency of QSPI NOR flash, Hz, default - 10 MHz
	 */

	constexpr QspiNorFlashSpiBased(SpiMaster& spiMaster, OutputPin& slaveSelectPin, const bool mode3 = {},
			const uint32_t clockFrequency = 10000000) :
					basicFlashParameters_{},
					busyDeadline_{},
					mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
					sectorMap_{},
					clockFrequency_{clockFrequency},
					slaveSelectPin_{slaveSelectPin},
					spiMaster_{spiMaster},
					commonEraseIndex_{},
					mode_{mode3 == false ? SpiMode::_0 : SpiMode::_3},
					openCount_{}
	{

	}

	/**
	 * \brief QspiNorFlashSpiBased's destructor
	 *
	 * \pre Device is closed.
	 */

	~QspiNorFlashSpiBased() override;

	/**
	 * \brief Closes QSPI NOR flash.
	 *
	 * \note Even if error code is returned, the device must not be used from the context which opened it (until it is
	 * successfully opened again).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by waitWhileWriteInProgress();
	 */

	int close() override;

	/**
	 * \brief Erases blocks on QSPI NOR flash.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a size are valid.
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of range that will be erased, must be a multiple of erase block size
	 * \param [in] size is the size of erased range, bytes, must be a multiple of erase block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by executeCommand();
	 * - error codes returned by executeWren();
	 * - error codes returned by waitWhileWriteInProgress();
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
	 * \return size of QSPI NOR flash, bytes
	 */

	uint64_t getSize() const override;

	/**
	 * \brief Locks QSPI NOR flash for exclusive use by current thread.
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
	 * \brief Opens QSPI NOR flash.
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
	 * \brief Programs data to QSPI NOR flash.
	 *
	 * Selected range of blocks must have been erased prior to being programmed.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a buffer and \a size are valid.
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of data that will be programmed
	 * \param [in] buffer is the buffer with data that will be programmed, must be valid
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by executeWren();
	 * - error codes returned by executeWriteCommand();
	 * - error codes returned by waitWhileWriteInProgress();
	 */

	int program(uint64_t address, const void* buffer, size_t size) override;

	/**
	 * \brief Reads data from QSPI NOR flash.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 * \pre \a address and \a buffer and \a size are valid.
	 * \pre Selected range is within address space of device.
	 *
	 * \param [in] address is the address of data that will be read
	 * \param [out] buffer is the buffer into which the data will be read, must be valid
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by executeReadCommand();
	 * - error codes returned by waitWhileWriteInProgress();
	 */

	int read(uint64_t address, void* buffer, size_t size) override;

	/**
	 * \brief Synchronizes state of QSPI NOR flash, ensuring all cached writes are finished.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Device is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by waitWhileWriteInProgress();
	 */

	int synchronize() override;

	/**
	 * \brief Unlocks QSPI NOR flash which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the device.
	 */

	void unlock() override;

private:

	/**
	 * \brief Deinitializes QSPI NOR flash.
	 */

	void deinitialize();

	/**
	 * \brief Handles manufacturer- and device-specific fixups.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by executeReadCommand();
	 * - error codes returned by executeWren();
	 * - error codes returned by executeWriteCommand();
	 * - error codes returned by readManufacturerDeviceId();
	 */

	int handleFixups();

	/**
	 * \brief Initializes QSPI NOR flash.
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOTSUP - detected erase types are not supported by this implementation;
	 * - error codes returned by handleFixups();
	 * - error codes returned by parseSfdp();
	 */

	int initialize();

	/**
	 * \brief Parses SFDP.
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOTSUP - SFDP is invalid and cannot be parsed;
	 * - ENOTSUP - parameter contained in SFDP is not supported by this implementation;
	 * - ETIMEDOUT - timed-out while waiting for QSPI NOR flash to respond after software reset;
	 * - error codes returned by executeCommand();
	 * - error codes returned by readParameterHeaders();
	 * - error codes returned by readSfdpHeader();
	 * - error codes returned by parseBasicFlashParameterTable();
	 * - error codes returned by parseSectorMapTable();
	 */

	int parseSfdp();

	/**
	 * \brief Waits while any write operation is currently in progress.
	 *
	 * \param [in] timePoint is the time point at which the wait will be terminated
	 *
	 * \return 0 on success, error code otherwise:
	 * - ETIMEDOUT - write operation was not finished before the specified timeout expired;
	 * - error codes returned by readStatusRegister1();
	 */

	int waitWhileWriteInProgress(TickClock::time_point timePoint);

	/// basic flash parameters
	BasicFlashParameters basicFlashParameters_;

	/// current deadline of waiting while write operation is in progress
	TickClock::time_point busyDeadline_;

	/// mutex used to serialize access to this object
	Mutex mutex_;

	/// currently selected sector map
	SectorMap sectorMap_;

	/// desired clock frequency of QSPI NOR flash, Hz
	uint32_t clockFrequency_;

	/// reference to slave select pin of this QSPI NOR flash
	OutputPin& slaveSelectPin_;

	/// reference to SPI master to which this QSPI NOR flash is connected
	SpiMaster& spiMaster_;

	/// index of largest erase type supported by each block of QSPI NOR flash
	uint8_t commonEraseIndex_;

	/// SPI mode used by QSPI NOR flash
	SpiMode mode_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_QSPINORFLASHSPIBASED_HPP_
