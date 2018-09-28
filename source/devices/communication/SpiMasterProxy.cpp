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
#include "distortos/devices/communication/SpiMasterTransfer.hpp"

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
		transfersRange_{},
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

std::pair<int, size_t> SpiMasterProxy::executeTransaction(const SpiMasterTransfersRange transfersRange)
{
	CHECK_FUNCTION_CONTEXT();

	if (transfersRange.size() == 0)
		return {EINVAL, {}};

	auto& spiMaster = getSpiMaster();
	if (spiDeviceProxy_.isOpened() == false || spiMaster.openCount_ == 0)
		return {EBADF, {}};

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
		const auto ret = spiMaster.spiMaster_.startTransfer(*this, transfer->getWriteBuffer(),
				transfer->getReadBuffer(), transfer->getSize());
		if (ret != 0)
			return {ret, {}};
	}

	while (semaphore.wait() != 0);

	const auto handledTransfers = transfersRange_.begin() - transfersRange.begin();
	return {ret_, handledTransfers};
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
	assert(transfersRange_.size() != 0 && "Invalid range of transfers!");

	{
		const auto previousTransfer = transfersRange_.begin();
		previousTransfer->finalize(errorSet, bytesTransfered);
	}

	const auto error = errorSet.any();
	if (error == false)	// handling of last transfer successful?
		transfersRange_ = {transfersRange_.begin() + 1, transfersRange_.end()};

	if (transfersRange_.size() == 0 || error == true)	// all transfers are done or handling of last one failed?
	{
		notifyWaiter(error == false ? 0 : EIO);
		return;
	}

	{
		const auto nextTransfer = transfersRange_.begin();
		const auto ret = getSpiMaster().spiMaster_.startTransfer(*this, nextTransfer->getWriteBuffer(),
				nextTransfer->getReadBuffer(), nextTransfer->getSize());
		if (ret != 0)
			notifyWaiter(ret);
	}
}

}	// namespace devices

}	// namespace distortos
