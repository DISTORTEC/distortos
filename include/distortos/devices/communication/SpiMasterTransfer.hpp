/**
 * \file
 * \brief SpiMasterTransfer class header
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERTRANSFER_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERTRANSFER_HPP_

#include <cstddef>

namespace distortos
{

namespace devices
{

/**
 * \brief SpiMasterTransfer class is a single transfer operation that can be executed by SPI master as part of a
 * transaction.
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
			size_{size}
	{

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
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERTRANSFER_HPP_
