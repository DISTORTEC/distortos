/**
 * \file
 * \brief SpiDeviceSelectGuard class implementation
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiDeviceSelectGuard.hpp"

#include "distortos/devices/communication/SpiDevice.hpp"
#include "distortos/devices/communication/SpiMasterHandle.hpp"

#include "distortos/devices/io/OutputPin.hpp"

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiDeviceSelectGuard::SpiDeviceSelectGuard(SpiMasterProxy& spiMasterProxy) :
		spiMasterProxy_{spiMasterProxy}
{
	getSpiDevice().slaveSelectPin_.set(false);
}

SpiDeviceSelectGuard::~SpiDeviceSelectGuard()
{
	getSpiDevice().slaveSelectPin_.set(true);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiDevice& SpiDeviceSelectGuard::getSpiDevice() const
{
	return spiMasterProxy_.getSpiDevice();
}

}	// namespace devices

}	// namespace distortos
