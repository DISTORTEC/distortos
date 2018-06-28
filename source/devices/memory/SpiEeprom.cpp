/**
 * \file
 * \brief SpiEeprom class implementation
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/SpiEeprom.hpp"

#include "distortos/devices/communication/SpiMasterOperation.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/assert.h"
#include "distortos/ThisThread.hpp"

#include "estd/ScopeGuard.hpp"

#include <tuple>

#include <cerrno>

namespace distortos
{

namespace devices
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// buffer for command and address
using CommandWithAddressBuffer = std::array<uint8_t, 4>;

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// RDSR (read status register) command
constexpr uint8_t rdsrCommand {0x5};

/// READ command
constexpr uint8_t readCommand {0x3};

/// WREN (write enable) command
constexpr uint8_t wrenCommand {0x6};

/// WRITE command
constexpr uint8_t writeCommand {0x2};

/// mask of WIP (write in progress) bit in status register
constexpr uint8_t statusRegisterWip {1 << 0};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Combines command with address into a "transfer" SpiMasterOperation.
 *
 * The address for READ and WRITE commands is encoded into variable number of bytes, depending on capacity of EEPROM
 * chip:
 * - capacity <= 256 bytes - 1 byte for address,
 * - capacity == 512 bytes - 1 byte for address, MSB of address (9th bit) is placed at 4th bit of command,
 * - 512 bytes < capacity <= 64k bytes - 2 bytes for address,
 * - 64k bytes < capacity - 3 bytes for address.
 *
 * The address is always big-endian.
 *
 * \param [in] capacity is the capacity of EEPROM, bytes
 * \param [in] command is the command that will be combined with \a address into \a buffer, {readCommand, writeCommand}
 * \param [in] address is the address that will be combined with \a command into \a buffer
 * \param [out] buffer is a reference to buffer into which \a command and \a address will be combined
 *
 * \return "transfer" SpiMasterOperation with combined \a command and \a address
 */

SpiMasterOperation::Transfer getCommandWithAddress(const size_t capacity, const uint8_t command, const uint32_t address,
		CommandWithAddressBuffer& buffer)
{
	buffer[0] = command | (capacity == 512 && (address & 0x100) != 0 ? 0x8 : 0);
	const auto addressLength = capacity <= 512 ? size_t{1} : capacity <= 65536 ? size_t{2} : size_t{3};
	for (size_t i {}; i < addressLength; ++i)
		buffer[addressLength - i] = address >> (8 * i);

	return {buffer.begin(), nullptr, 1 + addressLength};
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int SpiEeprom::close()
{
	return spiDevice_.close();
}

std::pair<int, bool> SpiEeprom::isWriteInProgress()
{
	const auto ret = readStatusRegister();
	return {ret.first, (ret.second & statusRegisterWip) != 0};
}

bool SpiEeprom::lock()
{
	return spiDevice_.lock();
}

int SpiEeprom::open()
{
	return spiDevice_.open();
}

std::pair<int, size_t> SpiEeprom::read(const uint32_t address, void* const buffer, const size_t size)
{
	CHECK_FUNCTION_CONTEXT();

	const auto capacity = getCapacity();
	if (address >= capacity || buffer == nullptr || size == 0)
		return {EINVAL, {}};

	const auto previousLockState = spiDevice_.lock();
	const auto unlockScopeGuard = estd::makeScopeGuard(
			[this, previousLockState]()
			{
				spiDevice_.unlock(previousLockState);
			});

	{
		const auto ret = waitWhileWriteInProgress();
		if (ret != 0)
			return {ret, {}};
	}

	CommandWithAddressBuffer commandBuffer;
	SpiMasterOperation operations[]
	{
			getCommandWithAddress(capacity, readCommand, address, commandBuffer),
			SpiMasterOperation::Transfer{nullptr, buffer, address + size <= capacity ? size : capacity - address},
	};
	const auto ret = spiDevice_.executeTransaction(SpiMasterOperationsRange{operations});
	return {ret.first, operations[1].getTransfer()->getBytesTransfered()};
}

void SpiEeprom::unlock(const bool previousLockState)
{
	spiDevice_.unlock(previousLockState);
}

int SpiEeprom::waitWhileWriteInProgress()
{
	decltype(isWriteInProgress().first) ret;
	decltype(isWriteInProgress().second) writeInProgress;
	while (std::tie(ret, writeInProgress) = isWriteInProgress(), ret == 0 && writeInProgress == true)
	{
		const auto sleepForRet = ThisThread::sleepFor(std::chrono::milliseconds{1});
		if (sleepForRet != 0)
			return sleepForRet;
	}

	return ret;
}

std::pair<int, size_t> SpiEeprom::write(const uint32_t address, const void* const buffer, const size_t size)
{
	CHECK_FUNCTION_CONTEXT();

	const auto capacity = getCapacity();
	if (address >= capacity || buffer == nullptr || size == 0)
		return {EINVAL, {}};

	const auto previousLockState = spiDevice_.lock();
	const auto unlockScopeGuard = estd::makeScopeGuard(
			[this, previousLockState]()
			{
				spiDevice_.unlock(previousLockState);
			});

	size_t written {};
	const auto writeSize = address + size <= capacity ? size : capacity - address;
	const auto bufferUint8 = static_cast<const uint8_t*>(buffer);
	while (written < writeSize)
	{
		const auto ret = writePage(address + written, &bufferUint8[written], writeSize - written);
		written += ret.second;
		if (ret.first != 0)
			return {ret.first, written};
	}

	return {{}, written};
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, uint8_t> SpiEeprom::readStatusRegister()
{
	uint8_t buffer[2] {rdsrCommand, 0xff};
	SpiMasterOperation operation {SpiMasterOperation::Transfer{buffer, buffer, sizeof(buffer)}};
	const auto ret = spiDevice_.executeTransaction(SpiMasterOperationsRange{operation});
	return {ret.first, buffer[1]};
}

int SpiEeprom::writeEnable()
{
	SpiMasterOperation operation {SpiMasterOperation::Transfer{&wrenCommand, nullptr, sizeof(wrenCommand)}};
	return spiDevice_.executeTransaction(SpiMasterOperationsRange{operation}).first;
}

std::pair<int, size_t> SpiEeprom::writePage(const uint32_t address, const void* const buffer, const size_t size)
{
	const auto capacity = getCapacity();
	assert(address < capacity && "Invalid address!");

	{
		const auto ret = waitWhileWriteInProgress();
		if (ret != 0)
			return {ret, {}};
	}
	{
		const auto ret = writeEnable();
		if (ret != 0)
			return {ret, {}};
	}

	const auto pageSize = getPageSize();
	const auto pageOffset = address & (pageSize - 1);
	const auto writeSize = pageOffset + size <= pageSize ? size : pageSize - pageOffset;
	CommandWithAddressBuffer commandBuffer;
	SpiMasterOperation operations[]
	{
			getCommandWithAddress(capacity, writeCommand, address, commandBuffer),
			SpiMasterOperation::Transfer{buffer, nullptr, writeSize},
	};
	const auto ret = spiDevice_.executeTransaction(SpiMasterOperationsRange{operations});
	return {ret.first, operations[1].getTransfer()->getBytesTransfered()};
}

}	// namespace devices

}	// namespace distortos
