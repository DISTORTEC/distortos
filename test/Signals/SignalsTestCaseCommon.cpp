/**
 * \file
 * \brief SignalsTestCaseCommon class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#include "SignalsTestCaseCommon.hpp"

#include "distortos/SignalSet.hpp"
#include "distortos/ThisThread-Signals.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsTestCaseCommon::finalize() const
{
	if (ThisThread::Signals::setSignalMask(SignalSet{SignalSet::empty}) != 0)
		return false;

	return PrioritizedTestCase::finalize();
}

bool SignalsTestCaseCommon::initialize() const
{
	return PrioritizedTestCase::initialize();
}

}	// namespace test

}	// namespace distortos
