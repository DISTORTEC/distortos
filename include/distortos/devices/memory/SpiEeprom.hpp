/**
 * \file
 * \brief SpiEeprom class header
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROM_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROM_HPP_

#include "distortos/devices/communication/SpiDevice.hpp"

namespace distortos
{

namespace devices
{

class SpiEepromProxy;

/**
 * SpiEeprom class is a SPI EEPROM memory: Atmel AT25xxx, ON Semiconductor CAT25xxx, ST M95xxx, Microchip 25xxxxx or
 * similar.
 *
 * \ingroup devices
 */

class SpiEeprom
{
	friend class SpiEepromProxy;

	/// bit shift of field with page size encoded in device's type
	constexpr static size_t pageSizeShift_ {0};

	/// bit width of field with page size encoded in device's type
	constexpr static size_t pageSizeWidth_ {3};

	/// bit mask of field with page size encoded in device's type
	constexpr static size_t pageSizeMask_ {((1 << pageSizeWidth_) - 1) << pageSizeShift_};

	/// bit shift of field with capacity encoded in device's type
	constexpr static size_t capacityShift_ {pageSizeWidth_};

	/// bit width of field with capacity encoded in device's type
	constexpr static size_t capacityWidth_ {8 - pageSizeWidth_};

	/// bit mask of field with capacity encoded in device's type
	constexpr static size_t capacityMask_ {((1 << capacityWidth_) - 1) << capacityShift_};

public:

	/// import SpiEepromProxy as SpiEeprom::Proxy
	using Proxy = SpiEepromProxy;

	/// type of device - determines capacity and page size
	enum class Type : uint8_t
	{
		/// 128 B (1 kb) capacity, 8 bytes per page
		_128Bytes8BytesPerPage = 0 << capacityShift_ | 0 << pageSizeShift_,
		/// 128 B (1 kb) capacity, 16 bytes per page
		_128Bytes16BytesPerPage = 0 << capacityShift_ | 1 << pageSizeShift_,
		/// 256 B (2 kb) capacity, 8 bytes per page
		_256Bytes8BytesPerPage = 1 << capacityShift_ | 0 << pageSizeShift_,
		/// 256 B (2 kb) capacity, 16 bytes per page
		_256Bytes16BytesPerPage = 1 << capacityShift_ | 1 << pageSizeShift_,
		/// 512 B (4 kb) capacity, 8 bytes per page
		_512Bytes8BytesPerPage = 2 << capacityShift_ | 0 << pageSizeShift_,
		/// 512 B (4 kb) capacity, 16 bytes per page
		_512Bytes16BytesPerPage = 2 << capacityShift_ | 1 << pageSizeShift_,
		/// 1 kB (8 kb) capacity, 16 bytes per page
		_1KBytes16BytesPerPage = 3 << capacityShift_ | 1 << pageSizeShift_,
		/// 1 kB (8 kb) capacity, 32 bytes per page
		_1KBytes32BytesPerPage = 3 << capacityShift_ | 2 << pageSizeShift_,
		/// 2 kB (16 kb) capacity, 16 bytes per page
		_2KBytes16BytesPerPage = 4 << capacityShift_ | 1 << pageSizeShift_,
		/// 2 kB (16 kb) capacity, 32 bytes per page
		_2KBytes32BytesPerPage = 4 << capacityShift_ | 2 << pageSizeShift_,
		/// 4 kB (32 kb) capacity, 32 bytes per page
		_4KBytes32BytesPerPage = 5 << capacityShift_ | 2 << pageSizeShift_,
		/// 8 kB (64 kb) capacity, 32 bytes per page
		_8KBytes32BytesPerPage = 6 << capacityShift_ | 2 << pageSizeShift_,
		/// 8 kB (64 kb) capacity, 64 bytes per page
		_8KBytes64BytesPerPage = 6 << capacityShift_ | 3 << pageSizeShift_,
		/// 16 kB (128 kb) capacity, 64 bytes per page
		_16KBytes64BytesPerPage = 7 << capacityShift_ | 3 << pageSizeShift_,
		/// 32 kB (256 kb) capacity, 64 bytes per page
		_32KBytes64BytesPerPage = 8 << capacityShift_ | 3 << pageSizeShift_,
		/// 64 kB (512 kb) capacity, 128 bytes per page
		_64KBytes128BytesPerPage = 9 << capacityShift_ | 4 << pageSizeShift_,
		/// 128 kB (1 Mb) capacity, 256 bytes per page
		_128KBytes256BytesPerPage = 10 << capacityShift_ | 5 << pageSizeShift_,
		/// 256 kB (2 Mb) capacity, 256 bytes per page
		_256KBytes256BytesPerPage = 11 << capacityShift_ | 5 << pageSizeShift_,

		/// Atmel AT25010
		at25010 = _128Bytes8BytesPerPage,
		/// Atmel AT25020
		at25020 = _256Bytes8BytesPerPage,
		/// Atmel AT25040
		at25040 = _512Bytes8BytesPerPage,
		/// Atmel AT25080
		at25080 = _1KBytes32BytesPerPage,
		/// Atmel AT25160
		at25160 = _2KBytes32BytesPerPage,
		/// Atmel AT25320
		at25320 = _4KBytes32BytesPerPage,
		/// Atmel AT25640
		at25640 = _8KBytes32BytesPerPage,
		/// Atmel AT25128
		at25128 = _16KBytes64BytesPerPage,
		/// Atmel AT25256
		at25256 = _32KBytes64BytesPerPage,
		/// Atmel AT25512
		at25512 = _64KBytes128BytesPerPage,
		/// Atmel AT25M01
		at25m01 = _128KBytes256BytesPerPage,
		/// Atmel AT25M02
		at25m02 = _256KBytes256BytesPerPage,

		/// ON Semiconductor CAT25010
		cat25010 = _128Bytes16BytesPerPage,
		/// ON Semiconductor CAT25020
		cat25020 = _256Bytes16BytesPerPage,
		/// ON Semiconductor CAT25040
		cat25040 = _512Bytes16BytesPerPage,
		/// ON Semiconductor CAT25080
		cat25080 = _1KBytes32BytesPerPage,
		/// ON Semiconductor CAT25160
		cat25160 = _2KBytes32BytesPerPage,
		/// ON Semiconductor CAT25320
		cat25320 = _4KBytes32BytesPerPage,
		/// ON Semiconductor CAT25640
		cat25640 = _8KBytes64BytesPerPage,
		/// ON Semiconductor CAT25128
		cat25128 = _16KBytes64BytesPerPage,
		/// ON Semiconductor CAT25256
		cat25256 = _32KBytes64BytesPerPage,
		/// ON Semiconductor CAT25512
		cat25512 = _64KBytes128BytesPerPage,
		/// ON Semiconductor CAT25M01
		cat25m01 = _128KBytes256BytesPerPage,
		/// ON Semiconductor CAT25M02
		cat25m02 = _256KBytes256BytesPerPage,

		/// ST M95010
		m95010 = _128Bytes16BytesPerPage,
		/// ST M95020
		m95020 = _256Bytes16BytesPerPage,
		/// ST M95040
		m95040 = _512Bytes16BytesPerPage,
		/// ST M95080
		m95080 = _1KBytes32BytesPerPage,
		/// ST M95160
		m95160 = _2KBytes32BytesPerPage,
		/// ST M95320
		m95320 = _4KBytes32BytesPerPage,
		/// ST M95640
		m95640 = _8KBytes32BytesPerPage,
		/// ST M95128
		m95128 = _16KBytes64BytesPerPage,
		/// ST M95256
		m95256 = _32KBytes64BytesPerPage,
		/// ST M95512
		m95512 = _64KBytes128BytesPerPage,
		/// ST M95M01
		m95m01 = _128KBytes256BytesPerPage,
		/// ST M95M02
		m95m02 = _256KBytes256BytesPerPage,

		/// Microchip 25xx010
		_25xx010 = _128Bytes16BytesPerPage,
		/// Microchip 25xx020
		_25xx020 = _256Bytes16BytesPerPage,
		/// Microchip 25xx040
		_25xx040 = _512Bytes16BytesPerPage,
		/// Microchip 25xx080
		_25xx080 = _1KBytes16BytesPerPage,
		/// Microchip 25xx080A
		_25xx080a = _25xx080,
		/// Microchip 25xx080B
		_25xx080b = _1KBytes32BytesPerPage,
		/// Microchip 25xx080C
		_25xx080c = _25xx080,
		/// Microchip 25xx080D
		_25xx080d = _25xx080b,
		/// Microchip 25xx160
		_25xx160 = _2KBytes16BytesPerPage,
		/// Microchip 25xx160A
		_25xx160a = _25xx160,
		/// Microchip 25xx160B
		_25xx160b = _2KBytes32BytesPerPage,
		/// Microchip 25xx160C
		_25xx160c = _25xx160,
		/// Microchip 25xx160D
		_25xx160d = _25xx160b,
		/// Microchip 25xx320
		_25xx320 = _4KBytes32BytesPerPage,
		/// Microchip 25xx640
		_25xx640 = _8KBytes32BytesPerPage,
		/// Microchip 25xx128
		_25xx128 = _16KBytes64BytesPerPage,
		/// Microchip 25xx256
		_25xx256 = _32KBytes64BytesPerPage,
		/// Microchip 25xx512
		_25xx512 = _64KBytes128BytesPerPage,
		/// Microchip 25xx1024
		_25xx1024 = _128KBytes256BytesPerPage,
	};

	/**
	 * \brief SpiEeprom's constructor
	 *
	 * \param [in] spiMaster is a reference to SPI master to which this SPI EEPROM is connected
	 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI EEPROM
	 * \param [in] type is the type of SPI EEPROM
	 * \param [in] mode3 selects whether SPI mode 0 - CPOL == 0, CPHA == 0 - (false) or SPI mode 3 - CPOL == 1,
	 * CPHA == 1 - (true) will be used, default - SPI mode 0 (false)
	 * \param [in] clockFrequency is the desired clock frequency of SPI EEPROM, Hz, default - 1 MHz
	 */

	constexpr SpiEeprom(SpiMaster& spiMaster, OutputPin& slaveSelectPin, const Type type, const bool mode3 = {},
			const uint32_t clockFrequency = 1000000) :
					spiDevice_{spiMaster, slaveSelectPin},
					slaveSelectPin_{slaveSelectPin},
					clockFrequency_{clockFrequency},
					mode_{mode3 == false ? SpiMode::_0 : SpiMode::_3},
					type_{type}
	{

	}

	/**
	 * \brief Closes SPI EEPROM.
	 *
	 * Wrapper for SpiDevice::close().
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SpiDevice::close();
	 */

	int close();

	/**
	 * \brief Wrapper for Proxy::erase()
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of range that will be erased
	 * \param [in] size is the size of erased range, bytes
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Proxy::erase();
	 */

	int erase(uint64_t address, uint64_t size);

	/**
	 * \return total capacity of the device, bytes
	 */

	size_t getCapacity() const
	{
		return 128 * (1 << ((static_cast<uint8_t>(type_) & capacityMask_) >> capacityShift_));
	}

	/**
	 * \return size of single page, bytes
	 */

	size_t getPageSize() const
	{
		return 8 * (1 << ((static_cast<uint8_t>(type_) & pageSizeMask_) >> pageSizeShift_));
	}

	/**
	 * \brief Wrapper for Proxy::isWriteInProgress()
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current status of device: false - device
	 * is idle, true - write operation is in progress; error codes:
	 * - error codes returned by Proxy::isWriteInProgress();
	 */

	std::pair<int, bool> isWriteInProgress();

	/**
	 * \brief Opens SPI EEPROM.
	 *
	 * Wrapper for SpiDevice::open().
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SpiDevice::open();
	 */

	int open();

	/**
	 * \brief Wrapper for Proxy::program()
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be programmed
	 * \param [in] buffer is the buffer with data that will be programmed
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of programmed bytes (valid even
	 * when error code is returned); error codes:
	 * - error codes returned by Proxy::program();
	 */

	std::pair<int, size_t> program(uint32_t address, const void* buffer, size_t size);

	/**
	 * \brief Wrapper for Proxy::read()
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be read
	 * \param [out] buffer is the buffer to which the data will be written
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - error codes returned by Proxy::read();
	 */

	std::pair<int, size_t> read(uint32_t address, void* buffer, size_t size);

	/**
	 * \brief Wrapper for Proxy::synchronize()
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Proxy::synchronize();
	 */

	int synchronize();

	/**
	 * \brief Wrapper for synchronize()
	 *
	 * \deprecated scheduled to be removed after v0.7.0, use SpiEeprom::synchronize()
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by synchronize();
	 */

	__attribute__ ((deprecated("Use SpiEeprom::synchronize()")))
	int waitWhileWriteInProgress()
	{
		return synchronize();
	}

	/**
	 * \brief Wrapper for program()
	 *
	 * \deprecated scheduled to be removed after v0.7.0, use SpiEeprom::program()
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be written
	 * \param [in] buffer is the buffer with data that will be written
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned); error codes:
	 * - error codes returned by program();
	 */

	__attribute__ ((deprecated("Use SpiEeprom::program()")))
	std::pair<int, size_t> write(const uint32_t address, const void* const buffer, const size_t size)
	{
		return program(address, buffer, size);
	}

private:

	/// internal SPI slave device
	SpiDevice spiDevice_;

	/// reference to slave select pin of this SPI EEPROM
	OutputPin& slaveSelectPin_;

	/// desired clock frequency of SPI EEPROM, Hz
	uint32_t clockFrequency_;

	/// SPI mode used by SPI EEPROM
	SpiMode mode_;

	/// type of SPI EEPROM
	Type type_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SPIEEPROM_HPP_
