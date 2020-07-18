/**
 * \file
 * \brief DynamicRawMessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICRAWMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_DYNAMICRAWMESSAGEQUEUE_HPP_

#include "distortos/RawMessageQueue.hpp"

namespace distortos
{

/**
 * \brief DynamicRawMessageQueue class is a variant of RawMessageQueue that has dynamic storage for queue's contents.
 *
 * \ingroup queues
 */

class DynamicRawMessageQueue : public RawMessageQueue
{
public:

	/**
	 * \brief DynamicRawMessageQueue's constructor
	 *
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] queueSize is the maximum number of elements in queue
	 */

	DynamicRawMessageQueue(size_t elementSize, size_t queueSize);
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICRAWMESSAGEQUEUE_HPP_
