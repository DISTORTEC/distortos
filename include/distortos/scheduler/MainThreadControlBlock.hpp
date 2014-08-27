/**
 * \file
 * \brief MainThreadControlBlock class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-27
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_MAINTHREADCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_MAINTHREADCONTROLBLOCK_HPP_

#include "distortos/scheduler/ThreadControlBlock.hpp"

namespace distortos
{

namespace scheduler
{

/// MainThreadControlBlock class is a ThreadControlBlock for main()
class MainThreadControlBlock : public ThreadControlBlock
{
public:

	/**
	 * \brief MainThreadControlBlock's constructor.
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	MainThreadControlBlock(uint8_t priority);

private:

	/**
	 * \brief "Run" function of thread
	 *
	 * Just a dummy which never gets used.
	 */

	virtual void run_() override;

	/**
	 * \brief Termination hook function of thread
	 *
	 * This function is called after run_() completes.
	 *
	 * Just a dummy which never gets used.
	 */

	virtual void terminationHook_() override;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_MAINTHREADCONTROLBLOCK_HPP_
