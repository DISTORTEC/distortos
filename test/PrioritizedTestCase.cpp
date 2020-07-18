/**
 * \file
 * \brief PrioritizedTestCase class implementation
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "PrioritizedTestCase.hpp"

#include "distortos/ThisThread.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

bool PrioritizedTestCase::finalize() const
{
	ThisThread::setPriority(DISTORTOS_MAIN_THREAD_PRIORITY);
	return true;
}

bool PrioritizedTestCase::initialize() const
{
	ThisThread::setPriority(priority_);
	return true;
}

}	// namespace test

}	// namespace distortos
