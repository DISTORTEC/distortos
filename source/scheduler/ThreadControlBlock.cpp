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
 * \date 2014-09-11
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

ThreadControlBlock::ThreadControlBlock(void* const buffer, const size_t size, const uint8_t priority) :
		stack_{buffer, size, threadRunner, *this, threadReturnTrap},
		list_{},
		priority_{priority},
		roundRobinQuantum_{},
		state_{State::New}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadControlBlock::ThreadControlBlock(architecture::Stack&& stack, const uint8_t priority) :
		stack_{stack},
		list_{},
		priority_{priority},
		roundRobinQuantum_{},
		state_{State::New}
{

}

}	// namespace scheduler

}	// namespace distortos
