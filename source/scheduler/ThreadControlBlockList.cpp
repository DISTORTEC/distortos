/**
 * \file
 * \brief ThreadControlBlockList class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-18
 */

#include "distortos/scheduler/ThreadControlBlockList.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadControlBlockList::~ThreadControlBlockList()
{
	for (auto& item : *this)
		item.get().setList(nullptr);
}

void ThreadControlBlockList::sortedSplice(ThreadControlBlockList& other, const iterator otherPosition)
{
	Base::sortedSplice(other, otherPosition);
	otherPosition->get().setList(this);
	otherPosition->get().setState(state_);
}

}	// namespace scheduler

}	// namespace distortos
