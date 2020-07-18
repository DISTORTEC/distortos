/**
 * \file
 * \brief Stack class header
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_STACK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_STACK_HPP_

#include "distortos/internal/scheduler/stackGuardSize.hpp"

#include <memory>

#if defined(DISTORTOS_ARCHITECTURE_ASCENDING_STACK) || defined(DISTORTOS_ARCHITECTURE_EMPTY_STACK)
#error "Support for \"empty ascending\", \"empty descending\" or \"full ascending\" stack not implemented!"
#endif	// defined(DISTORTOS_ARCHITECTURE_ASCENDING_STACK) || defined(DISTORTOS_ARCHITECTURE_EMPTY_STACK)

namespace distortos
{

namespace internal
{

class RunnableThread;

/// Stack class is an abstraction of architecture's stack
class Stack
{
public:

	/// unique_ptr (with deleter) to storage
	using StorageUniquePointer = std::unique_ptr<void, void(&)(void*)>;

	/**
	 * \brief Stack's constructor
	 *
	 * Storage's address and size are adjusted to suit architecture's alignment requirements.
	 *
	 * \param [in] storageUniquePointer is a rvalue reference to StorageUniquePointer with storage for stack (\a size
	 * bytes long) and appropriate deleter
	 * \param [in] size is the size of stack's storage, bytes
	 */

	Stack(StorageUniquePointer&& storageUniquePointer, size_t size);

	/**
	 * \brief Stack's constructor
	 *
	 * This function adopts existing valid architecture-specific stack in provided storage. No adjustments are done,
	 * no stack frame is created and stack pointer register's value is not calculated.
	 *
	 * This is meant to adopt main()'s stack.
	 *
	 * \param [in] storage is a pointer to stack's storage
	 * \param [in] size is the size of stack's storage, bytes
	 */

	Stack(void* storage, size_t size);

	/**
	 * \brief Stack's destructor
	 */

	~Stack();

	/**
	 * \return true if "stack guard" contains unmodified sentinel values, false if stack overflowed into "stack guard"
	 */

	bool checkStackGuard() const;

	/**
	 * \brief Checks whether stack pointer value is within range of this stack.
	 *
	 * \note The "stack guard" area is not a valid range for stack pointer.
	 *
	 * \param [in] stackPointer is the value of stack pointer that will be checked
	 *
	 * \return true if \a stackPointer is within range of this stack, false otherwise
	 */

	bool checkStackPointer(const void* const stackPointer) const
	{
		return stackPointer >= static_cast<uint8_t*>(adjustedStorage_) + stackGuardSize &&
				stackPointer <= static_cast<uint8_t*>(adjustedStorage_) + adjustedSize_;
	}

	/**
	 * \return stack's "high water mark" (max usage), excluding "stack guard", bytes
	 */

	size_t getHighWaterMark() const;

	/**
	 * \return adjusted size of stack's storage, excluding "stack guard", bytes
	 */

	size_t getSize() const
	{
		return adjustedSize_ > stackGuardSize ? adjustedSize_ - stackGuardSize : 0;
	}

	/**
	 * \brief Gets current value of stack pointer.
	 *
	 * \return current value of stack pointer
	 */

	void* getStackPointer() const
	{
		return stackPointer_;
	}

	/**
	 * \brief Fills the stack with stack sentinel, initializes its contents and stack pointer value.
	 *
	 * \param [in] runnableThread is a reference to RunnableThread object that is being run
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by architecture::initializeStack();
	 */

	int initialize(RunnableThread& runnableThread);

	/**
	 * \brief Sets value of stack pointer.
	 *
	 * \param [in] stackPointer is the new value of stack pointer
	 */

	void setStackPointer(void* const stackPointer)
	{
		stackPointer_ = stackPointer;
	}

	Stack(const Stack&) = delete;
	Stack(Stack&&) = default;
	const Stack& operator=(const Stack&) = delete;
	Stack& operator=(Stack&&) = delete;

private:

	/// storage for stack
	StorageUniquePointer storageUniquePointer_;

	/// adjusted address of stack's storage
	void* const adjustedStorage_;

	/// adjusted size of stack's storage
	const size_t adjustedSize_;

	/// current value of stack pointer register
	void* stackPointer_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_STACK_HPP_
