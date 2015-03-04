/**
 * \file
 * \brief MainThread class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-04
 */

#include "distortos/scheduler/MainThread.hpp"

#include "distortos/architecture/architecture.hpp"

namespace distortos
{

namespace scheduler
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Wrapper for Stack's constructor.
 *
 * Enables use of architecture::getMainStack() result to construct Stack.
 *
 * \param [in] stackBuffer is a pair with beginning of stack and its size in bytes
 *
 * \return constructed Stack object
 */

architecture::Stack stackWrapper(const std::pair<void*, size_t> stackBuffer)
{
	return {stackBuffer.first, stackBuffer.second};
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

MainThread::MainThread(const uint8_t priority) :
		ThreadBase{stackWrapper(architecture::getMainStack()), priority, SchedulingPolicy::RoundRobin, nullptr}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void MainThread::run()
{
	while (1);	/// \todo add some form of assertion that this never gets called
}

}	// namespace scheduler

}	// namespace distortos
