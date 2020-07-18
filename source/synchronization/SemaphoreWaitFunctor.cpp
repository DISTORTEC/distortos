/**
 * \file
 * \brief SemaphoreWaitFunctor class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/synchronization/SemaphoreWaitFunctor.hpp"

#include "distortos/Semaphore.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int SemaphoreWaitFunctor::operator()(Semaphore& semaphore) const
{
	return semaphore.wait();
}

}	// namespace internal

}	// namespace distortos
