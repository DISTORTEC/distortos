/**
 * \file
 * \brief MessageQueueBase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-13
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_MESSAGEQUEUEBASE_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_MESSAGEQUEUEBASE_HPP_

#include "distortos/Semaphore.hpp"

#include "distortos/synchronization/QueueFunctor.hpp"
#include "distortos/synchronization/SemaphoreFunctor.hpp"

#include "distortos/allocators/FeedablePool.hpp"

#include <forward_list>

namespace distortos
{

namespace synchronization
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
				priority{priorityy},
				storage{storagee}
		{

		}

		/// default copy constructor
		constexpr Entry(const Entry&) = default;

		/// priority of the entry
		uint8_t priority;

		/// storage for the entry
		void* storage;
	};

	/// link and Entry
	using LinkAndEntry = std::pair<void*, Entry>;

	/// type of uninitialized storage for Entry with link
	using EntryStorage = typename std::aligned_storage<sizeof(LinkAndEntry), alignof(LinkAndEntry)>::type;

	/**
	 * type of uninitialized storage for data
	 *
	 * \param T is the type of data in queue
	 */

	template<typename T>
	struct Storage
	{
		/// storage for Entry
		EntryStorage entryStorage;

		/// storage for value
		typename std::aligned_storage<sizeof(T), alignof(T)>::type valueStorage;
	};

	/// functor which gives descending priority order of elements on the list
	struct DescendingPriority
	{
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

	/// type of pool
	using Pool = allocators::FeedablePool;

	/// type of pool allocator
	using PoolAllocator = allocators::PoolAllocator<Entry, Pool>;

	/// type of free entry list
	using FreeEntryList = std::forward_list<Entry, PoolAllocator>;

	/// type of entry list
	using EntryList = containers::SortedContainer<FreeEntryList, DescendingPriority>;

	/**
	 * \brief InternalFunctor is a type-erased interface for functors which execute common code of pop() and push()
	 * operations.
	 *
	 * The functor will be called by MessageQueueBase internals with references to \a entryList_ and \a freeEntryList_.
	 * It should perform common actions and execute the QueueFunctor passed from callers.
	 */

	using InternalFunctor = estd::TypeErasedFunctor<void(EntryList&, FreeEntryList&)>;

	/**
	 * \brief MessageQueueBase's constructor
	 *
	 * \param T is the type of data in queue
	 *
	 * \param [in] storage is an array of Storage elements
	 * \param [in] maxElements is the number of elements in \a storage array
	 */

	template<typename T>
	MessageQueueBase(Storage<T>* storage, size_t maxElements);

	/**
	 * \brief MessageQueueBase's constructor
	 *
	 * \param [in] entryStorage is an array of EntryStorage elements
	 * \param [in] valueStorage is a memory block for elements, sufficiently large for \a maxElements, each
	 * \a elementSize bytes long
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] maxElements is the number of elements in \a entryStorage array and valueStorage memory block
	 */

	MessageQueueBase(EntryStorage* entryStorage, void* valueStorage, size_t elementSize, size_t maxElements);

	/**
	 * \brief Implementation of pop() using type-erased functor
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a popSemaphore_
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [in] functor is a reference to QueueFunctor which will execute actions related to popping - it will get a
	 * pointer to storage with element
	 *
	 * \return zero if element was popped successfully, error code otherwise:
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
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int push(const SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, const QueueFunctor& functor);

private:

	/**
	 * \brief MessageQueueBase's constructor - internal version
	 *
	 * \param [in] maxElements is the maximum number of elements the queue can hold
	 */

	explicit MessageQueueBase(size_t maxElements);

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
	 * \return zero if operation was successful, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popPush(const SemaphoreFunctor& waitSemaphoreFunctor, const InternalFunctor& internalFunctor,
			Semaphore& waitSemaphore, Semaphore& postSemaphore);

	/// semaphore guarding access to "pop" functions - its value is equal to the number of available elements
	Semaphore popSemaphore_;

	/// semaphore guarding access to "push" functions - its value is equal to the number of free slots
	Semaphore pushSemaphore_;

	/// FeedablePool used by \a poolAllocator_
	Pool pool_;

	/// PoolAllocator used by \a entryList_ and \a freeList_
	PoolAllocator poolAllocator_;

	/// list of available entries, sorted in descending order of priority
	EntryList entryList_;

	/// list of "free" entries
	FreeEntryList freeEntryList_;
};

template<typename T>
MessageQueueBase::MessageQueueBase(Storage<T>* const storage, const size_t maxElements) :
		MessageQueueBase{maxElements}
{
	for (size_t i = 0; i < maxElements; ++i)
	{
		pool_.feed(storage[i].entryStorage);
		freeEntryList_.emplace_front(uint8_t{}, &storage[i].valueStorage);
	}
}

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_MESSAGEQUEUEBASE_HPP_
