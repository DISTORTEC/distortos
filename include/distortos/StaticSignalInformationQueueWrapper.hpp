/**
 * \file
 * \brief StaticSignalInformationQueueWrapper class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-26
 */

#ifndef INCLUDE_DISTORTOS_STATICSIGNALINFORMATIONQUEUEWRAPPER_HPP_
#define INCLUDE_DISTORTOS_STATICSIGNALINFORMATIONQUEUEWRAPPER_HPP_

#include "distortos/SignalInformationQueueWrapper.hpp"

namespace distortos
{

/**
 * \brief StaticSignalInformationQueueWrapper class is a variant of SignalInformationQueueWrapper that has automatic
 * storage for queue's contents.
 *
 * \param QueueSize is the maximum number of elements in queue
 */

template<size_t QueueSize>
class StaticSignalInformationQueueWrapper : public SignalInformationQueueWrapper
{
public:

	/**
	 * \brief StaticSignalInformationQueueWrapper's constructor
	 */

	StaticSignalInformationQueueWrapper() :
			SignalInformationQueueWrapper{storage_}
	{

	}

private:

	/// storage for queue's contents
	std::array<Storage, QueueSize> storage_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICSIGNALINFORMATIONQUEUEWRAPPER_HPP_
