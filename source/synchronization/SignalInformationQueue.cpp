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
 * \date 2015-10-24
 */

#include "distortos/synchronization/SignalInformationQueue.hpp"

#include "distortos/SignalSet.hpp"

#include "distortos/memory/dummyDeleter.hpp"

#include <cerrno>

namespace distortos
{

namespace synchronization
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SignalInformationQueue::SignalInformationQueue(Storage* const storage, const size_t maxElements) :
		storageUniquePointer_{storage, memory::dummyDeleter},
		pool_{},
		poolAllocator_{pool_},
		signalInformationList_{poolAllocator_},
		freeSignalInformationList_{poolAllocator_}
{
	for (size_t i {}; i < maxElements; ++i)
	{
		pool_.feed(storage[i]);
		freeSignalInformationList_.emplace_front(uint8_t{}, SignalInformation::Code{}, sigval{});
	}
}

std::pair<int, SignalInformation> SignalInformationQueue::acceptQueuedSignal(const uint8_t signalNumber)
{
	auto it = signalInformationList_.before_begin();
	auto next = signalInformationList_.begin();
	const auto last = signalInformationList_.end();

	while (next != last)
	{
		if (next->getSignalNumber() == signalNumber)
		{
			const auto signalInformation = *next;
			signalInformationList_.erase_after(it);
			freeSignalInformationList_.emplace_front(uint8_t{}, SignalInformation::Code{}, sigval{});
			return {0, signalInformation};
		}

		it = next;
		++next;
	}

	return {EAGAIN, SignalInformation{uint8_t{}, SignalInformation::Code{}, sigval{}}};
}

SignalSet SignalInformationQueue::getQueuedSignalSet() const
{
	SignalSet queuedSignalSet {SignalSet::empty};
	for (const auto& signalInformation : signalInformationList_)
		queuedSignalSet.add(signalInformation.getSignalNumber());
	return queuedSignalSet;
}

int SignalInformationQueue::queueSignal(const uint8_t signalNumber, const sigval value)
{
	if (signalNumber >= SignalSet::Bitset{}.size())
		return EINVAL;

	if (freeSignalInformationList_.empty() == true)
		return EAGAIN;

	auto it = signalInformationList_.before_begin();
	auto next = signalInformationList_.begin();
	const auto last = signalInformationList_.end();

	while (next != last)	// find the iterator to the last element on the list ("before end")
	{
		it = next;
		++next;
	}

	freeSignalInformationList_.pop_front();
	signalInformationList_.emplace_after(it, signalNumber, SignalInformation::Code::Queued, value);
	return 0;
}

}	// namespace synchronization

}	// namespace distortos
