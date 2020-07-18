/**
 * \file
 * \brief MutexControlBlock class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCK_HPP_

#include "distortos/internal/scheduler/ThreadList.hpp"

#include "distortos/internal/synchronization/MutexListNode.hpp"

#include "distortos/MutexProtocol.hpp"
#include "distortos/MutexType.hpp"
#include "distortos/TickClock.hpp"

#include <climits>

namespace distortos
{

namespace internal
{

/// MutexControlBlock class is a control block for Mutex
class MutexControlBlock : public MutexListNode
{
public:

	/// mutex protocols
	using Protocol = MutexProtocol;

	/// type used for counting recursive locks
	using RecursiveLocksCount = uint16_t;

	/// type of mutex
	using Type = MutexType;

	/**
	 * \brief Gets "boosted priority" of the mutex.
	 *
	 * "Boosted priority" of the mutex depends on the selected priority protocol:
	 * - none - 0,
	 * - priorityInheritance - effective priority of the highest priority thread blocked on this mutex or 0 if no
	 * threads are blocked,
	 * - priorityProtect - priority ceiling.
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

	/// shift of "type" subfield, bits
	constexpr static uint8_t typeShift {0};

	/// width of "type" subfield, bits
	constexpr static uint8_t typeWidth {CHAR_BIT / 2};

	/// shift of "protocol" subfield, bits
	constexpr static uint8_t protocolShift {typeShift + typeWidth};

	/// width of "protocol" subfield, bits
	constexpr static uint8_t protocolWidth {CHAR_BIT / 2};

protected:

	/**
	 * \brief MutexControlBlock's constructor
	 *
	 * \param [in] type is the type of mutex
	 * \param [in] protocol is the mutex protocol
	 * \param [in] priorityCeiling is the priority ceiling of mutex, ignored when protocol != Protocol::priorityProtect
	 */

	constexpr MutexControlBlock(const Type type, const Protocol protocol, const uint8_t priorityCeiling) :
			MutexListNode{},
			blockedList_{},
			owner_{},
			recursiveLocksCount_{},
			priorityCeiling_{priorityCeiling},
			typeProtocol_{static_cast<uint8_t>(static_cast<uint8_t>(type) << typeShift |
					static_cast<uint8_t>(protocol) << protocolShift)}
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
	 * Mutex is unlocked if blockedList_ is empty, otherwise the ownership is transferred to the next thread.
	 *
	 * \attention mutex must be locked
	 */

	void doUnlockOrTransferLock();

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
		return static_cast<Protocol>((typeProtocol_ >> protocolShift) & ((1 << protocolWidth) - 1));
	}

	/**
	 * \return reference to number of recursive locks
	 */

	RecursiveLocksCount& getRecursiveLocksCount()
	{
		return recursiveLocksCount_;
	}

	/**
	 * \return type of mutex
	 */

	Type getType() const
	{
		return static_cast<Type>((typeProtocol_ >> typeShift) & ((1 << typeWidth) - 1));
	}

private:

	/**
	 * \brief Performs any actions required before actually blocking on the mutex.
	 *
	 * In case of priorityInheritance protocol, priority of owner thread is boosted and this mutex is set as the
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

	/// number of recursive locks, used when mutex type is recursive
	RecursiveLocksCount recursiveLocksCount_;

	/// priority ceiling of mutex, valid only when protocol_ == Protocol::priorityProtect
	uint8_t priorityCeiling_;

	/// type of mutex and its protocol
	uint8_t typeProtocol_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MUTEXCONTROLBLOCK_HPP_
