/**
 * \file
 * \brief SpiEepromProxy class implementation
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/SpiEepromProxy.hpp"

#include "distortos/devices/communication/SpiDeviceSelectGuard.hpp"
#include "distortos/devices/communication/SpiMasterProxy.hpp"

#include "distortos/devices/memory/SpiEeprom.hpp"

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiEepromProxy::SpiEepromProxy(SpiEeprom& spiEeprom) :
		spiDeviceProxy_{spiEeprom.spiDevice_},
		spiEeprom_{spiEeprom}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, size_t> SpiEepromProxy::executeTransaction(const SpiMasterOperationsRange operationsRange) const
{
	SpiMasterProxy spiMasterProxy {spiDeviceProxy_};

	{
		const auto ret = spiMasterProxy.configure(spiEeprom_.mode_, spiEeprom_.clockFrequency_, 8, false);
		if (ret.first != 0)
			return {ret.first, {}};
	}

	const SpiDeviceSelectGuard spiDeviceSelectGuard {spiMasterProxy};

	return spiMasterProxy.executeTransaction(operationsRange);
}

}	// namespace devices

}	// namespace distortos
