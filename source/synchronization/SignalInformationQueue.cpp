/**
 * \file
 * \brief SignalInformationQueue class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-26
 */

#include "distortos/synchronization/SignalInformationQueue.hpp"

namespace distortos
{

namespace synchronization
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SignalInformationQueue::SignalInformationQueue() :
		pool_{},
		poolAllocator_{pool_},
		signalInformationList_{poolAllocator_},
		freeSignalInformationList_{poolAllocator_}
{

}

}	// namespace synchronization

}	// namespace distortos
