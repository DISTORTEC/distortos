/**
 * \file
 * \brief DynamicRawFifoQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICRAWFIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_DYNAMICRAWFIFOQUEUE_HPP_

#include "RawFifoQueue.hpp"

namespace distortos
{

/**
 * \brief DynamicRawFifoQueue class is a variant of RawFifoQueue that has dynamic storage for queue's contents.
 *
 * \ingroup queues
 */

class DynamicRawFifoQueue : public RawFifoQueue
{
public:

	/**
	 * \brief DynamicRawFifoQueue's constructor
	 *
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] queueSize is the maximum number of elements in queue
	 */

	DynamicRawFifoQueue(size_t elementSize, size_t queueSize);
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICRAWFIFOQUEUE_HPP_
