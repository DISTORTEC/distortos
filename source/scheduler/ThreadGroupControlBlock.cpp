/**
 * \file
 * \brief ThreadGroupControlBlock class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/ThreadGroupControlBlock.hpp"

#include "distortos/internal/scheduler/ThreadControlBlock.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void ThreadGroupControlBlock::add(ThreadControlBlock& threadControlBlock)
{
	threadList_.push_back(threadControlBlock);
}

}	// namespace internal

}	// namespace distortos
