/**
 * \file
 * \brief UndetachableThread class implementation
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/UndetachableThread.hpp"

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

#include <cerrno>

#endif	// def DISTORTOS_THREAD_DETACH_ENABLE

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

int UndetachableThread::detach()
{
	return ENOTSUP;
}

#endif	// def DISTORTOS_THREAD_DETACH_ENABLE

}	// namespace distortos
