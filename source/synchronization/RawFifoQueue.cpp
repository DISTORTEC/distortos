/**
 * \file
 * \brief RawFifoQueue class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-12-30
 */

#include "distortos/RawFifoQueue.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

RawFifoQueue::RawFifoQueue(void* const storage, const size_t elementSize, const size_t maxElements) :
		fifoQueueBase_{storage, static_cast<uint8_t*>(storage) + elementSize * maxElements, maxElements},
		elementSize_{elementSize}
{

}

}	// namespace distortos
