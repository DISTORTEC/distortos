/**
 * \file
 * \brief RunnableThread class header
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_RUNNABLETHREAD_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_RUNNABLETHREAD_HPP_

#include "distortos/Thread.hpp"

namespace distortos
{

namespace internal
{

/// RunnableThread class defines additional interface functions required for the thread to be actually runnable
class RunnableThread : public Thread
{
	friend void threadExiter(RunnableThread&);
	friend void threadRunner(RunnableThread&);

protected:

	/**
	 * \brief Thread's "exit 0" hook function
	 *
	 * This hook will be called early during thread's exit - while the thread is still runnable.
	 */

	virtual void exit0Hook() = 0;

	/**
	 * \brief Thread's "exit 1" hook function
	 *
	 * This hook will be called late during thread's exit - after the thread is removed from the scheduler.
	 */

	virtual void exit1Hook() = 0;

	/**
	 * \brief Thread's "run" function
	 */

	virtual void run() = 0;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_RUNNABLETHREAD_HPP_
