/**
 * \file
 * \brief ThreadIdentifier class header
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_THREADIDENTIFIER_HPP_
#define INCLUDE_DISTORTOS_THREADIDENTIFIER_HPP_

#include <cstdint>

namespace distortos
{

class Thread;

namespace internal
{

class ThreadControlBlock;

}	// namespace internal

/**
 * \brief ThreadIdentifier class is an identifier of thread
 *
 * Similar to std::thread::id - http://en.cppreference.com/w/cpp/thread/thread/id
 * Similar to POSIX pthread_t - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_types.h.html
 *
 * It is true that the address of a thread object (`DynamicThread` or `StaticThread`) can be used as an identifier in
 * most cases, however it is worth noting that there are two problems with such use case. These problems may or may not
 * affect the application, depending on its architecture, system configuration and used features.
 *
 * First of all, when thread detachment is enabled, the "outer" `DynamicThread` object is just a holder of an "inner"
 * thread object. In that case, the address of this "outer" object will not be equal to the address of the reference
 * returned by calling `ThisThread::get()` from thread's function.
 *
 * Another issue is that using an address gives no protection against accessing an object after it was deleted.
 *
 * ThreadIdentifier tries to solve these issues, as the instance is tied to the actual executing thread and - to some
 * degree - is unique. That's why the probability of accessing wrong or destructed thread via a "dangling" identifier is
 * negligible.
 *
 * \ingroup threads
 */

class ThreadIdentifier
{
public:

	/**
	 * \brief ThreadIdentifier's constructor
	 *
	 * Similar to std::thread::id::id() - http://en.cppreference.com/w/cpp/thread/thread/id/id
	 *
	 * Default-constructed identifier does not represent valid thread.
	 */

	constexpr ThreadIdentifier() :
			sequenceNumber_{},
			threadControlBlock_{}
	{

	}

	/**
	 * \brief ThreadIdentifier's constructor
	 *
	 * \param [in] threadControlBlock is a reference to thread control block
	 * \param [in] sequenceNumber is the sequence number of \a threadControlBlock
	 */

	ThreadIdentifier(const internal::ThreadControlBlock& threadControlBlock, const uintptr_t sequenceNumber) :
			sequenceNumber_{sequenceNumber},
			threadControlBlock_{&threadControlBlock}
	{

	}

	/**
	 * \brief Tries to obtain the thread represented by this identifier.
	 *
	 * \return pointer to thread represented by this identifier, nullptr if this identifier doesn't represent valid
	 * thread
	 */

	Thread* getThread() const;

private:

	/// expected sequence number of \a threadControlBlock_
	uintptr_t sequenceNumber_;

	/// pointer to thread control block
	const internal::ThreadControlBlock* threadControlBlock_;
};

/**
 * \brief ThreadIdentifier's equality comparison operator
 *
 * Similar to std::operator==(std::thread::id, std::thread::id) -
 * http://en.cppreference.com/w/cpp/thread/thread/id/operator_cmp
 * Similar to pthread_equal() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_equal.html
 *
 * \relates ThreadIdentifier
 *
 * \param [in] left is the object on the left-hand side of comparison
 * \param [in] right is the object on the right-hand side of comparison
 *
 * \return true if \a left and \b right both refer to the same thread or if both of them are invalid, false otherwise
 */

inline bool operator==(const ThreadIdentifier& left, const ThreadIdentifier& right)
{
	return left.getThread() == right.getThread();
}

/**
 * \brief ThreadIdentifier's inequality comparison operator
 *
 * Similar to std::operator!=(std::thread::id, std::thread::id) -
 * http://en.cppreference.com/w/cpp/thread/thread/id/operator_cmp
 *
 * \relates ThreadIdentifier
 *
 * \param [in] left is the object on the left-hand side of comparison
 * \param [in] right is the object on the right-hand side of comparison
 *
 * \return false if \a left and \b right both refer to the same thread or if both of them are invalid, true otherwise
 */

inline bool operator!=(const ThreadIdentifier& left, const ThreadIdentifier& right)
{
	return !(left == right);
}

/**
 * \brief ThreadIdentifier's less-than comparison operator
 *
 * Similar to std::operator<(std::thread::id, std::thread::id) -
 * http://en.cppreference.com/w/cpp/thread/thread/id/operator_cmp
 *
 * \relates ThreadIdentifier
 *
 * \param [in] left is the object on the left-hand side of comparison
 * \param [in] right is the object on the right-hand side of comparison
 *
 * \return true if \a left should be ordered before \a right, false otherwise
 */

inline bool operator<(const ThreadIdentifier& left, const ThreadIdentifier& right)
{
	return left.getThread() < right.getThread();
}

/**
 * \brief ThreadIdentifier's greater-than comparison operator
 *
 * Similar to std::operator>(std::thread::id, std::thread::id) -
 * http://en.cppreference.com/w/cpp/thread/thread/id/operator_cmp
 *
 * \relates ThreadIdentifier
 *
 * \param [in] left is the object on the left-hand side of comparison
 * \param [in] right is the object on the right-hand side of comparison
 *
 * \return true if \a right should be ordered before \a left, false otherwise
 */

inline bool operator>(const ThreadIdentifier& left, const ThreadIdentifier& right)
{
	return right < left;
}

/**
 * \brief ThreadIdentifier's less-than-or-equal comparison operator
 *
 * Similar to std::operator<=(std::thread::id, std::thread::id) -
 * http://en.cppreference.com/w/cpp/thread/thread/id/operator_cmp
 *
 * \relates ThreadIdentifier
 *
 * \param [in] left is the object on the left-hand side of comparison
 * \param [in] right is the object on the right-hand side of comparison
 *
 * \return false if \a left should be ordered before \a right, true otherwise
 */

inline bool operator<=(const ThreadIdentifier& left, const ThreadIdentifier& right)
{
	return !(left > right);
}

/**
 * \brief ThreadIdentifier's greater-than-or-equal comparison operator
 *
 * Similar to std::operator>=(std::thread::id, std::thread::id) -
 * http://en.cppreference.com/w/cpp/thread/thread/id/operator_cmp
 *
 * \relates ThreadIdentifier
 *
 * \param [in] left is the object on the left-hand side of comparison
 * \param [in] right is the object on the right-hand side of comparison
 *
 * \return false if \a right should be ordered before \a left, true otherwise
 */

inline bool operator>=(const ThreadIdentifier& left, const ThreadIdentifier& right)
{
	return !(left < right);
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THREADIDENTIFIER_HPP_
