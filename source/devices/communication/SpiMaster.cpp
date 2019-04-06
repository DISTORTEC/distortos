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
