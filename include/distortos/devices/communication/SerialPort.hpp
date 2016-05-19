/**
 * \file
 * \brief SerialPort class header
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SERIALPORT_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SERIALPORT_HPP_

#include "distortos/devices/communication/UartParity.hpp"

#include "distortos/internal/devices/UartBase.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

class Semaphore;

namespace internal
{

class UartLowLevel;

}

namespace devices
{

/**
 * SerialPort class is a serial port with an interface similar to standard files
 *
 * \ingroup devices
 */

class SerialPort : private internal::UartBase
{
public:

	/// thread-safe, lock-free circular buffer for one-producer and one-consumer
	class CircularBuffer
	{
	public:

		/**
		 * \brief CircularBuffer's constructor
		 *
		 * \param [in] buffer is a buffer for data
		 * \param [in] size is the size of \a buffer, bytes, should be even, must be greater than or equal to 4
		 */

		constexpr CircularBuffer(void* const buffer, const size_t size) :
				buffer_{static_cast<uint8_t*>(buffer)},
				size_{size},
				readPosition_{},
				writePosition_{}
		{

		}

		/**
		 * \brief Clears circular buffer
		 */

		void clear()
		{
			readPosition_ = 0;
			writePosition_ = 0;
		}

		/**
		 * \return First contiguous block (as a pair with pointer and size) available for reading
		 */

		std::pair<const uint8_t*, size_t> getReadBlock() const;

		/**
		 * \return total size of circular buffer, bytes
		 */

		size_t getSize() const
		{
			return size_;
		}

		/**
		 * \return First contiguous block (as a pair with pointer and size) available for writing
		 */

		std::pair<uint8_t*, size_t> getWriteBlock() const;

		/**
		 * \brief Increases read position by given value
		 *
		 * \param [in] value is the value which will be added to read position, must come from previous call to
		 * getReadBlock()
		 */

		void increaseReadPosition(const size_t value)
		{
			readPosition_ += value;
			readPosition_ %= size_;
		}

		/**
		 * \brief Increases write position by given value
		 *
		 * \param [in] value is the value which will be added to write position, must come from previous call to
		 * getWriteBlock()
		 */

		void increaseWritePosition(const size_t value)
		{
			writePosition_ += value;
			writePosition_ %= size_;
		}

		/**
		 * \return true if circular buffer is empty, false otherwise
		 */

		bool isEmpty() const
		{
			return writePosition_ == readPosition_;
		}

		/**
		 * \return true if circular buffer is full, false otherwise
		 */

		bool isFull() const
		{
			return readPosition_ == (writePosition_ + 2) % size_;
		}

	private:

		/// pointer to beginning of buffer
		uint8_t* buffer_;

		/// size of \a buffer_, bytes
		size_t size_;

		/// current read position
		volatile size_t readPosition_;

		/// current write position
		volatile size_t writePosition_;
	};

	/**
	 * \brief SerialPort's constructor
	 *
	 * \param [in] uart is a reference to low-level implementation of internal::UartLowLevel interface
	 * \param [in] readBuffer is a buffer for read operations
	 * \param [in] readBufferSize is the size of \a readBuffer, bytes, should be even, must be greater than or equal to
	 * 4
	 * \param [in] writeBuffer is a buffer to write operations
	 * \param [in] writeBufferSize is the size of \a writeBuffer, bytes, should be even, must be greater than or equal
	 * to 4
	 */

	constexpr SerialPort(internal::UartLowLevel& uart, void* const readBuffer, const size_t readBufferSize,
			void* const writeBuffer, const size_t writeBufferSize) :
					readMutex_{Mutex::Type::normal, Mutex::Protocol::priorityInheritance},
					writeMutex_{Mutex::Type::normal, Mutex::Protocol::priorityInheritance},
					readBuffer_{readBuffer, (readBufferSize / 2) * 2},
					writeBuffer_{writeBuffer, (writeBufferSize / 2) * 2},
					readSemaphore_{},
					transmitSemaphore_{},
					writeSemaphore_{},
					writeLimit_{},
					uart_{uart},
					baudRate_{},
					characterLength_{},
					parity_{},
					_2StopBits_{},
					openCount_{},
					transmitInProgress_{},
					writeInProgress_{}
	{

	}

	/**
	 * \brief SerialPort's destructor
	 *
	 * Does nothing if all users already closed this device. If they did not, performs forced close of device.
	 */

	~SerialPort() override;

	/**
	 * \brief Closes SerialPort
	 *
	 * Does nothing if any user still has this device opened. Otherwise all transfers and low-level driver are stopped.
	 * If any write transfer is still in progress, this function will wait for physical end of transmission before
	 * shutting the device down.
	 *
	 * If the function is interrupted by a signal, the device is not closed - the user should try to close it again.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the device is already completely closed;
	 * - EINTR - the wait was interrupted by an unmasked, caught signal;
	 * - error codes returned by internal::UartLowLevel::stop();
	 */

	int close();

	/**
	 * \brief Opens SerialPort
	 *
	 * Does nothing if any user already has this device opened. Otherwise low-level driver and buffered reads are
	 * started.
	 *
	 * \param [in] baudRate is the desired baud rate, bps
	 * \param [in] characterLength selects character length, bits
	 * \param [in] parity selects parity
	 * \param [in] _2StopBits selects whether 1 (false) or 2 (true) stop bits are used
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - provided arguments don't match current configuration of already opened device;
	 * - EMFILE - this device is already opened too many times;
	 * - ENOBUFS - read and/or write buffers are too small;
	 * - error codes returned by internal::UartLowLevel::start();
	 * - error codes returned by internal::UartLowLevel::startRead();
	 */

	int open(uint32_t baudRate, uint8_t characterLength, devices::UartParity parity, bool _2StopBits);

	/**
	 * \brief Reads data from SerialPort
	 *
	 * Similar to POSIX read() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/read.html#
	 *
	 * This function will block until at least one character can be read.
	 *
	 * \param [out] buffer is the buffer to which the data will be written
	 * \param [in] size is the size of \a buffer, bytes, must be even if selected character length is greater than 8
	 * bits
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned);
	 * error codes:
	 * - EBADF - the device is not opened;
	 * - EINTR - the wait was interrupted by an unmasked, caught signal;
	 * - EINVAL - \a buffer and/or \a size are invalid;
	 * - error codes returned by internal::UartLowLevel::startRead();
	 */

	std::pair<int, size_t> read(void* buffer, size_t size);

	/**
	 * \brief Writes data to SerialPort
	 *
	 * Similar to POSIX write() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/write.html#
	 *
	 * This function will block until all character are written.
	 *
	 * \param [in] buffer is the buffer with data that will be transmitted
	 * \param [in] size is the size of \a buffer, bytes, must be even if selected character length is greater than 8
	 * bits
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned);
	 * error codes:
	 * - EBADF - the device is not opened;
	 * - EINTR - the wait was interrupted by an unmasked, caught signal;
	 * - EINVAL - \a buffer and/or \a size are invalid;
	 * - error codes returned by internal::UartLowLevel::startWrite();
	 */

	std::pair<int, size_t> write(const void* buffer, size_t size);

private:

	/**
	 * \brief "Read complete" event
	 *
	 * Called by low-level UART driver when whole read buffer is filled.
	 *
	 * Updates position of read circular buffer and notifies any thread waiting for this event. If the read circular
	 * buffer is not full, next read operation is started.
	 *
	 * \param [in] bytesRead is the number of bytes read by low-level UART driver (and written to read buffer)
	 */

	void readCompleteEvent(size_t bytesRead) override;

	/**
	 * \brief "Receive error" event
	 *
	 * Called by low-level UART driver when the last character was received with an error. This character is written to
	 * the read buffer before this function is called.
	 *
	 * Does nothing.
	 *
	 * \param [in] errorSet is the set of error bits
	 */

	void receiveErrorEvent(ErrorSet errorSet) override;

	/**
	 * \brief Wrapper for internal::UartLowLevel::startRead()
	 *
	 * Starts read operation with size that is the smallest of: size of first available write block, half the size of
	 * read circular buffer and value of \a limit.
	 *
	 * \param [in] limit is the size limit of started read operation, bytes
	 *
	 * \return values returned by internal::UartLowLevel::startRead()
	 */

	int startReadWrapper(size_t limit);

	/**
	 * \brief Wrapper for internal::UartLowLevel::startWrite()
	 *
	 * Does nothing if write is already in progress or if write circular buffer is empty. Otherwise sets "transmit in
	 * progress" and "write in progress" flags, starts write operation with size that is the smallest of: size of first
	 * available read block, half the size of write circular buffer and current size limit of write operations (only if
	 * it's not equal to 0).
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by internal::UartLowLevel::startWrite();
	 */

	int startWriteWrapper();

	/**
	 * \brief Wrapper for internal::UartLowLevel::stopWrite()
	 *
	 * Stops write operation, updates position of write circular buffer, updates size limit of write operations and
	 * clears "write in progress" flag.
	 *
	 * \return values returned by internal::UartLowLevel::stopWrite();
	 */

	size_t stopWriteWrapper();

	/**
	 * \brief "Transmit complete" event
	 *
	 * Called by low-level UART driver when the transmission is physically finished.
	 *
	 * Notifies any thread waiting for this event and clears "transmit in progress" flag.
	 */

	void transmitCompleteEvent() override;

	/**
	 * \brief "Write complete" event
	 *
	 * Called by low-level UART driver when whole write buffer was transfered - the transmission may still be in
	 * progress.
	 *
	 * Updates position of write circular buffer, updates size limit of write operations, clears "write in progress"
	 * flag and notifies any thread waiting for this event. Next write operation is started if there's anything in the
	 * write circular buffer.
	 *
	 * \param [in] bytesWritten is the number of bytes written by low-level UART driver (and read from write buffer)
	 */

	void writeCompleteEvent(size_t bytesWritten) override;

	/// mutex used to serialize access to read(), close() and open()
	Mutex readMutex_;

	/// mutex used to serialize access to write(), close() and open()
	Mutex writeMutex_;

	/// circular buffer for read operations
	CircularBuffer readBuffer_;

	/// circular buffer for write operations
	CircularBuffer writeBuffer_;

	/// pointer to semaphore used for "read complete" event notifications
	Semaphore* volatile readSemaphore_;

	/// pointer to semaphore used for "transmit complete" event notifications
	Semaphore* volatile transmitSemaphore_;

	/// pointer to semaphore used for "write complete" event notifications
	Semaphore* volatile writeSemaphore_;

	/// size limit of write operations, 0 if no limiting is needed, bytes
	volatile size_t writeLimit_;

	/// reference to low-level implementation of internal::UartLowLevel interface
	internal::UartLowLevel& uart_;

	/// current baud rate, bps
	uint32_t baudRate_;

	/// current character length, bits
	uint8_t characterLength_;

	/// current parity
	devices::UartParity parity_;

	/// current configuration of stop bits: 1 (false) or 2 (true)
	bool _2StopBits_;

	/// number of times this device was opened but not yet closed
	uint8_t openCount_;

	/// "transmit in progress" flag
	volatile bool transmitInProgress_;

	/// "write in progress" flag
	volatile bool writeInProgress_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SERIALPORT_HPP_
