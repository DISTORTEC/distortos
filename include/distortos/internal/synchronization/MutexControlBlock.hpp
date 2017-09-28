/**
 * \file
 * \brief MutexControlBlock class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCK_HPP_

#include "distortos/internal/scheduler/ThreadList.hpp"

#include "distortos/internal/synchronization/MutexListNode.hpp"

#include "distortos/TickClock.hpp"

namespace distortos
{

namespace internal
{

/// MutexControlBlock class is a control block for Mutex
class MutexControlBlock : public MutexListNode
{
public:

	/// mutex protocols
	enum class Protocol : uint8_t
	{
		/// no protocol, similar to PTHREAD_PRIO_NONE
		none,
		/// priority inheritance protocol, similar to PTHREAD_PRIO_INHERIT
		priorityInheritance,
		/// priority protection protocol (Immediate Ceiling Priority Protocol), similar to PTHREAD_PRIO_PROTECT
		priorityProtect,
	};

	/// type used for counting recursive locks
	using RecursiveLocksCount = uint16_t;

	/// type of mutex
	enum class Type : uint8_t
	{
		/// normal mutex, similar to PTHREAD_MUTEX_NORMAL
		normal,
		/// mutex with additional error checking, similar to PTHREAD_MUTEX_ERRORCHECK
		errorChecking,
		/// recursive mutex, similar to PTHREAD_MUTEX_RECURSIVE
		recursive
	};

	/**
	 * \brief MutexControlBlock constructor
	 *
	 * \param [in] type is the type of mutex
	 * \param [in] protocol is the mutex protocol
	 * \param [in] priorityCeiling is the priority ceiling of mutex, ignored when protocol != Protocol::priorityProtect
	 */

	constexpr MutexControlBlock(const Type type, const Protocol protocol, const uint8_t priorityCeiling) :
			MutexListNode{},
			blockedList_{},
			owner_{},
			protocol_{protocol},
			priorityCeiling_{priorityCeiling},
			type_{type}
	{

	}

	/**
	 * \brief Blocks current thread, transferring it to blockedList_.
	 *
	 * \return 0 on success, error code otherwise:
	 * - values returned by Scheduler::block();
	 */

	int doBlock();

	/**
	 * \brief Blocks current thread with timeout, transferring it to blockedList_.
	 *
	 * \param [in] timePoint is the time point at which the thread will be unblocked (if not already unblocked)
	 *
	 * \return 0 on success, error code otherwise:
	 * - values returned by Scheduler::blockUntil();
	 */

	int doBlockUntil(TickClock::time_point timePoint);

	/**
	 * \brief Performs actual locking of previously unlocked mutex.
	 *
	 * \attention mutex must be unlocked
	 */

	void doLock();

	/**
	 * \brief Performs unlocking or transfer of lock from current owner to next thread on the list.
	 *
	 * Mutex is unlocked if blockedList_ is empty, otherwise the ownership is transfered to the next thread.
	 *
	 * \attention mutex must be locked
	 */

	void doUnlockOrTransferLock();

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
	 * \return priority ceiling of mutex, valid only when protocol_ == Protocol::priorityProtect
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
	 * \return type of mutex
	 */

	Type getType() const
	{
		return type_;
	}

private:

	/**
	 * \brief Performs any actions required before actually blocking on the mutex.
	 *
	 * In case of PriorityInheritance protocol, priority of owner thread is boosted and this mutex is set as the
	 * blocking mutex of the calling thread. In all other cases this function does nothing.
	 *
	 * \attention must be called in block() and blockUntil() before actually blocking of the calling thread.
	 */

	void beforeBlock() const;

	/**
	 * \brief Performs transfer of lock from current owner to next thread on the list.
	 *
	 * \attention mutex must be locked and blockedList_ must not be empty
	 */

	void doTransferLock();

	/**
	 * \brief Performs actual unlocking of previously locked mutex.
	 *
	 * \attention mutex must be locked and blockedList_ must be empty
	 */

	void doUnlock();

	/// ThreadControlBlock objects blocked on mutex
	ThreadList blockedList_;

	/// owner of the mutex
	ThreadControlBlock* owner_;

	/// mutex protocol
	Protocol protocol_;

	/// priority ceiling of mutex, valid only when protocol_ == Protocol::priorityProtect
	uint8_t priorityCeiling_;

	/// type of mutex
	Type type_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCK_HPP_
