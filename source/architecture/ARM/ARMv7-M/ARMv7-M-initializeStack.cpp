/**
 * \file
 * \brief initializeStack() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-16
 */

#include "distortos/architecture.hpp"

#include <type_traits>

#include <cstdint>

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void* initializeStack(void* const buffer, const size_t size, void (&function)(void*), void* const arguments,
		void (&trap)())
{
	auto stack_pointer = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(buffer) + size);

	using StackElement = std::decay<decltype(*stack_pointer)>::type;

	*--stack_pointer = 0x01000000;										// xPSR
	*--stack_pointer = reinterpret_cast<StackElement>(&function);		// pc
	*--stack_pointer = reinterpret_cast<StackElement>(&trap);			// lr
	*--stack_pointer = 0xcccccccc;										// r12
	*--stack_pointer = 0x33333333;										// r3
	*--stack_pointer = 0x22222222;										// r2
	*--stack_pointer = 0x11111111;										// r1
	*--stack_pointer = reinterpret_cast<StackElement>(arguments);		// r0
	*--stack_pointer = 0xbbbbbbbb;										// r11
	*--stack_pointer = 0xaaaaaaaa;										// r10
	*--stack_pointer = 0x99999999;										// r9
	*--stack_pointer = 0x88888888;										// r8
	*--stack_pointer = 0x77777777;										// r7
	*--stack_pointer = 0x66666666;										// r6
	*--stack_pointer = 0x55555555;										// r5
	*--stack_pointer = 0x44444444;										// r4

	return stack_pointer;
}

}	// namespace architecture

}	// namespace distortos
