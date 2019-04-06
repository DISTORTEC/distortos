/**
 * \file
 * \brief SpiMasterHandle class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERHANDLE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERHANDLE_HPP_

#include "distortos/devices/communication/SpiMaster.hpp"

namespace distortos
{

namespace devices
{

/**
 * SpiMasterHandle is a [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)-like class for locking
 * SpiMaster and for accessing its core functionality.
 *
 * \note Locks are recursive.
 *
 * \ingroup devices
 */

class SpiMasterHandle
{
public:

	/**
	 * \brief SpiMasterHandle's constructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] spiMaster is a reference to SpiMaster associated with this handle
	 */

	explicit SpiMasterHandle(SpiMaster& spiMaster) :
			spiMaster_{spiMaster}
	{
		spiMaster_.lock();
	}

	/**
	 * \brief SpiMasterHandle's destructor
	 *
	 * \warning This function must not be called from interrupt context!
	 */

	~SpiMasterHandle()
	{
		spiMaster_.unlock();
	}

	/**
	 * \brief Closes associated SPI master.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SpiMaster::close();
	 */

	int close() const
	{
		return spiMaster_.close();
	}

	/**
	 * \brief Configures parameters of associated SPI master.
	 *
	 * \param [in] mode is the desired SPI mode
	 * \param [in] clockFrequency is the desired clock frequency, Hz
	 * \param [in] wordLength selects word length, bits, [1; 32]
	 * \param [in] lsbFirst selects whether MSB (false) or LSB (true) is transmitted first
	 * \param [in] dummyData is the dummy data that will be sent if write buffer of transfer is nullptr
	 *
	 * \return pair with return code (0 on success, error code otherwise) and real clock frequency; error codes:
	 * - error codes returned by SpiMaster::configure();
	 */

	std::pair<int, uint32_t> configure(const SpiMode mode, const uint32_t clockFrequency, const uint8_t wordLength,
			const bool lsbFirst,const uint32_t dummyData) const
	{
		return spiMaster_.configure(mode, clockFrequency, wordLength, lsbFirst, dummyData);
	}

	/**
	 * \brief Executes series of transfers as a single atomic transaction.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] transfersRange is the range of transfers that will be executed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of successfully completed transfers
	 * from \a transfersRange; error codes:
	 * - error codes returned by SpiMasterLowLevel::executeTransaction();
	 */

	std::pair<int, size_t> executeTransaction(const SpiMasterTransfersRange transfersRange) const
	{
		return spiMaster_.executeTransaction(transfersRange);
	}

	/**
	 * \brief Opens associated SPI master.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SpiMaster::open();
	 */

	int open() const
	{
		return spiMaster_.open();
	}

	SpiMasterHandle(const SpiMasterHandle&) = delete;
	SpiMasterHandle& operator=(const SpiMasterHandle&) = delete;

private:

	/// reference to SpiMaster associated with this handle
	SpiMaster& spiMaster_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERHANDLE_HPP_
