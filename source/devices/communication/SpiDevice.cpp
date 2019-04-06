/**
 * \file
 * \brief SpiDevice class implementation
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiDevice.hpp"

#include "distortos/devices/communication/SpiDeviceHandle.hpp"
#include "distortos/devices/communication/SpiDeviceSelectGuard.hpp"
#include "distortos/devices/communication/SpiMaster.hpp"
#include "distortos/devices/communication/SpiMasterHandle.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include <cerrno>

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiDevice::~SpiDevice()
{
	CHECK_FUNCTION_CONTEXT();

	if (openCount_ == 0)
		return;

	const SpiDeviceHandle handle {*this};

	spiMaster_.close();
}

int SpiDevice::close()
{
	const SpiDeviceHandle handle {*this};

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

std::pair<int, size_t> SpiDevice::executeTransaction(const SpiMasterTransfersRange transfersRange)
{
	const SpiDeviceHandle handle {*this};
	SpiMasterHandle spiMasterHandle {handle};

	{
		const auto ret = spiMasterHandle.configure(mode_, maxClockFrequency_, wordLength_, lsbFirst_, {});
		if (ret.first != 0)
			return {ret.first, {}};
	}

	const SpiDeviceSelectGuard spiDeviceSelectGuard {spiMasterHandle};

	return spiMasterHandle.executeTransaction(transfersRange);
}

int SpiDevice::open()
{
	const SpiDeviceHandle handle {*this};

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
