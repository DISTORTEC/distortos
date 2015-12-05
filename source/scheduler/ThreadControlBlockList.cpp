/**
 * \file
 * \brief ThreadList class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-05
 */

#include "distortos/internal/scheduler/ThreadControlBlockList.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadList::~ThreadList()
{

}

void ThreadList::sortedSplice(ThreadList& other, const iterator otherPosition)
{
	Base::sortedSplice(other, otherPosition);
}

}	// namespace internal

}	// namespace distortos
