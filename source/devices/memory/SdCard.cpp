/**
 * \file
 * \brief SdCard class implementation
 *
 * \author Copyright (C) 2018-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/SdCard.hpp"

#include "distortos/ThisThread.hpp"

#include "estd/EnumClassFlags.hpp"
#include "estd/extractBitField.hpp"
#include "estd/ScopeGuard.hpp"

#include <mutex>

#include <cassert>

namespace distortos
{

namespace devices
{

namespace
{

static_assert(DISTORTOS_SDMMCCARD_BUFFER_ALIGNMENT <= DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT,
		"Buffer alignment for SdCard is greater than for BlockDevice!");

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// state of the card
enum class CardState : uint8_t
{
	/// idle
	idle,
	/// ready
	ready,
	/// identification (ident)
	identification,
	/// standby (stby)
	standby,
	/// transfer (tran)
	transfer,
	/// sending data (data)
	sendingData,
	/// receive data (rcv)
	receiveData,
	/// programming (prg)
	programming,
	/// disconnect (dis)
	disconnect,
	/// reserved
	reserved9,
	/// reserved
	reserved10,
	/// reserved
	reserved11,
	/// reserved
	reserved12,
	/// reserved
	reserved13,
	/// reserved
	reserved14,
	/// reserved for I/O mode
	reservedForIOMode,
};

/// alias for short response
using ShortResponse = std::array<uint32_t, 1>;

/// alias for long response
using LongResponse = std::array<uint32_t, 4>;

/// CID, card identification register
class Cid
{
public:

	/// type of raw CID data
	using RawCid = LongResponse;

	/**
	 * \brief Cid's constructor
	 *
	 * \param [in] cid is the raw CID data
	 */

	constexpr explicit Cid(const RawCid cid) :
			cid_{cid}
	{

	}

private:

	/// raw CID data
	RawCid cid_;
};

/// CSD version 1.0, card-specific data register
class CsdV1
{
public:

	/// type of raw CSD version 1.0 data
	using RawCsdV1 = LongResponse;

	/**
	 * \brief CsdV1's constructor
	 *
	 * \param [in] csdV1 is the raw CSD version 1.0 data
	 */

	constexpr explicit CsdV1(const RawCsdV1 csdV1) :
			csdV1_{csdV1}
	{

	}

	/**
	 * \return value of C_SIZE (device size) bit field
	 */

	uint16_t getCSize() const
	{
		return estd::extractBitField<62, 12>(csdV1_);
	}

	/**
	 * \return value of C_SIZE_MULT (device size multiplier) bit field
	 */

	uint8_t getCSizeMult() const
	{
		return estd::extractBitField<47, 3>(csdV1_);
	}

	/**
	 * \return value of READ_BL_LEN (max. read data block length) bit field
	 */

	uint8_t getReadBlLen() const
	{
		return estd::extractBitField<80, 4>(csdV1_);
	}

private:

	/// raw CSD version 1.0 data
	RawCsdV1 csdV1_;
};

/// CSD version 2.0, card-specific data register
class CsdV2
{
public:

	/// type of raw CSD version 2.0 data
	using RawCsdV2 = LongResponse;

	/**
	 * \brief CsdV2's constructor
	 *
	 * \param [in] csdV2 is the raw CSD version 2.0 data
	 */

	constexpr explicit CsdV2(const RawCsdV2 csdV2) :
			csdV2_{csdV2}
	{

	}

	/**
	 * \return value of C_SIZE (device size) bit field
	 */

	uint32_t getCSize() const
	{
		return estd::extractBitField<48, 22>(csdV2_);
	}

private:

	/// raw CSD version 2.0 data
	RawCsdV2 csdV2_;
};

/// CSD, card-specific data register
class Csd
{
public:

	/// type of raw CSD data
	using RawCsd = LongResponse;

	Csd() = default;

	/**
	 * \brief Csd's constructor
	 *
	 * \param [in] csd is the raw CSD data
	 */

	constexpr explicit Csd(const RawCsd csd) :
			csd_{csd}
	{

	}

	/**
	 * \return value of CSD_STRUCTURE (CSD structure) bit field
	 */

	uint8_t getCsdStructure() const
	{
		return estd::extractBitField<126, 2>(csd_);
	}

	/**
	 * \return CSD version 1.0, valid only if CSD_STRUCTURE equals 0
	 */

	CsdV1 getCsdV1() const
	{
		return CsdV1{csd_};
	}

	/**
	 * \return CSD version 2.0, valid only if CSD_STRUCTURE equals 1
	 */

	CsdV2 getCsdV2() const
	{
		return CsdV2{csd_};
	}

	/**
	 * \return value of ERASE_BLK_EN (erase single block enable) bit field
	 */

	uint8_t getEraseBlkEn() const
	{
		return estd::extractBitField<46, 1>(csd_);
	}

private:

	/// raw CSD data
	RawCsd csd_;
};

/// R1 response errors
enum class R1ResponseErrors : ShortResponse::value_type
{
	/// AKE_SEQ_ERROR
	akeSeqError = 1u << 3,
	/// WP_ERASE_SKIP
	wpEraseSkip = 1u << 15,
	/// CSD_OVERWRITE
	csdOverwrite = 1u << 16,
	/// ERROR
	error = 1u << 19,
	/// CC_ERROR
	ccError = 1u << 20,
	/// CARD_ECC_FAILED
	cardEccFailed = 1u << 21,
	/// ILLEGAL_COMMAND
	illegalCommand = 1u << 22,
	/// COM_CRC_ERROR
	comCrcError = 1u << 23,
	/// LOCK_UNLOCK_FAILED
	lockUnlockFailed = 1u << 24,
	/// WP_VIOLATION
	wpViolation = 1u << 26,
	/// ERASE_PARAM
	eraseParam = 1u << 27,
	/// ERASE_SEQ_ERROR
	eraseSeqError = 1u << 28,
	/// BLOCK_LEN_ERROR
	blockLenError = 1u << 29,
	/// ADDRESS_ERROR
	addressError = 1u << 30,
	/// OUT_OF_RANGE
	outOfRange = 1u << 31,
};

}	// namespace

}	// namespace devices

}	// namespace distortos

namespace estd
{

/// \brief Enable bitwise operators for distortos::devices::R1ResponseErrors
template<>
struct isEnumClassFlags<distortos::devices::R1ResponseErrors> : std::true_type
{

};

}	// namespace estd

namespace distortos
{

namespace devices
{

namespace
{

/// R1 response
class R1Response
{
public:

	/// import R1ResponseErrors
	using Errors = R1ResponseErrors;

	/// type of raw R1 response
	using RawR1Response = ShortResponse;

	R1Response() = default;

	/**
	 * \brief R1Response's constructor
	 *
	 * \param [in] r1 is the raw R1 response data
	 */

	constexpr explicit R1Response(const RawR1Response r1) :
			r1_{r1}
	{

	}

	/**
	 * \return value of CURRENT_STATE bit field
	 */

	CardState getCurrentState() const
	{
		return static_cast<CardState>(estd::extractBitField<9, 4>(r1_));
	}

	/**
	 * \brief Checks whether R1 response contains any error.
	 *
	 * \param [in] ignoredErrors are all errors which should be ignored in the check, default - none
	 *
	 * \return true if R1 response contains any error, false otherwise
	 */

	bool isError(const Errors ignoredErrors = {}) const
	{
		constexpr auto allErrors = Errors::akeSeqError | Errors::wpEraseSkip | Errors::csdOverwrite | Errors::error |
				Errors::ccError | Errors::cardEccFailed | Errors::illegalCommand | Errors::comCrcError |
				Errors::lockUnlockFailed | Errors::wpViolation | Errors::eraseParam | Errors::eraseSeqError |
				Errors::blockLenError | Errors::addressError | Errors::outOfRange;

		return (r1_[0] & static_cast<uint32_t>(allErrors & ~ignoredErrors)) != 0;
	}

private:

	/// raw R1 response data
	RawR1Response r1_;
};

/// R2 response
class R2Response
{
public:

	/// type of raw R2 response
	using RawR2Response = LongResponse;

	/**
	 * \brief R2Response's constructor
	 *
	 * \param [in] r2 is the raw R2 response data
	 */

	constexpr explicit R2Response(const RawR2Response r2) :
			r2_{r2}
	{

	}

	/**
	 * \return CID, valid only if this response contains CID
	 */

	Cid getCid() const
	{
		return Cid{r2_};
	}

	/**
	 * \return CSD, valid only if this response contains CSD
	 */

	Csd getCsd() const
	{
		return Csd{r2_};
	}

private:

	/// raw R2 response data
	RawR2Response r2_;
};

/// R3 response
class R3Response
{
public:

	/// type of raw R3 response
	using RawR3Response = ShortResponse;

	R3Response() = default;

	/**
	 * \brief R3Response's constructor
	 *
	 * \param [in] r3 is the raw R3 response data
	 */

	constexpr explicit R3Response(const RawR3Response r3) :
			r3_{r3}
	{

	}

	/**
	 * \return value of card capacity status (CCS) bit field
	 */

	bool getCardCapacityStatus() const
	{
		return estd::extractBitField<30, 1>(r3_) != 0;
	}

	/**
	 * \return true if card is busy with power up initialization, false otherwise
	 */

	bool isBusy() const
	{
		return estd::extractBitField<31, 1>(r3_) == 0;
	}

private:

	/// raw R3 response data
	RawR3Response r3_;
};

/// R6 response
class R6Response
{
public:

	/// type of raw R6 response
	using RawR6Response = ShortResponse;

	/// card status bits
	class CardStatusBits
	{
	public:

		/// type of raw card status bits data
		using RawCardStatusBits = std::array<uint16_t, 1>;

		/**
		 * \brief CardStatusBits's constructor
		 *
		 * \param [in] cardStatusBits is the raw card status bits data
		 */

		constexpr explicit CardStatusBits(const RawCardStatusBits cardStatusBits) :
				cardStatusBits_{cardStatusBits}
		{

		}

		/**
		 * \return value of CURRENT_STATE bit field
		 */

		CardState getCurrentState() const
		{
			return static_cast<CardState>(estd::extractBitField<9, 4>(cardStatusBits_));
		}

		/**
		 * \return true if card status bits contains any error, false otherwise
		 */

		bool isError() const
		{
			constexpr uint32_t comCrcError {1u << 15};
			constexpr uint32_t illegalCommand {1u << 14};
			constexpr uint32_t error {1u << 13};
			constexpr uint32_t akeSeqError {1u << 3};

			constexpr uint32_t allErrors {comCrcError | illegalCommand | error | akeSeqError};

			return (cardStatusBits_[0] & allErrors) != 0;
		}

	private:

		/// raw card status bits data
		RawCardStatusBits cardStatusBits_;
	};

	R6Response() = default;

	/**
	 * \brief R6Response's constructor
	 *
	 * \param [in] r6 is the raw R6 response data
	 */

	constexpr explicit R6Response(const RawR6Response r6) :
			r6_{r6}
	{

	}

	/**
	 * \return card status bits
	 */

	CardStatusBits getCardStatusBits() const
	{
		return CardStatusBits{CardStatusBits::RawCardStatusBits{estd::extractBitField<0, 16>(r6_)}};
	}

	/**
	 * \return value of relative card address (RCA) bit field
	 */

	uint16_t getRca() const
	{
		return estd::extractBitField<16, 16>(r6_);
	}

private:

	/// raw R6 response data
	RawR6Response r6_;
};

/// R7 response
class R7Response
{
public:

	/// type of raw R7 response
	using RawR7Response = ShortResponse;

	R7Response() = default;

	/**
	 * \brief R7Response's constructor
	 *
	 * \param [in] r7 is the R7 response
	 */

	constexpr explicit R7Response(const RawR7Response r7) :
			r7_{r7}
	{

	}

	/**
	 * \return value of check pattern bit field
	 */

	uint8_t getCheckPattern() const
	{
		return estd::extractBitField<0, 8>(r7_);
	}

	/**
	 * \return value of voltage accepted bit field
	 */

	uint8_t getVoltageAccepted() const
	{
		return estd::extractBitField<8, 4>(r7_);
	}

private:

	/// raw R7 response data
	RawR7Response r7_;
};

/// import SdMmcCardBase::Result as Result
using Result = SdMmcCardBase::Result;

/// SD status
class SdStatus
{
public:

	/// type of raw SD status data
	using RawSdStatus = std::array<uint8_t, 512 / (sizeof(uint8_t) * CHAR_BIT)>;

	SdStatus() = default;

	/**
	 * \brief SdStatus' constructor
	 *
	 * \param [in] sdStatus is the raw SD status data
	 */

	constexpr explicit SdStatus(const RawSdStatus sdStatus) :
			sdStatus_{sdStatus}
	{

	}

	/**
	 * \return value of AU_SIZE (size of AU) bit field
	 */

	uint8_t getAuSize() const
	{
		return estd::extractBitField<428, 4, true>(sdStatus_);
	}

	/**
	 * \return value of ERASE_OFFSET (fixed offset value added to erase time) bit field
	 */

	uint8_t getEraseOffset() const
	{
		return estd::extractBitField<400, 2, true>(sdStatus_);
	}

	/**
	 * \return value of ERASE_SIZE (number of AUs to be erased at a time) bit field
	 */

	uint16_t getEraseSize() const
	{
		return estd::extractBitField<408, 16, true>(sdStatus_);
	}

	/**
	 * \return value of ERASE_TIMEOUT (timeout value for erasing areas specified by UNIT_OF_ERASE_AU) bit field
	 */

	uint8_t getEraseTimeout() const
	{
		return estd::extractBitField<402, 6, true>(sdStatus_);
	}

private:

	/// raw SD status data
	RawSdStatus sdStatus_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Executes transaction with command that has no response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 */

void executeCmdWithoutResponse(SynchronousSdMmcCardLowLevel& sdCard, const uint8_t command, const uint32_t argument)
{
	const auto ret = sdCard.executeTransaction(command, argument, {}, {});
	assert(ret == Result::success);
}

/**
 * \brief Executes transaction with command that has short response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 * \param [in,out] transfer is the transfer associated with transaction
 *
 * \return pair with result of transaction and short response
 */

std::pair<Result, ShortResponse> executeCmdWithShortResponse(SynchronousSdMmcCardLowLevel& sdCard,
		const uint8_t command, const uint32_t argument, const SdMmcCardLowLevel::Transfer transfer)
{
	ShortResponse shortResponse {};
	const auto result = sdCard.executeTransaction(command, argument, SdMmcCardLowLevel::Response{shortResponse},
			transfer);
	return {result, shortResponse};
}

/**
 * \brief Executes transaction with command that has long response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 * \param [in,out] transfer is the transfer associated with transaction
 *
 * \return pair with result of transaction and long response
 */

std::pair<Result, LongResponse> executeCmdWithLongResponse(SynchronousSdMmcCardLowLevel& sdCard,
		const uint8_t command, const uint32_t argument, const SdMmcCardLowLevel::Transfer transfer)
{
	LongResponse longResponse {};
	const auto result = sdCard.executeTransaction(command, argument, SdMmcCardLowLevel::Response{longResponse},
			transfer);
	return {result, longResponse};
}

/**
 * \brief Converts result of transaction to error code.
 *
 * \param [in] result is the result of transaction
 *
 * \return \a result converted to error code
 */

int resultToErrorCode(const Result result)
{
	if (result == Result::success)
		return {};
	if (result == Result::responseTimeout || result == Result::dataTimeout)
		return ETIMEDOUT;

	return EIO;
}

/**
 * \brief Executes transaction with command that has R1 response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 * \param [in,out] transfer is the transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - result of transaction returned by executeCmdWithShortResponse() converted to error code;
 */

std::pair<int, R1Response> executeCmdWithR1Response(SynchronousSdMmcCardLowLevel& sdCard, const uint8_t command,
		const uint32_t argument, const SdMmcCardLowLevel::Transfer transfer)
{
	const auto ret = executeCmdWithShortResponse(sdCard, command, argument, transfer);
	return {resultToErrorCode(ret.first), R1Response{ret.second}};
}

/**
 * \brief Executes transaction with command that has R2 response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 * \param [in,out] transfer is the transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R2 response; error codes:
 * - result of transaction returned by executeCmdWithLongResponse() converted to error code;
 */

std::pair<int, R2Response> executeCmdWithR2Response(SynchronousSdMmcCardLowLevel& sdCard, const uint8_t command,
		const uint32_t argument, const SdMmcCardLowLevel::Transfer transfer)
{
	const auto ret = executeCmdWithLongResponse(sdCard, command, argument, transfer);
	return {resultToErrorCode(ret.first), R2Response{ret.second}};
}

/**
 * \brief Executes transaction with command that has R3 response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 * \param [in,out] transfer is the transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R3 response; error codes:
 * - result of transaction returned by executeCmdWithShortResponse() converted to error code;
 */

std::pair<int, R3Response> executeCmdWithR3Response(SynchronousSdMmcCardLowLevel& sdCard, const uint8_t command,
		const uint32_t argument, const SdMmcCardLowLevel::Transfer transfer)
{
	const auto ret = executeCmdWithShortResponse(sdCard, command, argument, transfer);
	const auto result = ret.first == Result::responseCrcMismatch ? Result::success : ret.first;
	return {resultToErrorCode(result), R3Response{ret.second}};
}

/**
 * \brief Executes transaction with command that has R6 response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 * \param [in,out] transfer is the transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R6 response; error codes:
 * - result of transaction returned by executeCmdWithShortResponse() converted to error code;
 */

std::pair<int, R6Response> executeCmdWithR6Response(SynchronousSdMmcCardLowLevel& sdCard, const uint8_t command,
		const uint32_t argument, const SdMmcCardLowLevel::Transfer transfer)
{
	const auto ret = executeCmdWithShortResponse(sdCard, command, argument, transfer);
	return {resultToErrorCode(ret.first), R6Response{ret.second}};
}

/**
 * \brief Executes transaction with command that has R7 response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 * \param [in,out] transfer is the transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R7 response; error codes:
 * - result of transaction returned by executeCmdWithShortResponse() converted to error code;
 */

std::pair<int, R7Response> executeCmdWithR7Response(SynchronousSdMmcCardLowLevel& sdCard, const uint8_t command,
		const uint32_t argument, const SdMmcCardLowLevel::Transfer transfer)
{
	const auto ret = executeCmdWithShortResponse(sdCard, command, argument, transfer);
	return {resultToErrorCode(ret.first), R7Response{ret.second}};
}

/**
 * \brief Executes CMD0 command on SD card.
 *
 * This is GO_IDLE_STATE command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 */

void executeCmd0(SynchronousSdMmcCardLowLevel& sdCard)
{
	executeCmdWithoutResponse(sdCard, 0, {});
}

/**
 * \brief Executes CMD2 command on SD card.
 *
 * This is ALL_SEND_CID command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 *
 * \return pair with return code (0 on success, error code otherwise) and CID; error codes:
 * - error codes returned by executeCmdWithR2Response();
 */

std::pair<int, Cid> executeCmd2(SynchronousSdMmcCardLowLevel& sdCard)
{
	const auto ret = executeCmdWithR2Response(sdCard, 2, {}, {});
	return {ret.first, ret.second.getCid()};
}

/**
 * \brief Executes CMD3 command on SD card.
 *
 * This is SEND_RELATIVE_ADDR command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 *
 * \return pair with return code (0 on success, error code otherwise) and R6 response; error codes:
 * - error codes returned by executeCmdWithR6Response();
 */

std::pair<int, R6Response> executeCmd3(SynchronousSdMmcCardLowLevel& sdCard)
{
	return executeCmdWithR6Response(sdCard, 3, {}, {});
}

/**
 * \brief Executes CMD7 command on SD card.
 *
 * This is SELECT/DESELECT_CARD command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd7(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca)
{
	return executeCmdWithR1Response(sdCard, 7, static_cast<uint32_t>(rca) << 16, {});
}

/**
 * \brief Executes CMD8 command on SD card.
 *
 * This is SEND_IF_COND command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 *
 * \return pair with return code (0 on success, error code otherwise) and a boolean value which tells whether pattern
 * was matched; error codes:
 * - error codes returned by executeCmdWithR7Response();
 */

std::pair<int, bool> executeCmd8(SynchronousSdMmcCardLowLevel& sdCard)
{
	constexpr uint32_t supplyVoltage {1};	// 2.7 - 3.6 V
	constexpr uint32_t checkPattern {0xaa};

	int ret;
	R7Response r7Response;
	std::tie(ret, r7Response) = executeCmdWithR7Response(sdCard, 8, supplyVoltage << 8 | checkPattern, {});
	const auto match = r7Response.getCheckPattern() == checkPattern && r7Response.getVoltageAccepted() == supplyVoltage;
	return {ret, match};
}

/**
 * \brief Executes CMD9 command on SD card.
 *
 * This is SEND_CSD command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 *
 * \return pair with return code (0 on success, error code otherwise) and CSD; error codes:
 * - error codes returned by executeCmdWithR2Response();
 */

std::pair<int, Csd> executeCmd9(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca)
{
	const auto ret = executeCmdWithR2Response(sdCard, 9, static_cast<uint32_t>(rca) << 16, {});
	return {ret.first, ret.second.getCsd()};
}

/**
 * \brief Executes CMD12 command on SD card.
 *
 * This is STOP_TRANSMISSION command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd12(SynchronousSdMmcCardLowLevel& sdCard)
{
	return executeCmdWithR1Response(sdCard, 12, {}, {});
}

/**
 * \brief Executes CMD13 command on SD card.
 *
 * This is SEND_STATUS command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd13(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca)
{
	return executeCmdWithR1Response(sdCard, 13, static_cast<uint32_t>(rca) << 16, {});
}

/**
 * \brief Executes CMD16 command on SD card.
 *
 * This is SET_BLOCKLEN command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] blockLength is the length of read/write block, bytes
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd16(SynchronousSdMmcCardLowLevel& sdCard, const uint32_t blockLength)
{
	return executeCmdWithR1Response(sdCard, 16, blockLength, {});
}

/**
 * \brief Executes CMD17 command on SD card.
 *
 * This is READ_SINGLE_BLOCK command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] address is the address from which data will be read, bytes or blocks
 * \param [in,out] readTransfer is the read transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd17(SynchronousSdMmcCardLowLevel& sdCard, const uint32_t address,
		const SdMmcCardLowLevel::ReadTransfer readTransfer)
{
	return executeCmdWithR1Response(sdCard, 17, address, readTransfer);
}

/**
 * \brief Executes CMD18 command on SD card.
 *
 * This is READ_MULTIPLE_BLOCK command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] address is the address from which data will be read, bytes or blocks
 * \param [in,out] readTransfer is the read transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd18(SynchronousSdMmcCardLowLevel& sdCard, const uint32_t address,
		const SdMmcCardLowLevel::ReadTransfer readTransfer)
{
	return executeCmdWithR1Response(sdCard, 18, address, readTransfer);
}

/**
 * \brief Executes CMD24 command on SD card.
 *
 * This is WRITE_BLOCK command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] address is the address to which data will be written, bytes or blocks
 * \param [in] writeTransfer is the write transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd24(SynchronousSdMmcCardLowLevel& sdCard, const uint32_t address,
		const SdMmcCardLowLevel::WriteTransfer writeTransfer)
{
	return executeCmdWithR1Response(sdCard, 24, address, writeTransfer);
}

/**
 * \brief Executes CMD25 command on SD card.
 *
 * This is WRITE_MULTIPLE_BLOCK command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] address is the address to which data will be written, bytes or blocks
 * \param [in] writeTransfer is the write transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd25(SynchronousSdMmcCardLowLevel& sdCard, const uint32_t address,
		const SdMmcCardLowLevel::WriteTransfer writeTransfer)
{
	return executeCmdWithR1Response(sdCard, 25, address, writeTransfer);
}

/**
 * \brief Executes CMD32 command on SD card.
 *
 * This is ERASE_WR_BLK_START_ADDR command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] address is the address of first block marked for erase, bytes or blocks
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd32(SynchronousSdMmcCardLowLevel& sdCard, const uint32_t address)
{
	return executeCmdWithR1Response(sdCard, 32, address, {});
}

/**
 * \brief Executes CMD33 command on SD card.
 *
 * This is ERASE_WR_BLK_END_ADDR command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] address is the address of last block marked for erase, bytes or blocks
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd33(SynchronousSdMmcCardLowLevel& sdCard, const uint32_t address)
{
	return executeCmdWithR1Response(sdCard, 33, address, {});
}

/**
 * \brief Executes CMD38 command on SD card.
 *
 * This is ERASE command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd38(SynchronousSdMmcCardLowLevel& sdCard)
{
	return executeCmdWithR1Response(sdCard, 38, {}, {});
}

/**
 * \brief Executes CMD55 command on SD card.
 *
 * This is APP_CMD command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeCmd55(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca)
{
	return executeCmdWithR1Response(sdCard, 55, static_cast<uint32_t>(rca) << 16, {});
}

/**
 * \brief Executes transaction with application command (ACMD) that has R1 response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 * \param [in,out] transfer is the transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - EIO - R1 response for CMD55 indicates an error;
 * - error codes returned by executeCmd55();
 * - error codes returned by executeCmdWithR1Response();
 */

std::pair<int, R1Response> executeAcmdWithR1Response(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca,
		const uint8_t command, const uint32_t argument, const SdMmcCardLowLevel::Transfer transfer)
{
	{
		int ret;
		R1Response r1Response;
		std::tie(ret, r1Response) = executeCmd55(sdCard, rca);
		if (ret != 0)
			return {ret, {}};
		if (r1Response.isError() == true)
			return {EIO, {}};
	}

	return executeCmdWithR1Response(sdCard, command, argument, transfer);
}

/**
 * \brief Executes transaction with application command (ACMD) that has R3 response.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 * \param [in] command is the command that will be executed, [0; SdMmcCardLowLevel::maxCommand]
 * \param [in] argument is the argument for \a command
 * \param [in,out] transfer is the transfer associated with transaction
 *
 * \return pair with return code (0 on success, error code otherwise) and R3 response; error codes:
 * - EIO - R1 response for CMD55 indicates an error;
 * - error codes returned by executeCmd55();
 * - error codes returned by executeCmdWithR3Response();
 */

std::pair<int, R3Response> executeAcmdWithR3Response(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca,
		const uint8_t command, const uint32_t argument, const SdMmcCardLowLevel::Transfer transfer)
{
	{
		int ret;
		R1Response r1Response;
		std::tie(ret, r1Response) = executeCmd55(sdCard, rca);
		if (ret != 0)
			return {ret, {}};
		if (r1Response.isError() == true)
			return {EIO, {}};
	}

	return executeCmdWithR3Response(sdCard, command, argument, transfer);
}

/**
 * \brief Executes ACMD6 command on SD card.
 *
 * This is SET_BUS_WIDTH command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 * \param [in] _4BitBusMode selects whether 1-bit (false) or 4-bit (true) bus mode will be selected
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeAcmdWithR1Response();
 */

std::pair<int, R1Response> executeAcmd6(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca,
		const bool _4BitBusMode)
{
	return executeAcmdWithR1Response(sdCard, rca, 6, static_cast<uint32_t>(_4BitBusMode) << 1, {});
}

/**
 * \brief Executes ACMD13 command on SD card.
 *
 * This is SD_STATUS command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 * \param [in] timeoutMs is the timeout of read transfer, milliseconds
 *
 * \return tuple with return code (0 on success, error code otherwise), R1 response and SD status; error codes:
 * - error codes returned by executeAcmdWithR1Response();
 */

std::tuple<int, R1Response, SdStatus> executeAcmd13(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca,
		const uint16_t timeoutMs)
{
	SdStatus::RawSdStatus sdStatus __attribute__((aligned(DISTORTOS_SDMMCCARD_BUFFER_ALIGNMENT))) {};
	const auto ret = executeAcmdWithR1Response(sdCard, rca, 13, {},
			SdMmcCardLowLevel::ReadTransfer{sdStatus.data(), sizeof(sdStatus), sizeof(sdStatus), timeoutMs});
	return std::make_tuple(ret.first, ret.second, SdStatus{sdStatus});
}

/**
 * \brief Executes ACMD23 command on SD card.
 *
 * This is SET_WR_BLK_ERASE_COUNT command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 * \param [in] blocksCount is the number of blocks to pre-erase with next multi-block write command
 *
 * \return pair with return code (0 on success, error code otherwise) and R1 response; error codes:
 * - error codes returned by executeAcmdWithR1Response();
 */

std::pair<int, R1Response> executeAcmd23(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca,
		const uint32_t blocksCount)
{
	return executeAcmdWithR1Response(sdCard, rca, 23, blocksCount, {});
}

/**
 * \brief Executes ACMD41 command on SD card.
 *
 * This is SD_SEND_OP_COND command.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 * \param [in] hcs is the value of HCS (host capacity support) bit, which selects whether host supports SDHC and SDXC
 * cards
 * \param [in] xpc is the value of XPC (SDXC power control) bit, which selects whether card should operate in power
 * saving (false) or maximum performance (true) mode
 * \param [in] s18r is the value of S18R (switching to 1.8 V request) bit, which selects whether current signal voltage
 * (false) or 1.8 V signal voltage should be used
 * \param [in] vddVoltageWindow is the value of VDD voltage window field
 *
 * \return pair with return code (0 on success, error code otherwise) and R3 response; error codes:
 * - error codes returned by executeAcmdWithR3Response();
 */

std::pair<int, R3Response> executeAcmd41(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca, const bool hcs,
		const bool xpc, const bool s18r, const uint32_t vddVoltageWindow)
{
	return executeAcmdWithR3Response(sdCard, rca, 41, static_cast<uint32_t>(hcs) << 30 |
			static_cast<uint32_t>(xpc) << 28 | static_cast<uint32_t>(s18r) << 24 | (vddVoltageWindow & 0xffffff), {});
}

/**
 * \brief Waits until card goes back to transfer (tran) state.
 *
 * \param [in] sdCard is a reference to synchronous low-level SD/MMC card driver
 * \param [in] rca is the relative card address
 * \param [in] timePoint is the time point at which the wait will be terminated
 *
 * \return 0 on success, error code otherwise:
 * - EIO - R1 response indicates an error;
 * - EIO - card is not in sending data (data), receive data (rcv) or programming (prg) state, transfer (tran) state
 * cannot be reached automatically;
 * - ETIMEDOUT - card did not went back to transfer (tran) state before the specified timeout expired;
 * - error codes returned by executeCmd13();
 */

int waitForTransferState(SynchronousSdMmcCardLowLevel& sdCard, const uint16_t rca,
		const TickClock::time_point timePoint)
{
	while (1)
	{
		int ret;
		R1Response r1Response;
		std::tie(ret, r1Response) = executeCmd13(sdCard, rca);
		if (ret != 0)
			return ret;
		if (r1Response.isError() == true)
			return EIO;

		const auto currentState = r1Response.getCurrentState();
		if (currentState == CardState::transfer)
			return {};
		if (currentState != CardState::sendingData && currentState != CardState::receiveData &&
				currentState != CardState::programming)
			return EIO;
		if (timePoint <= TickClock::now())
			return ETIMEDOUT;

		ThisThread::sleepFor({});
	}
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SdCard::~SdCard()
{
	assert(openCount_ == 0);
}

int SdCard::close()
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);

	int ret {};
	if (openCount_ == 1)	// last close?
	{
		ret = waitForTransferState(sdCard_, rca_, busyDeadline_);
		sdCard_.stop();
		deinitialize();
	}

	--openCount_;
	return ret;
}

int SdCard::erase(const uint64_t address, const uint64_t size)
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);
	assert(address % blockSize == 0 && size % blockSize == 0);

	const auto firstBlock = address / blockSize;
	const auto blocks = size / blockSize;
	assert(firstBlock + blocks <= blocksCount_);

	if (size == 0)
		return {};

	uint64_t erased {};
	while (erased < size)
	{
		{
			const auto ret = waitForTransferState(sdCard_, rca_, busyDeadline_);
			if (ret != 0)
				return ret;
		}

		const auto beginAddress = address + erased;
		// erase no more than 1 AU at a time
		const auto endAddress = std::min(address + size, beginAddress / auSize_ * auSize_ + auSize_);

		{
			int ret;
			R1Response r1Response;
			const auto commandAddress = blockAddressing_ == true ? beginAddress / blockSize : beginAddress;
			std::tie(ret, r1Response) = executeCmd32(sdCard_, commandAddress);
			if (ret != 0)
				return ret;
			if (r1Response.isError() == true)
				return EIO;
		}
		{
			int ret;
			R1Response r1Response;
			const auto commandAddress = blockAddressing_ == true ? (endAddress - blockSize) / blockSize :
					(endAddress - blockSize);
			std::tie(ret, r1Response) = executeCmd33(sdCard_, commandAddress);
			if (ret != 0)
				return ret;
			if (r1Response.isError() == true)
				return EIO;
		}
		{
			int ret;
			R1Response r1Response;
			std::tie(ret, r1Response) = executeCmd38(sdCard_);
			if (ret != 0)
				return ret;
			if (r1Response.isError() == true)
				return EIO;
		}

		erased += endAddress - beginAddress;

		const auto beginPartial = beginAddress % auSize_ != 0;
		const auto endPartial = endAddress % auSize_ != 0;
		busyDeadline_ = TickClock::now() + std::chrono::milliseconds{eraseTimeoutMs_} +
				std::chrono::milliseconds{250} * (beginPartial + endPartial);
	}

	return {};
}

size_t SdCard::getBlockSize() const
{
	return blockSize;
}

uint64_t SdCard::getSize() const
{
	return static_cast<decltype(getSize())>(blockSize) * blocksCount_;
}

void SdCard::lock()
{
	const auto ret = mutex_.lock();
	assert(ret == 0);
}

int SdCard::open()
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ < std::numeric_limits<decltype(openCount_)>::max());

	if (openCount_ == 0)	// first open?
	{
		const auto ret = sdCard_.start();
		if (ret != 0)
			return ret;
	}

	++openCount_;

	if (openCount_ > 1)
		return {};

	auto closeScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				sdCard_.stop();
				deinitialize();
				openCount_ = {};
			});

	{
		decltype(initialize()) ret;
		unsigned int attempt {};
		while (ret = initialize(), ret != 0)
			if (++attempt >= 100 || ret == ENOTSUP)
				return ret;
	}

	closeScopeGuard.release();
	return {};
}

int SdCard::read(const uint64_t address, void* const buffer, const size_t size)
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);
	assert(buffer != nullptr && address % blockSize == 0 && size % blockSize == 0);

	const auto firstBlock = address / blockSize;
	const auto blocks = size / blockSize;
	assert(firstBlock + blocks <= blocksCount_);

	if (size == 0)
		return {};

	{
		const auto ret = waitForTransferState(sdCard_, rca_, busyDeadline_);
		if (ret != 0)
			return ret;
	}

	int ret;

	{
		R1Response r1Response;
		const auto commandAddress = blockAddressing_ == true ? firstBlock : address;
		const SdMmcCardLowLevel::ReadTransfer readTransfer {buffer, size, blockSize, readTimeoutMs_};
		std::tie(ret, r1Response) = blocks == 1 ? executeCmd17(sdCard_, commandAddress, readTransfer) :
				executeCmd18(sdCard_, commandAddress, readTransfer);
		if (ret == 0 && r1Response.isError() == true)
			ret = EIO;
	}

	if (blocks != 1 || ret != 0)
	{
		int cmd12Ret;
		R1Response r1Response;
		std::tie(cmd12Ret, r1Response) = executeCmd12(sdCard_);
		if (ret == 0)
		{
			// a multi block read which includes the last block of the card may cause an OUT_OF_RANGE error - ignore it
			if (cmd12Ret != 0)
				ret = cmd12Ret;
			else if (r1Response.isError(R1Response::Errors::outOfRange) == true)
				ret = EIO;
		}
	}

	busyDeadline_ = TickClock::now() + std::chrono::milliseconds{readTimeoutMs_};

	return ret;
}

int SdCard::synchronize()
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);

	return waitForTransferState(sdCard_, rca_, busyDeadline_);
}

void SdCard::unlock()
{
	const auto ret = mutex_.unlock();
	assert(ret == 0);
}

int SdCard::write(const uint64_t address, const void* const buffer, const size_t size)
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(openCount_ != 0);
	assert(buffer != nullptr && address % blockSize == 0 && size % blockSize == 0);

	const auto firstBlock = address / blockSize;
	const auto blocks = size / blockSize;
	assert(firstBlock + blocks <= blocksCount_);

	if (size == 0)
		return {};

	{
		const auto ret = waitForTransferState(sdCard_, rca_, busyDeadline_);
		if (ret != 0)
			return ret;
	}

	if (blocks != 1)
	{
		int ret;
		R1Response r1Response;
		std::tie(ret, r1Response) = executeAcmd23(sdCard_, rca_, blocks);
		if (ret != 0)
			return ret;
		if (r1Response.isError() == true)
			return EIO;
	}

	int ret;

	{
		R1Response r1Response;
		const auto commandAddress = blockAddressing_ == true ? firstBlock : address;
		const SdMmcCardLowLevel::WriteTransfer writeTransfer {buffer, size, blockSize, writeTimeoutMs_};
		std::tie(ret, r1Response) = blocks == 1 ? executeCmd24(sdCard_, commandAddress, writeTransfer) :
				executeCmd25(sdCard_, commandAddress, writeTransfer);
		if (ret == 0 && r1Response.isError() == true)
			ret = EIO;
	}

	if (blocks != 1 || ret != 0)
	{
		int cmd12Ret;
		R1Response r1Response;
		std::tie(cmd12Ret, r1Response) = executeCmd12(sdCard_);
		if (ret == 0)
		{
			if (cmd12Ret != 0)
				ret = cmd12Ret;
			else if (r1Response.isError() == true)
				ret = EIO;
		}
	}

	busyDeadline_ = TickClock::now() + std::chrono::milliseconds{writeTimeoutMs_};

	return ret;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SdCard::deinitialize()
{
	busyDeadline_ = {};
	auSize_ = {};
	blocksCount_ = {};
	eraseTimeoutMs_ = {};
	rca_ = {};
	readTimeoutMs_ = {};
	writeTimeoutMs_ = {};
	blockAddressing_ = {};
}

int SdCard::initialize()
{
	sdCard_.configure(SdMmcCardLowLevel::BusMode::_1Bit, 400000);

	executeCmd0(sdCard_);

	{
		int ret;
		bool match;
		std::tie(ret, match) = executeCmd8(sdCard_);
		if (ret != 0)
			return ret;
		if (match == false)
			return ENOTSUP;	// fatal error, voltage range not supported
	}
	{
		const auto deadline = TickClock::now() + std::chrono::seconds{1};
		while (1)
		{
			int ret;
			R3Response r3Response;
			std::tie(ret, r3Response) = executeAcmd41(sdCard_, {}, true, true, false, 0xff8000);
			if (ret != 0)
				return ret;
			if (r3Response.isBusy() == false)
			{
				if (r3Response.getCardCapacityStatus() == true)
					blockAddressing_ = true;

				break;
			}
			if (TickClock::now() >= deadline)
				return ETIMEDOUT;

			ThisThread::sleepFor({});
		}
	}

	{
		int ret;
		std::tie(ret, std::ignore) = executeCmd2(sdCard_);
		if (ret != 0)
			return ret;
	}

	{
		int ret;
		R6Response r6Response;
		std::tie(ret, r6Response) = executeCmd3(sdCard_);
		if (ret != 0)
			return ret;
		const auto cardStatusBits = r6Response.getCardStatusBits();
		if (cardStatusBits.isError() == true || cardStatusBits.getCurrentState() != CardState::identification)
			return EIO;

		rca_ = r6Response.getRca();
	}

	{
		int ret;
		Csd csd;
		std::tie(ret, csd) = executeCmd9(sdCard_, rca_);
		if (ret != 0)
			return ret;

		const auto csdStructure = csd.getCsdStructure();
		if (csdStructure == 0)	// CSD version 1.0
		{
			const auto csdV1 = csd.getCsdV1();
			blocksCount_ = (csdV1.getCSize() + 1) * (1 << (csdV1.getCSizeMult() + 2)) * (1 << csdV1.getReadBlLen()) /
					blockSize;
			/// \todo compute read and write timeout from CSD contents
			readTimeoutMs_ = 100;
			writeTimeoutMs_ = 250;
		}
		else if (csdStructure == 1)	// CSD version 2.0
		{
			const auto csdV2 = csd.getCsdV2();
			blocksCount_ = (static_cast<uint64_t>(csdV2.getCSize()) + 1) * 512 * 1024 / blockSize;
			readTimeoutMs_ = 100;
			// SDHC (<= 32 GB) - 250 ms, SDXC - 500 ms
			writeTimeoutMs_ = getSize() <= 32ull * 1024 * 1024 * 1024 ? 250 : 500;
		}
		else
			return ENOTSUP;

		if (csd.getEraseBlkEn() == 0)
			return ENOTSUP;	/// \todo add support for cards with ERASE_BLK_EN == 0 (sector erase granularity)
	}

	{
		int ret;
		R1Response r1Response;
		std::tie(ret, r1Response) = executeCmd7(sdCard_, rca_);
		if (ret != 0)
			return ret;
		if (r1Response.isError() == true || r1Response.getCurrentState() != CardState::standby)
			return EIO;
	}

	{
		int ret;
		R1Response r1Response;
		std::tie(ret, r1Response) = executeAcmd6(sdCard_, rca_, _4BitBusMode_);
		if (ret != 0)
			return ret;
		if (r1Response.isError() == true || r1Response.getCurrentState() != CardState::transfer)
			return EIO;
	}

	{
		const auto busMode =
				_4BitBusMode_ == false ? SdMmcCardLowLevel::BusMode::_1Bit : SdMmcCardLowLevel::BusMode::_4Bit;
		sdCard_.configure(busMode, maxClockFrequency_);
	}

	if (blockAddressing_ == false)
	{
		int ret;
		R1Response r1Response;
		std::tie(ret, r1Response) = executeCmd16(sdCard_, blockSize);
		if (ret != 0)
			return ret;
		if (r1Response.isError() == true || r1Response.getCurrentState() != CardState::transfer)
			return EIO;
	}

	{
		int ret;
		R1Response r1Response;
		SdStatus sdStatus;
		std::tie(ret, r1Response, sdStatus) = executeAcmd13(sdCard_, rca_, readTimeoutMs_);
		if (ret != 0)
			return ret;
		if (r1Response.isError() == true || r1Response.getCurrentState() != CardState::transfer)
			return EIO;

		const auto auSize = sdStatus.getAuSize();
		const auto eraseSize = sdStatus.getEraseSize();
		const auto eraseTimeout = sdStatus.getEraseTimeout();
		if (auSize == 0 || eraseSize == 0 || eraseTimeout == 0)
			return ENOTSUP;	/// \todo add support for estimating erase timeout

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
		assert(auSize - 1u < sizeof(auSizeAssociation) / sizeof(*auSizeAssociation));
		auSize_ = auSizeAssociation[auSize - 1u];
		eraseTimeoutMs_ = std::max(eraseTimeout * 1000 / eraseSize + sdStatus.getEraseOffset() * 1000, 1000);
	}

	return {};
}

}	// namespace devices

}	// namespace distortos
