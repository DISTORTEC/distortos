/**
 * \file
 * \brief UndetachableThread class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_UNDETACHABLETHREAD_HPP_
#define INCLUDE_DISTORTOS_UNDETACHABLETHREAD_HPP_

#include "distortos/internal/scheduler/ThreadCommon.hpp"

namespace distortos
{

/**
 * \brief UndetachableThread class is a ThreadCommon that cannot be detached
 *
 * \ingroup threads
 */

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

class UndetachableThread : public internal::ThreadCommon
{
public:

	using ThreadCommon::ThreadCommon;

	/**
	 * \brief Detaches the thread.
	 *
	 * Similar to std::thread::detach() - http://en.cppreference.com/w/cpp/thread/thread/detach
	 * Similar to POSIX pthread_detach() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_detach.html
	 *
	 * Detaches the executing thread from the Thread object, allowing execution to continue independently. All resources
	 * allocated for the thread will be deallocated when the thread terminates.
	 *
	 * \return ENOTSUP - this thread cannot be detached;
	 */

	int detach() override;
};

#else	// !def DISTORTOS_THREAD_DETACH_ENABLE

using UndetachableThread = internal::ThreadCommon;

#endif	// !def DISTORTOS_THREAD_DETACH_ENABLE

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_UNDETACHABLETHREAD_HPP_
