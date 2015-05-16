/**
 * \file
 * \brief PrioritizedTestCase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#include "PrioritizedTestCase.hpp"

#include "mainTestThreadParameters.hpp"

#include "distortos/ThisThread.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool PrioritizedTestCase::run_() const
{
	ThisThread::setPriority(priority_);
	const auto ret = testCase_.run();
	ThisThread::setPriority(mainTestThreadPriority);
	return ret;
}

}	// namespace test

}	// namespace distortos
