/**
 * \file
 * \brief MessageQueueBase class implementation
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/synchronization/MessageQueueBase.hpp"

#include "distortos/InterruptMaskingLock.hpp"

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// PopInternalFunctor class is a MessageQueueBase::InternalFunctor used for popping of elements from the queue
class PopInternalFunctor : public MessageQueueBase::InternalFunctor
{
public:

	/**
	 * \brief PopInternalFunctor's constructor
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [in] functor is a reference to QueueFunctor which will execute actions related to popping - it will get a
	 * pointer to storage with element
	 */

	constexpr PopInternalFunctor(uint8_t& priority, const QueueFunctor& functor) :
			priority_{priority},
			functor_{functor}
	{

	}

	/**
	 * \brief PushInternalFunctor's function call operator
	 *
	 * Gets oldest entry with highest priority from \a entryList, passes the storage to \a functor_ and splices this
	 * (now free) entry to \a freeEntryList.
	 *
	 * \param [in] entryList is a reference to EntryList of MessageQueueBase
	 * \param [in] freeEntryList is a reference to FreeEntryList of MessageQueueBase
	 */

	void operator()(MessageQueueBase::EntryList& entryList, MessageQueueBase::FreeEntryList& freeEntryList) const
			override
	{
		const auto& entry = entryList.front();
		priority_ = entry.priority;

		functor_(entry.storage);

		MessageQueueBase::FreeEntryList::splice_after(freeEntryList.before_begin(), entryList.before_begin());
	}

private:

	/// reference to variable that will be used to return priority of popped value
	uint8_t& priority_;

	/// reference to QueueFunctor which will execute actions related to popping
	const QueueFunctor& functor_;
};

/// PushInternalFunctor class is a MessageQueueBase::InternalFunctor used for pushing of elements to the queue
class PushInternalFunctor : public MessageQueueBase::InternalFunctor
{
public:

	/**
	 * \brief PushInternalFunctor's constructor
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] functor is a reference to QueueFunctor which will execute actions related to pushing - it will get a
	 * pointer to storage for element
	 */

	constexpr PushInternalFunctor(const uint8_t priority, const QueueFunctor& functor) :
			functor_{functor},
			priority_{priority}
	{

	}

	/**
	 * \brief PushInternalFunctor's function call operator
	 *
	 * Gets one entry from \a freeEntryList, passes the storage to \a functor_ and splices this entry to \a entryList.
	 *
	 * \param [in] entryList is a reference to EntryList of MessageQueueBase
	 * \param [in] freeEntryList is a reference to FreeEntryList of MessageQueueBase
	 */

	void operator()(MessageQueueBase::EntryList& entryList, MessageQueueBase::FreeEntryList& freeEntryList) const
			override
	{
		auto& entry = freeEntryList.front();

		entry.priority = priority_;

		functor_(entry.storage);

		entryList.splice_after(freeEntryList.before_begin());
	}

private:

	/// reference to QueueFunctor which will execute actions related to pushing
	const QueueFunctor& functor_;

	/// priority of new element
	const uint8_t priority_;
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

MessageQueueBase::MessageQueueBase(EntryStorageUniquePointer&& entryStorageUniquePointer,
		ValueStorageUniquePointer&& valueStorageUniquePointer, size_t elementSize, size_t maxElements) :
		popSemaphore_{0, maxElements},
		pushSemaphore_{maxElements, maxElements},
		entryStorageUniquePointer_{std::move(entryStorageUniquePointer)},
		valueStorageUniquePointer_{std::move(valueStorageUniquePointer)},
		entryList_{},
		freeEntryList_{}
{
	for (size_t i = 0; i < maxElements; ++i)
	{
		const auto valueStorage = &reinterpret_cast<uint8_t*>(valueStorageUniquePointer_.get())[elementSize * i];
		auto& element = *new (&entryStorageUniquePointer_[i]) Entry{{}, valueStorage};
		freeEntryList_.push_front(element);
	}
}

MessageQueueBase::~MessageQueueBase()
{

}

int MessageQueueBase::pop(const SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority, const QueueFunctor& functor)
{
	const PopInternalFunctor popInternalFunctor {priority, functor};
	return popPush(waitSemaphoreFunctor, popInternalFunctor, popSemaphore_, pushSemaphore_);
}

int MessageQueueBase::push(const SemaphoreFunctor& waitSemaphoreFunctor, const uint8_t priority,
		const QueueFunctor& functor)
{
	const PushInternalFunctor pushInternalFunctor {priority, functor};
	return popPush(waitSemaphoreFunctor, pushInternalFunctor, pushSemaphore_, popSemaphore_);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int MessageQueueBase::popPush(const SemaphoreFunctor& waitSemaphoreFunctor, const InternalFunctor& internalFunctor,
		Semaphore& waitSemaphore, Semaphore& postSemaphore)
{
	const InterruptMaskingLock interruptMaskingLock;

	const auto ret = waitSemaphoreFunctor(waitSemaphore);
	if (ret != 0)
		return ret;

	internalFunctor(entryList_, freeEntryList_);

	return postSemaphore.post();
}

}	// namespace internal

}	// namespace distortos
