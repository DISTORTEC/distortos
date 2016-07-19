/**
 * \file
 * \brief SpiDevice class implementation
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiDevice.hpp"

#include "distortos/devices/communication/SpiMaster.hpp"
#include "distortos/devices/communication/SpiMasterOperation.hpp"

#include "estd/ScopeGuard.hpp"

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiDevice::~SpiDevice()
{
	if (openCount_ == 0)
		return;

	mutex_.lock();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				mutex_.unlock();
			});

	spiMaster_.close();
}

int SpiDevice::close()
{
	mutex_.lock();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				mutex_.unlock();
			});

	if (openCount_ == 0)	// device is not open anymore?
		return EBADF;

	if (openCount_ == 1)	// last close?
	{
		const auto ret = spiMaster_.close();
		if (ret != 0)
			return ret;
	}

	--openCount_;
	return 0;
}

std::pair<int, size_t> SpiDevice::executeTransaction(const SpiMasterOperationRange operationRange)
{
	if (operationRange.size() == 0)
		return {EINVAL, {}};

	mutex_.lock();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				mutex_.unlock();
			});

	return spiMaster_.executeTransaction(*this, operationRange);
}

int SpiDevice::open()
{
	mutex_.lock();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				mutex_.unlock();
			});

	if (openCount_ == std::numeric_limits<decltype(openCount_)>::max())	// device is already opened too many times?
		return EMFILE;

	if (openCount_ == 0)	// first open?
	{
		const auto ret = spiMaster_.open();
		if (ret != 0)
			return ret;
	}

	++openCount_;
	return 0;
}

}	// namespace devices

}	// namespace distortos
