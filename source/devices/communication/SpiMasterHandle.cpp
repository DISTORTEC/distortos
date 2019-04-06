/**
 * \file
 * \brief SpiMasterHandle class implementation
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiMasterHandle.hpp"

#include "distortos/devices/communication/SpiMasterLowLevel.hpp"

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, uint32_t> SpiMasterHandle::configure(const SpiMode mode, const uint32_t clockFrequency,
		const uint8_t wordLength, const bool lsbFirst, const uint32_t dummyData) const
{
	if (spiMaster_.openCount_ == 0)
		return {EBADF, {}};

	return spiMaster_.spiMaster_.configure(mode, clockFrequency, wordLength, lsbFirst, dummyData);
}

}	// namespace devices

}	// namespace distortos
