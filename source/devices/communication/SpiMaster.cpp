/**
 * \file
 * \brief SpiMaster class implementation
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiMaster.hpp"

#include "distortos/devices/communication/SpiDevice.hpp"
#include "distortos/devices/communication/SpiMasterLowLevel.hpp"
#include "distortos/devices/communication/SpiMasterOperation.hpp"

#include "distortos/devices/io/OutputPin.hpp"

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
	CHECK_FUNCTION_CONTEXT();

	if (openCount_ == 0)
		return;

	const std::lock_guard<distortos::Mutex> lock {mutex_};

	spiMaster_.stop();
}

int SpiMaster::close()
{
	const std::lock_guard<distortos::Mutex> lock {mutex_};

	if (openCount_ == 0)	// device is not open anymore?
		return EBADF;

	if (openCount_ == 1)	// last close?
	{
		const auto ret = spiMaster_.stop();
		if (ret != 0)
			return ret;
	}

	--openCount_;
	return 0;
}

std::pair<int, size_t> SpiMaster::executeTransaction(const SpiDevice& device,
		const SpiMasterOperationRange operationRange)
{
	CHECK_FUNCTION_CONTEXT();

	if (operationRange.size() == 0)
		return {EINVAL, {}};

	const std::lock_guard<distortos::Mutex> lock {mutex_};

	if (openCount_ == 0)
		return {EBADF, {}};

	{
		const auto ret = spiMaster_.configure(device.getMode(), device.getMaxClockFrequency(), device.getWordLength(),
				device.getLsbFirst());
		if (ret.first != 0)
			return {ret.first, {}};
	}

	device.getSlaveSelectPin().set(false);
	Semaphore semaphore {0};
	semaphore_ = &semaphore;
	operationRange_ = operationRange;
	ret_ = {};
	const auto cleanupScopeGuard = estd::makeScopeGuard(
			[this, &device]()
			{
				operationRange_ = {};
				semaphore_ = {};
				device.getSlaveSelectPin().set(true);
			});

	{
		const auto transfer = operationRange_.begin()->getTransfer();
		assert(transfer != nullptr);
		const auto ret = spiMaster_.startTransfer(transfer->getWriteBuffer(), transfer->getReadBuffer(),
				transfer->getSize());
		if (ret != 0)
			return {ret, {}};
	}

	while (semaphore.wait() != 0);

	const auto handledOperations = operationRange_.begin() - operationRange.begin();
	return {ret_, handledOperations};
}

int SpiMaster::open()
{
	const std::lock_guard<distortos::Mutex> lock {mutex_};

	if (openCount_ == std::numeric_limits<decltype(openCount_)>::max())	// device is already opened too many times?
		return EMFILE;

	if (openCount_ == 0)	// first open?
	{
		const auto ret = spiMaster_.start(*this);
		if (ret != 0)
			return ret;
	}

	++openCount_;
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SpiMaster::notifyWaiter(const int ret)
{
	ret_ = ret;
	const auto semaphore = semaphore_;
	assert(semaphore != nullptr);
	semaphore->post();
}

void SpiMaster::transferCompleteEvent(SpiMasterErrorSet errorSet, size_t bytesTransfered)
{
	assert(operationRange_.size() != 0 && "Invalid range of operations!");

	{
		const auto previousTransfer = operationRange_.begin()->getTransfer();
		assert(previousTransfer != nullptr && "Invalid type of previous operation!");
		previousTransfer->finalize(errorSet, bytesTransfered);
	}

	const auto error = errorSet.any();
	if (error == false)	// handling of last operation successful?
		operationRange_ = {operationRange_.begin() + 1, operationRange_.end()};

	if (operationRange_.size() == 0 || error == true)	// all operations are done or handling of last one failed?
	{
		notifyWaiter(error == false ? 0 : EIO);
		return;
	}

	{
		const auto nextTransfer = operationRange_.begin()->getTransfer();
		assert(nextTransfer != nullptr && "Invalid type of next operation!");
		const auto ret = spiMaster_.startTransfer(nextTransfer->getWriteBuffer(), nextTransfer->getReadBuffer(),
				nextTransfer->getSize());
		if (ret != 0)
			notifyWaiter(ret);
	}
}

}	// namespace devices

}	// namespace distortos
