/**
 * \file
 * \brief SpiEepromProxy class implementation
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/SpiEepromProxy.hpp"

#include "distortos/devices/communication/SpiDeviceSelectGuard.hpp"
#include "distortos/devices/communication/SpiMasterOperation.hpp"
#include "distortos/devices/communication/SpiMasterProxy.hpp"

#include "distortos/devices/memory/SpiEeprom.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/assert.h"
#include "distortos/ThisThread.hpp"

#include <tuple>

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

SpiEepromProxy::SpiEepromProxy(SpiEeprom& spiEeprom) :
		spiDeviceProxy_{spiEeprom.spiDevice_},
		spiEeprom_{spiEeprom}
{

}

std::pair<int, bool> SpiEepromProxy::isWriteInProgress() const
{
	CHECK_FUNCTION_CONTEXT();

	const auto ret = readStatusRegister();
	return {ret.first, (ret.second & statusRegisterWip) != 0};
}

std::pair<int, size_t> SpiEepromProxy::read(const uint32_t address, void* const buffer, const size_t size) const
{
	CHECK_FUNCTION_CONTEXT();

	const auto capacity = spiEeprom_.getCapacity();
	if (address >= capacity || buffer == nullptr || size == 0)
		return {EINVAL, {}};

	{
		const auto ret = waitWhileWriteInProgress();
		if (ret != 0)
			return {ret, {}};
	}

	CommandWithAddressBuffer commandBuffer;
	SpiMasterOperation operations[]
	{
			getCommandWithAddress(capacity, readCommand, address, commandBuffer),
			{{nullptr, buffer, address + size <= capacity ? size : capacity - address}},
	};
	const auto ret = executeTransaction(SpiMasterOperationsRange{operations});
	return {ret.first, operations[1].getTransfer()->getBytesTransfered()};
}

int SpiEepromProxy::waitWhileWriteInProgress() const
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

std::pair<int, size_t> SpiEepromProxy::write(const uint32_t address, const void* const buffer, const size_t size) const
{
	CHECK_FUNCTION_CONTEXT();

	const auto capacity = spiEeprom_.getCapacity();
	if (address >= capacity || buffer == nullptr || size == 0)
		return {EINVAL, {}};

	size_t written {};
	const auto writeSize = address + size <= capacity ? size : capacity - address;
	const auto bufferUint8 = static_cast<const uint8_t*>(buffer);
	while (written < writeSize)
	{
		const auto ret = eraseOrWritePage(address + written, &bufferUint8[written], writeSize - written);
		written += ret.second;
		if (ret.first != 0)
			return {ret.first, written};
	}

	return {{}, written};
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, size_t> SpiEepromProxy::eraseOrWritePage(const uint32_t address, const void* const buffer,
		const size_t size) const
{
	const auto capacity = spiEeprom_.getCapacity();
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

	const auto pageSize = spiEeprom_.getPageSize();
	const auto pageOffset = address & (pageSize - 1);
	const auto writeSize = pageOffset + size <= pageSize ? size : pageSize - pageOffset;
	CommandWithAddressBuffer commandBuffer;
	SpiMasterOperation operations[]
	{
			getCommandWithAddress(capacity, writeCommand, address, commandBuffer),
			{{buffer, nullptr, writeSize}},
	};
	const auto ret = executeTransaction(SpiMasterOperationsRange{operations});
	return {ret.first, operations[1].getTransfer()->getBytesTransfered()};
}

std::pair<int, size_t> SpiEepromProxy::executeTransaction(const SpiMasterOperationsRange operationsRange) const
{
	SpiMasterProxy spiMasterProxy {spiDeviceProxy_};

	{
		// only datasheet for ST M95xxx series says that erased state is 0, assume this is true for all other devices
		const auto ret = spiMasterProxy.configure(spiEeprom_.mode_, spiEeprom_.clockFrequency_, 8, false, {});
		if (ret.first != 0)
			return {ret.first, {}};
	}

	const SpiDeviceSelectGuard spiDeviceSelectGuard {spiMasterProxy};

	return spiMasterProxy.executeTransaction(operationsRange);
}

std::pair<int, uint8_t> SpiEepromProxy::readStatusRegister() const
{
	uint8_t buffer[2] {rdsrCommand, 0xff};
	SpiMasterOperation operation {{buffer, buffer, sizeof(buffer)}};
	const auto ret = executeTransaction(SpiMasterOperationsRange{operation});
	return {ret.first, buffer[1]};
}

int SpiEepromProxy::writeEnable() const
{
	SpiMasterOperation operation {{&wrenCommand, nullptr, sizeof(wrenCommand)}};
	const auto ret = executeTransaction(SpiMasterOperationsRange{operation});
	return ret.first;
}

}	// namespace devices

}	// namespace distortos
