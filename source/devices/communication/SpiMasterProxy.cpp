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
