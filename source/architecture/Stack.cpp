/**
 * \file
 * \brief Stack class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-27
 */

#include "distortos/architecture/Stack.hpp"

#include "distortos/architecture/initializeStack.hpp"
#include "distortos/architecture/parameters.hpp"

#include "distortos/memory/dummyDeleter.hpp"

#include <cstring>

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
 * \param [in] stackAlignment is required stack alignment
 *
 * \return adjusted buffer's address
 */

void* adjustBuffer(void* const buffer, const size_t stackAlignment)
{
	const auto bufferSizeT = reinterpret_cast<size_t>(buffer);
	const auto offset = (-bufferSizeT) & (stackAlignment - 1);
	return reinterpret_cast<void*>(bufferSizeT + offset);
}

/**
 * \brief Adjusts buffer's size to suit architecture's divisibility requirements,
 *
 * \param [in] buffer is a pointer to stack's buffer
 * \param [in] size is the size of stack's buffer, bytes
 * \param [in] adjustedBuffer is an adjusted buffer's address
 * \param [in] divisibility is required stack's size divisibility
 *
 * \return adjusted buffer's size
 */

size_t adjustSize(void* const buffer, const size_t size, void* const adjustedBuffer, const size_t divisibility)
{
	const auto offset = static_cast<uint8_t*>(adjustedBuffer) - static_cast<uint8_t*>(buffer);
	return ((size - offset) / divisibility) * divisibility;
}

/**
 * \brief Proxy for initializeStack() which fills stack with 0 before actually initializing it.
 *
 * \param [in] buffer is a pointer to stack's buffer
 * \param [in] size is the size of stack's buffer, bytes
 * \param [in] function is a reference to thread's function, this function must not return
 * \param [in] threadBase is a reference to ThreadBase object passed to function
 *
 * \return value that can be used as thread's stack pointer, ready for context switching
 */

void* initializeStackProxy(void* const buffer, const size_t size, void (& function)(ThreadBase&),
		ThreadBase& threadBase)
{
	memset(buffer, 0, size);
	return initializeStack(buffer, size, function, threadBase);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Stack::Stack(void* const buffer, const size_t size, void (& function)(ThreadBase&), ThreadBase& threadBase) :
		storageUniquePointer_{buffer, memory::dummyDeleter<void*>},
		adjustedBuffer_{adjustBuffer(buffer, stackAlignment)},
		adjustedSize_{adjustSize(buffer, size, adjustedBuffer_, stackSizeDivisibility)},
		stackPointer_{initializeStackProxy(adjustedBuffer_, adjustedSize_, function, threadBase)}
{
	/// \todo implement minimal size check
}

Stack::Stack(void* const buffer, const size_t size) :
		storageUniquePointer_{buffer, memory::dummyDeleter<void*>},
		adjustedBuffer_{buffer},
		adjustedSize_{size},
		stackPointer_{}
{
	/// \todo implement minimal size check
}

Stack::~Stack()
{

}

}	// namespace architecture

}	// namespace distortos
