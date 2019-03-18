/**
 * \file
 * \brief SynchronousSdMmcCardLowLevel class implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/SynchronousSdMmcCardLowLevel.hpp"

#include "distortos/assert.h"

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SynchronousSdMmcCardLowLevel::transactionCompleteEvent(const Result result)
{
	result_ = result;
	const auto ret = semaphore_.post();
	assert(ret == 0);
}

}	// namespace devices

}	// namespace distortos
