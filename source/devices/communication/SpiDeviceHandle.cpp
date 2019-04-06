/**
 * \file
 * \brief SpiDeviceProxy class implementation
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiDeviceHandle.hpp"

#include "distortos/devices/communication/SpiDevice.hpp"

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiDeviceProxy::SpiDeviceProxy(SpiDevice& spiDevice) :
		spiDevice_{spiDevice}
{
	spiDevice_.lock();
}

SpiDeviceProxy::~SpiDeviceProxy()
{
	spiDevice_.unlock();
}

bool SpiDeviceProxy::isOpened() const
{
	return spiDevice_.openCount_ != 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiMaster& SpiDeviceProxy::getSpiMaster() const
{
	return spiDevice_.spiMaster_;
}

}	// namespace devices

}	// namespace distortos
