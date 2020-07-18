/**
 * \file
 * \brief SemaphoreTryWaitFunctor class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/synchronization/SemaphoreTryWaitFunctor.hpp"

#include "distortos/Semaphore.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int SemaphoreTryWaitFunctor::operator()(Semaphore& semaphore) const
{
	return semaphore.tryWait();
}

}	// namespace internal

}	// namespace distortos
