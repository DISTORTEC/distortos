/**
 * \file
 * \brief MutexControlBlock class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-12
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_MUTEXCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_MUTEXCONTROLBLOCK_HPP_

#include "distortos/scheduler/ThreadControlBlockList.hpp"

namespace distortos
{

namespace scheduler
{

/// MutexControlBlock class is a control block for Mutex
class MutexControlBlock
{
public:

	/// mutex protocols
	enum class Protocol : uint8_t
	{
		/// no protocol, similar to PTHREAD_PRIO_NONE
		None,
		/// priority inheritance protocol, similar to PTHREAD_PRIO_INHERIT
		PriorityInheritance,
		/// priority protection protocol (Immediate Ceiling Priority Protocol), similar to PTHREAD_PRIO_PROTECT
		PriorityProtect,
	};

	/**
	 * \brief MutexControlBlock constructor
	 *
	 * \param [in] protocol is the mutex protocol
	 * \param [in] priorityCeiling is the priority ceiling of mutex, ignored when protocol != Protocol::PriorityProtect
	 */

	MutexControlBlock(Protocol protocol, uint8_t priorityCeiling);

	/**
	 * \brief Blocks current thread, transferring it to blockedList_.
	 */

	void block();

	/**
	 * \brief Blocks current thread with timeout, transferring it to blockedList_.
	 *
	 * \param [in] timePoint is the time point at which the thread will be unblocked (if not already unblocked)
	 *
	 * \return 0 on success, error code otherwise:
	 * - ETIMEDOUT - thread was unblocked because timePoint was reached;
	 */

	int blockUntil(TickClock::time_point timePoint);

	/**
	 * \brief Gets "boosted priority" of the mutex.
	 *
	 * "Boosted priority" of the mutex depends on the selected priority protocol:
	 * - None - 0,
	 * - PriorityInheritance - effective priority of the highest priority thread blocked on this mutex or 0 if no
	 * threads are blocked,
	 * - PriorityProtect - priority ceiling.
	 *
	 * \return "boosted priority" of the mutex
	 */

	uint8_t getBoostedPriority() const;

	/**
	 * \return owner of the mutex, nullptr if mutex is currently unlocked
	 */

	ThreadControlBlock* getOwner() const
	{
		return owner_;
	}

	/**
	 * \return priority ceiling of mutex, valid only when protocol_ == Protocol::PriorityProtect
	 */

	uint8_t getPriorityCeiling() const
	{
		return priorityCeiling_;
	}

	/**
	 * \return mutex protocol
	 */

	Protocol getProtocol() const
	{
		return protocol_;
	}

	/**
	 * \brief Performs actual locking of previously unlocked mutex.
	 *
	 * \attention mutex must be unlocked
	 */

	void lock();

	/**
	 * \brief Performs unlocking or transfer of lock from current owner to next thread on the list.
	 *
	 * Mutex is unlocked if blockedList_ is empty, otherwise the ownership is transfered to the next thread.
	 *
	 * \attention mutex must be locked
	 */

	void unlockOrTransferLock();

private:

	/// type of object used as storage for MutexControlBlockList elements - 3 pointers
	using Link = std::array<std::aligned_storage<sizeof(void*), alignof(void*)>::type, 3>;

	/**
	 * \brief Performs action required for priority inheritance before actually blocking on the mutex.
	 *
	 * This must be called in block() and blockUntil() before actually blocking of the calling thread.
	 *
	 * \attantion mutex's protocol must be PriorityInheritance
	 */

	void priorityInheritanceBeforeBlock() const;

	/**
	 * \brief Performs transfer of lock from current owner to next thread on the list.
	 *
	 * \attention mutex must be locked and blockedList_ must not be empty
	 */

	void transferLock();

	/**
	 * \brief Performs actual unlocking of previously locked mutex.
	 *
	 * \attention mutex must be locked and blockedList_ must be empty
	 */

	void unlock();

	/// ThreadControlBlock objects blocked on mutex
	ThreadControlBlockList blockedList_;

	/// storage for list link
	Link link_;

	/// pointer to list that has this object
	scheduler::MutexControlBlockList* list_;

	/// iterator to the element on the list, valid only when list_ != nullptr
	scheduler::MutexControlBlockList::iterator iterator_;

	/// owner of the mutex
	ThreadControlBlock* owner_;

	/// mutex protocol
	Protocol protocol_;

	/// priority ceiling of mutex, valid only when protocol_ == Protocol::PriorityProtect
	uint8_t priorityCeiling_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_MUTEXCONTROLBLOCK_HPP_
