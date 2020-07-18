/**
 * \file
 * \brief QspiNorFlashSpiBased class implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/QspiNorFlashSpiBased.hpp"

#include "distortos/devices/communication/SpiDeviceSelectGuard.hpp"
#include "distortos/devices/communication/SpiMasterHandle.hpp"
#include "distortos/devices/communication/SpiMasterTransfer.hpp"

#include "distortos/assert.h"
#include "distortos/ThisThread.hpp"

#include "estd/EnumClassFlags.hpp"
#include "estd/extractBitField.hpp"
#include "estd/ScopeGuard.hpp"

#include <mutex>

#include <cstring>

namespace estd
{

/// \brief Enable bitwise operators for distortos::devices::QspiNorFlashSpiBased::AddressFlags
template<>
struct isEnumClassFlags<distortos::devices::QspiNorFlashSpiBased::AddressFlags> : std::true_type
{

};

/// \brief Enable bitwise operators for distortos::devices::QspiNorFlashSpiBased::SoftwareResetFlags
template<>
struct isEnumClassFlags<distortos::devices::QspiNorFlashSpiBased::SoftwareResetFlags> : std::true_type
{

};

}	// namespace estd

namespace distortos
{

namespace devices
{

namespace
{

static_assert(DISTORTOS_SPIMASTER_BUFFER_ALIGNMENT <= DISTORTOS_MEMORYTECHNOLOGYDEVICE_BUFFER_ALIGNMENT,
		"Buffer alignment for SpiMaster is greater than for MemoryTechnologyDevice!");

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// import QspiNorFlashSpiBased::AddressFlags
using AddressFlags = QspiNorFlashSpiBased::AddressFlags;

/// import QspiNorFlashSpiBased::BasicFlashParameters
using BasicFlashParameters = QspiNorFlashSpiBased::BasicFlashParameters;

/// single DWORD of SFDP
using Dword = uint32_t;

/// import QspiNorFlashSpiBased::SectorMap
using SectorMap = QspiNorFlashSpiBased::SectorMap;

/// import QspiNorFlashSpiBased::SoftwareResetFlags
using SoftwareResetFlags = QspiNorFlashSpiBased::SoftwareResetFlags;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Extracts a bit field from array of DWORDs.
 *
 * \tparam dword is the index of starting DWORD
 * \tparam index is the index of starting bit in selected DWORD
 * \tparam size is the size of bit field to extract, bits
 * \tparam Ret is the type of returned value, default - fixed width type with at least \a size bits
 * \tparam arraySize is the number of elements in \a array, default - deduced from argument
 *
 * \param [in] array is a reference to array with raw DWORDs from which the bit field will be extracted
 *
 * \return bit field extracted from \a array
 */

template<size_t dword, size_t index, size_t size, typename Ret = estd::TypeFromSize<(size + CHAR_BIT - 1) / CHAR_BIT>,
		size_t arraySize>
inline static Ret extractBitField(const std::array<Dword, arraySize>& array)
{
	return estd::extractBitField<dword * sizeof(Dword) * CHAR_BIT + index, size>(array);
}

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// first DWORD of Configuration Detection Command Descriptor
class ConfigurationDetectionCommandDescriptor
{
public:

	/// type of raw data with first DWORD of Configuration Detection Command Descriptor
	using RawData = std::array<Dword, 1>;

	/**
	 * \brief ConfigurationDetectionCommandDescriptor's constructor
	 *
	 * \param [in] rawData is the raw data with first DWORD of Configuration Detection Command Descriptor
	 */

	constexpr explicit ConfigurationDetectionCommandDescriptor(const RawData rawData) :
			rawData_{rawData}
	{

	}

	/**
	 * \return value of Address Length bit field
	 */

	uint8_t getAddressLength() const
	{
		return extractBitField<0, 22, 2>(rawData_);
	}

	/**
	 * \return value of Instruction bit field
	 */

	uint8_t getInstruction() const
	{
		return extractBitField<0, 8, 8>(rawData_);
	}

	/**
	 * \return value of Read Data Mask bit field
	 */

	uint8_t getReadDataMask() const
	{
		return extractBitField<0, 24, 8>(rawData_);
	}

	/**
	 * \return value of Read Latency bit field
	 */

	uint8_t getReadLatency() const
	{
		return extractBitField<0, 16, 4>(rawData_);
	}

	/**
	 * \return value of Sequence End Indicator bit field
	 */

	uint8_t getSequenceEndIndicator() const
	{
		return extractBitField<0, 0, 1>(rawData_);
	}

private:

	/// raw data with first DWORD of Configuration Detection Command Descriptor
	RawData rawData_;
};

/// Configuration Map Descriptor Header
class ConfigurationMapDescriptorHeader
{
public:

	/// type of raw data with Configuration Map Descriptor Header
	using RawData = std::array<Dword, 1>;

	/**
	 * \brief ConfigurationMapDescriptorHeader's constructor
	 *
	 * \param [in] rawData is the raw data with Configuration Map Descriptor Header
	 */

	constexpr explicit ConfigurationMapDescriptorHeader(const RawData rawData) :
			rawData_{rawData}
	{

	}

	/**
	 * \return value of Configuration ID bit field
	 */

	uint8_t getConfigurationId() const
	{
		return extractBitField<0, 8, 8>(rawData_);
	}

	/**
	 * \return value of Region Count bit field
	 */

	uint8_t getRegionCount() const
	{
		return extractBitField<0, 16, 8>(rawData_);
	}

	/**
	 * \return value of Sequence End Indicator bit field
	 */

	uint8_t getSequenceEndIndicator() const
	{
		return extractBitField<0, 0, 1>(rawData_);
	}

private:

	/// raw data with Configuration Map Descriptor Header
	RawData rawData_;
};

/// Manufacturer and Device ID
class ManufacturerDeviceId
{
public:

	/// type of raw data with Manufacturer and Device ID
	using RawData = std::array<uint8_t, 3>;

	/**
	 * \brief ManufacturerDeviceId's constructor
	 */

	constexpr ManufacturerDeviceId() :
			rawData_{}
	{

	}

	/**
	 * \brief ManufacturerDeviceId's constructor
	 *
	 * \param [in] rawData is the raw data with Manufacturer and Device ID
	 */

	constexpr explicit ManufacturerDeviceId(const RawData rawData) :
			rawData_{rawData}
	{

	}

	/**
	 * \return value of Device ID bit field
	 */

	uint16_t getDeviceId() const
	{
		const auto deviceId = estd::extractBitField<8, 16>(rawData_);
		return __builtin_bswap16(deviceId);
	}

	/**
	 * \return value of Manufacturer ID bit field
	 */

	uint8_t getManufacturerId() const
	{
		return estd::extractBitField<0, 8>(rawData_);
	}

private:

	/// raw data with Manufacturer and Device ID
	RawData rawData_;
};

/// Parameter Header
class ParameterHeader
{
public:

	/// type of raw data with Parameter Header
	using RawData = std::array<Dword, 2>;

	/**
	 * \brief ParameterHeader's constructor
	 */

	constexpr ParameterHeader() :
			rawData_{}
	{

	}

	/**
	 * \brief ParameterHeader's constructor
	 *
	 * \param [in] rawData is the raw data with Parameter Header
	 */

	constexpr explicit ParameterHeader(const RawData rawData) :
			rawData_{rawData}
	{

	}

	/**
	 * \return value of ID bit field
	 */

	uint16_t getId() const
	{
		const uint16_t msb = extractBitField<1, 24, 8>(rawData_);
		const uint16_t lsb = extractBitField<0, 0, 8>(rawData_);
		return msb << CHAR_BIT | lsb << 0;
	}

	/**
	 * \return value of Table Length bit field
	 */

	uint8_t getTableLength() const
	{
		return extractBitField<0, 24, 8>(rawData_);
	}

	/**
	 * \return value of Table Pointer bit field
	 */

	uint32_t getTablePointer() const
	{
		return extractBitField<1, 0, 24>(rawData_);
	}

	/**
	 * \return value of Table Revision Number bit field
	 */

	uint16_t getTableRevisionNumber() const
	{
		return extractBitField<0, 8, 16>(rawData_);
	}

private:

	/// raw data with Parameter Header
	RawData rawData_;
};

/// Region
class Region
{
public:

	/// type of raw data with Region
	using RawData = std::array<Dword, 1>;

	/**
	 * \brief Region's constructor
	 *
	 * \param [in] rawData is the raw data with Region
	 */

	constexpr explicit Region(const RawData rawData) :
			rawData_{rawData}
	{

	}

	/**
	 * \return value of Erase Types bit field
	 */

	uint8_t getEraseTypes() const
	{
		return extractBitField<0, 0, 4>(rawData_);
	}

	/**
	 * \return value of Size bit field
	 */

	uint32_t getSize() const
	{
		return extractBitField<0, 8, 24>(rawData_);
	}

private:

	/// raw data with Region
	RawData rawData_;
};

/// Sector Map Descriptor
class SectorMapDescriptor
{
public:

	/// type of raw data with Sector Map Descriptor
	using RawData = std::array<Dword, 1>;

	/**
	 * \brief SectorMapDescriptor's constructor
	 *
	 * \param [in] rawData is the raw data with Sector Map Descriptor
	 */

	constexpr explicit SectorMapDescriptor(const RawData rawData) :
			rawData_{rawData}
	{

	}

	/**
	 * \return first DWORD of Configuration Detection Command Descriptor, valid only if Type equals 0
	 */

	ConfigurationDetectionCommandDescriptor getConfigurationDetectionCommandDescriptor() const
	{
		return ConfigurationDetectionCommandDescriptor{rawData_};
	}

	/**
	 * \return Configuration Map Descriptor Header, valid only if Type equals 1
	 */

	ConfigurationMapDescriptorHeader getConfigurationMapDescriptorHeader() const
	{
		return ConfigurationMapDescriptorHeader{rawData_};
	}

	/**
	 * \return value of Sequence End Indicator bit field
	 */

	uint8_t getSequenceEndIndicator() const
	{
		return extractBitField<0, 0, 1>(rawData_);
	}

	/**
	 * \return value of Type bit field
	 */

	uint8_t getType() const
	{
		return extractBitField<0, 1, 1>(rawData_);
	}

private:

	/// raw data with Sector Map Descriptor
	RawData rawData_;
};

/// SFDP Header
class SfdpHeader
{
public:

	/// expected SFDP header's signature
	constexpr static Dword expectedSignature {Dword{'S'} << 0 * CHAR_BIT | Dword{'F'} << 1 * CHAR_BIT |
			Dword{'D'} << 2 * CHAR_BIT | Dword{'P'} << 3 * CHAR_BIT};

	/// type of raw data with SFDP Header
	using RawData = std::array<Dword, 2>;

	/**
	 * \brief SfdpHeader's constructor
	 */

	constexpr SfdpHeader() :
			rawData_{}
	{

	}

	/**
	 * \brief SfdpHeader's constructor
	 *
	 * \param [in] rawData is the raw data with SFDP Header
	 */

	constexpr explicit SfdpHeader(const RawData rawData) :
			rawData_{rawData}
	{

	}

	/**
	 * \return value of Number of Parameter Headers (NPH) bit field
	 */

	uint8_t getNumberOfParameterHeaders() const
	{
		return extractBitField<1, 16, 8>(rawData_);
	}

	/**
	 * \return value of Revision Number bit field
	 */

	uint16_t getRevisionNumber() const
	{
		return extractBitField<1, 0, 16>(rawData_);
	}

	/**
	 * \return value of Signature bit field
	 */

	uint32_t getSignature() const
	{
		return extractBitField<0, 0, 32>(rawData_);
	}

private:

	/// raw data with SFDP Header
	RawData rawData_;
};

/// Status Register 1
class StatusRegister1
{
public:

	/// type of raw data with Status Register 1
	using RawData = std::array<uint8_t, 1>;

	/**
	 * \brief StatusRegister1's constructor
	 */

	constexpr StatusRegister1() :
			rawData_{}
	{

	}

	/**
	 * \brief StatusRegister1's constructor
	 *
	 * \param [in] rawData is the raw data with Status Register 1
	 */

	constexpr explicit StatusRegister1(const RawData rawData) :
			rawData_{rawData}
	{

	}

	/**
	 * \return value of Write in Progress (WIP) bit field
	 */

	uint8_t getWriteInProgress() const
	{
		return estd::extractBitField<0, 1>(rawData_);
	}

private:

	/// raw data with Status Register 1
	RawData rawData_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Implementation of executeCommand(), executeReadCommand() and executeWriteCommand()
 *
 * \pre \a addressLength is 0, 3 or 4.
 * \pre \a dummyCycles is a multiple of 8.
 * \pre \a dummyCycles is less than or equal to 32.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 * \param [in] instruction is the instruction for this command
 * \param [in] address is the address for the command
 * \param [in] addressLength is the length of address for the command, bytes
 * \param [in] dummyCycles is the number of dummy cycles sent before reading/writing of data
 * \param [in] writeBuffer is the buffer with data that will be written, nullptr to send dummy data
 * \param [out] readBuffer is the buffer for data that will be read, nullptr to ignore received data
 * \param [in] size is the size of \a writeBuffer and \a readBuffer
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by SpiMasterHandle::executeTransaction();
 */

int executeCommandImplementation(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin,
		const uint8_t instruction, const uint32_t address, const size_t addressLength, const size_t dummyCycles,
		const void* const writeBuffer, void* const readBuffer, const size_t size)
{
	assert(addressLength == 0 || addressLength == 3 || addressLength == 4);
	assert(dummyCycles % CHAR_BIT == 0);
	const auto dummyBytes = dummyCycles / CHAR_BIT;
	assert(dummyBytes <= sizeof(uint32_t));

	uint8_t instructionAddressDummy[sizeof(instruction) + sizeof(address) + sizeof(uint32_t)];
	auto iterator = std::begin(instructionAddressDummy);
	*iterator++ = instruction;
	if (addressLength == 4)
		*iterator++ = address >> CHAR_BIT * 3;
	if (addressLength != 0)
	{
		*iterator++ = address >> CHAR_BIT * 2;
		*iterator++ = address >> CHAR_BIT * 1;
		*iterator++ = address >> CHAR_BIT * 0;
	}
	memset(iterator, {}, dummyBytes);
	iterator += dummyBytes;

	const SpiMasterTransfer transfers[]
	{
			{instructionAddressDummy, nullptr, static_cast<size_t>(iterator - instructionAddressDummy)},
			{writeBuffer, readBuffer, size},
	};
	const SpiMasterTransfersRange transfersRange {transfers, sizeof(transfers) / sizeof(*transfers) - (size == 0)};

	const SpiDeviceSelectGuard spiDeviceSelectGuard {slaveSelectPin};
	return spiMasterHandle.executeTransaction(transfersRange);
}

/**
 * \brief Executes generic command.
 *
 * \pre \a addressLength is 0, 3 or 4.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 * \param [in] instruction is the instruction for this command
 * \param [in] address is the address for the command
 * \param [in] addressLength is the length of address for the command, bytes
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by executeCommandImplementation();
 */

int executeCommand(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin, const uint8_t instruction,
		const uint32_t address, const size_t addressLength)
{
	return executeCommandImplementation(spiMasterHandle, slaveSelectPin, instruction, address, addressLength, {}, {},
			{}, {});
}

/**
 * \brief Executes read command.
 *
 * \pre \a addressLength is 0, 3 or 4.
 * \pre \a dummyCycles is a multiple of 8.
 * \pre \a dummyCycles is less than or equal to 32.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 * \param [in] instruction is the instruction for this command
 * \param [in] address is the address for the command
 * \param [in] addressLength is the length of address for the command, bytes
 * \param [in] dummyCycles is the number of dummy cycles sent before reading of data
 * \param [out] buffer is the buffer for data that will be read, nullptr to ignore received data
 * \param [in] size is the size of \a buffer
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by executeCommandImplementation();
 */

int executeReadCommand(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin, const uint8_t instruction,
		const uint32_t address, const size_t addressLength, const size_t dummyCycles, void* const buffer,
		const size_t size)
{
	return executeCommandImplementation(spiMasterHandle, slaveSelectPin, instruction, address, addressLength,
			dummyCycles, {}, buffer, size);
}

/**
 * \brief Executes write command.
 *
 * \pre \a addressLength is 0, 3 or 4.
 * \pre \a dummyCycles is a multiple of 8.
 * \pre \a dummyCycles is less than or equal to 32.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 * \param [in] instruction is the instruction for this command
 * \param [in] address is the address for the command
 * \param [in] addressLength is the length of address for the command, bytes
 * \param [in] dummyCycles is the number of dummy cycles sent before writing of data
 * \param [in] buffer is the buffer with data that will be written, nullptr to send dummy data
 * \param [in] size is the size of \a buffer
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by executeCommandImplementation();
 */

int executeWriteCommand(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin, const uint8_t instruction,
		const uint32_t address, const size_t addressLength, const size_t dummyCycles, const void* const buffer,
		const size_t size)
{
	return executeCommandImplementation(spiMasterHandle, slaveSelectPin, instruction, address, addressLength,
			dummyCycles, buffer, {}, size);
}

/**
 * \brief Executes RSFDP command.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 * \param [in] address is the address for the command
 * \param [out] buffer is the buffer for data that will be read, nullptr to ignore received data
 * \param [in] size is the size of \a buffer
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by executeReadCommand();
 */

int executeRsfdp(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin, const uint32_t address,
		void* const buffer, const size_t size)
{
	return executeReadCommand(spiMasterHandle, slaveSelectPin, 0x5a, address, 3, 8, buffer, size);
}

/**
 * \brief Executes WREN command.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by executeCommand();
 */

int executeWren(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin)
{
	return executeCommand(spiMasterHandle, slaveSelectPin, 0x06, {}, {});
}

/**
 * \brief Reads manufacturer and device ID.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 *
 * \return pair with return code (0 on success, error code otherwise) and Manufacturer and Device ID; error codes:
 * - error codes returned by executeReadCommand();
 */

std::pair<int, ManufacturerDeviceId> readManufacturerDeviceId(const SpiMasterHandle& spiMasterHandle,
		OutputPin& slaveSelectPin)
{
	ManufacturerDeviceId::RawData rawManufacturerDeviceId;
	const auto ret = executeReadCommand(spiMasterHandle, slaveSelectPin, 0x9f, {}, {}, {},
			rawManufacturerDeviceId.data(), sizeof(rawManufacturerDeviceId));
	return {ret, ManufacturerDeviceId{rawManufacturerDeviceId}};
}

/**
 * \brief Reads one Parameter Header.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 * \param [in] index is the index of parameter header that will be read
 *
 * \return pair with return code (0 on success, error code otherwise) and Parameter Header; error codes:
 * - error codes returned by executeRsfdp();
 */

std::pair<int, ParameterHeader> readParameterHeader(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin,
		const uint8_t index)
{
	ParameterHeader::RawData rawParameterHeader;
	const auto ret = executeRsfdp(spiMasterHandle, slaveSelectPin, sizeof(Dword[2]) * (index + 1),
			rawParameterHeader.data(), sizeof(rawParameterHeader));
	return {ret, ParameterHeader{rawParameterHeader}};
}

/**
 * \brief Reads Basic Flash and Sector Map parameter headers.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 * \param [in] numberOfParameterHeaders is the number of parameter headers available
 *
 * \return tuple with return code (0 on success, error code otherwise), Basic Flash and Sector Map parameter headers;
 * error codes:
 * - error codes returned by readParameterHeader();
 */

std::tuple<int, ParameterHeader, ParameterHeader> readParameterHeaders(const SpiMasterHandle& spiMasterHandle,
		OutputPin& slaveSelectPin, const uint8_t numberOfParameterHeaders)
{
	ParameterHeader basicFlashParameterHeader {};
	ParameterHeader sectorMapParameterHeader {};
	for (size_t index {}; index < numberOfParameterHeaders; ++index)
	{
		int ret;
		ParameterHeader parameterHeader;
		std::tie(ret, parameterHeader) = readParameterHeader(spiMasterHandle, slaveSelectPin, index);
		if (ret != 0)
			return std::make_tuple(ret, ParameterHeader{}, ParameterHeader{});

		const auto revisionNumber = parameterHeader.getTableRevisionNumber();
		if ((revisionNumber & 0xff00) != 0x0100)	// ignore if major revision number is not 1
			continue;

		const auto id = parameterHeader.getId();
		if (id == 0xff00 && basicFlashParameterHeader.getTableRevisionNumber() < revisionNumber)
			basicFlashParameterHeader = parameterHeader;
		if (id == 0xff81 && sectorMapParameterHeader.getTableRevisionNumber() < revisionNumber)
			sectorMapParameterHeader = parameterHeader;
	}

	return std::make_tuple(int{}, basicFlashParameterHeader, sectorMapParameterHeader);
}

/**
 * \brief Reads SFDP Header.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 *
 * \return pair with return code (0 on success, error code otherwise) and SFDP Header; error codes:
 * - error codes returned by executeRsfdp();
 */

std::pair<int, SfdpHeader> readSfdpHeader(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin)
{
	SfdpHeader::RawData rawSfdpHeader;
	const auto ret = executeRsfdp(spiMasterHandle, slaveSelectPin, {}, rawSfdpHeader.data(), sizeof(rawSfdpHeader));
	return {ret, SfdpHeader{rawSfdpHeader}};
}

/**
 * \brief Reads Status Register 1.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 *
 * \return pair with return code (0 on success, error code otherwise) and Status Register 1; error codes:
 * - error codes returned by executeReadCommand();
 */

std::pair<int, StatusRegister1> readStatusRegister1(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin)
{
	StatusRegister1::RawData rawStatusRegister1;
	const auto ret = executeReadCommand(spiMasterHandle, slaveSelectPin, 0x05, {}, {}, {}, rawStatusRegister1.data(),
			sizeof(rawStatusRegister1));
	return {ret, StatusRegister1{rawStatusRegister1}};
}

/**
 * \brief Parses Basic Flash Parameter Table.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 * \param [in] header is a reference to Basic Flash parameter header
 *
 * \return pair with return code (0 on success, error code otherwise) and parsed basic flash parameters; error codes:
 * - ENOTSUP - Basic Flash Parameter Table is invalid and cannot be parsed;
 * - error codes returned by executeRsfdp();
 */

std::pair<int, BasicFlashParameters> parseBasicFlashParameterTable(const SpiMasterHandle& spiMasterHandle,
		OutputPin& slaveSelectPin, const ParameterHeader& header)
{
	const auto revision = header.getTableRevisionNumber();
	using RevisionExpectedLength = std::pair<uint16_t, uint8_t>;	// revision + expected length
	static const RevisionExpectedLength expectedLengths[]
	{
			{0x0107, 20},	// JESD216C & JESD216D (revision 1.7)
			{0x0106, 16},	// JESD216B (revision 1.6)
			{0x0105, 16},	// JESD216A (revision 1.5)
			{0x0100, 9},	// JESD216 (revision 1.0)
	};
	const auto iterator = std::find_if(std::begin(expectedLengths), std::end(expectedLengths),
			[&revision](const RevisionExpectedLength& entry)
			{
				return revision >= entry.first;
			});
	assert(iterator != std::end(expectedLengths));
	const auto expectedLengthDwords = iterator->second;
	const auto lengthDwords = header.getTableLength();
	if (lengthDwords < expectedLengthDwords)
		return {ENOTSUP, {}};

	std::array<Dword, 20> basicFlashParameterTable;
	{
		const auto ret = executeRsfdp(spiMasterHandle, slaveSelectPin, header.getTablePointer(),
				basicFlashParameterTable.data(), lengthDwords * sizeof(Dword));
		if (ret != 0)
			return {ret, {}};
	}

	BasicFlashParameters basicFlashParameters {};

	// JESD216 (revision 1.0) and above - first 9 DWORDs

	const auto writeGranularity = extractBitField<0, 2, 1>(basicFlashParameterTable);
	basicFlashParameters.pageSize = writeGranularity == 0 ? 1 : 64;

	const auto addressBytes = extractBitField<0, 17, 2>(basicFlashParameterTable);
	if (addressBytes == 3)
		return {ENOTSUP, {}};

	basicFlashParameters.addressFlags = addressBytes == 0 ? AddressFlags::_3 :
		addressBytes == 1 ? AddressFlags::_3 | AddressFlags::_4 : AddressFlags::_4;

	const auto flashMemoryDensity = basicFlashParameterTable[1];
	if ((flashMemoryDensity & (1 << 31)) == 0)
		basicFlashParameters.size = (flashMemoryDensity + 1) / CHAR_BIT;
	else
	{
		const auto n = flashMemoryDensity & ~(1 << 31);
		if (n < 32 || n >= sizeof(basicFlashParameters.size) * CHAR_BIT)
			return {ENOTSUP, {}};
		basicFlashParameters.size = 1 << n;
	}

	basicFlashParameters.eraseInstructions[0] = extractBitField<7, 8, 8>(basicFlashParameterTable);
	basicFlashParameters.eraseInstructions[1] = extractBitField<7, 24, 8>(basicFlashParameterTable);
	basicFlashParameters.eraseInstructions[2] = extractBitField<8, 8, 8>(basicFlashParameterTable);
	basicFlashParameters.eraseInstructions[3] = extractBitField<8, 24, 8>(basicFlashParameterTable);

	const uint8_t eraseSizes[BasicFlashParameters::maxEraseTypes]
	{
			extractBitField<7, 0, 8>(basicFlashParameterTable),
			extractBitField<7, 16, 8>(basicFlashParameterTable),
			extractBitField<8, 0, 8>(basicFlashParameterTable),
			extractBitField<8, 16, 8>(basicFlashParameterTable),
	};
	for (size_t i {}; i < BasicFlashParameters::maxEraseTypes; ++i)
		if (eraseSizes[i] != 0)
		{
			if (eraseSizes[i] >= sizeof(basicFlashParameters.eraseSizes[i]) * CHAR_BIT)
				return {ENOTSUP, {}};
			basicFlashParameters.eraseSizes[i] = 1 << eraseSizes[i];
		}

	if (revision <= 0x0100)
		return {{}, basicFlashParameters};

	// JESD216A (revision 1.5) and above - first 16 DWORDs

	const auto typicalEraseTimeDecoder =
			[](const uint8_t count, const uint8_t units)
			{
				const auto decodedUnits = std::chrono::milliseconds{units == 0 ? 1 :
						units == 1 ? 16 :
						units == 2 ? 128 : 1000};
				return (count + 1) * decodedUnits;
			};

	const auto maximumEraseTimeMultiplierCount = extractBitField<9, 0, 4>(basicFlashParameterTable);
	const auto maximumEraseTimeMultiplier = 2 * (maximumEraseTimeMultiplierCount + 1);
	const uint8_t typicalEraseTimeCounts[BasicFlashParameters::maxEraseTypes]
	{
			extractBitField<9, 4, 5>(basicFlashParameterTable),
			extractBitField<9, 11, 5>(basicFlashParameterTable),
			extractBitField<9, 18, 5>(basicFlashParameterTable),
			extractBitField<9, 25, 5>(basicFlashParameterTable),
	};
	const uint8_t typicalEraseTimeUnits[BasicFlashParameters::maxEraseTypes]
	{
			extractBitField<9, 9, 2>(basicFlashParameterTable),
			extractBitField<9, 16, 2>(basicFlashParameterTable),
			extractBitField<9, 23, 2>(basicFlashParameterTable),
			extractBitField<9, 30, 2>(basicFlashParameterTable),
	};
	for (size_t i {}; i < BasicFlashParameters::maxEraseTypes; ++i)
		if (basicFlashParameters.eraseSizes[i] != 0)
		{
			const auto typicalEraseTime = typicalEraseTimeDecoder(typicalEraseTimeCounts[i], typicalEraseTimeUnits[i]);
			basicFlashParameters.maximumEraseTimesMs[i] = typicalEraseTime.count() * maximumEraseTimeMultiplier;
		}

	const auto pageSize = extractBitField<10, 4, 4>(basicFlashParameterTable);
	basicFlashParameters.pageSize = 1 << pageSize;

	basicFlashParameters.softwareResetFlags =
			static_cast<SoftwareResetFlags>(extractBitField<15, 8, 6>(basicFlashParameterTable));

	return {{}, basicFlashParameters};
};

/**
 * \brief Parses Sector Map Table.
 *
 * \param [in] spiMasterHandle is a reference to SpiMasterHandle object used for communication
 * \param [in] slaveSelectPin is a reference to slave select pin of this SPI slave device
 * \param [in] header is a reference to Sector Map parameter header
 *
 * \return pair with return code (0 on success, error code otherwise) and parsed sector map; error codes:
 * - ENOTSUP - Sector Map Table is invalid and cannot be parsed;
 * - error codes returned by executeReadCommand();
 * - error codes returned by executeRsfdp();
 */

std::pair<int, SectorMap> parseSectorMapTable(const SpiMasterHandle& spiMasterHandle, OutputPin& slaveSelectPin,
		const ParameterHeader& header)
{
	auto address = header.getTablePointer();
	const auto addressEnd = address + header.getTableLength() * sizeof(Dword);
	uint8_t selectedConfigurationId {};

	auto reader =
			[&address, addressEnd, &spiMasterHandle, &slaveSelectPin](void* const buffer, const size_t size) -> int
			{
				if (address >= addressEnd)
					return ENOTSUP;
				const auto ret = executeRsfdp(spiMasterHandle, slaveSelectPin, address, buffer, size);
				address += size;
				return ret;
			};

	while (address < addressEnd)
	{
		SectorMapDescriptor::RawData rawSectorMapDescriptor;
		{
			const auto ret = reader(rawSectorMapDescriptor.data(), sizeof(rawSectorMapDescriptor));
			if (ret != 0)
				return {ret, {}};
		}
		SectorMapDescriptor sectorMapDescriptor {rawSectorMapDescriptor};

		if (sectorMapDescriptor.getType() == 0)	// configuration detection command descriptor
		{
			const auto configurationDetectionCommandDescriptor =
					sectorMapDescriptor.getConfigurationDetectionCommandDescriptor();

			const auto readLatency = configurationDetectionCommandDescriptor.getReadLatency();
			if (readLatency != 0 && readLatency != CHAR_BIT)
				return {ENOTSUP, {}};

			Dword commandAddress;
			{
				const auto ret = reader(&commandAddress, sizeof(commandAddress));
				if (ret != 0)
					return {ret, {}};
			}

			const auto instruction = configurationDetectionCommandDescriptor.getInstruction();
			const auto addressLength = configurationDetectionCommandDescriptor.getAddressLength();
			const auto decodedAddressLength = addressLength == 0 ? 0 :
					addressLength == 2 ? 4 : 3;

			uint8_t byte;
			{
				const auto ret = executeReadCommand(spiMasterHandle, slaveSelectPin, instruction, commandAddress,
						decodedAddressLength, readLatency, &byte, sizeof(byte));
				if (ret != 0)
					return {ret, {}};
			}
			const auto readDataMask = configurationDetectionCommandDescriptor.getReadDataMask();
			const auto bit = (byte & readDataMask) != 0;
			selectedConfigurationId = selectedConfigurationId << 1 | bit;
		}
		else	// configuration map descriptor
		{
			const auto configurationMapDescriptorHeader = sectorMapDescriptor.getConfigurationMapDescriptorHeader();

			const uint8_t configurationId = configurationMapDescriptorHeader.getConfigurationId();
			const auto regionCount = configurationMapDescriptorHeader.getRegionCount() + 1u;
			if (configurationId != selectedConfigurationId)
			{
				address += regionCount * sizeof(Region::RawData);
				continue;
			}

			SectorMap sectorMap;
			if (regionCount > sectorMap.maxRegionCount)
				return {ENOTSUP, {}};

			for (size_t i {}; i < regionCount; ++i)
			{
				Region::RawData rawRegion;
				{
					const auto ret = reader(rawRegion.data(), sizeof(rawRegion));
					if (ret != 0)
						return {ret, {}};
				}

				Region region {rawRegion};
				sectorMap.sizes[i] = (static_cast<uint64_t>(region.getSize()) + 1) * 256;
				sectorMap.eraseTypes[i] = region.getEraseTypes();
			}

			sectorMap.regionCount = regionCount;
			return {{}, sectorMap};
		}
	}

	return {ENOTSUP, {}};
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

QspiNorFlashSpiBased::~QspiNorFlashSpiBased()
{
	assert(openCount_ == 0);
}

int QspiNorFlashSpiBased::close()
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);

	int ret {};
	if (openCount_ == 1)	// last close?
	{
		ret = waitWhileWriteInProgress(busyDeadline_);
		SpiMasterHandle{spiMaster_}.close();
		deinitialize();
	}

	--openCount_;
	return ret;
}

int QspiNorFlashSpiBased::erase(const uint64_t address, const uint64_t size)
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);
	const auto eraseBlockSize = getEraseBlockSize();
	assert(address % eraseBlockSize == 0 && size % eraseBlockSize == 0);

	assert(address + size <= basicFlashParameters_.size);

	if (size == 0)
		return {};

	const auto eraseInstruction = basicFlashParameters_.eraseInstructions[commonEraseIndex_];
	const auto maximumEraseTime =
			std::chrono::milliseconds{basicFlashParameters_.maximumEraseTimesMs[commonEraseIndex_]};
	uint64_t erased {};
	while (erased < size)
	{
		{
			const auto ret = waitWhileWriteInProgress(busyDeadline_);
			if (ret != 0)
				return ret;
		}

		const SpiMasterHandle spiMasterHandle {spiMaster_};
		spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});

		{
			const auto ret = executeWren(spiMasterHandle, slaveSelectPin_);
			if (ret != 0)
				return ret;
		}
		{
			const auto ret = executeCommand(spiMasterHandle, slaveSelectPin_, eraseInstruction, address + erased, 3);
			if (ret != 0)
				return ret;
		}

		busyDeadline_ = TickClock::now() + maximumEraseTime;
		erased += eraseBlockSize;
	}

	return {};
}

size_t QspiNorFlashSpiBased::getEraseBlockSize() const
{
	assert(commonEraseIndex_ < BasicFlashParameters::maxEraseTypes);
	return basicFlashParameters_.eraseSizes[commonEraseIndex_];
}

size_t QspiNorFlashSpiBased::getProgramBlockSize() const
{
	return 1;
}

size_t QspiNorFlashSpiBased::getReadBlockSize() const
{
	return 1;
}

uint64_t QspiNorFlashSpiBased::getSize() const
{
	return basicFlashParameters_.size;
}

void QspiNorFlashSpiBased::lock()
{
	const auto ret = mutex_.lock();
	assert(ret == 0);
}

int QspiNorFlashSpiBased::open()
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ < std::numeric_limits<decltype(openCount_)>::max());

	if (openCount_ == 0)	// first open?
	{
		const auto ret = SpiMasterHandle{spiMaster_}.open();
		if (ret != 0)
			return ret;
	}

	++openCount_;

	if (openCount_ > 1)
		return {};

	auto closeScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				SpiMasterHandle{spiMaster_}.close();
				deinitialize();
				openCount_ = {};
			});

	{
		const auto ret = initialize();
		if (ret != 0)
			return ret;
	}

	closeScopeGuard.release();
	return {};
}

int QspiNorFlashSpiBased::program(const uint64_t address, const void* const buffer, const size_t size)
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);
	assert(buffer != nullptr);

	assert(address + size <= basicFlashParameters_.size);

	if (size == 0)
		return {};

	const auto pageSize = basicFlashParameters_.pageSize;
	size_t bytesWritten {};
	const auto bufferUint8 = static_cast<const uint8_t*>(buffer);
	while (bytesWritten < size)
	{
		{
			const auto ret = waitWhileWriteInProgress(busyDeadline_);
			if (ret != 0)
				return ret;
		}

		const SpiMasterHandle spiMasterHandle {spiMaster_};
		spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});

		{
			const auto ret = executeWren(spiMasterHandle, slaveSelectPin_);
			if (ret != 0)
				return ret;
		}

		const decltype(pageSize) pageOffset = (address + bytesWritten) & (pageSize - 1);	// page size is always 2^N
		const auto chunk = std::min<decltype(size)>(pageSize - pageOffset, size - bytesWritten);
		{
			const auto ret = executeWriteCommand(spiMasterHandle, slaveSelectPin_, 0x02, address + bytesWritten, 3, {},
					bufferUint8 + bytesWritten, chunk);
			if (ret != 0)
				return ret;
		}

		// ~66 ms is the absolute maximum page program time which can be represented in SFDP
		busyDeadline_ = TickClock::now() + std::chrono::milliseconds{66};
		bytesWritten += chunk;
	}

	return {};
}

int QspiNorFlashSpiBased::read(const uint64_t address, void* const buffer, const size_t size)
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);
	assert(buffer != nullptr);

	assert(address + size <= basicFlashParameters_.size);

	if (size == 0)
		return {};

	{
		const auto ret = waitWhileWriteInProgress(busyDeadline_);
		if (ret != 0)
			return ret;
	}

	const SpiMasterHandle spiMasterHandle {spiMaster_};
	spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});
	return executeReadCommand(spiMasterHandle, slaveSelectPin_, 0x03, address, 3, 0, buffer, size);
}

int QspiNorFlashSpiBased::synchronize()
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);

	return waitWhileWriteInProgress(busyDeadline_);
}

void QspiNorFlashSpiBased::unlock()
{
	const auto ret = mutex_.unlock();
	assert(ret == 0);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void QspiNorFlashSpiBased::deinitialize()
{
	basicFlashParameters_ = {};
	busyDeadline_ = {};
	sectorMap_ = {};
	commonEraseIndex_ = {};
}

int QspiNorFlashSpiBased::handleFixups()
{
	ManufacturerDeviceId manufacturerDeviceId;
	{
		const SpiMasterHandle spiMasterHandle {spiMaster_};
		spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});
		int ret;
		std::tie(ret, manufacturerDeviceId) = readManufacturerDeviceId(spiMasterHandle, slaveSelectPin_);
		if (ret != 0)
			return ret;
	}

	/// \todo improve the identification, because just the first 3 bytes is not enough for reliable identification...
	if (manufacturerDeviceId.getManufacturerId() == 0x01)	// Cypress
	{
		if (manufacturerDeviceId.getDeviceId() == 0x2018)	// S25FL127S
		{
			{
				// for the purpose of erase time, pages which are 64 kB must be treated as 16 x 4 kB page
				basicFlashParameters_.maximumEraseTimesMs[1] = basicFlashParameters_.maximumEraseTimesMs[0] * (64 / 4);

				const SpiMasterHandle spiMasterHandle {spiMaster_};
				spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});

				std::array<uint8_t, 1> statusRegister2;
				{
					const auto ret = executeReadCommand(spiMasterHandle, slaveSelectPin_, 0x07, {}, {}, {},
							statusRegister2.data(), sizeof(statusRegister2));
					if (ret != 0)
						return ret;
				}

				constexpr size_t pageBufferWrapPosition {6};
				const auto pageBufferWrap = estd::extractBitField<pageBufferWrapPosition, 1>(statusRegister2);
				if (pageBufferWrap == 1)	// page buffer wrap already set?
					return {};

				std::array<uint8_t, 3> registers;
				{
					// status register 1
					const auto ret = executeReadCommand(spiMasterHandle, slaveSelectPin_, 0x05, {}, {}, {},
							&registers[0], sizeof(registers[0]));
					if (ret != 0)
						return ret;
				}
				{
					// configuration register
					const auto ret = executeReadCommand(spiMasterHandle, slaveSelectPin_, 0x35, {}, {}, {},
							&registers[1], sizeof(registers[1]));
					if (ret != 0)
						return ret;
				}
				registers[2] = statusRegister2[0] | 1 << pageBufferWrapPosition;

				{
					const auto ret = executeWren(spiMasterHandle, slaveSelectPin_);
					if (ret != 0)
						return ret;
				}
				{
					const auto ret = executeWriteCommand(spiMasterHandle, slaveSelectPin_, 0x01, {}, {}, {},
							registers.data(), sizeof(registers));
					if (ret != 0)
						return ret;
				}
			}

			{
				const auto ret = waitWhileWriteInProgress(TickClock::now() + std::chrono::milliseconds{800});
				if (ret != 0)
					return ret;
			}

			return {};
		}
	}

	return {};
}

int QspiNorFlashSpiBased::initialize()
{
	{
		const auto ret = parseSfdp();
		if (ret != 0)
			return ret;
	}

	uint8_t commonEraseTypes {0xf};
	for (size_t i {}; i < sectorMap_.regionCount; ++i)
		commonEraseTypes &= sectorMap_.eraseTypes[i];

	if (commonEraseTypes == 0)
		return ENOTSUP;

	size_t commonEraseSize {SIZE_MAX};
	for (size_t i {}; i < BasicFlashParameters::maxEraseTypes; ++i)
	{
		if ((commonEraseTypes & (1 << i)) == 0)
			continue;
		if (basicFlashParameters_.eraseSizes[i] == 0)
			return ENOTSUP;
		if (commonEraseSize > basicFlashParameters_.eraseSizes[i])
		{
			commonEraseSize = basicFlashParameters_.eraseSizes[i];
			commonEraseIndex_ = i;
		}
	}

	{
		const auto ret = handleFixups();
		if (ret != 0)
			return ret;
	}

	return {};
}

int QspiNorFlashSpiBased::parseSfdp()
{
	ParameterHeader sectorMapParameterHeader;

	{
		const SpiMasterHandle spiMasterHandle {spiMaster_};
		spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});

		uint8_t numberOfParameterHeaders;
		{
			int ret;
			SfdpHeader sfdpHeader;
			std::tie(ret, sfdpHeader) = readSfdpHeader(spiMasterHandle, slaveSelectPin_);
			if (ret != 0)
				return ret;
			if (sfdpHeader.getSignature() != SfdpHeader::expectedSignature)
				return ENOTSUP;
			if ((sfdpHeader.getRevisionNumber() & 0xff00) != 0x0100 || sfdpHeader.getNumberOfParameterHeaders() == 255)
				return ENOTSUP;

			numberOfParameterHeaders = sfdpHeader.getNumberOfParameterHeaders() + 1;
		}

		ParameterHeader basicFlashParameterHeader;
		{
			int ret;
			std::tie(ret, basicFlashParameterHeader, sectorMapParameterHeader) =
					readParameterHeaders(spiMasterHandle, slaveSelectPin_, numberOfParameterHeaders);
			if (ret != 0)
				return ret;
		}
		{
			int ret;
			std::tie(ret, basicFlashParameters_) = parseBasicFlashParameterTable(spiMasterHandle, slaveSelectPin_,
					basicFlashParameterHeader);
			if (ret != 0)
				return ret;
		}

		if (basicFlashParameters_.size > 1 << (3 * CHAR_BIT))	/// \todo add support for 4-byte addressing
			return ENOTSUP;
		if ((basicFlashParameters_.addressFlags & AddressFlags::_3) == AddressFlags{})	// as above
			return ENOTSUP;
		if (basicFlashParameters_.softwareResetFlags == SoftwareResetFlags{})	/// \todo handle unknown soft reset
			return ENOTSUP;

		if ((basicFlashParameters_.softwareResetFlags & SoftwareResetFlags::_0xf0) != SoftwareResetFlags{})
		{
			const auto ret = executeCommand(spiMasterHandle, slaveSelectPin_, 0xf0, {}, {});
			if (ret != 0)
				return ret;
		}
		else if ((basicFlashParameters_.softwareResetFlags & SoftwareResetFlags::_0x66_0x99) != SoftwareResetFlags{})
		{
			{
				const auto ret = executeCommand(spiMasterHandle, slaveSelectPin_, 0x66, {}, {});
				if (ret != 0)
					return ret;
			}
			{
				const auto ret = executeCommand(spiMasterHandle, slaveSelectPin_, 0x99, {}, {});
				if (ret != 0)
					return ret;
			}
		}
	}

	{
		const auto deadline = TickClock::now() + std::chrono::milliseconds{100};
		while (1)
		{
			int ret;
			SfdpHeader sfdpHeader {};
			{
				const SpiMasterHandle spiMasterHandle {spiMaster_};
				spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});
				std::tie(ret, sfdpHeader) = readSfdpHeader(spiMasterHandle, slaveSelectPin_);
			}
			if (ret != 0)
				return ret;
			if (sfdpHeader.getSignature() == SfdpHeader::expectedSignature)
				break;
			if (deadline <= TickClock::now())
				return ETIMEDOUT;

			ThisThread::sleepFor({});
		}
	}

	{
		const SpiMasterHandle spiMasterHandle {spiMaster_};
		spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});

		int ret;
		std::tie(ret, sectorMap_) = parseSectorMapTable(spiMasterHandle, slaveSelectPin_, sectorMapParameterHeader);
		if (ret != 0)
			return ret;
	}

	return {};
}

int QspiNorFlashSpiBased::waitWhileWriteInProgress(TickClock::time_point timePoint)
{
	while (1)
	{
		int ret;
		StatusRegister1 statusRegister1;
		{
			const SpiMasterHandle spiMasterHandle {spiMaster_};
			spiMasterHandle.configure(mode_, clockFrequency_, 8, false, {});
			std::tie(ret, statusRegister1) = readStatusRegister1(spiMasterHandle, slaveSelectPin_);
		}
		if (ret != 0)
			return ret;
		if (statusRegister1.getWriteInProgress() == 0)
			return {};
		if (timePoint <= TickClock::now())
			return ETIMEDOUT;

		ThisThread::sleepFor({});
	}
}

}	// namespace devices

}	// namespace distortos
