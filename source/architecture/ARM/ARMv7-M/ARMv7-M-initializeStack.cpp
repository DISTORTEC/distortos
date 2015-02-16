/**
 * \file
 * \brief initializeStack() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-16
 */

#include "distortos/architecture/architecture.hpp"

#include <type_traits>

#include <cstdint>

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void* initializeStack(void* const buffer, const size_t size, void (&function)(ThreadBase&), ThreadBase& threadBase)
{
	auto stackPointer = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(buffer) + size);

	using StackElement = std::decay<decltype(*stackPointer)>::type;

	*--stackPointer = 0x01000000;									// xPSR
	*--stackPointer = reinterpret_cast<StackElement>(&function);	// pc
	*--stackPointer = 0;											// lr
	*--stackPointer = 0xcccccccc;									// r12
	*--stackPointer = 0x33333333;									// r3
	*--stackPointer = 0x22222222;									// r2
	*--stackPointer = 0x11111111;									// r1
	*--stackPointer = reinterpret_cast<StackElement>(&threadBase);	// r0
	*--stackPointer = 0xbbbbbbbb;									// r11
	*--stackPointer = 0xaaaaaaaa;									// r10
	*--stackPointer = 0x99999999;									// r9
	*--stackPointer = 0x88888888;									// r8
	*--stackPointer = 0x77777777;									// r7
	*--stackPointer = 0x66666666;									// r6
	*--stackPointer = 0x55555555;									// r5
	*--stackPointer = 0x44444444;									// r4

	return stackPointer;
}

}	// namespace architecture

}	// namespace distortos
