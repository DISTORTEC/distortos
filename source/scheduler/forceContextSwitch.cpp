/**
 * \file
 * \brief forceContextSwitch() definition
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/forceContextSwitch.hpp"

#include "distortos/architecture/requestContextSwitch.hpp"

#include "distortos/internal/synchronization/InterruptUnmaskingLock.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void forceContextSwitch()
{
	architecture::requestContextSwitch();
	const InterruptUnmaskingLock interruptUnmaskingLock;
}

}	// namespace internal

}	// namespace distortos
