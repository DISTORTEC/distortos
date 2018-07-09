/**
 * \file
 * \brief SpiSdMmcCard class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SPISDMMCCARD_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SPISDMMCCARD_HPP_

#include "distortos/devices/communication/SpiDevice.hpp"

namespace distortos
{

namespace devices
{

class SpiSdMmcCardProxy;

/**
 * SpiSdMmcCard class is a SD or MMC card connected via SPI.
 *
 * \ingroup devices
 */

class SpiSdMmcCard
{
	friend class SpiSdMmcCardProxy;

public:

	/// import SpiSdMmcCardProxy as SpiSdMmcCard::Proxy
	using Proxy = SpiSdMmcCardProxy;

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
			clockFrequency_{clockFrequency},
			blockAddressing_{},
			type_{}
	{

	}

	/**
	 * \brief Closes SD or MMC card connected via SPI.
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
	 * \param [in] address is the address of range that will be erased, must be a multiple of erase block size
	 * \param [in] size is the size of erased range, bytes, must be a multiple of erase block size
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Proxy::erase();
	 */

	int erase(uint64_t address, uint64_t size);

	/**
	 * \brief Opens SD or MMC card connected via SPI.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Proxy::initialize();
	 * - error codes returned by SpiDevice::open();
	 */

	int open();

	/**
	 * \brief Wrapper for Proxy::program()
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be programmed, must be a multiple of block size
	 * \param [in] buffer is the buffer with data that will be programmed
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of block size
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of programmed bytes (valid even when
	 * error code is returned); error codes:
	 * - error codes returned by Proxy::program();
	 */

	std::pair<int, size_t> program(uint64_t address, const void* buffer, size_t size);

	/**
	 * \brief Wrapper for Proxy::read()
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] address is the address of data that will be read, must be a multiple of block size
	 * \param [out] buffer is the buffer to which the data will be written
	 * \param [in] size is the size of \a buffer, bytes, must be a multiple of block size
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - error codes returned by Proxy::read();
	 */

	std::pair<int, size_t> read(uint64_t address, void* buffer, size_t size);

private:

	/// internal SPI slave device
	SpiDevice spiDevice_;

	/// number of blocks available on SD or MMC card
	size_t blocksCount_;

	/// desired clock frequency of SD or MMC card, Hz
	uint32_t clockFrequency_;

	/// selects whether card uses byte (false) or block (true) addressing
	bool blockAddressing_;

	/// type of card connected via SPI
	Type type_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SPISDMMCCARD_HPP_
