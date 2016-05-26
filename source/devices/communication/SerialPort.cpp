/**
 * \file
 * \brief SerialPort class implementation
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SerialPort.hpp"

#include "distortos/internal/devices/UartLowLevel.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/Semaphore.hpp"

#include "estd/ScopeGuard.hpp"

#include <cerrno>
#include <cstring>

namespace distortos
{

namespace devices
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Reads data from circular buffer.
 *
 * \param [in] circularBuffer is a reference to circular buffer from which the data will be read
 * \param [out] buffer is a buffer to which the data will be written
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return number of bytes read from \a circularBuffer and written to \a buffer
 */

size_t readFromCircularBuffer(SerialPort::CircularBuffer& circularBuffer, uint8_t* const buffer, const size_t size)
{
	decltype(circularBuffer.getReadBlock()) readBlock;
	size_t bytesRead {};
	while (readBlock = circularBuffer.getReadBlock(), readBlock.second != 0 && bytesRead != size)
	{
		const auto copySize = std::min(readBlock.second, size - bytesRead);
		memcpy(buffer + bytesRead, readBlock.first, copySize);
		circularBuffer.increaseReadPosition(copySize);
		bytesRead += copySize;
	}

	return bytesRead;
}

/**
 * \brief Writes data to circular buffer.
 *
 * \param [in] buffer is a buffer from which the data will be read
 * \param [in] size is the size of \a buffer, bytes
 * \param [in] circularBuffer is a reference to circular buffer to which the data will be written
 *
 * \return number of bytes read from \a buffer and written to \a circularBuffer
 */

size_t writeToCircularBuffer(const uint8_t* const buffer, const size_t size, SerialPort::CircularBuffer& circularBuffer)
{
	decltype(circularBuffer.getWriteBlock()) writeBlock;
	size_t bytesWritten {};
	while (writeBlock = circularBuffer.getWriteBlock(), writeBlock.second != 0 && bytesWritten != size)
	{
		const auto copySize = std::min(writeBlock.second, size - bytesWritten);
		memcpy(writeBlock.first, buffer + bytesWritten, copySize);
		circularBuffer.increaseWritePosition(copySize);
		bytesWritten += copySize;
	}

	return bytesWritten;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| SerialPort::CircularBuffer public functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<const uint8_t*, size_t> SerialPort::CircularBuffer::getReadBlock() const
{
	const auto readPosition = readPosition_;
	const auto writePosition = writePosition_;
	return {buffer_ + readPosition, (writePosition >= readPosition ? writePosition : size_) - readPosition};
}

std::pair<uint8_t*, size_t> SerialPort::CircularBuffer::getWriteBlock() const
{
	const auto readPosition = readPosition_;
	const auto writePosition = writePosition_;
	const auto freeBytes = (size_ - writePosition + readPosition - 2) % size_;
	const auto writeBlockSize = (readPosition > writePosition ? readPosition : size_) - writePosition;
	return {buffer_ + writePosition, std::min(freeBytes, writeBlockSize)};
}

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SerialPort::~SerialPort()
{
	if (openCount_ == 0)
		return;

	readMutex_.lock();
	writeMutex_.lock();
	const auto readWriteMutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				writeMutex_.unlock();
				readMutex_.unlock();
			});

	uart_.stopRead();
	uart_.stopWrite();
	uart_.stop();
}

int SerialPort::close()
{
	readMutex_.lock();
	writeMutex_.lock();
	const auto readWriteMutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				writeMutex_.unlock();
				readMutex_.unlock();
			});

	if (openCount_ == 0)	// device is not open anymore?
		return EBADF;

	if (openCount_ == 1)	// last close?
	{
		while (transmitInProgress_ == true)	// wait for physical end of write operation
		{
			Semaphore semaphore {0};
			transmitSemaphore_ = &semaphore;
			const auto transmitSemaphoreScopeGuard = estd::makeScopeGuard(
					[this]()
					{
						transmitSemaphore_ = {};
					});

			if (transmitInProgress_ == true)
			{
				const auto ret = semaphore.wait();
				if (ret != 0)
					return ret;
			}
		}

		stopReadWrapper();

		const auto ret = uart_.stop();
		if (ret != 0)
			return ret;

		readBuffer_.clear();
		writeBuffer_.clear();
	}

	--openCount_;
	return 0;
}

int SerialPort::open(const uint32_t baudRate, const uint8_t characterLength, const devices::UartParity parity,
			const bool _2StopBits)
{
	readMutex_.lock();
	writeMutex_.lock();
	const auto readWriteMutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				writeMutex_.unlock();
				readMutex_.unlock();
			});

	if (openCount_ == std::numeric_limits<decltype(openCount_)>::max())	// device is already opened too many times?
		return EMFILE;

	if (openCount_ == 0)	// first open?
	{
		if (readBuffer_.getCapacity() < 2 || writeBuffer_.getCapacity() < 2)
			return ENOBUFS;

		{
			const auto ret = uart_.start(*this, baudRate, characterLength, parity, _2StopBits);
			if (ret.first != 0)
				return ret.first;
		}
		{
			const auto ret = startReadWrapper();
			if (ret != 0)
				return ret;
		}

		baudRate_ = baudRate;
		characterLength_ = characterLength;
		parity_ = parity;
		_2StopBits_ = _2StopBits;
	}
	else	// if (openCount_ != 0)
	{
		// provided arguments don't match current configuration of already opened device?
		if (baudRate_ != baudRate || characterLength_ != characterLength || parity_ != parity ||
				_2StopBits_ != _2StopBits)
			return EINVAL;
	}

	++openCount_;
	return 0;
}

std::pair<int, size_t> SerialPort::read(void* const buffer, const size_t size, const size_t minSize)
{
	if (buffer == nullptr || size == 0)
		return {EINVAL, {}};

	{
		const auto ret = minSize != 0 ? readMutex_.lock() : readMutex_.tryLock();
		if (ret != 0)
			return {ret != EBUSY ? ret : EAGAIN, {}};
	}
	const auto readMutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				readMutex_.unlock();
			});

	if (openCount_ == 0)
		return {EBADF, {}};

	if (characterLength_ > 8 && size % 2 != 0)
		return {EINVAL, {}};

	// when character length is greater than 8 bits, round up "minSize" value
	const auto adjustedMinSize = std::min(size, characterLength_ <= 8 ? minSize : ((minSize + 1) / 2) * 2);
	const auto bufferUint8 = static_cast<uint8_t*>(buffer);

	// initially try to read as much data as possible from circular buffer with no blocking
	auto bytesRead = readFromCircularBuffer(*currentReadBuffer_, bufferUint8, size);
	{
		const auto ret = startReadWrapper();
		if (ret != 0 || bytesRead == size)
			return {ret, bytesRead};
	}

	const auto readLimitScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				readLimit_ = 0;
			});

	// Full buffer could not be read from circular buffer? The current read transfer (if any) must be stopped for a
	// short moment to get the amount of data available in the circular buffer (interrupts are masked to prevent
	// preemption, which could make this "short moment" very long). By subtracting this value from the minimum amount of
	// data required for reading we get size limit of read operation. Notification after exactly that number of bytes
	// will mean that the buffer has enough data to satisfy requested minimum size.
	{
		architecture::InterruptMaskingLock interruptMaskingLock;
		stopReadWrapper();
		const auto bytesAvailable = currentReadBuffer_->getSize();
		readLimit_ = adjustedMinSize > (bytesRead + bytesAvailable) ?
				adjustedMinSize - (bytesRead + bytesAvailable) : 0;
		{
			const auto ret = startReadWrapper();
			if (ret != 0)
				return {ret, bytesRead};
		}
	}

	do
	{
		Semaphore semaphore {0};
		readSemaphore_ = &semaphore;
		const auto readSemaphoreScopeGuard = estd::makeScopeGuard(
				[this]()
				{
					readSemaphore_ = {};
				});

		bytesRead += readFromCircularBuffer(*currentReadBuffer_, bufferUint8 + bytesRead, size - bytesRead);
		{
			const auto ret = startReadWrapper();
			if (ret != 0)
				return {ret, bytesRead};
		}

		if (bytesRead < adjustedMinSize)	// wait for data only if requested minimum is not already read
		{
			const auto ret = semaphore.wait();
			if (ret != 0)
				return {ret, bytesRead};
		}
	} while (bytesRead < adjustedMinSize);

	return {bytesRead != 0 ? 0 : EAGAIN, bytesRead};
}

std::pair<int, size_t> SerialPort::write(const void* const buffer, const size_t size, const size_t minSize)
{
	if (buffer == nullptr || size == 0)
		return {EINVAL, {}};

	{
		const auto ret = minSize != 0 ? writeMutex_.lock() : writeMutex_.tryLock();
		if (ret != 0)
			return {ret != EBUSY ? ret : EAGAIN, {}};
	}
	const auto writeMutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				writeMutex_.unlock();
			});

	if (openCount_ == 0)
		return {EBADF, {}};

	if (characterLength_ > 8 && size % 2 != 0)
		return {EINVAL, {}};

	// when character length is greater than 8 bits, round up "minSize" value
	const auto adjustedMinSize = std::min(size, characterLength_ <= 8 ? minSize : ((minSize + 1) / 2) * 2);
	const auto bufferUint8 = static_cast<const uint8_t*>(buffer);

	// initially try to write as much data as possible to circular buffer with no blocking
	auto bytesWritten = writeToCircularBuffer(bufferUint8, size, *currentWriteBuffer_);
	{
		const auto ret = startWriteWrapper();
		if (ret != 0 || bytesWritten == size)
			return {ret, bytesWritten};
	}

	const auto writeLimitScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				writeLimit_ = 0;
			});

	// Not whole data could be written to circular buffer? The current write transfer (if any) must be stopped for a
	// short moment to get the amount of free space in the circular buffer (interrupts are masked to prevent preemption,
	// which could make this "short moment" very long). By subtracting this value from the amount of data left to write
	// we get size limit of write operation. Notification after exactly that number of bytes will mean that the buffer
	// is "empty enough" to receive everything that is left to write.
	{
		architecture::InterruptMaskingLock interruptMaskingLock;
		stopWriteWrapper();
		const auto bytesFree = currentWriteBuffer_->getCapacity() - currentWriteBuffer_->getSize();
		writeLimit_ = adjustedMinSize > (bytesWritten + bytesFree) ? adjustedMinSize - (bytesWritten + bytesFree) : 0;
		{
			const auto ret = startWriteWrapper();
			if (ret != 0)
				return {ret, bytesWritten};
		}
	}

	do
	{
		Semaphore semaphore {0};
		writeSemaphore_ = &semaphore;
		const auto writeSemaphoreScopeGuard = estd::makeScopeGuard(
				[this]()
				{
					writeSemaphore_ = {};
				});

		bytesWritten += writeToCircularBuffer(bufferUint8 + bytesWritten, size - bytesWritten, *currentWriteBuffer_);
		{
			const auto ret = startWriteWrapper();
			if (ret != 0)
				return {ret, bytesWritten};
		}

		if (bytesWritten < adjustedMinSize)	// wait for free space only if requested minimum is not already written
		{
			const auto ret = semaphore.wait();
			if (ret != 0)
				return {ret, bytesWritten};
		}
	} while (bytesWritten < adjustedMinSize);

	return {bytesWritten != 0 ? 0 : EAGAIN, bytesWritten};
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

void SerialPort::readCompleteEvent(const size_t bytesRead)
{
	currentReadBuffer_->increaseWritePosition(bytesRead);
	const auto readLimit = readLimit_;
	readLimit_ = readLimit - (bytesRead < readLimit ? bytesRead : readLimit);
	readInProgress_ = false;

	const auto readSemaphore = readSemaphore_;
	if (readSemaphore != nullptr)
	{
		readSemaphore->post();
		readSemaphore_ = {};
	}

	startReadWrapper();
}

void SerialPort::receiveErrorEvent(ErrorSet)
{

}

void SerialPort::transmitCompleteEvent()
{
	const auto transmitSemaphore = transmitSemaphore_;
	if (transmitSemaphore != nullptr)
	{
		transmitSemaphore->post();
		transmitSemaphore_ = {};
	}

	transmitInProgress_ = false;
}

void SerialPort::transmitStartEvent()
{
	transmitInProgress_ = true;
}

void SerialPort::writeCompleteEvent(const size_t bytesWritten)
{
	currentWriteBuffer_->increaseReadPosition(bytesWritten);
	const auto writeLimit = writeLimit_;
	writeLimit_ = writeLimit - (bytesWritten < writeLimit ? bytesWritten : writeLimit);
	writeInProgress_ = false;

	const auto writeSemaphore = writeSemaphore_;
	if (writeSemaphore != nullptr)
	{
		writeSemaphore->post();
		writeSemaphore_ = {};
	}

	startWriteWrapper();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int SerialPort::startReadWrapper()
{
	if (readInProgress_ == true || currentReadBuffer_->isFull() == true)
		return 0;

	readInProgress_ = true;
	const auto writeBlock = currentReadBuffer_->getWriteBlock();
	// rounding up is valid, capacity is never less than 2 and is always even
	const auto readBufferHalf = ((currentReadBuffer_->getCapacity() / 2 + 1) / 2) * 2;
	const auto readLimit = readLimit_;
	return uart_.startRead(writeBlock.first,
			std::min({writeBlock.second, readBufferHalf, readLimit != 0 ? readLimit : SIZE_MAX}));
}

int SerialPort::startWriteWrapper()
{
	if (writeInProgress_ == true || currentWriteBuffer_->isEmpty() == true)
		return 0;

	writeInProgress_ = true;
	const auto readBlock = currentWriteBuffer_->getReadBlock();
	// rounding up is valid, capacity is never less than 2 and is always even
	const auto writeBufferHalf = ((currentWriteBuffer_->getCapacity() / 2 + 1) / 2) * 2;
	const auto writeLimit = writeLimit_;
	return uart_.startWrite(readBlock.first,
			std::min({readBlock.second, writeBufferHalf, writeLimit != 0 ? writeLimit : SIZE_MAX}));
}

size_t SerialPort::stopReadWrapper()
{
	const auto bytesRead = uart_.stopRead();
	currentReadBuffer_->increaseWritePosition(bytesRead);
	const auto readLimit = readLimit_;
	readLimit_ = readLimit - (bytesRead < readLimit ? bytesRead : readLimit);
	readInProgress_ = false;
	return bytesRead;
}

size_t SerialPort::stopWriteWrapper()
{
	const auto bytesWritten = uart_.stopWrite();
	currentWriteBuffer_->increaseReadPosition(bytesWritten);
	const auto writeLimit = writeLimit_;
	writeLimit_ = writeLimit - (bytesWritten < writeLimit ? bytesWritten : writeLimit);
	writeInProgress_ = false;
	return bytesWritten;
}

}	// namespace devices

}	// namespace distortos
