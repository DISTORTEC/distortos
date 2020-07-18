/**
 * \file
 * \brief Stack class implementation
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/Stack.hpp"

#include "distortos/architecture/initializeStack.hpp"

#include "distortos/internal/memory/dummyDeleter.hpp"

#include <algorithm>

#if DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT <= 0
#error "Stack alignment must be greater than 0!"
#endif	// DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT <= 0

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// alignment of stack, bytes
constexpr size_t stackAlignment {DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT};

/// sentinel used for stack usage/overflow detection
constexpr uint32_t stackSentinel {0xed419f25};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Adjusts storage's address to suit architecture's alignment requirements.
 *
 * \param [in] storage is a pointer to stack's storage
 * \param [in] alignment is the required stack alignment, bytes
 *
 * \return adjusted storage's address
 */

void* adjustStorage(void* const storage, const size_t alignment)
{
	return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(storage) + alignment - 1) / alignment * alignment);
}

/**
 * \brief Adjusts storage's size to suit architecture's alignment requirements.
 *
 * \param [in] storage is a pointer to stack's storage
 * \param [in] size is the size of stack's storage, bytes
 * \param [in] adjustedStorage is an adjusted storage's address
 * \param [in] alignment is the required stack alignment, bytes
 *
 * \return adjusted storage's size
 */

size_t adjustSize(void* const storage, const size_t size, void* const adjustedStorage, const size_t alignment)
{
	const auto storageEnd = reinterpret_cast<uintptr_t>(storage) + size;
	const auto adjustedStorageEnd = storageEnd / alignment * alignment;
	return adjustedStorageEnd - reinterpret_cast<uintptr_t>(adjustedStorage);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Stack::Stack(StorageUniquePointer&& storageUniquePointer, const size_t size) :
		storageUniquePointer_{std::move(storageUniquePointer)},
		adjustedStorage_{adjustStorage(storageUniquePointer_.get(), stackAlignment)},
		adjustedSize_{adjustSize(storageUniquePointer_.get(), size, adjustedStorage_, stackAlignment)},
		stackPointer_{}
{

}

Stack::Stack(void* const storage, const size_t size) :
		storageUniquePointer_{storage, dummyDeleter<void*>},
		adjustedStorage_{storage},
		adjustedSize_{size},
		stackPointer_{}
{
	/// \todo implement minimal size check
}

Stack::~Stack()
{

}

bool Stack::checkStackGuard() const
{
	return std::all_of(static_cast<decltype(&stackSentinel)>(adjustedStorage_),
			static_cast<decltype(&stackSentinel)>(adjustedStorage_) + stackGuardSize / sizeof(stackSentinel),
			[](decltype(stackSentinel)& element) -> bool
			{
				return element == stackSentinel;
			});
}

size_t Stack::getHighWaterMark() const
{
	const auto begin =
			static_cast<decltype(&stackSentinel)>(adjustedStorage_) + stackGuardSize / sizeof(stackSentinel);
	const auto end = static_cast<decltype(&stackSentinel)>(adjustedStorage_) + adjustedSize_ / sizeof(stackSentinel);
	const auto usedElement = std::find_if_not(begin, end,
			[](decltype(stackSentinel)& element) -> bool
			{
				return element == stackSentinel;
			});
	return (end - usedElement) * sizeof(*begin);
}

int Stack::initialize(RunnableThread& runnableThread)
{
	std::fill_n(static_cast<std::decay<decltype(stackSentinel)>::type*>(adjustedStorage_),
			adjustedSize_ / sizeof(stackSentinel), stackSentinel);
	int ret;
	std::tie(ret, stackPointer_) =
			architecture::initializeStack(static_cast<uint8_t*>(adjustedStorage_) + stackGuardSize, getSize(),
					runnableThread);
	return ret;
}

}	// namespace internal

}	// namespace distortos
