/**
 * \file
 * \brief Stack class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-15
 */

#include "distortos/architecture/Stack.hpp"

#include "distortos/architecture.hpp"

#include <cstdint>

namespace distortos
{

namespace architecture
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Adjusts buffer's address to suit architecture's alignment requirements.
 *
 * \param [in] buffer is a pointer to stack's buffer
 * \param [in] stack_alignment is required stack alignment
 *
 * \return adjusted buffer's address
 */

void* adjustBuffer_(void* const buffer, const size_t stack_alignment)
{
	const auto buffer_size_t = reinterpret_cast<size_t>(buffer);
	const auto offset = (-buffer_size_t) & (stack_alignment - 1);
	return reinterpret_cast<void*>(buffer_size_t + offset);
}

/**
 * \brief Adjusts buffer's size to suit architecture's divisibility requirements,
 *
 * \param [in] buffer is a pointer to stack's buffer
 * \param [in] size is the size of stack's buffer, bytes
 * \param [in] adjusted_buffer is an adjusted buffer's address
 * \param [in] divisibility is required stack's size divisibility
 *
 * \return adjusted buffer's size
 */

size_t adjustSize_(void* const buffer, const size_t size, void* const adjusted_buffer, const size_t divisibility)
{
	const auto offset = static_cast<uint8_t*>(adjusted_buffer) - static_cast<uint8_t*>(buffer);
	return ((size - offset) / divisibility) * divisibility;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Stack::Stack(void* const buffer, const size_t size, void (&function)(void*), void* const arguments,
		void (&trap)(void*)) :
		adjustedBuffer_{adjustBuffer_(buffer, stackAlignment)},
		adjustedSize_{adjustSize_(buffer, size, adjustedBuffer_, stackSizeDivisibility)},
		stackPointer_{initializeStack(adjustedBuffer_, adjustedSize_, function, arguments, trap)}
{
	/// \todo implement minimal size check
}

}	// namespace architecture

}	// namespace distortos
