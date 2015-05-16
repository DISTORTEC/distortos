/**
 * \file
 * \brief mainTestThread object definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#include "mainTestThread.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

MainTestThread mainTestThread = makeStaticThread<mainTestThreadStackSize, mainTestThreadCanReceiveSignals,
		mainTestThreadQueuedSignals, mainTestThreadCaughtSignals>(mainTestThreadPriority, testThreadFunction);

}	// namespace test

}	// namespace distortos
