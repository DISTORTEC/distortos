/**
 * \file
 * \brief threadRunner() definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-24
 */

#include "distortos/scheduler/threadRunner.hpp"

#include "distortos/scheduler/ThreadControlBlock.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void threadRunner(ThreadControlBlock& threadControlBlock)
{
	threadControlBlock.run_();
}

}	// namespace scheduler

}	// namespace distortos
