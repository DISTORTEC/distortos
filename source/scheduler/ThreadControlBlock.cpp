/**
 * \file
 * \brief ThreadControlBlock class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-13
 */

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include "distortos/scheduler/threadReturnTrap.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadControlBlock::ThreadControlBlock(void * const buffer, const size_t size, void * (&function)(void *),
		void * const arguments, const uint8_t priority) :
		stack_{buffer, size, runTrampoline_, this, threadReturnTrap},
		function_{function},
		arguments_{arguments},
		priority_{priority},
		roundRobinQuantum_{},
		state_{State::New}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| private static functions
+---------------------------------------------------------------------------------------------------------------------*/

void* ThreadControlBlock::runTrampoline_(void* const argument)
{
	auto& that = *reinterpret_cast<const ThreadControlBlock*>(argument);
	return that.run_();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void* ThreadControlBlock::run_() const
{
	return function_(arguments_);
}

}	// namespace scheduler

}	// namespace distortos
