/**
 * \file
 * \brief SignalInformationQueue class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/synchronization/SignalInformationQueue.hpp"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/SignalSet.hpp"

#include <cerrno>

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SignalInformationQueue::SignalInformationQueue(StorageUniquePointer&& storageUniquePointer, const size_t maxElements) :
		storageUniquePointer_{std::move(storageUniquePointer)},
		signalInformationList_{},
		freeSignalInformationList_{}
{
	for (size_t i {}; i < maxElements; ++i)
	{
		auto& element = *new (&storageUniquePointer_[i]) QueueNode{{}, {{}, {}, {}}};
		freeSignalInformationList_.push_front(element);
	}
}

SignalInformationQueue::~SignalInformationQueue()
{

}

std::pair<int, SignalInformation> SignalInformationQueue::acceptQueuedSignal(const uint8_t signalNumber)
{
	auto it = signalInformationList_.before_begin();
	auto next = signalInformationList_.begin();
	const auto last = signalInformationList_.end();

	while (next != last)
	{
		if (next->signalInformation.getSignalNumber() == signalNumber)
		{
			const auto signalInformation = next->signalInformation;
			List::splice_after(freeSignalInformationList_.before_begin(), it);
			return {{}, signalInformation};
		}

		it = next;
		++next;
	}

	return {EAGAIN, {{}, {}, {}}};
}

SignalSet SignalInformationQueue::getQueuedSignalSet() const
{
	SignalSet queuedSignalSet {SignalSet::empty};
	for (const auto& node : signalInformationList_)
		queuedSignalSet.add(node.signalInformation.getSignalNumber());
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

	freeSignalInformationList_.front().signalInformation = {signalNumber, SignalInformation::Code::queued, value};
	signalInformationList_.splice_after(it, freeSignalInformationList_.before_begin());
	return 0;
}

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_SIGNALS_ENABLE == 1
