/**
 * \file
 * \brief SpiMasterOperation class header
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATION_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATION_HPP_

#include "distortos/devices/communication/SpiMasterErrorSet.hpp"

namespace distortos
{

namespace devices
{

/**
 * SpiMasterOperation class is a single operation that can be executed by SPI master as part of a transaction.
 *
 * \ingroup devices
 */

class SpiMasterOperation
{
public:

	/// Transfer class is a single transfer operation
	class Transfer
	{
	public:

		/**
		 * \brief Transfer's constructor
		 *
		 * \param [in] writeBuffer is the buffer with data that will be written, nullptr to send dummy data
		 * \param [out] readBuffer is the buffer with data that will be read, nullptr to ignore received data
		 * \param [in] size is the size of transfer (size of \a writeBuffer and/or \a readBuffer), bytes, must be even
		 * if number of data bits is in range (8; 16], divisible by 3 if number of data bits is in range (16; 24] or
		 * divisible by 4 if number of data bits is in range (24; 32]
		 */

		constexpr Transfer(const void* const writeBuffer, void* const readBuffer, const size_t size) :
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
		 * \param [in] bytesTransfered is the number of bytes transferred by low-level SPI master driver (read from
		 * write buffer and/or written to read buffer), may be unreliable if \a errorSet is not empty (i.e. transfer
		 * error was detected)
		 */

		void finalize(const SpiMasterErrorSet errorSet, const size_t bytesTransfered)
		{
			errorSet_ = errorSet;
			bytesTransfered_ = bytesTransfered;
		}

		/**
		 * \return number of bytes transferred by low-level SPI master driver (read from write buffer and/or written to
		 * read buffer), may be unreliable if error set is not empty (i.e. transfer error was detected)
		 */

		size_t getBytesTransfered() const
		{
			return bytesTransfered_;
		}

		/**
		 * \return set of error bits detected during the transfer
		 */

		SpiMasterErrorSet getErrorSet() const
		{
			return errorSet_;
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

		/// size of transfer (size of \a readBuffer_ and/or \a writeBuffer_), bytes
		size_t size_;

		/// number of bytes transferred by low-level SPI master driver (read from \a writeBuffer_ and/or written to
		/// \a readBuffer_), may be unreliable if \a errorSet_ is not empty (i.e. transfer error was detected)
		size_t bytesTransfered_;

		/// set of error bits detected during the transfer
		SpiMasterErrorSet errorSet_;
	};

	/**
	 * \brief SpiMasterOperation's constructor with transfer operation
	 *
	 * \param [in] transfer is the transfer operation that will be executed
	 *
	 * \note non-explicit on purpose
	 */

	constexpr SpiMasterOperation(const Transfer transfer) :
			transfer_{transfer}
	{

	}

	/**
	 * \return pointer to contained transfer operation, nullptr if this object contains different type of operation
	 */

	Transfer* getTransfer()
	{
		return &transfer_;
	}

private:

	/// single transfer operation
	Transfer transfer_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATION_HPP_
