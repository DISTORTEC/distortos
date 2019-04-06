/**
 * \file
 * \brief SpiEeprom class implementation
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/SpiEeprom.hpp"

#include "distortos/devices/communication/SpiDeviceHandle.hpp"
#include "distortos/devices/communication/SpiDeviceSelectGuard.hpp"
#include "distortos/devices/communication/SpiMasterHandle.hpp"
#include "distortos/devices/communication/SpiMasterTransfer.hpp"

#include "distortos/assert.h"
#include "distortos/ThisThread.hpp"

#include <tuple>

namespace distortos
{

namespace devices
{

namespace
{

static_assert(CONFIG_SPIMASTER_BUFFER_ALIGNMENT <= CONFIG_BLOCKDEVICE_BUFFER_ALIGNMENT,
		"Buffer alignment for SpiMaster is greater than for BlockDevice!");

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
 * \brief Combines command with address into a SpiMasterTransfer.
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
 * \return SpiMasterTransfer with combined \a command and \a address
 */

SpiMasterTransfer getCommandWithAddress(const size_t capacity, const uint8_t command, const uint32_t address,
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

SpiEeprom::~SpiEeprom()
{
	assert(spiDevice_.isOpened() == false);
}

int SpiEeprom::close()
{
	const SpiDeviceHandle spiDeviceHandle {spiDevice_};

	assert(spiDevice_.isOpened() == true);

	return spiDevice_.close();
}

int SpiEeprom::erase(const uint64_t address, const uint64_t size)
{
	const SpiDeviceHandle spiDeviceHandle {spiDevice_};

	return eraseOrWrite(spiDeviceHandle, address, nullptr, size);
}

size_t SpiEeprom::getBlockSize() const
{
	return 1;
}

uint64_t SpiEeprom::getSize() const
{
	return 128 * (1 << ((static_cast<uint8_t>(type_) & sizeMask_) >> sizeShift_));
}

void SpiEeprom::lock()
{
	const auto ret = spiDevice_.lock();
	assert(ret == 0);
}

int SpiEeprom::open()
{
	const auto ret = spiDevice_.open();
	assert(ret != EMFILE);
	return ret;
}

int SpiEeprom::read(const uint64_t address, void* const buffer, const size_t size)
{
	const SpiDeviceHandle spiDeviceHandle {spiDevice_};

	assert(spiDevice_.isOpened() == true);
	assert(buffer != nullptr);

	const auto capacity = getSize();
	assert(address + size <= capacity);

	if (size == 0)
		return {};

	{
		const auto ret = synchronize(spiDeviceHandle);
		if (ret != 0)
			return ret;
	}

	CommandWithAddressBuffer commandBuffer;
	SpiMasterTransfer transfers[]
	{
			getCommandWithAddress(capacity, readCommand, address, commandBuffer),
			{nullptr, buffer, address + size <= capacity ? size : static_cast<size_t>(capacity - address)},
	};
	const auto ret = executeTransaction(SpiMasterTransfersRange{transfers});
	return ret.first;
}

int SpiEeprom::synchronize()
{
	const SpiDeviceHandle spiDeviceHandle {spiDevice_};

	assert(spiDevice_.isOpened() == true);

	return synchronize(spiDeviceHandle);
}

void SpiEeprom::unlock()
{
	const auto ret = spiDevice_.unlock();
	assert(ret == 0);
}

int SpiEeprom::write(const uint64_t address, const void* const buffer, const size_t size)
{
	const SpiDeviceHandle spiDeviceHandle {spiDevice_};

	assert(buffer != nullptr);

	return eraseOrWrite(spiDeviceHandle, address, buffer, size);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int SpiEeprom::eraseOrWrite(const SpiDeviceHandle& spiDeviceHandle, const uint64_t address, const void* const buffer,
		const uint64_t size)
{
	assert(spiDevice_.isOpened() == true);

	const auto capacity = getSize();
	assert(address + size <= capacity);

	if (size == 0)
		return {};

	size_t written {};
	const auto writeSize = address + size <= capacity ? size : capacity - address;
	const auto bufferUint8 = static_cast<const uint8_t*>(buffer);
	while (written < writeSize)
	{
		const auto ret = eraseOrWritePage(spiDeviceHandle, address + written,
				bufferUint8 != nullptr ? bufferUint8 + written : bufferUint8, writeSize - written);
		written += ret.second;
		if (ret.first != 0)
			return ret.first;
	}

	return {};
}

std::pair<int, size_t> SpiEeprom::eraseOrWritePage(const SpiDeviceHandle& spiDeviceHandle, const uint32_t address,
		const void* const buffer, const size_t size)
{
	const auto capacity = getSize();
	assert(address < capacity && "Invalid address!");

	{
		const auto ret = synchronize(spiDeviceHandle);
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
	SpiMasterTransfer transfers[]
	{
			getCommandWithAddress(capacity, writeCommand, address, commandBuffer),
			{buffer, nullptr, writeSize},
	};
	const auto ret = executeTransaction(SpiMasterTransfersRange{transfers});
	return {ret.first, transfers[1].getBytesTransfered()};
}

std::pair<int, size_t> SpiEeprom::executeTransaction(const SpiMasterTransfersRange transfersRange) const
{
	SpiMasterHandle spiMasterHandle {spiMaster_};

	{
		// only datasheet for ST M95xxx series says that erased state is 0, assume this is true for all other devices
		const auto ret = spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});
		if (ret.first != 0)
			return {ret.first, {}};
	}

	const SpiDeviceSelectGuard spiDeviceSelectGuard {slaveSelectPin_};

	return spiMasterHandle.executeTransaction(transfersRange);
}

std::pair<int, bool> SpiEeprom::isWriteInProgress(const SpiDeviceHandle&)
{
	const auto ret = readStatusRegister();
	return {ret.first, (ret.second & statusRegisterWip) != 0};
}

std::pair<int, uint8_t> SpiEeprom::readStatusRegister() const
{
	uint8_t buffer[2] {rdsrCommand, 0xff};
	SpiMasterTransfer transfer {buffer, buffer, sizeof(buffer)};
	const auto ret = executeTransaction(SpiMasterTransfersRange{transfer});
	return {ret.first, buffer[1]};
}

int SpiEeprom::synchronize(const SpiDeviceHandle& spiDeviceHandle)
{
	decltype(isWriteInProgress(spiDeviceHandle).first) ret;
	decltype(isWriteInProgress(spiDeviceHandle).second) writeInProgress;
	while (std::tie(ret, writeInProgress) = isWriteInProgress(spiDeviceHandle), ret == 0 && writeInProgress == true)
	{
		const auto sleepForRet = ThisThread::sleepFor(std::chrono::milliseconds{1});
		if (sleepForRet != 0)
			return sleepForRet;
	}

	return ret;
}

int SpiEeprom::writeEnable() const
{
	SpiMasterTransfer transfer {&wrenCommand, nullptr, sizeof(wrenCommand)};
	const auto ret = executeTransaction(SpiMasterTransfersRange{transfer});
	return ret.first;
}

}	// namespace devices

}	// namespace distortos
