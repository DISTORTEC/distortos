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
#include "distortos/devices/communication/SpiDeviceProxy.hpp"
#include "distortos/devices/communication/SpiDeviceSelectGuard.hpp"
#include "distortos/devices/communication/SpiMasterLowLevel.hpp"
#include "distortos/devices/communication/SpiMasterProxy.hpp"
#include "distortos/devices/communication/SpiMasterTransfer.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

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

	const std::lock_guard<Mutex> lockGuard {mutex_};

	spiMaster_.stop();
}

int SpiMaster::close()
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

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

std::pair<int, size_t> SpiMaster::executeTransaction(SpiDevice& device, const SpiMasterTransfersRange transfersRange)
{
	CHECK_FUNCTION_CONTEXT();

	if (transfersRange.size() == 0)
		return {EINVAL, {}};

	const SpiDevice::Proxy spiDeviceProxy {device};
	Proxy proxy {spiDeviceProxy};

	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

		const auto ret = proxy.configure(device.getMode(), device.getMaxClockFrequency(), device.getWordLength(),
				device.getLsbFirst(), {});

#pragma GCC diagnostic pop

		if (ret.first != 0)
			return {ret.first, {}};
	}

	const SpiDeviceSelectGuard spiDeviceSelectGuard {proxy};

	return proxy.executeTransaction(transfersRange);
}

int SpiMaster::open()
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	if (openCount_ == std::numeric_limits<decltype(openCount_)>::max())	// device is already opened too many times?
		return EMFILE;

	if (openCount_ == 0)	// first open?
	{
		const auto ret = spiMaster_.start();
		if (ret != 0)
			return ret;
	}

	++openCount_;
	return 0;
}

}	// namespace devices

}	// namespace distortos
