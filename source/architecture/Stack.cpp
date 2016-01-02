/**
 * \file
 * \brief Stack class implementation
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2016-01-02
 */

#include "distortos/architecture/Stack.hpp"

#include "distortos/architecture/initializeStack.hpp"
#include "distortos/architecture/parameters.hpp"

#include "distortos/internal/memory/dummyDeleter.hpp"

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
 * \brief Adjusts storage's address to suit architecture's alignment requirements.
 *
 * \param [in] storage is a pointer to stack's storage
 * \param [in] stackAlignment is required stack alignment
 *
 * \return adjusted storage's address
 */

void* adjustStorage(void* const storage, const size_t stackAlignment)
{
	const auto storageSizeT = reinterpret_cast<size_t>(storage);
	const auto offset = (-storageSizeT) & (stackAlignment - 1);
	return reinterpret_cast<void*>(storageSizeT + offset);
}

/**
 * \brief Adjusts storage's size to suit architecture's divisibility requirements,
 *
 * \param [in] storage is a pointer to stack's storage
 * \param [in] size is the size of stack's storage, bytes
 * \param [in] adjustedStorage is an adjusted storage's address
 * \param [in] divisibility is required stack's size divisibility
 *
 * \return adjusted storage's size
 */

size_t adjustSize(void* const storage, const size_t size, void* const adjustedStorage, const size_t divisibility)
{
	const auto offset = static_cast<uint8_t*>(adjustedStorage) - static_cast<uint8_t*>(storage);
	return ((size - offset) / divisibility) * divisibility;
}

/**
 * \brief Proxy for initializeStack() which fills stack with 0 before actually initializing it.
 *
 * \param [in] storage is a pointer to stack's storage
 * \param [in] size is the size of stack's storage, bytes
 * \param [in] thread is a reference to Thread object passed to function
 * \param [in] run is a reference to Thread's "run" function
 * \param [in] preTerminationHook is a pointer to Thread's pre-termination hook, nullptr to skip
 * \param [in] terminationHook is a reference to Thread's termination hook
 *
 * \return value that can be used as thread's stack pointer, ready for context switching
 */

void* initializeStackProxy(void* const storage, const size_t size, Thread& thread, void (& run)(Thread&),
		void (* preTerminationHook)(Thread&), void (& terminationHook)(Thread&))
{
	memset(storage, 0, size);
	return initializeStack(storage, size, thread, run, preTerminationHook, terminationHook);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Stack::Stack(StorageUniquePointer&& storageUniquePointer, const size_t size, Thread& thread, void (& run)(Thread&),
		void (* preTerminationHook)(Thread&), void (& terminationHook)(Thread&)) :
				storageUniquePointer_{std::move(storageUniquePointer)},
				adjustedStorage_{adjustStorage(storageUniquePointer_.get(), stackAlignment)},
				adjustedSize_{adjustSize(storageUniquePointer_.get(), size, adjustedStorage_, stackSizeDivisibility)},
				stackPointer_{initializeStackProxy(adjustedStorage_, adjustedSize_, thread, run, preTerminationHook,
						terminationHook)}
{
	/// \todo implement minimal size check
}

Stack::Stack(void* const storage, const size_t size) :
		storageUniquePointer_{storage, internal::dummyDeleter<void*>},
		adjustedStorage_{storage},
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
