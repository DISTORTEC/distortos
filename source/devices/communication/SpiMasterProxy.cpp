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
		spiDeviceProxy_{spiDeviceProxy}
{
	getSpiMaster().mutex_.lock();
}

SpiMasterProxy::~SpiMasterProxy()
{
	getSpiMaster().mutex_.unlock();
}

std::pair<int, uint32_t> SpiMasterProxy::configure(const SpiMode mode, const uint32_t clockFrequency,
		const uint8_t wordLength, const bool lsbFirst) const
{
	auto& spiMaster = getSpiMaster();
	if (spiDeviceProxy_.isOpened() == false || spiMaster.openCount_ == 0)
		return {EBADF, {}};

	return spiMaster.spiMaster_.configure(mode, clockFrequency, wordLength, lsbFirst);
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
	spiMaster.semaphore_ = &semaphore;
	spiMaster.operationsRange_ = operationsRange;
	spiMaster.ret_ = {};
	const auto cleanupScopeGuard = estd::makeScopeGuard(
			[&spiMaster]()
			{
				spiMaster.operationsRange_ = {};
				spiMaster.semaphore_ = {};
			});

	{
		const auto transfer = spiMaster.operationsRange_.begin()->getTransfer();
		assert(transfer != nullptr);
		const auto ret = spiMaster.spiMaster_.startTransfer(spiMaster, transfer->getWriteBuffer(),
				transfer->getReadBuffer(), transfer->getSize());
		if (ret != 0)
			return {ret, {}};
	}

	while (semaphore.wait() != 0);

	const auto handledOperations = spiMaster.operationsRange_.begin() - operationsRange.begin();
	return {spiMaster.ret_, handledOperations};
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

}	// namespace devices

}	// namespace distortos
