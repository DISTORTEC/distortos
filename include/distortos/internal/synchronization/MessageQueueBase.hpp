/**
 * \file
 * \brief MessageQueueBase class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MESSAGEQUEUEBASE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MESSAGEQUEUEBASE_HPP_

#include "distortos/Semaphore.hpp"

#include "distortos/internal/synchronization/QueueFunctor.hpp"
#include "distortos/internal/synchronization/SemaphoreFunctor.hpp"

#include "estd/SortedIntrusiveForwardList.hpp"

#include <memory>

namespace distortos
{

namespace internal
{

/// MessageQueueBase class implements basic functionality of MessageQueue template class
class MessageQueueBase
{
public:

	/// entry in the MessageQueueBase
	struct Entry
	{
		/**
		 * \brief Entry's constructor
		 *
		 * \param [in] priorityy is the priority of the entry
		 * \param [in] storagee is the storage for the entry
		 */

		constexpr Entry(const uint8_t priorityy, void* const storagee) :
				node{},
				priority{priorityy},
				storage{storagee}
		{

		}

		/// node for intrusive forward list
		estd::IntrusiveForwardListNode node;

		/// priority of the entry
		uint8_t priority;

		/// storage for the entry
		void* storage;
	};

	/// type of uninitialized storage for Entry
	using EntryStorage = typename std::aligned_storage<sizeof(Entry), alignof(Entry)>::type;

	/// unique_ptr (with deleter) to EntryStorage[]
	using EntryStorageUniquePointer = std::unique_ptr<EntryStorage[], void(&)(EntryStorage*)>;

	/**
	 * \brief type of uninitialized storage for value
	 *
	 * \tparam T is the type of data in queue
	 */

	template<typename T>
	using ValueStorage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

	/// unique_ptr (with deleter) to storage
	using ValueStorageUniquePointer = std::unique_ptr<void, void(&)(void*)>;

	/// functor which gives descending priority order of elements on the list
	struct DescendingPriority
	{
		/**
		 * \brief DescendingPriority's constructor
		 */

		constexpr DescendingPriority()
		{

		}

		/**
		 * \brief DescendingPriority's function call operator
		 *
		 * \param [in] left is the object on the left side of comparison
		 * \param [in] right is the object on the right side of comparison
		 *
		 * \return true if left's priority is less than right's priority
		 */

		bool operator()(const Entry& left, const Entry& right) const
		{
			return left.priority < right.priority;
		}
	};

	/// type of entry list
	using EntryList = estd::SortedIntrusiveForwardList<DescendingPriority, Entry, &Entry::node>;

	/// type of free entry list
	using FreeEntryList = EntryList::UnsortedIntrusiveForwardList;

	/**
	 * \brief InternalFunctor is a type-erased interface for functors which execute common code of pop() and push()
	 * operations.
	 *
	 * The functor will be called by MessageQueueBase internals with references to \a entryList_ and \a freeEntryList_.
	 * It should perform common actions and execute the QueueFunctor passed from callers.
	 */

	class InternalFunctor : public estd::TypeErasedFunctor<void(EntryList&, FreeEntryList&)>
	{

	};

	/**
	 * \brief MessageQueueBase's constructor
	 *
	 * \param [in] entryStorageUniquePointer is a rvalue reference to EntryStorageUniquePointer with storage for queue
	 * entries (sufficiently large for \a maxElements EntryStorage objects) and appropriate deleter
	 * \param [in] valueStorageUniquePointer is a rvalue reference to ValueStorageUniquePointer with storage for queue
	 * elements (sufficiently large for \a maxElements, each \a elementSize bytes long) and appropriate deleter
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] maxElements is the number of elements in \a entryStorage array and valueStorage memory block
	 */

	MessageQueueBase(EntryStorageUniquePointer&& entryStorageUniquePointer,
			ValueStorageUniquePointer&& valueStorageUniquePointer, size_t elementSize, size_t maxElements);

	/**
	 * \brief MessageQueueBase's destructor
	 */

	~MessageQueueBase();

	/**
	 * \return maximum number of elements in queue
	 */

	size_t getCapacity() const
	{
		return popSemaphore_.getMaxValue();
	}

	/**
	 * \brief Implementation of pop() using type-erased functor
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a popSemaphore_
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [in] functor is a reference to QueueFunctor which will execute actions related to popping - it will get a
	 * pointer to storage with element
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pop(const SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority, const QueueFunctor& functor);

	/**
	 * \brief Implementation of push() using type-erased functor
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] priority is the priority of new element
	 * \param [in] functor is a reference to QueueFunctor which will execute actions related to pushing - it will get a
	 * pointer to storage for element
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int push(const SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, const QueueFunctor& functor);

private:

	/**
	 * \brief Implementation of pop() and push() using type-erased internal functor
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a waitSemaphore
	 * \param [in] internalFunctor is a reference to InternalFunctor which will execute actions related to
	 * popping/pushing
	 * \param [in] waitSemaphore is a reference to semaphore that will be waited for, \a popSemaphore_ for pop(), \a
	 * pushSemaphore_ for push()
	 * \param [in] postSemaphore is a reference to semaphore that will be posted after the operation, \a pushSemaphore_
	 * for pop(), \a popSemaphore_ for push()
	 *
	 * \return 0 if operation was successful, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popPush(const SemaphoreFunctor& waitSemaphoreFunctor, const InternalFunctor& internalFunctor,
			Semaphore& waitSemaphore, Semaphore& postSemaphore);

	/// semaphore guarding access to "pop" functions - its value is equal to the number of available elements
	Semaphore popSemaphore_;

	/// semaphore guarding access to "push" functions - its value is equal to the number of free slots
	Semaphore pushSemaphore_;

	/// storage for queue entries
	const EntryStorageUniquePointer entryStorageUniquePointer_;

	/// storage for queue elements
	const ValueStorageUniquePointer valueStorageUniquePointer_;

	/// list of available entries, sorted in descending order of priority
	EntryList entryList_;

	/// list of "free" entries
	FreeEntryList freeEntryList_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MESSAGEQUEUEBASE_HPP_
