/**
 * \file
 * \brief Stack class header
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_STACK_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_STACK_HPP_

#include <memory>

namespace distortos
{

class Thread;

namespace architecture
{

/// Stack class is an abstraction of architecture's stack
class Stack
{
public:

	/// unique_ptr (with deleter) to storage
	using StorageUniquePointer = std::unique_ptr<void, void(&)(void*)>;

	/**
	 * \brief Stack's constructor
	 *
	 * This function initializes valid architecture-specific stack in provided storage. This requires following steps:
	 * - adjustment of storage's address to suit architecture's alignment requirements,
	 * - adjustment of storage's size to suit architecture's divisibility requirements,
	 * - creating hardware and software stack frame in suitable place in the stack,
	 * - calculation of stack pointer register value.
	 *
	 * After this constructor completes, stack is ready for context switching.
	 *
	 * \param [in] storageUniquePointer is a rvalue reference to StorageUniquePointer with storage for stack (\a size
	 * bytes long) and appropriate deleter
	 * \param [in] size is the size of stack's storage, bytes
	 * \param [in] thread is a reference to Thread object passed to function
	 * \param [in] run is a reference to Thread's "run" function
	 * \param [in] preTerminationHook is a pointer to Thread's pre-termination hook, nullptr to skip
	 * \param [in] terminationHook is a reference to Thread's termination hook
	 */

	Stack(StorageUniquePointer&& storageUniquePointer, size_t size, Thread& thread, void (& run)(Thread&),
			void (* preTerminationHook)(Thread&), void (& terminationHook)(Thread&));

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
	 * \brief Gets current value of stack pointer.
	 *
	 * \return current value of stack pointer
	 */

	void* getStackPointer() const
	{
		return stackPointer_;
	}

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

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_STACK_HPP_
