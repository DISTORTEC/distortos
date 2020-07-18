/**
 * \file
 * \brief Declaration of unique device ID in ST,NUCLEO-L476RG (ST,STM32L476RG chip)
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#ifndef SOURCE_BOARD_ST_NUCLEO_L476RG_INCLUDE_DISTORTOS_CHIP_UNIQUEDEVICEID_HPP_
#define SOURCE_BOARD_ST_NUCLEO_L476RG_INCLUDE_DISTORTOS_CHIP_UNIQUEDEVICEID_HPP_

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/// 96-bit unique device ID
union UniqueDeviceId
{
	/// value of 96-bit unique device ID as uint8_t
	uint8_t uint8[12];

	/// value of 96-bit unique device ID as uint16_t
	uint16_t uint16[6];

	/// value of 96-bit unique device ID as uint32_t
	uint32_t uint32[3];

	struct
	{
		/// value of 96-bit unique device ID as uint8_t, byte 0
		uint8_t uint8_0;

		/// value of 96-bit unique device ID as uint8_t, byte 1
		uint8_t uint8_1;

		/// value of 96-bit unique device ID as uint8_t, byte 2
		uint8_t uint8_2;

		/// value of 96-bit unique device ID as uint8_t, byte 3
		uint8_t uint8_3;

		/// value of 96-bit unique device ID as uint8_t, byte 4
		uint8_t uint8_4;

		/// value of 96-bit unique device ID as uint8_t, byte 5
		uint8_t uint8_5;

		/// value of 96-bit unique device ID as uint8_t, byte 6
		uint8_t uint8_6;

		/// value of 96-bit unique device ID as uint8_t, byte 7
		uint8_t uint8_7;

		/// value of 96-bit unique device ID as uint8_t, byte 8
		uint8_t uint8_8;

		/// value of 96-bit unique device ID as uint8_t, byte 9
		uint8_t uint8_9;

		/// value of 96-bit unique device ID as uint8_t, byte 10
		uint8_t uint8_10;

		/// value of 96-bit unique device ID as uint8_t, byte 11
		uint8_t uint8_11;
	};

	struct
	{
		/// value of 96-bit unique device ID as uint16_t, half-word 0
		uint16_t uint16_0;

		/// value of 96-bit unique device ID as uint16_t, half-word 1
		uint16_t uint16_1;

		/// value of 96-bit unique device ID as uint16_t, half-word 2
		uint16_t uint16_2;

		/// value of 96-bit unique device ID as uint16_t, half-word 3
		uint16_t uint16_3;

		/// value of 96-bit unique device ID as uint16_t, half-word 4
		uint16_t uint16_4;

		/// value of 96-bit unique device ID as uint16_t, half-word 5
		uint16_t uint16_5;
	};

	struct
	{
		/// value of 96-bit unique device ID as uint32_t, word 0
		uint32_t uint32_0;

		/// value of 96-bit unique device ID as uint32_t, word 1
		uint32_t uint32_1;

		/// value of 96-bit unique device ID as uint32_t, word 2
		uint32_t uint32_2;
	};
};

/// pointer to 96-bit unique device ID
static const UniqueDeviceId* const uniqueDeviceId {reinterpret_cast<const UniqueDeviceId*>(UID_BASE)};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_BOARD_ST_NUCLEO_L476RG_INCLUDE_DISTORTOS_CHIP_UNIQUEDEVICEID_HPP_
