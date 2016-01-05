/**
 * \file
 * \brief UndetachableThread class implementation
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/UndetachableThread.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_THREAD_DETACH_ENABLE

int UndetachableThread::detach()
{
	return ENOTSUP;
}

#endif	// def CONFIG_THREAD_DETACH_ENABLE

}	// namespace distortos
