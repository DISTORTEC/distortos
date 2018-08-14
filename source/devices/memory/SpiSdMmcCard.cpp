/**
 * \file
 * \brief SpiSdMmcCard class implementation
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/SpiSdMmcCard.hpp"

#include "distortos/devices/communication/SpiDeviceProxy.hpp"
#include "distortos/devices/communication/SpiDeviceSelectGuard.hpp"
#include "distortos/devices/communication/SpiMasterOperation.hpp"
#include "distortos/devices/communication/SpiMasterProxy.hpp"

#include "distortos/assert.h"
#include "distortos/ThisThread.hpp"

#include "estd/ScopeGuard.hpp"

#include <tuple>

#include <cstring>

namespace distortos
{

namespace devices
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// range of uint8_t elements
using Uint8Range = estd::ContiguousRange<uint8_t>;

/// range of const uint8_t elements
using ConstUint8Range = estd::ContiguousRange<const uint8_t>;

/// fields unique to CSD version 1.0
struct CsdV1
{
	/// C_SIZE, device size
	uint16_t cSize;

	/// VDD_R_CURR_MIN, max. read current @VDD min
	uint8_t vddRCurrMin;
	/// VDD_R_CURR_MAX, max. read current @VDD max
	uint8_t vddRCurrMax;
	/// VDD_W_CURR_MIN, max. write current @VDD min
	uint8_t vddWCurrMin;
	/// VDD_W_CURR_MAX, max. write current @VDD max
	uint8_t vddWCurrMax;
	/// C_SIZE_MULT, device size multiplier
	uint8_t cSizeMult;
};

/// fields unique to CSD version 2.0
struct CsdV2
{
	/// C_SIZE, device size
	uint32_t cSize;
};

/// CSD
struct Csd
{
	union
	{
		/// fields unique to CSD version 1.0, valid only if csdStructure == 0
		CsdV1 csdV1;
		/// fields unique to CSD version 2.0, valid only if csdStructure == 1
		CsdV2 csdV2;
	};

	/// CCC, card command classes
	uint16_t ccc;

	/// CSD_STRUCTURE, CSD structure
	uint8_t csdStructure;
	/// TAAC, data read access-time
	uint8_t taac;
	/// NSAC, data read access-time in CLK cycles (NSAC*100)
	uint8_t nsac;
	/// TRAN_SPEED, max. data transfer rate
	uint8_t tranSpeed;
	/// READ_BL_LEN, max. read data block length
	uint8_t readBlLen;
	/// READ_BL_PARTIAL, partial blocks for read allowed
	uint8_t readBlPartial;
	/// WRITE_BLK_MISALIGN, write block misalignment
	uint8_t writeBlkMisalign;
	/// READ_BLK_MISALIGN, read block misalignment
	uint8_t readBlkMisalign;
	/// DSR_IMP, DSR implemented
	uint8_t dsrImp;
	/// ERASE_BLK_EN, erase single block enable
	uint8_t eraseBlkEn;
	/// SECTOR_SIZE, erase sector size
	uint8_t sectorSize;
	/// WP_GRP_SIZE, write protect group size
	uint8_t wpGrpSize;
	/// WP_GRP_ENABLE, write protect group enable
	uint8_t wpGrpEnable;
	/// R2W_FACTOR, write speed factor
	uint8_t r2wFactor;
	/// WRITE_BL_LEN, max. write data block length
	uint8_t writeBlLen;
	/// WRITE_BL_PARTIAL, partial blocks for write allowed
	uint8_t writeBlPartial;
	/// FILE_FORMAT_GRP, file format group
	uint8_t fileFormatGrp;
	/// COPY, copy flag
	uint8_t copy;
	/// PERM_WRITE_PROTECT, permanent write protection
	uint8_t permWriteProtect;
	/// TMP_WRITE_PROTECT, temporary write protection
	uint8_t tmpWriteProtect;
	/// FILE_FORMAT, file format
	uint8_t fileFormat;
};

/// R2 response
struct R2Response
{
	/// part with R1 response
	uint8_t r1;
	/// remaining part of R2 response
	uint8_t r2;
};

/// R3 response
struct R3Response
{
	/// OCR register
	uint32_t ocr;
	/// part with R1 response
	uint8_t r1;
};

/// R7 response
struct R7Response
{
	/// reserved bits
	uint16_t reservedBits;
	/// part with R1 response
	uint8_t r1;
	/// command version
	uint8_t commandVersion;
	/// voltage range accepted by the card
	uint8_t voltageAccepted;
	/// check pattern echoed back to the host
	uint8_t checkPattern;
};

/// SD Status
struct SdStatus
{
	/// SIZE_OF_PROTECTED_AREA, size of protected area
	uint32_t sizeOfProtectedArea;
	/// SUS_ADDR, suspension address
	uint32_t susAddr;
	/// SD_CARD_TYPE, type of SD card
	uint16_t sdCardType;
	/// ERASE_SIZE, number of AUs to be erased at a time
	uint16_t eraseSize;
	/// VSC_AU_SIZE, AU size in MB for video speed class
	uint16_t vscAuSize;
	/// DAT_BUS_WIDTH, currently defined data bus width
	uint8_t datBusWidth;
	/// SECURED_MODE, tells whether card is in secured mode of operation
	uint8_t securedMode;
	/// SPEED_CLASS, speed class of the card
	uint8_t speedClass;
	/// PERFORMANCE_MOVE, performance of move indicated by 1 [MB/s] step
	uint8_t performanceMove;
	/// AU_SIZE, size of AU
	uint8_t auSize;
	/// ERASE_TIMEOUT, timeout value for erasing areas specified by UNIT_OF_ERASE_AU
	uint8_t eraseTimeout;
	/// ERASE_OFFSET, fixed offset value added to erase time
	uint8_t eraseOffset;
	/// UHS_SPEED_GRADE, speed grade for UHS mode
	uint8_t uhsSpeedGrade;
	/// UHS_AU_SIZE, size of AU for UHS mode
	uint8_t uhsAuSize;
	/// VIDEO_SPEED_CLASS, video speed class value of the card
	uint8_t videoSpeedClass;
	/// APP_PERF_CLASS, application performance class value of the card
	uint8_t appPerfClass;
	/// PERFORMANCE_ENHANCE, support for performance enhancement functionalities
	uint8_t performanceEnhance;
	/// DISCARD_SUPPORT, discard support
	uint8_t discardSupport;
	/// FULE_SUPPORT, full user area logical erase support
	uint8_t fuleSupport;
};

/// select guard for SD or MMC card connected via SPI
class SelectGuard : public SpiDeviceSelectGuard
{
public:

	/// import SpiDeviceSelectGuard's constructor
	using SpiDeviceSelectGuard::SpiDeviceSelectGuard;

	/**
	 * \brief SelectGuard's destructor
	 */

	~SelectGuard()
	{
		SpiMasterOperation operation {{nullptr, nullptr, 1}};
		getSpiMasterProxy().executeTransaction(SpiMasterOperationsRange{operation});
	}
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// mask for data response token
constexpr uint8_t dataResponseTokenMask {0b00011111};
/// data response token - data accepted
constexpr uint8_t dataResponseTokenDataAccepted {0b010 << 1 | 1};

/// control token - start block
constexpr uint8_t startBlockToken {0b11111110};
/// control token - start block for CMD25
constexpr uint8_t startBlockWriteToken {0b11111100};
/// control token - stop tran
constexpr uint8_t stopTranToken {0b11111101};

/// R1 - in idle state
constexpr uint8_t r1InIdleStateMask {1 << 0};

/// OCR - CCS bit mask
constexpr uint32_t ocrCcsMask {1 << 30};

/// ACMD41 argument - HCS bit position
constexpr uint8_t acmd41HcsPosition {30};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Extracts up to 32 bits from data range.
 *
 * Bits are numbered just like in the CSD.
 *
 * \param [in] range is the data range from which bits will be extracted
 * \param [in] index is the index of starting bit, 0 - LSB of last element in data range
 * \param [in] size is the number of bits to extract
 *
 * \return bits extracted from data range
 */

uint32_t extractBits(const ConstUint8Range range, const size_t index, const size_t size)
{
	using Type = decltype(extractBits(range, index, size));
	assert(size <= sizeof(Type) * CHAR_BIT);

	constexpr auto divider = sizeof(decltype(range)::value_type) * CHAR_BIT;
	const auto begin = index / divider;
	const auto end = (index + size + divider - 1) / divider;
	assert(end <= range.size());

	const auto offset = index % divider;
	Type value {};
	for (size_t i {begin}; i < end; ++i)
	{
		const decltype(value) byte = range.rbegin()[i];
		const auto shift = static_cast<int>((i - begin) * divider) - static_cast<int>(offset);
		if (shift >= 0)
			value |= byte << shift;
		else
			value |= byte >> -shift;
	}

	value &= (1u << size) - 1;
	return value;
}

/**
 * \brief Decodes raw data into CSD.
 *
 * \param [in] buffer is a reference to array with raw data containing CSD
 *
 * \return decoded CSD
 */

Csd decodeCsd(const std::array<uint8_t, 16>& buffer)
{
	Csd csd {};
	csd.csdStructure = extractBits(ConstUint8Range{buffer}, 126, 2);
	csd.taac = extractBits(ConstUint8Range{buffer}, 112, 8);
	csd.nsac = extractBits(ConstUint8Range{buffer}, 104, 8);
	csd.tranSpeed = extractBits(ConstUint8Range{buffer}, 96, 8);
	csd.ccc = extractBits(ConstUint8Range{buffer}, 84, 12);
	csd.readBlLen = extractBits(ConstUint8Range{buffer}, 80, 4);
	csd.readBlPartial = extractBits(ConstUint8Range{buffer}, 79, 1);
	csd.writeBlkMisalign = extractBits(ConstUint8Range{buffer}, 78, 1);
	csd.readBlkMisalign = extractBits(ConstUint8Range{buffer}, 77, 1);
	csd.dsrImp = extractBits(ConstUint8Range{buffer}, 76, 1);

	if (csd.csdStructure == 0)
	{
		csd.csdV1.cSize = extractBits(ConstUint8Range{buffer}, 62, 12);
		csd.csdV1.vddRCurrMin = extractBits(ConstUint8Range{buffer}, 59, 3);
		csd.csdV1.vddRCurrMax = extractBits(ConstUint8Range{buffer}, 56, 3);
		csd.csdV1.vddWCurrMin = extractBits(ConstUint8Range{buffer}, 53, 3);
		csd.csdV1.vddWCurrMax = extractBits(ConstUint8Range{buffer}, 50, 3);
		csd.csdV1.cSizeMult = extractBits(ConstUint8Range{buffer}, 47, 3);
	}
	else if (csd.csdStructure == 1)
		csd.csdV2.cSize = extractBits(ConstUint8Range{buffer}, 48, 22);

	csd.eraseBlkEn = extractBits(ConstUint8Range{buffer}, 46, 1);
	csd.sectorSize = extractBits(ConstUint8Range{buffer}, 39, 7);
	csd.wpGrpSize = extractBits(ConstUint8Range{buffer}, 32, 7);
	csd.wpGrpEnable = extractBits(ConstUint8Range{buffer}, 31, 1);
	csd.r2wFactor = extractBits(ConstUint8Range{buffer}, 26, 3);
	csd.writeBlLen = extractBits(ConstUint8Range{buffer}, 22, 4);
	csd.writeBlPartial = extractBits(ConstUint8Range{buffer}, 21, 1);
	csd.fileFormatGrp = extractBits(ConstUint8Range{buffer}, 15, 1);
	csd.copy = extractBits(ConstUint8Range{buffer}, 14, 1);
	csd.permWriteProtect = extractBits(ConstUint8Range{buffer}, 13, 1);
	csd.tmpWriteProtect = extractBits(ConstUint8Range{buffer}, 12, 1);
	csd.fileFormat = extractBits(ConstUint8Range{buffer}, 10, 2);
	return csd;
}

/**
 * \brief Decodes raw data into SD Status.
 *
 * \param [in] buffer is a reference to array with raw data containing SD Status
 *
 * \return decoded SD Status
 */

SdStatus decodeSdStatus(const std::array<uint8_t, 64>& buffer)
{
	SdStatus sdStatus {};
	sdStatus.datBusWidth = extractBits(ConstUint8Range{buffer}, 510, 511 - 510 + 1);
	sdStatus.securedMode = extractBits(ConstUint8Range{buffer}, 509, 509 - 509 + 1);
	sdStatus.sdCardType = extractBits(ConstUint8Range{buffer}, 480, 495 - 480 + 1);
	sdStatus.sizeOfProtectedArea = extractBits(ConstUint8Range{buffer}, 448, 479 - 448 + 1);
	sdStatus.speedClass = extractBits(ConstUint8Range{buffer}, 440, 447 - 440 + 1);
	sdStatus.performanceMove = extractBits(ConstUint8Range{buffer}, 432, 439 - 432 + 1);
	sdStatus.auSize = extractBits(ConstUint8Range{buffer}, 428, 431 - 428 + 1);
	sdStatus.eraseSize = extractBits(ConstUint8Range{buffer}, 408, 423 - 408 + 1);
	sdStatus.eraseTimeout = extractBits(ConstUint8Range{buffer}, 402, 407 - 402 + 1);
	sdStatus.eraseOffset = extractBits(ConstUint8Range{buffer}, 400, 401 - 400 + 1);
	sdStatus.uhsSpeedGrade = extractBits(ConstUint8Range{buffer}, 396, 399 - 396 + 1);
	sdStatus.uhsAuSize = extractBits(ConstUint8Range{buffer}, 392, 395 - 392 + 1);
	sdStatus.videoSpeedClass = extractBits(ConstUint8Range{buffer}, 384, 391 - 384 + 1);
	sdStatus.vscAuSize = extractBits(ConstUint8Range{buffer}, 368, 377 - 368 + 1);
	sdStatus.susAddr = extractBits(ConstUint8Range{buffer}, 346, 367 - 346 + 1);
	sdStatus.appPerfClass = extractBits(ConstUint8Range{buffer}, 336, 339 - 336 + 1);
	sdStatus.performanceEnhance = extractBits(ConstUint8Range{buffer}, 328, 335 - 328 + 1);
	sdStatus.discardSupport = extractBits(ConstUint8Range{buffer}, 313, 313 - 313 + 1);
	sdStatus.fuleSupport = extractBits(ConstUint8Range{buffer}, 312, 312 - 312 + 1);
	return sdStatus;
}

/**
 * \brief Waits while byte received via SPI satisfies predicate.
 *
 * \tparam Functor is the type of functor, should be callable as bool(const uint8_t&)
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] duration is the duration of wait before giving up
 * \param [in] functor is the functor used to check predicate
 *
 * \return pair with return code (0 on success, error code otherwise) and last byte that was received; error codes:
 * - ETIMEDOUT - the wait could not be completed before the specified timeout expired;
 * - error codes returned by SpiMasterProxy::executeTransaction();
 */

template<typename Functor>
std::pair<int, uint8_t> waitWhile(SpiMasterProxy& spiMasterProxy, const distortos::TickClock::duration duration,
		Functor functor)
{
	const auto deadline = distortos::TickClock::now() + duration;
	while (distortos::TickClock::now() < deadline)
	{
		uint8_t byte;
		SpiMasterOperation operation {{nullptr, &byte, sizeof(byte)}};
		const auto ret = spiMasterProxy.executeTransaction(SpiMasterOperationsRange{operation});
		if (ret.first != 0)
			return {ret.first, {}};
		if (functor(byte) == false)
			return {{}, byte};
	}

	return {ETIMEDOUT, {}};
}

/**
 * \brief Waits while SD or MMC card connected via SPI is busy.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] duration is the duration of wait before giving up
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by waitWhile();
 */

int waitWhileBusy(SpiMasterProxy& spiMasterProxy, const distortos::TickClock::duration duration)
{
	const auto ret = waitWhile(spiMasterProxy, duration,
			[](const uint8_t& byte)
			{
				return byte != 0xff;
			});
	return ret.first;
}

/**
 * \brief Reads data block from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [out] buffer is a pointer to buffer for received data
 * \param [in] size is the size of data block that should be read, bytes
 * \param [in] duration is the duration of wait before giving up
 *
 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when error
 * code is returned); error codes:
 * - EIO - unexpected control token was read;
 * - error codes returned by waitWhile();
 * - error codes returned by SpiMasterProxy::executeTransaction();
 */

std::pair<int, size_t> readDataBlock(SpiMasterProxy& spiMasterProxy, void* const buffer, const size_t size,
		const distortos::TickClock::duration duration)
{
	{
		const auto ret = waitWhile(spiMasterProxy, duration,
				[](const uint8_t& byte)
				{
					return byte == 0xff;
				});
		if (ret.first != 0)
			return {ret.first, {}};
		if (ret.second != startBlockToken)
			return {EIO, {}};
	}

	SpiMasterOperation operations[]
	{
			{{nullptr, buffer, size}},
			{{nullptr, nullptr, 2}},	// crc
	};
	const auto ret = spiMasterProxy.executeTransaction(SpiMasterOperationsRange{operations});
	const auto bytesRead = operations[0].getTransfer()->getBytesTransfered();
	return {ret.first, bytesRead};
}

/**
 * \brief Writes data block to SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] token is the token which will be used to start data block
 * \param [in] buffer is a pointer to buffer with written data
 * \param [in] size is the size of data block that should be written, bytes
 * \param [in] duration is the duration of wait before giving up
 *
 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when error
 * code is returned); error codes:
 * - EIO - unexpected data response token was read;
 * - error codes returned by waitWhileBusy();
 * - error codes returned by SpiMasterProxy::executeTransaction();
 */

std::pair<int, size_t> writeDataBlock(SpiMasterProxy& spiMasterProxy, const uint8_t token,
		const void* const buffer, const size_t size, const distortos::TickClock::duration duration)
{
	uint8_t footer[3];	// crc + data response token
	size_t bytesWritten {};
	{
		const uint8_t header[] {0xff, token};
		SpiMasterOperation operations[]
		{
				{{&header, nullptr, sizeof(header)}},
				{{buffer, nullptr, size}},
				{{nullptr, footer, sizeof(footer)}},
		};
		const auto ret = spiMasterProxy.executeTransaction(SpiMasterOperationsRange{operations});
		bytesWritten = operations[1].getTransfer()->getBytesTransfered();
		if (ret.first != 0)
			return {ret.first, bytesWritten};
	}
	{
		const auto ret = waitWhileBusy(spiMasterProxy, duration);
		if (ret != 0)
			return {ret, bytesWritten};
	}

	const auto dataResponseToken = footer[2];
	if ((dataResponseToken & dataResponseTokenMask) != dataResponseTokenDataAccepted)
		return {EIO, bytesWritten};

	return {{}, bytesWritten};
}

/**
 * \brief Reads response from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] buffer is a buffer for received response
 *
 * \return 0 on success, error code otherwise:
 * - ETIMEDOUT - expected number of valid bytes could not be received within allowed number of transfers;
 * - error codes returned by SpiMasterProxy::executeTransaction();
 */

int readResponse(SpiMasterProxy& spiMasterProxy, const Uint8Range buffer)
{
	size_t bytesRead {};
	size_t validBytesRead {};
	const size_t maxBytesRead {buffer.size() + 8};
	while (bytesRead < maxBytesRead)
	{
		const auto readSize = buffer.size() - validBytesRead;
		SpiMasterOperation operation {{nullptr, buffer.begin() + validBytesRead, readSize}};
		const auto ret = spiMasterProxy.executeTransaction(SpiMasterOperationsRange{operation});
		if (ret.first != 0)
			return ret.first;

		if (validBytesRead == 0)
		{
			const auto invalidBytes = std::find_if(buffer.begin(), buffer.end(),
					[](const uint8_t& value)
					{
						return value != 0xff;
					}) - buffer.begin();
			const auto validBytes = buffer.size() - invalidBytes;
			if (validBytes != 0 && invalidBytes != 0)
				memmove(buffer.begin(), buffer.begin() + invalidBytes, validBytes);

			validBytesRead = validBytes;
		}
		else
			validBytesRead += readSize;

		if (validBytesRead == buffer.size())
			return {};

		bytesRead += readSize;
	}

	return ETIMEDOUT;
}

/**
 * \brief Reads R1 response from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by readResponse();
 */

std::pair<int, uint8_t> readR1(SpiMasterProxy& spiMasterProxy)
{
	uint8_t r1;
	const auto ret = readResponse(spiMasterProxy, Uint8Range{r1});
	return {ret, r1};
}

/**
 * \brief Reads R2 response from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return pair with return code (0 on success, error code otherwise) and R2 response; error codes:
 * - error codes returned by readResponse();
 */

std::pair<int, R2Response> readR2(SpiMasterProxy& spiMasterProxy)
{
	uint8_t r2[2];
	const auto ret = readResponse(spiMasterProxy, Uint8Range{r2});
	return {ret, {r2[0], r2[1]}};
}

/**
 * \brief Reads R3 response from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return pair with return code (0 on success, error code otherwise) and R3 response; error codes:
 * - error codes returned by readResponse();
 */

std::pair<int, R3Response> readR3(SpiMasterProxy& spiMasterProxy)
{
	uint8_t r3[5];
	const auto ret = readResponse(spiMasterProxy, Uint8Range{r3});
	R3Response r3Response {};
	r3Response.r1 = r3[0];
	r3Response.ocr = r3[1] << 24 | r3[2] << 16 | r3[3] << 8 | r3[4];
	return {ret, r3Response};
}

/**
 * \brief Reads R7 response from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return pair with return code (0 on success, error code otherwise) and R7 response; error codes:
 * - error codes returned by readResponse();
 */

std::pair<int, R7Response> readR7(SpiMasterProxy& spiMasterProxy)
{
	uint8_t r7[5];
	const auto ret = readResponse(spiMasterProxy, Uint8Range{r7});
	R7Response r7Response {};
	r7Response.r1 = r7[0];
	r7Response.commandVersion = r7[1] >> 4;
	r7Response.reservedBits = (r7[1] & 0xf) << 12 | r7[2] << 4 | r7[3] >> 4;
	r7Response.voltageAccepted = r7[3] & 0xf;
	r7Response.checkPattern = r7[4];
	return {ret, r7Response};
}

/**
 * \brief Writes regular (CMD) command to SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] command is the command that will be written
 * \param [in] argument is the argument for command, default - 0
 * \param [in] crc7 is the value of CRC-7 appended to the transferred block, default - 0
 * \param [in] stuffByte selects whether stuff byte will be appended to the transferred block, default - false
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by SpiMasterProxy::executeTransaction();
 */

int writeCmd(SpiMasterProxy& spiMasterProxy, const uint8_t command, const uint32_t argument = {},
		const uint8_t crc7 = {}, const bool stuffByte = {})
{
	const uint8_t buffer[]
	{
			0xff,	// dummy byte as a delay before the command
			static_cast<uint8_t>(0x40 | command),
			static_cast<uint8_t>(argument >> 24),
			static_cast<uint8_t>(argument >> 16),
			static_cast<uint8_t>(argument >> 8),
			static_cast<uint8_t>(argument),
			static_cast<uint8_t>(crc7 << 1 | 1),
			0xff,	// stuff byte
	};
	SpiMasterOperation operation {{buffer, nullptr, sizeof(buffer) - !stuffByte}};
	const auto ret = spiMasterProxy.executeTransaction(SpiMasterOperationsRange{operation});
	return ret.first;
}

/**
 * \brief Writes regular (CMD) command and reads R1 response to/from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] command is the command that will be written
 * \param [in] argument is the argument for command, default - 0
 * \param [in] crc7 is the value of CRC-7 appended to the transferred block, default - 0
 * \param [in] stuffByte selects whether stuff byte will be appended to the transferred block, default - false
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by readR1();
 * - error codes returned by writeCmd();
 */

std::pair<int, uint8_t> writeCmdReadR1(SpiMasterProxy& spiMasterProxy, const uint8_t command,
		const uint32_t argument = {}, const uint8_t crc7 = {}, const bool stuffByte = {})
{
	const auto ret = writeCmd(spiMasterProxy, command, argument, crc7, stuffByte);
	if (ret != 0)
		return {ret, {}};

	return readR1(spiMasterProxy);
}

/**
 * \brief Writes regular (CMD) command and reads R3 response to/from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] command is the command that will be written
 * \param [in] argument is the argument for command, default - 0
 * \param [in] crc7 is the value of CRC-7 appended to the transferred block, default - 0
 * \param [in] stuffByte selects whether stuff byte will be appended to the transferred block, default - false
 *
 * \return pair with return code (0 on success, error code otherwise) and R3 response; error codes:
 * - error codes returned by readR3();
 * - error codes returned by writeCmd();
 */

std::pair<int, R3Response> writeCmdReadR3(SpiMasterProxy& spiMasterProxy, const uint8_t command,
		const uint32_t argument = {}, const uint8_t crc7 = {}, const bool stuffByte = {})
{
	const auto ret = writeCmd(spiMasterProxy, command, argument, crc7, stuffByte);
	if (ret != 0)
		return {ret, {}};

	return readR3(spiMasterProxy);
}

/**
 * \brief Writes regular (CMD) command and reads R7 response to/from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] command is the command that will be written
 * \param [in] argument is the argument for command, default - 0
 * \param [in] crc7 is the value of CRC-7 appended to the transferred block, default - 0
 * \param [in] stuffByte selects whether stuff byte will be appended to the transferred block, default - false
 *
 * \return pair with return code (0 on success, error code otherwise) and R7 response; error codes:
 * - error codes returned by readR7();
 * - error codes returned by writeCmd();
 */

std::pair<int, R7Response> writeCmdReadR7(SpiMasterProxy& spiMasterProxy, const uint8_t command,
		const uint32_t argument = {}, const uint8_t crc7 = {}, const bool stuffByte = {})
{
	const auto ret = writeCmd(spiMasterProxy, command, argument, crc7, stuffByte);
	if (ret != 0)
		return {ret, {}};

	return readR7(spiMasterProxy);
}

/**
 * \brief Executes CMD0 command on SD or MMC card connected via SPI.
 *
 * This is GO_IDLE_STATE command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd0(SpiMasterProxy& spiMasterProxy)
{
	return writeCmdReadR1(spiMasterProxy, 0, {}, 0x4a);
}

/**
 * \brief Executes CMD1 command on SD or MMC card connected via SPI.
 *
 * This is SEND_OP_COND command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd1(SpiMasterProxy& spiMasterProxy)
{
	return writeCmdReadR1(spiMasterProxy, 1);
}

/**
 * \brief Executes CMD8 command on SD or MMC card connected via SPI.
 *
 * This is SEND_IF_COND command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return tuple with return code (0 on success, error code otherwise), R1 response and a boolean value which tells
 * whether pattern was matched; error codes:
 * - error codes returned by writeCmdReadR7();
 */

std::tuple<int, uint8_t, bool> executeCmd8(SpiMasterProxy& spiMasterProxy)
{
	constexpr uint8_t supplyVoltage {1};	// 2.7 - 3.6 V
	constexpr uint8_t checkPattern {0xaa};
	const auto ret = writeCmdReadR7(spiMasterProxy, 8, supplyVoltage << 8 | checkPattern, 0x43);
	const auto match = ret.second.checkPattern == checkPattern && ret.second.voltageAccepted == supplyVoltage;
	return std::make_tuple(ret.first, ret.second.r1, match);
}

/**
 * \brief Executes CMD9 command on SD or MMC card connected via SPI.
 *
 * This is SEND_CSD command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return tuple with return code (0 on success, error code otherwise), R1 response and array with raw data containing
 * CSD; error codes:
 * - error codes returned by readDataBlock();
 * - error codes returned by writeCmdReadR1();
 */

std::tuple<int, uint8_t, std::array<uint8_t, 16>> executeCmd9(SpiMasterProxy& spiMasterProxy)
{
	{
		const auto ret = writeCmdReadR1(spiMasterProxy, 9);
		if (ret.first != 0 || ret.second != 0)
			return decltype(executeCmd9(spiMasterProxy)){ret.first, ret.second, {}};
	}
	std::array<uint8_t, 16> csdBuffer;
	// "7.2.6 Read CID/CSD Registers" of Physical Layer Simplified Specification Version 6.00 - use fixed read timeout
	const auto ret = readDataBlock(spiMasterProxy, csdBuffer.begin(), csdBuffer.size(), std::chrono::milliseconds{100});
	return decltype(executeCmd9(spiMasterProxy)){ret.first, {}, csdBuffer};
}

/**
 * \brief Executes CMD12 command on SD or MMC card connected via SPI.
 *
 * This is STOP_TRANSMISSION command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] duration is the duration of wait before giving up
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by waitWhileBusy();
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd12(SpiMasterProxy& spiMasterProxy, const distortos::TickClock::duration duration)
{
	const auto response = writeCmdReadR1(spiMasterProxy, 12, {}, {}, true);
	if (response.first != 0)
		return response;

	const auto ret = waitWhileBusy(spiMasterProxy, duration);
	return {ret, response.second};
}

/**
 * \brief Executes CMD16 command on SD or MMC card connected via SPI.
 *
 * This is SET_BLOCKLEN command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] blockLength is the length of read/write block, bytes
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd16(SpiMasterProxy& spiMasterProxy, const uint32_t blockLength)
{
	return writeCmdReadR1(spiMasterProxy, 16, blockLength);
}

/**
 * \brief Executes CMD17 command on SD or MMC card connected via SPI.
 *
 * This is READ_SINGLE_BLOCK command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] address is the address from which data will be read, bytes or blocks
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd17(SpiMasterProxy& spiMasterProxy, const uint32_t address)
{
	return writeCmdReadR1(spiMasterProxy, 17, address);
}

/**
 * \brief Executes CMD18 command on SD or MMC card connected via SPI.
 *
 * This is READ_MULTIPLE_BLOCK command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] address is the address from which data will be read, bytes or blocks
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd18(SpiMasterProxy& spiMasterProxy, const uint32_t address)
{
	return writeCmdReadR1(spiMasterProxy, 18, address);
}

/**
 * \brief Executes CMD24 command on SD or MMC card connected via SPI.
 *
 * This is WRITE_BLOCK command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] address is the address to which data will be written, bytes or blocks
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd24(SpiMasterProxy& spiMasterProxy, const uint32_t address)
{
	return writeCmdReadR1(spiMasterProxy, 24, address);
}

/**
 * \brief Executes CMD25 command on SD or MMC card connected via SPI.
 *
 * This is WRITE_MULTIPLE_BLOCK command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] address is the address to which data will be written, bytes or blocks
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd25(SpiMasterProxy& spiMasterProxy, const uint32_t address)
{
	return writeCmdReadR1(spiMasterProxy, 25, address);
}

/**
 * \brief Executes CMD32 command on SD or MMC card connected via SPI.
 *
 * This is ERASE_WR_BLK_START_ADDR command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] address is the address of first block marked for erase, bytes or blocks
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd32(SpiMasterProxy& spiMasterProxy, const uint32_t address)
{
	return writeCmdReadR1(spiMasterProxy, 32, address);
}

/**
 * \brief Executes CMD33 command on SD or MMC card connected via SPI.
 *
 * This is ERASE_WR_BLK_END_ADDR command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] address is the address of last block marked for erase, bytes or blocks
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd33(SpiMasterProxy& spiMasterProxy, const uint32_t address)
{
	return writeCmdReadR1(spiMasterProxy, 33, address);
}

/**
 * \brief Executes CMD38 command on SD or MMC card connected via SPI.
 *
 * This is ERASE command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] duration is the duration of wait before giving up
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by waitWhileBusy();
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd38(SpiMasterProxy& spiMasterProxy, const distortos::TickClock::duration duration)
{
	const auto response = writeCmdReadR1(spiMasterProxy, 38);
	if (response.first != 0)
		return response;

	const auto ret = waitWhileBusy(spiMasterProxy, duration);
	return {ret, response.second};
}

/**
 * \brief Executes CMD55 command on SD or MMC card connected via SPI.
 *
 * This is APP_CMD command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeCmdReadR1();
 */

std::pair<int, uint8_t> executeCmd55(SpiMasterProxy& spiMasterProxy)
{
	return writeCmdReadR1(spiMasterProxy, 55);
}

/**
 * \brief Executes CMD58 command on SD or MMC card connected via SPI.
 *
 * This is READ_OCR command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 *
 * \return pair with return code (0 on success, error code otherwise) and R3 response; error codes:
 * - error codes returned by writeCmdReadR3();
 */

std::pair<int, R3Response> executeCmd58(SpiMasterProxy& spiMasterProxy)
{
	return writeCmdReadR3(spiMasterProxy, 58);
}

/**
 * \brief Writes application (ACMD) command to SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] command is the command that will be written
 * \param [in] argument is the argument for command, default - 0
 * \param [in] crc7 is the value of CRC-7 appended to the transferred block, default - 0
 * \param [in] stuffByte selects whether stuff byte will be appended to the transferred block, default - false
 *
 * \return 0 on success, error code otherwise:
 * - EIO - unexpected R1 response for CMD55 was read;
 * - error codes returned by executeCmd55();
 * - error codes returned by writeCmd();
 */

int writeAcmd(SpiMasterProxy& spiMasterProxy, const uint8_t command, const uint32_t argument = {},
		const uint8_t crc7 = {}, const bool stuffByte = {})
{
	const auto ret = executeCmd55(spiMasterProxy);
	if (ret.first != 0)
		return ret.first;
	if (ret.second != 0 && ret.second != r1InIdleStateMask)
		return EIO;

	return writeCmd(spiMasterProxy, command, argument, crc7, stuffByte);
}

/**
 * \brief Writes application (ACMD) command and reads R1 response to/from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] command is the command that will be written
 * \param [in] argument is the argument for command, default - 0
 * \param [in] crc7 is the value of CRC-7 appended to the transferred block, default - 0
 * \param [in] stuffByte selects whether stuff byte will be appended to the transferred block, default - false
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by readR1();
 * - error codes returned by writeAcmd();
 */

std::pair<int, uint8_t> writeAcmdReadR1(SpiMasterProxy& spiMasterProxy, const uint8_t command,
		const uint32_t argument = {}, const uint8_t crc7 = {}, const bool stuffByte = {})
{
	const auto ret = writeAcmd(spiMasterProxy, command, argument, crc7, stuffByte);
	if (ret != 0)
		return {ret, {}};

	return readR1(spiMasterProxy);
}

/**
 * \brief Writes application (ACMD) command and reads R2 response to/from SD or MMC card connected via SPI.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] command is the command that will be written
 * \param [in] argument is the argument for command, default - 0
 * \param [in] crc7 is the value of CRC-7 appended to the transferred block, default - 0
 * \param [in] stuffByte selects whether stuff byte will be appended to the transferred block, default - false
 *
 * \return pair with return code (0 on success, error code otherwise) and R2 response; error codes:
 * - error codes returned by readR2();
 * - error codes returned by writeAcmd();
 */

std::pair<int, R2Response> writeAcmdReadR2(SpiMasterProxy& spiMasterProxy, const uint8_t command,
		const uint32_t argument = {}, const uint8_t crc7 = {}, const bool stuffByte = {})
{
	const auto ret = writeAcmd(spiMasterProxy, command, argument, crc7, stuffByte);
	if (ret != 0)
		return {ret, {}};

	return readR2(spiMasterProxy);
}

/**
 * \brief Executes ACMD13 command on SD or MMC card connected via SPI.
 *
 * This is SD_STATUS command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] duration is the duration of wait before giving up
 *
 * \return tuple with return code (0 on success, error code otherwise), R2 response and array with raw data containing
 * SD Status register; error codes:
 * - error codes returned by readDataBlock();
 * - error codes returned by writeAcmdReadR2();
 */

std::tuple<int, R2Response, std::array<uint8_t, 64>> executeAcmd13(SpiMasterProxy& spiMasterProxy,
		const distortos::TickClock::duration duration)
{
	{
		const auto ret = writeAcmdReadR2(spiMasterProxy, 13);
		if (ret.first != 0 || ret.second.r1 != 0)
			return decltype(executeAcmd13(spiMasterProxy, duration)){ret.first, ret.second, {}};
	}
	std::array<uint8_t, 64> sdStatusBuffer;
	const auto ret = readDataBlock(spiMasterProxy, sdStatusBuffer.begin(), sdStatusBuffer.size(), duration);
	return decltype(executeAcmd13(spiMasterProxy, duration)){ret.first, {}, sdStatusBuffer};
}

/**
 * \brief Executes ACMD41 command on SD or MMC card connected via SPI.
 *
 * This is SD_SEND_OP_COND command.
 *
 * \param [in] spiMasterProxy is a reference to SpiMasterProxy object used for communication
 * \param [in] hcs is the value of HCS (Host Capacity Support) bit sent to the SD or MMC card, which selects whether
 * host supports SDHC or SDXC cards
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by writeAcmdReadR1();
 */

std::pair<int, uint8_t> executeAcmd41(SpiMasterProxy& spiMasterProxy, const bool hcs)
{
	return writeAcmdReadR1(spiMasterProxy, 41, hcs << acmd41HcsPosition);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiSdMmcCard::~SpiSdMmcCard()
{

}

int SpiSdMmcCard::close()
{
	const SpiDeviceProxy spiDeviceProxy {spiDevice_};

	const auto ret = spiDevice_.close();

	if (spiDeviceProxy.isOpened() == false)
		deinitialize();

	return ret;
}

int SpiSdMmcCard::erase(const uint64_t address, const uint64_t size)
{
	const SpiDeviceProxy spiDeviceProxy {spiDevice_};

	if (type_ == Type::unknown)
		return EBADF;

	if (size == 0)
		return {};

	if (address % blockSize != 0 || size % blockSize != 0)
		return EINVAL;

	const auto firstBlock = address / blockSize;
	const auto blocks = size / blockSize;

	if (firstBlock + blocks > blocksCount_)
		return ENOSPC;

	SpiMasterProxy spiMasterProxy {spiDeviceProxy};

	{
		const auto ret = spiMasterProxy.configure(SpiMode::_0, clockFrequency_, 8, false, UINT32_MAX);
		if (ret.first != 0)
			return ret.first;
	}

	uint64_t erased {};
	while (erased < size)
	{
		const auto beginAddress = address + erased;
		// erase no more than 1 AU at a time
		const auto endAddress = std::min(address + size, beginAddress / auSize_ * auSize_ + auSize_);

		{
			const SelectGuard selectGuard {spiMasterProxy};

			const auto commandAddress = blockAddressing_ == true ? beginAddress / blockSize : beginAddress;
			const auto ret = executeCmd32(spiMasterProxy, commandAddress);
			if (ret.first != 0)
				return ret.first;
			if (ret.second != 0)
				return EIO;
		}
		{
			const SelectGuard selectGuard {spiMasterProxy};

			const auto commandAddress = blockAddressing_ == true ? (endAddress - blockSize) / blockSize :
					(endAddress - blockSize);
			const auto ret = executeCmd33(spiMasterProxy, commandAddress);
			if (ret.first != 0)
				return ret.first;
			if (ret.second != 0)
				return EIO;
		}
		{
			const SelectGuard selectGuard {spiMasterProxy};

			const auto beginPartial = beginAddress % auSize_ != 0;
			const auto endPartial = endAddress % auSize_ != 0;
			const auto ret = executeCmd38(spiMasterProxy, std::chrono::milliseconds{eraseTimeoutMs_} +
					std::chrono::milliseconds{250} * (beginPartial + endPartial));
			if (ret.first != 0)
				return ret.first;
			if (ret.second != 0)
				return EIO;
		}

		erased += endAddress - beginAddress;
	}

	return {};
}

size_t SpiSdMmcCard::getEraseBlockSize() const
{
	return blockSize;
}

std::pair<bool, uint8_t> SpiSdMmcCard::getErasedValue() const
{
	/// \todo implement by reading DATA_STAT_AFTER_ERASE from SCR register
	return {};
}

size_t SpiSdMmcCard::getProgramBlockSize() const
{
	return blockSize;
}

size_t SpiSdMmcCard::getReadBlockSize() const
{
	return blockSize;
}

uint64_t SpiSdMmcCard::getSize() const
{
	return static_cast<decltype(getSize())>(blockSize) * blocksCount_;
}

int SpiSdMmcCard::lock()
{
	return spiDevice_.lock();
}

int SpiSdMmcCard::open()
{
	const SpiDeviceProxy spiDeviceProxy {spiDevice_};

	const auto opened = spiDeviceProxy.isOpened();

	{
		const auto ret = spiDevice_.open();
		if (ret != 0)
			return ret;
	}

	if (opened == true)
		return {};

	auto closeScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				close();
			});

	{
		decltype(initialize(spiDeviceProxy)) ret;
		unsigned int attempt {};
		while (ret = initialize(spiDeviceProxy), ret != 0)
			if (++attempt >= 100)
				return ret;
	}

	closeScopeGuard.release();
	return 0;
}

std::pair<int, size_t> SpiSdMmcCard::program(const uint64_t address, const void* const buffer, const size_t size)
{
	const SpiDeviceProxy spiDeviceProxy {spiDevice_};

	if (type_ == Type::unknown)
		return {EBADF, {}};

	if (size == 0)
		return {{}, {}};

	if (buffer == nullptr || address % blockSize != 0 || size % blockSize != 0)
		return {EINVAL, {}};

	const auto firstBlock = address / blockSize;
	const auto blocks = size / blockSize;

	if (firstBlock + blocks > blocksCount_)
		return {ENOSPC, {}};

	SpiMasterProxy spiMasterProxy {spiDeviceProxy};

	{
		const auto ret = spiMasterProxy.configure(SpiMode::_0, clockFrequency_, 8, false, UINT32_MAX);
		if (ret.first != 0)
			return {ret.first, {}};
	}

	const SelectGuard selectGuard {spiMasterProxy};

	{
		const auto commandAddress = blockAddressing_ == true ? firstBlock : address;
		const auto ret = blocks == 1 ? executeCmd24(spiMasterProxy, commandAddress) :
				executeCmd25(spiMasterProxy, commandAddress);
		if (ret.first != 0)
			return {ret.first, {}};
		if (ret.second != 0)
			return {EIO, {}};
	}

	const auto bufferUint8 = static_cast<const uint8_t*>(buffer);
	size_t bytesWritten {};
	for (size_t block {}; block < blocks; ++block)
	{
		const auto ret = writeDataBlock(spiMasterProxy, blocks == 1 ? startBlockToken : startBlockWriteToken,
				bufferUint8 + block * blockSize, blockSize, std::chrono::milliseconds{writeTimeoutMs_});
		bytesWritten += ret.second;
		if (ret.first != 0)
			return {ret.first, bytesWritten};
	}

	if (blocks != 1)
	{
		{
			const uint8_t stopTransfer[]
			{
					stopTranToken,
					0xff,
			};
			SpiMasterOperation operation {{stopTransfer, nullptr, sizeof(stopTransfer)}};
			const auto ret = spiMasterProxy.executeTransaction(SpiMasterOperationsRange{operation});
			if (ret.first != 0)
				return {ret.first, bytesWritten};
		}
		{
			const auto ret = waitWhileBusy(spiMasterProxy, std::chrono::milliseconds{writeTimeoutMs_});
			if (ret != 0)
				return {ret, bytesWritten};
		}
	}

	return {{}, bytesWritten};
}

std::pair<int, size_t> SpiSdMmcCard::read(const uint64_t address, void* const buffer, const size_t size)
{
	const SpiDeviceProxy spiDeviceProxy {spiDevice_};

	if (type_ == Type::unknown)
		return {EBADF, {}};

	if (size == 0)
		return {{}, {}};

	if (buffer == nullptr || address % blockSize != 0 || size % blockSize != 0)
		return {EINVAL, {}};

	const auto firstBlock = address / blockSize;
	const auto blocks = size / blockSize;

	if (firstBlock + blocks > blocksCount_)
		return {ENOSPC, {}};

	SpiMasterProxy spiMasterProxy {spiDeviceProxy};

	{
		const auto ret = spiMasterProxy.configure(SpiMode::_0, clockFrequency_, 8, false, UINT32_MAX);
		if (ret.first != 0)
			return {ret.first, {}};
	}

	size_t bytesRead {};

	{
		const SelectGuard selectGuard {spiMasterProxy};

		{
			const auto commandAddress = blockAddressing_ == true ? firstBlock : address;
			const auto ret = blocks == 1 ? executeCmd17(spiMasterProxy, commandAddress) :
					executeCmd18(spiMasterProxy, commandAddress);
			if (ret.first != 0)
				return {ret.first, {}};
			if (ret.second != 0)
				return {EIO, {}};
		}

		const auto bufferUint8 = static_cast<uint8_t*>(buffer);
		for (size_t block {}; block < blocks; ++block)
		{
			const auto ret = readDataBlock(spiMasterProxy, bufferUint8 + block * blockSize, blockSize,
					std::chrono::milliseconds{readTimeoutMs_});
			bytesRead += ret.second;
			if (ret.first != 0)
				return {ret.first, bytesRead};
		}
	}

	if (blocks != 1)
	{
		const SelectGuard selectGuard {spiMasterProxy};

		const auto ret = executeCmd12(spiMasterProxy, std::chrono::milliseconds{readTimeoutMs_});
		if (ret.first != 0)
			return {ret.first, bytesRead};
		if (ret.second != 0)
			return {EIO, bytesRead};
	}

	return {{}, bytesRead};
}

int SpiSdMmcCard::synchronize()
{
	return {};
}

int SpiSdMmcCard::trim(uint64_t, uint64_t)
{
	return {};
}

int SpiSdMmcCard::unlock()
{
	return spiDevice_.unlock();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SpiSdMmcCard::deinitialize()
{
	blocksCount_ = {};
	auSize_ = {};
	eraseTimeoutMs_ = {};
	readTimeoutMs_ = {};
	writeTimeoutMs_ = {};
	blockAddressing_ = {};
	type_ = {};
}

int SpiSdMmcCard::initialize(const SpiDeviceProxy& spiDeviceProxy)
{
	SpiMasterProxy spiMasterProxy {spiDeviceProxy};

	{
		const auto ret = spiMasterProxy.configure(SpiMode::_0, 400000, 8, false, UINT32_MAX);
		if (ret.first != 0)
			return ret.first;
	}
	{
		SpiMasterOperation operation {{nullptr, nullptr, (74 + CHAR_BIT - 1) / CHAR_BIT}};
		const auto ret = spiMasterProxy.executeTransaction(SpiMasterOperationsRange{operation});
		if (ret.first != 0)
			return ret.first;
	}
	{
		const SelectGuard spiDeviceSelectGuard {spiMasterProxy};

		// 3500 milliseconds - max time of single AU partial erase
		const auto ret = waitWhileBusy(spiMasterProxy, std::chrono::milliseconds{3500});
		if (ret != 0)
			return ret;
	}
	{
		const SelectGuard spiDeviceSelectGuard {spiMasterProxy};

		const auto ret = executeCmd0(spiMasterProxy);
		if (ret.first != 0)
			return ret.first;
		if (ret.second != r1InIdleStateMask)
			return EIO;
	}
	{
		const SelectGuard spiDeviceSelectGuard {spiMasterProxy};

		const auto ret = executeCmd8(spiMasterProxy);
		if (std::get<0>(ret) != 0)
			return std::get<0>(ret);

		if (std::get<1>(ret) == r1InIdleStateMask)
		{
			if (std::get<2>(ret) == false)
				return EIO;	// voltage range not supported

			type_ = Type::sdVersion2;
		}
	}
	{
		const auto deadline = TickClock::now() + std::chrono::seconds{1};
		while (1)
		{
			{
				const SelectGuard spiDeviceSelectGuard {spiMasterProxy};

				const auto ret = executeAcmd41(spiMasterProxy, type_ == Type::sdVersion2);
				if (ret.first != 0)
					return ret.first;
				if (ret.second == 0)
				{
					if (type_ == Type::unknown)
						type_ = Type::sdVersion1;

					break;
				}
				if (ret.second != r1InIdleStateMask || TickClock::now() >= deadline)
				{
					if (type_ == Type::sdVersion2)
						return ret.second != r1InIdleStateMask ? EIO : ETIMEDOUT;
					else
						break;
				}
			}

			ThisThread::sleepFor({});
		}
	}

	if (type_ == Type::unknown)
	{
		const auto deadline = TickClock::now() + std::chrono::seconds{1};
		while (1)
		{
			{
				const SelectGuard spiDeviceSelectGuard {spiMasterProxy};

				const auto ret = executeCmd1(spiMasterProxy);
				if (ret.first != 0)
					return ret.first;
				if (ret.second == 0)
				{
					type_ = Type::mmc;
					break;
				}
				if (ret.second != r1InIdleStateMask || TickClock::now() >= deadline)
					return ret.second != r1InIdleStateMask ? EIO : ETIMEDOUT;
			}

			ThisThread::sleepFor({});
		}
	}
	{
		const auto ret = spiMasterProxy.configure(SpiMode::_0, clockFrequency_, 8, false, UINT32_MAX);
		if (ret.first != 0)
			return ret.first;
	}

	if (type_ == Type::sdVersion2)
	{
		const SelectGuard spiDeviceSelectGuard {spiMasterProxy};

		const auto ret = executeCmd58(spiMasterProxy);
		if (ret.first != 0)
			return ret.first;
		if (ret.second.r1 != 0)
			return EIO;

		blockAddressing_ = (ret.second.ocr & ocrCcsMask) != 0;
	}

	if (blockAddressing_ == false)
	{
		const SelectGuard spiDeviceSelectGuard {spiMasterProxy};

		const auto ret = executeCmd16(spiMasterProxy, blockSize);
		if (ret.first != 0)
			return ret.first;
		if (ret.second != 0)
			return EIO;
	}

	{
		const SelectGuard spiDeviceSelectGuard {spiMasterProxy};

		const auto ret = executeCmd9(spiMasterProxy);
		if (std::get<0>(ret) != 0)
			return std::get<0>(ret);
		if (std::get<1>(ret) != 0)
			return EIO;

		const auto csd = decodeCsd(std::get<2>(ret));
		if (csd.csdStructure == 0)	// CSD version 1.0
		{
			blocksCount_ = (csd.csdV1.cSize + 1) * (1 << (csd.csdV1.cSizeMult + 2)) * (1 << csd.readBlLen) / blockSize;
			/// \todo compute read and write timeout from CSD contents
			readTimeoutMs_ = 100;
			writeTimeoutMs_ = 250;
		}
		else if (csd.csdStructure == 1)	// CSD version 2.0
		{
			blocksCount_ = (static_cast<uint64_t>(csd.csdV2.cSize) + 1) * 512 * 1024 / blockSize;
			readTimeoutMs_ = 100;
			// SDHC (<= 32 GB) - 250 ms, SDXC - 500 ms
			writeTimeoutMs_ = getSize() <= 32ull * 1024 * 1024 * 1024 ? 250 : 500;
		}
		else
			return EIO;
	}
	{
		const SelectGuard spiDeviceSelectGuard {spiMasterProxy};

		const auto ret = executeAcmd13(spiMasterProxy, std::chrono::milliseconds{readTimeoutMs_});
		if (std::get<0>(ret) != 0)
			return std::get<0>(ret);
		if (std::get<1>(ret).r1 != 0 || std::get<1>(ret).r2 != 0)
			return EIO;

		const auto sdStatus = decodeSdStatus(std::get<2>(ret));
		if (sdStatus.auSize == 0 || sdStatus.eraseSize == 0 || sdStatus.eraseTimeout == 0)
			return EIO;	/// \todo add support for estimating erase timeout
		
		static const uint32_t auSizeAssociation[]
		{
				16 * 1024,
				32 * 1024,
				64 * 1024,
				128 * 1024,
				256 * 1024,
				512 * 1024,
				1 * 1024 * 1024,
				2 * 1024 * 1024,
				4 * 1024 * 1024,
				8 * 1024 * 1024,
				12 * 1024 * 1024,
				16 * 1024 * 1024,
				24 * 1024 * 1024,
				32 * 1024 * 1024,
				64 * 1024 * 1024,
		};
		assert(sdStatus.auSize - 1u < sizeof(auSizeAssociation) / sizeof(*auSizeAssociation));
		auSize_ = auSizeAssociation[sdStatus.auSize - 1u];
		eraseTimeoutMs_ =
				std::max(sdStatus.eraseTimeout * 1000 / sdStatus.eraseSize + sdStatus.eraseOffset * 1000, 1000);
	}

	return 0;
}

}	// namespace devices

}	// namespace distortos
