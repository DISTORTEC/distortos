/**
 * \file
 * \brief SpiMasterTransfer class header
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERTRANSFER_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERTRANSFER_HPP_

#include "distortos/devices/communication/SpiMasterErrorSet.hpp"

namespace distortos
{

namespace devices
{

/**
 * SpiMasterTransfer class is a single transfer operation that can be executed by SPI master as part of a transaction.
 *
 * \ingroup devices
 */

class SpiMasterTransfer
{
public:

	/**
	 * \brief SpiMasterTransfer's constructor
	 *
	 * \param [in] writeBuffer is the buffer with data that will be written, nullptr to send dummy data
	 * \param [out] readBuffer is the buffer with data that will be read, nullptr to ignore received data
	 * \param [in] size is the size of transfer, bytes, must be even if number of data bits is in range (8; 16],
	 * divisible by 3 if number of data bits is in range (16; 24] or divisible by 4 if number of data bits is in range
	 * (24; 32]
	 */

	constexpr SpiMasterTransfer(const void* const writeBuffer, void* const readBuffer, const size_t size) :
			readBuffer_{readBuffer},
			writeBuffer_{writeBuffer},
			size_{size},
			bytesTransfered_{},
			errorSet_{}
	{

	}

	/**
	 * \brief Finalizes transfer operation.
	 *
	 * \param [in] errorSet is the set of error bits
	 * \param [in] bytesTransfered is the number of bytes transferred by low-level SPI master driver (read from write
	 * buffer and/or written to read buffer), may be unreliable if \a errorSet is not empty (i.e. transfer error was
	 * detected)
	 */

	void finalize(const SpiMasterErrorSet errorSet, const size_t bytesTransfered)
	{
		errorSet_ = errorSet;
		bytesTransfered_ = bytesTransfered;
	}

	/**
	 * \return number of bytes transferred by low-level SPI master driver (read from write buffer and/or written to read
	 * buffer), may be unreliable if error set is not empty (i.e. transfer error was detected)
	 */

	size_t getBytesTransfered() const
	{
		return bytesTransfered_;
	}

	/**
	 * \return buffer with data that will be read, nullptr to ignore received data
	 */

	void* getReadBuffer() const
	{
		return readBuffer_;
	}

	/**
	 * \return size of transfer (size of read and/or \a write buffers), bytes
	 */

	size_t getSize() const
	{
		return size_;
	}

	/**
	 * \return buffer with data that will be written, nullptr to send dummy data
	 */

	const void* getWriteBuffer() const
	{
		return writeBuffer_;
	}

private:

	/// buffer with data that will be read, nullptr to ignore received data
	void* readBuffer_;

	/// buffer with data that will be written, nullptr to send dummy data
	const void* writeBuffer_;

	/// size of transfer, bytes
	size_t size_;

	/// number of bytes transferred by low-level SPI master driver (read from \a writeBuffer_ and/or written to
	/// \a readBuffer_), may be unreliable if \a errorSet_ is not empty (i.e. transfer error was detected)
	size_t bytesTransfered_;

	/// set of error bits detected during the transfer
	SpiMasterErrorSet errorSet_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERTRANSFER_HPP_
