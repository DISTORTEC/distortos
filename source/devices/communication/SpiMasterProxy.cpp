/**
 * \file
 * \brief SpiMasterProxy class implementation
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiMasterProxy.hpp"

#include "distortos/devices/communication/SpiDeviceProxy.hpp"
#include "distortos/devices/communication/SpiMaster.hpp"
#include "distortos/devices/communication/SpiMasterLowLevel.hpp"
#include "distortos/devices/communication/SpiMasterOperation.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/assert.h"
#include "distortos/Semaphore.hpp"

#include "estd/ScopeGuard.hpp"

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiMasterProxy::SpiMasterProxy(const SpiDeviceProxy& spiDeviceProxy) :
		operationsRange_{},
		spiDeviceProxy_{spiDeviceProxy},
		ret_{},
		semaphore_{}
{
	getSpiMaster().mutex_.lock();
}

SpiMasterProxy::~SpiMasterProxy()
{
	getSpiMaster().mutex_.unlock();
}

std::pair<int, uint32_t> SpiMasterProxy::configure(const SpiMode mode, const uint32_t clockFrequency,
		const uint8_t wordLength, const bool lsbFirst, const uint32_t dummyData) const
{
	auto& spiMaster = getSpiMaster();
	if (spiDeviceProxy_.isOpened() == false || spiMaster.openCount_ == 0)
		return {EBADF, {}};

	return spiMaster.spiMaster_.configure(mode, clockFrequency, wordLength, lsbFirst, dummyData);
}

std::pair<int, size_t> SpiMasterProxy::executeTransaction(const SpiMasterOperationsRange operationsRange)
{
	CHECK_FUNCTION_CONTEXT();

	if (operationsRange.size() == 0)
		return {EINVAL, {}};

	auto& spiMaster = getSpiMaster();
	if (spiDeviceProxy_.isOpened() == false || spiMaster.openCount_ == 0)
		return {EBADF, {}};

	Semaphore semaphore {0};
	semaphore_ = &semaphore;
	operationsRange_ = operationsRange;
	ret_ = {};
	const auto cleanupScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				operationsRange_ = {};
				semaphore_ = {};
			});

	{
		const auto transfer = operationsRange_.begin();
		const auto ret = spiMaster.spiMaster_.startTransfer(*this, transfer->getWriteBuffer(),
				transfer->getReadBuffer(), transfer->getSize());
		if (ret != 0)
			return {ret, {}};
	}

	while (semaphore.wait() != 0);

	const auto handledOperations = operationsRange_.begin() - operationsRange.begin();
	return {ret_, handledOperations};
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiDevice& SpiMasterProxy::getSpiDevice() const
{
	return spiDeviceProxy_.spiDevice_;
}

SpiMaster& SpiMasterProxy::getSpiMaster() const
{
	return spiDeviceProxy_.getSpiMaster();
}

void SpiMasterProxy::notifyWaiter(const int ret)
{
	ret_ = ret;
	const auto semaphore = semaphore_;
	assert(semaphore != nullptr);
	semaphore->post();
}

void SpiMasterProxy::transferCompleteEvent(SpiMasterErrorSet errorSet, size_t bytesTransfered)
{
	assert(operationsRange_.size() != 0 && "Invalid range of operations!");

	{
		const auto previousTransfer = operationsRange_.begin();
		previousTransfer->finalize(errorSet, bytesTransfered);
	}

	const auto error = errorSet.any();
	if (error == false)	// handling of last operation successful?
		operationsRange_ = {operationsRange_.begin() + 1, operationsRange_.end()};

	if (operationsRange_.size() == 0 || error == true)	// all operations are done or handling of last one failed?
	{
		notifyWaiter(error == false ? 0 : EIO);
		return;
	}

	{
		const auto nextTransfer = operationsRange_.begin();
		const auto ret = getSpiMaster().spiMaster_.startTransfer(*this, nextTransfer->getWriteBuffer(),
				nextTransfer->getReadBuffer(), nextTransfer->getSize());
		if (ret != 0)
			notifyWaiter(ret);
	}
}

}	// namespace devices

}	// namespace distortos
