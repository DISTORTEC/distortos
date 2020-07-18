/**
 * \file
 * \brief threadRunner() definition
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/threadRunner.hpp"

#include "distortos/internal/scheduler/RunnableThread.hpp"
#include "distortos/internal/scheduler/threadExiter.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void threadRunner(RunnableThread& runnableThread)
{
	runnableThread.run();
	threadExiter(runnableThread);
}

}	// namespace internal

}	// namespace distortos
