/**
 * \file
 * \brief Rs485 class implementation
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/Rs485.hpp"

#include "distortos/devices/io/OutputPin.hpp"

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Rs485::~Rs485()
{
	enableDriver(false);
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

void Rs485::transmitCompleteEvent()
{
	enableDriver(false);
	SerialPort::transmitCompleteEvent();
}

void Rs485::transmitStartEvent()
{
	enableDriver(true);
	SerialPort::transmitStartEvent();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void Rs485::enableDriver(const bool enable) const
{
	driverEnablePin_.set(enable == true ? driverEnabledState_ : !driverEnabledState_);
}

}	// namespace devices

}	// namespace distortos
