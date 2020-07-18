/**
 * \file
 * \brief SerialPort class implementation
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SerialPort.hpp"

#include "distortos/devices/communication/UartLowLevel.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/InterruptMaskingLock.hpp"
#include "distortos/Semaphore.hpp"

#include "estd/ScopeGuard.hpp"

#include <mutex>

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
 * \brief Copies single block from one raw circular buffer to another.
 *
 * \param [in] source is a reference to raw circular buffer from which the data will be read
 * \param [out] destination is a reference to raw circular buffer to which the data will be written
 *
 * \return number of bytes read from \a source and written to \a destination
 */

size_t copySingleBlock(estd::RawCircularBuffer& source, estd::RawCircularBuffer& destination)
{
	const auto readBlock = source.getReadBlock();
	if (readBlock.second == 0)
		return 0;
	const auto writeBlock = destination.getWriteBlock();
	if (writeBlock.second == 0)
		return 0;

	const auto size = std::min(readBlock.second, writeBlock.second);
	memcpy(writeBlock.first, readBlock.first, size);
	source.increaseReadPosition(size);
	destination.increaseWritePosition(size);
	return size;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SerialPort::~SerialPort()
{
	CHECK_FUNCTION_CONTEXT();

	if (openCount_ == 0)
		return;

	const std::lock_guard<Mutex> readLockGuard {readMutex_};
	const std::lock_guard<Mutex> writeLockGuard {writeMutex_};

	uart_.stopRead();
	uart_.stopWrite();
	uart_.stop();
}

int SerialPort::close()
{
	const std::lock_guard<Mutex> readLockGuard {readMutex_};
	const std::lock_guard<Mutex> writeLockGuard {writeMutex_};

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

int SerialPort::open(const uint32_t baudRate, const uint8_t characterLength, const UartParity parity,
			const bool _2StopBits)
{
	const std::lock_guard<Mutex> readLockGuard {readMutex_};
	const std::lock_guard<Mutex> writeLockGuard {writeMutex_};

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

std::pair<int, size_t> SerialPort::read(void* const buffer, const size_t size, const size_t minSize,
		const TickClock::time_point* const timePoint)
{
	CHECK_FUNCTION_CONTEXT();

	if (buffer == nullptr || size == 0)
		return {EINVAL, {}};

	{
		const auto ret = minSize == 0 ? readMutex_.tryLock() :
				timePoint != nullptr ? readMutex_.tryLockUntil(*timePoint) : readMutex_.lock();
		if (ret != 0)
			return {ret != EBUSY ? ret : EAGAIN, {}};
	}

	const std::lock_guard<Mutex> readLockGuard {readMutex_, std::adopt_lock};

	if (openCount_ == 0)
		return {EBADF, {}};

	if (characterLength_ > 8 && size % 2 != 0)
		return {EINVAL, {}};

	estd::RawCircularBuffer localReadBuffer {buffer, size};
	const auto ret = readImplementation(localReadBuffer, minSize, timePoint);
	const auto bytesRead = localReadBuffer.getSize();
	return {ret != 0 || bytesRead != 0 ? ret : EAGAIN, bytesRead};
}

std::pair<int, size_t> SerialPort::write(const void* const buffer, const size_t size, const size_t minSize,
		const TickClock::time_point* const timePoint)
{
	CHECK_FUNCTION_CONTEXT();

	if (buffer == nullptr || size == 0)
		return {EINVAL, {}};

	{
		const auto ret = minSize == 0 ? writeMutex_.tryLock() :
				timePoint != nullptr ? writeMutex_.tryLockUntil(*timePoint) : writeMutex_.lock();
		if (ret != 0)
			return {ret != EBUSY ? ret : EAGAIN, {}};
	}

	const std::lock_guard<Mutex> writeLockGuard {writeMutex_, std::adopt_lock};

	if (openCount_ == 0)
		return {EBADF, {}};

	if (characterLength_ > 8 && size % 2 != 0)
		return {EINVAL, {}};

	estd::RawCircularBuffer localWriteBuffer {buffer, size};	// local buffer is read-only
	localWriteBuffer.increaseWritePosition(size);	// make the buffer "full"
	const auto ret = writeImplementation(localWriteBuffer, minSize, timePoint);
	const auto bytesWritten = localWriteBuffer.getCapacity() - localWriteBuffer.getSize();
	return {ret != 0 || bytesWritten != 0 ? ret : EAGAIN, bytesWritten};
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

void SerialPort::readCompleteEvent(const size_t bytesRead)
{
	const auto currentReadBuffer = currentReadBuffer_;
	currentReadBuffer->increaseWritePosition(bytesRead);

	const auto nextReadBuffer = nextReadBuffer_;
	if (nextReadBuffer != nullptr && currentReadBuffer->isFull() == true)
	{
		nextReadBuffer_ = {};
		currentReadBuffer_ = nextReadBuffer;
	}

	const auto oldReadLimit = readLimit_;
	const auto newReadLimit = oldReadLimit - (bytesRead < oldReadLimit ? bytesRead : oldReadLimit);
	readLimit_ = newReadLimit;
	if (newReadLimit == 0 && oldReadLimit != 0)
	{
		const auto readSemaphore = readSemaphore_;
		if (readSemaphore != nullptr)
		{
			readSemaphore->post();
			readSemaphore_ = {};
		}
	}

	readInProgress_ = false;

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
	const auto currentWriteBuffer = currentWriteBuffer_;
	currentWriteBuffer->increaseReadPosition(bytesWritten);

	const auto nextWriteBuffer = nextWriteBuffer_;
	if (nextWriteBuffer != nullptr && currentWriteBuffer->isEmpty() == true)
	{
		nextWriteBuffer_ = {};
		currentWriteBuffer_ = nextWriteBuffer;
	}

	const auto oldWriteLimit = writeLimit_;
	const auto newWriteLimit = oldWriteLimit - (bytesWritten < oldWriteLimit ? bytesWritten : oldWriteLimit);
	writeLimit_ = newWriteLimit;
	if (newWriteLimit == 0 && oldWriteLimit != 0)
	{
		const auto writeSemaphore = writeSemaphore_;
		if (writeSemaphore != nullptr)
		{
			writeSemaphore->post();
			writeSemaphore_ = {};
		}
	}

	writeInProgress_ = false;

	startWriteWrapper();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int SerialPort::readFromRawCircularBufferAndStartRead(estd::RawCircularBuffer& buffer)
{
	while (copySingleBlock(readBuffer_, buffer) != 0)
	{
		const auto ret = startReadWrapper();
		if (ret != 0)
			return ret;
	}

	return 0;
}

int SerialPort::readImplementation(estd::RawCircularBuffer& buffer, const size_t minSize,
		const TickClock::time_point* const timePoint)
{
	// when character length is greater than 8 bits, round up "minSize" value
	const auto adjustedMinSize =
			std::min(buffer.getCapacity(), characterLength_ <= 8 ? minSize : ((minSize + 1) / 2) * 2);

	// initially try to read as much data as possible from raw circular buffer with no blocking
	{
		const auto ret = readFromRawCircularBufferAndStartRead(buffer);
		if (ret != 0 || buffer.isFull() == true)
			return ret;
	}

	decltype(startReadWrapper()) scopeGuardRet {};
	decltype(std::declval<Semaphore>().wait()) semaphoreRet {};
	{
		Semaphore semaphore {0};
		const auto scopeGuard = estd::makeScopeGuard(
				[this, &scopeGuardRet]()
				{
					readLimit_ = {};
					readSemaphore_ = {};

					if (currentReadBuffer_ == &readBuffer_)
						return;

					// restore internal circular read buffer
					const InterruptMaskingLock interruptMaskingLock;
					stopReadWrapper();
					nextReadBuffer_ = {};
					currentReadBuffer_ = &readBuffer_;
					scopeGuardRet = startReadWrapper();
				});

		decltype(buffer.getWriteBlock()) writeBlock {};
		decltype(startReadWrapper()) startReadRet {};
		{
			// Full buffer could not be read from raw circular buffer? The current read transfer (if any) must be
			// stopped for a short moment to get the amount of data available in the raw circular buffer (interrupts are
			// masked to prevent preemption, which could make this "short moment" very long). By subtracting this value
			// from the minimum amount of data required for reading we get size limit of read operation. Notification
			// after exactly that number of bytes will mean that the buffer has enough data to satisfy requested minimum
			// size.
			const InterruptMaskingLock interruptMaskingLock;
			stopReadWrapper();
			writeBlock = buffer.getWriteBlock();
			writeBlock.second = std::min(writeBlock.second, readBuffer_.getSize());
			buffer.increaseWritePosition(writeBlock.second);
			const auto bytesRead = buffer.getSize();

			if (adjustedMinSize > bytesRead)	// is blocking required?
			{
				// arrange read operation directly to output buffer
				nextReadBuffer_ = &readBuffer_;
				currentReadBuffer_ = &buffer;
				readLimit_ = adjustedMinSize - bytesRead;
				readSemaphore_ = &semaphore;
			}

			startReadRet = startReadWrapper();
		}

		estd::RawCircularBuffer subBuffer {writeBlock.first, writeBlock.second};
		{
			// read the data that was "released" by stopping and staring read operation
			const auto ret = readFromRawCircularBufferAndStartRead(subBuffer);
			if (startReadRet != 0 || ret != 0 || buffer.getSize() >= adjustedMinSize)
				return startReadRet != 0 ? startReadRet : ret;
		}

		semaphoreRet = timePoint != nullptr ? semaphore.tryWaitUntil(*timePoint) : semaphore.wait();
	}

	return scopeGuardRet != 0 ? scopeGuardRet : semaphoreRet;
}

int SerialPort::startReadWrapper()
{
	if (readInProgress_ == true)
		return 0;

	const auto currentReadBuffer = currentReadBuffer_;
	if (currentReadBuffer->isFull() == true)
		return 0;

	readInProgress_ = true;
	const auto writeBlock = currentReadBuffer->getWriteBlock();
	// rounding up is valid for internal buffer, capacity is never less than 2 and is always even
	const auto readBufferHalf = currentReadBuffer == &readBuffer_ ?
			((currentReadBuffer->getCapacity() / 2 + 1) / 2) * 2 : SIZE_MAX;
	const auto readLimit = readLimit_;
	return uart_.startRead(writeBlock.first,
			std::min({writeBlock.second, readBufferHalf, readLimit != 0 ? readLimit : SIZE_MAX}));
}

int SerialPort::startWriteWrapper()
{
	if (writeInProgress_ == true)
			return 0;

	const auto currentWriteBuffer = currentWriteBuffer_;
	if (currentWriteBuffer->isEmpty() == true)
		return 0;

	writeInProgress_ = true;
	const auto readBlock = currentWriteBuffer->getReadBlock();
	// rounding up is valid for internal buffer, capacity is never less than 2 and is always even
	const auto writeBufferHalf = currentWriteBuffer == &writeBuffer_ ?
			((currentWriteBuffer->getCapacity() / 2 + 1) / 2) * 2 : SIZE_MAX;
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

int SerialPort::writeImplementation(estd::RawCircularBuffer& buffer, const size_t minSize,
		const TickClock::time_point* const timePoint)
{
	// when character length is greater than 8 bits, round up "minSize" value
	const auto adjustedMinSize =
			std::min(buffer.getCapacity(), characterLength_ <= 8 ? minSize : ((minSize + 1) / 2) * 2);

	decltype(startWriteWrapper()) scopeGuardRet {};
	decltype(std::declval<Semaphore>().wait()) semaphoreRet {};
	{
		Semaphore semaphore {0};
		const auto scopeGuard = estd::makeScopeGuard(
				[this, &scopeGuardRet]()
				{
					writeLimit_ = {};
					writeSemaphore_ = {};

					// restore internal circular write buffer
					const InterruptMaskingLock interruptMaskingLock;
					stopWriteWrapper();
					nextWriteBuffer_ = {};
					currentWriteBuffer_ = &writeBuffer_;
					scopeGuardRet = startWriteWrapper();
				});

		bool block {};
		{
			// Current write transfer (if any) must be stopped for a short moment to get the amount of free space in the
			// raw circular buffer (interrupts are masked to prevent preemption, which could make this "short moment"
			// very long). By subtracting this value from the minimum amount of data to write we get size limit of write
			// operation. Notification after exactly that number of bytes will mean that the buffer is "empty enough" to
			// receive everything that is left to write.
			const InterruptMaskingLock interruptMaskingLock;
			stopWriteWrapper();
			const auto capacity = writeBuffer_.getCapacity();
			const auto size = writeBuffer_.getSize();
			const auto bytesFree = capacity - size;
			// arrange write operation directly from output buffer
			if (size == 0)	// internal buffer is empty?
				currentWriteBuffer_ = &buffer;	// start with local buffer
			nextWriteBuffer_ = size == 0 ? &writeBuffer_ : &buffer;
			if (adjustedMinSize > bytesFree)	// is blocking required?
			{
				writeLimit_ = adjustedMinSize - bytesFree;
				writeSemaphore_ = &semaphore;
				block = true;
			}
			const auto ret = startWriteWrapper();
			if (ret != 0)
				return ret;
		}

		if (block == true)
			semaphoreRet = timePoint != nullptr ? semaphore.tryWaitUntil(*timePoint) : semaphore.wait();
	}

	const auto ret = writeToRawCircularBufferAndStartWrite(buffer);
	return scopeGuardRet != 0 ? scopeGuardRet : semaphoreRet != 0 ? semaphoreRet : ret;
}

int SerialPort::writeToRawCircularBufferAndStartWrite(estd::RawCircularBuffer& buffer)
{
	while (copySingleBlock(buffer, writeBuffer_) != 0)
	{
		const auto ret = startWriteWrapper();
		if (ret != 0)
			return ret;
	}

	return 0;
}

}	// namespace devices

}	// namespace distortos
