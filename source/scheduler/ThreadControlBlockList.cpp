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
 * \date 2015-12-02
 */

#include "distortos/internal/scheduler/ThreadControlBlockList.hpp"

namespace distortos
{

namespace internal
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
}

}	// namespace internal

}	// namespace distortos
