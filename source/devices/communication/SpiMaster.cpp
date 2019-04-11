/**
 * \file
 * \brief SpiMaster class implementation
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiMaster.hpp"

#include "distortos/devices/communication/SpiMasterLowLevel.hpp"
#include "distortos/devices/communication/SpiMasterTransfer.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/assert.h"
#include "distortos/Semaphore.hpp"

#include "estd/ScopeGuard.hpp"

#include <mutex>

#include <cerrno>

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiMaster::~SpiMaster()
{
	assert(openCount_ == 0);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SpiMaster::close()
{
	assert(openCount_ != 0);

	if (openCount_ == 1)	// last close?
		spiMaster_.stop();

	--openCount_;
}

uint32_t SpiMaster::configure(const SpiMode mode, const uint32_t clockFrequency, const uint8_t wordLength,
		const bool lsbFirst, const uint32_t dummyData) const
{
	assert(openCount_ != 0);

	return spiMaster_.configure(mode, clockFrequency, wordLength, lsbFirst, dummyData);
}

std::pair<int, size_t> SpiMaster::executeTransaction(const SpiMasterTransfersRange transfersRange)
{
	CHECK_FUNCTION_CONTEXT();

	assert(openCount_ != 0);
	assert(transfersRange.size() != 0);

	Semaphore semaphore {0};
	semaphore_ = &semaphore;
	transfersRange_ = transfersRange;
	ret_ = {};
	const auto cleanupScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				transfersRange_ = {};
				semaphore_ = {};
			});

	{
		const auto transfer = transfersRange_.begin();
		spiMaster_.startTransfer(*this, transfer->getWriteBuffer(), transfer->getReadBuffer(), transfer->getSize());
	}

	while (semaphore.wait() != 0);

	const auto handledTransfers = transfersRange_.begin() - transfersRange.begin();
	return {ret_, handledTransfers};
}

void SpiMaster::lock()
{
	const auto ret = mutex_.lock();
	assert(ret == 0);
}

void SpiMaster::notifyWaiter(const int ret)
{
	ret_ = ret;
	const auto semaphore = semaphore_;
	assert(semaphore != nullptr);
	semaphore->post();
}

int SpiMaster::open()
{
	assert(openCount_ < std::numeric_limits<decltype(openCount_)>::max());

	if (openCount_ == 0)	// first open?
	{
		const auto ret = spiMaster_.start();
		if (ret != 0)
			return ret;
	}

	++openCount_;
	return 0;
}

void SpiMaster::transferCompleteEvent(const size_t bytesTransfered)
{
	assert(transfersRange_.size() != 0);

	const auto previousTransfer = transfersRange_.begin();
	previousTransfer->finalize(bytesTransfered);

	const auto success = previousTransfer->getSize() == bytesTransfered;
	if (success == true)	// handling of last transfer successful?
		transfersRange_ = {transfersRange_.begin() + 1, transfersRange_.end()};

	if (transfersRange_.size() == 0 || success == false)	// all transfers are done or handling of last one failed?
	{
		notifyWaiter(success == true ? 0 : EIO);
		return;
	}

	{
		const auto nextTransfer = transfersRange_.begin();
		spiMaster_.startTransfer(*this, nextTransfer->getWriteBuffer(), nextTransfer->getReadBuffer(),
				nextTransfer->getSize());
	}
}

void SpiMaster::unlock()
{
	const auto ret = mutex_.unlock();
	assert(ret == 0);
}

}	// namespace devices

}	// namespace distortos
