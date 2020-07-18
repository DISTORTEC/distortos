/**
 * \file
 * \brief SdMmcCardLowLevel class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_MEMORY_SDMMCCARDLOWLEVEL_HPP_
#define INCLUDE_DISTORTOS_DEVICES_MEMORY_SDMMCCARDLOWLEVEL_HPP_

#include "estd/ContiguousRange.hpp"

namespace distortos
{

namespace devices
{

class SdMmcCardBase;

/**
 * \brief SdMmcCardLowLevel class is an interface for low-level SD/MMC card driver.
 *
 * \ingroup devices
 */

class SdMmcCardLowLevel
{
public:

	/// bus mode
	enum class BusMode : uint8_t
	{
		/// 1-bit bus mode
		_1Bit,
		/// 4-bit bus mode
		_4Bit,
		/// 8-bit bus mode
		_8Bit,
	};

	/// command response
	class Response : public estd::ContiguousRange<uint32_t>
	{
	public:

		/// base of Response - range of uint32_t
		using Base = estd::ContiguousRange<uint32_t>;

		/**
		 * \brief Response's constructor
		 *
		 * Used when no response is expected.
		 */

		constexpr Response() :
				Base{}
		{

		}

		/**
		 * \brief Response's constructor
		 *
		 * Used when short response is expected.
		 *
		 * \param [out] shortResponse is a reference to variable into which the short response will be read
		 */

		constexpr explicit Response(uint32_t& shortResponse) :
				Base{shortResponse}
		{

		}

		/**
		 * \brief Response's constructor
		 *
		 * Used when short response is expected.
		 *
		 * \param [out] shortResponse is a reference to array into which the short response will be read
		 */

		constexpr explicit Response(uint32_t (& shortResponse)[1]) :
				Base{shortResponse}
		{

		}

		/**
		 * \brief Response's constructor
		 *
		 * Used when short response is expected.
		 *
		 * \param [out] shortResponse is a reference to array into which the short response will be read
		 */

		constexpr explicit Response(std::array<uint32_t, 1>& shortResponse) :
				Base{shortResponse}
		{

		}

		/**
		 * \brief Response's constructor
		 *
		 * Used when long response is expected.
		 *
		 * \param [out] longResponse is a reference to array into which the long response will be read
		 */

		constexpr explicit Response(uint32_t (& longResponse)[4]) :
				Base{longResponse}
		{

		}

		/**
		 * \brief Response's constructor
		 *
		 * Used when long response is expected.
		 *
		 * \param [out] longResponse is a reference to array into which the long response will be read
		 */

		constexpr explicit Response(std::array<uint32_t, 4>& longResponse) :
				Base{longResponse}
		{

		}
	};

	/// transfer associated with transaction
	class Transfer
	{
	public:

		/**
		 * \brief Transfer's constructor
		 *
		 * Used when no transfer is associated with transaction.
		 */

		constexpr Transfer() :
				readBuffer_{},
				size_{},
				blockSize_{},
				timeoutMs_{},
				writeTransfer_{}
		{

		}

		/**
		 * \brief Transfer's constructor
		 *
		 * Used when read transfer is associated with transaction.
		 *
		 * \param [out] readBuffer is the buffer into which the data will be read
		 * \param [in] size is the size of \a readBuffer, bytes, must be a multiple of block size
		 * \param [in] blockSize is the block size, bytes
		 * \param [in] timeoutMs is the timeout of read transfer, milliseconds
		 */

		constexpr Transfer(void* const readBuffer, const size_t size, const size_t blockSize,
				const uint16_t timeoutMs) :
						readBuffer_{readBuffer},
						size_{size},
						blockSize_{blockSize},
						timeoutMs_{timeoutMs},
						writeTransfer_{}
		{

		}

		/**
		 * \brief Transfer's constructor
		 *
		 * Used when write transfer is associated with transaction.
		 *
		 * \param [in] writeBuffer is the buffer with data that will be written
		 * \param [in] size is the size of \a writeBuffer, bytes, must be a multiple of block size
		 * \param [in] blockSize is the block size, bytes
		 * \param [in] timeoutMs is the timeout of write transfer, milliseconds
		 */

		constexpr Transfer(const void* const writeBuffer, const size_t size, const size_t blockSize,
				const uint16_t timeoutMs) :
						writeBuffer_{writeBuffer},
						size_{size},
						blockSize_{blockSize},
						timeoutMs_{timeoutMs},
						writeTransfer_{true}
		{

		}

		/**
		 * \return block size, bytes
		 */

		size_t getBlockSize() const
		{
			return blockSize_;
		}

		/**
		 * \return buffer into which the data will be read, valid only if isWriteTransfer() returns false
		 */

		void* getReadBuffer() const
		{
			return readBuffer_;
		}

		/**
		 * \return size of read buffer or write buffer, bytes
		 */

		size_t getSize() const
		{
			return size_;
		}

		/**
		 * \return timeout of transfer, milliseconds
		 */

		uint16_t getTimeoutMs() const
		{
			return timeoutMs_;
		}

		/**
		 * \return buffer with data that will be written, valid only if isWriteTransfer() returns true
		 */

		const void* getWriteBuffer() const
		{
			return writeBuffer_;
		}

		/**
		 * \return false if this is a read transfer, true if this is a write transfer
		 */

		bool isWriteTransfer() const
		{
			return writeTransfer_;
		}

	private:

		union
		{
			/// buffer into which the data will be read, valid only if \a writeTransfer_ is false
			void* readBuffer_;

			/// buffer with data that will be written, valid only if \a writeTransfer_ is true
			const void* writeBuffer_;
		};

		/// size of \a readBuffer_ or \a writeBuffer_, bytes
		size_t size_;

		/// block size, bytes
		size_t blockSize_;

		/// timeout of transfer, milliseconds
		uint16_t timeoutMs_;

		/// selects whether this is a read (false) or write (true) transfer
		bool writeTransfer_;
	};

	/// read transfer associated with transaction
	struct ReadTransfer : public Transfer
	{
		/**
		 * \brief ReadTransfer's constructor
		 *
		 * \param [out] readBuffer is the buffer into which the data will be read
		 * \param [in] size is the size of \a readBuffer, bytes, must be a multiple of block size
		 * \param [in] blockSize is the block size, bytes
		 * \param [in] timeoutMs is the timeout of read transfer, milliseconds
		 */

		constexpr ReadTransfer(void* const readBuffer, const size_t size, const size_t blockSize,
				const uint16_t timeoutMs) :
						Transfer{readBuffer, size, blockSize, timeoutMs}
		{

		}
	};

	/// write transfer associated with transaction
	struct WriteTransfer : public Transfer
	{

		/**
		 * \brief WriteTransfer's constructor
		 *
		 * \param [in] writeBuffer is the buffer with data that will be written
		 * \param [in] size is the size of \a writeBuffer, bytes, must be a multiple of block size
		 * \param [in] blockSize is the block size, bytes
		 * \param [in] timeoutMs is the timeout of write transfer, milliseconds
		 */

		constexpr WriteTransfer(const void* const writeBuffer, const size_t size, const size_t blockSize,
				const uint16_t timeoutMs) :
						Transfer{writeBuffer, size, blockSize, timeoutMs}
		{

		}
	};

	/// maximum allowed command
	constexpr static uint8_t maxCommand {(1 << 6) - 1};

	/**
	 * \brief SdMmcCardLowLevel's destructor
	 *
	 * \pre Driver is stopped.
	 */

	virtual ~SdMmcCardLowLevel() = default;

	/**
	 * \brief Configures parameters of low-level SD/MMC card driver.
	 *
	 * \pre Driver is started.
	 * \pre No transaction is in progress.
	 * \pre \a clockFrequency is valid.
	 *
	 * \param [in] busMode is the desired bus mode
	 * \param [in] clockFrequency is the desired clock frequency, Hz, must be greater than or equal to 400 kHz
	 */

	virtual void configure(BusMode busMode, uint32_t clockFrequency) = 0;

	/**
	 * \brief Starts low-level SD/MMC card driver.
	 *
	 * \pre Driver is stopped.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int start() = 0;

	/**
	 * \brief Starts asynchronous transaction.
	 *
	 * This function returns immediately. When the transaction is physically finished (either command, its argument,
	 * response and associated transfer were sent/received or an error was detected),
	 * SdMmcCardBase::transactionCompleteEvent() will be executed.
	 *
	 * \pre Driver is started.
	 * \pre No transaction is in progress.
	 * \pre \a command is valid.
	 * \pre When there is an associated transfer:
	 * - either short or long response is expected;
	 * - transfer's read buffer (for read transfers) or write buffer (for write transfers) is valid and its address is
	 * aligned to DISTORTOS_SDMMCCARD_BUFFER_ALIGNMENT;
	 * - transfer's block size is a power of two, greater than or equal to 4 and less than or equal to 2^14;
	 * - transfer's size is an integer multiple of block size and less than or equal to 2^25 - 1;
	 * - transfer's timeout converted to clock cycles must be less than or equal to 2^32 - 1;
	 *
	 * \post Transaction is in progress.
	 *
	 * \param [in] sdMmcCardBase is a reference to SdMmcCardBase object that will be notified about completed
	 * transaction
	 * \param [in] command is the command associated with the transaction, [0; maxCommand]
	 * \param [in] argument is the argument for \a command
	 * \param [out] response is the buffer into which the command response will be read, it's size determines what type
	 * of response is expected (none, short or long)
	 * \param [in,out] transfer is the transfer associated with transaction
	 */

	virtual void startTransaction(SdMmcCardBase& sdMmcCardBase, uint8_t command, uint32_t argument, Response response,
			Transfer transfer) = 0;

	/**
	 * \brief Stops low-level SD/MMC card driver.
	 *
	 * \pre Driver is started.
	 * \pre No transaction is in progress.
	 *
	 * \post Driver is stopped.
	 */

	virtual void stop() = 0;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_MEMORY_SDMMCCARDLOWLEVEL_HPP_
