/**
 * \file
 * \brief SpiMasterTransfersRange type alias header
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERTRANSFERSRANGE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERTRANSFERSRANGE_HPP_

#include "estd/ContiguousRange.hpp"

namespace distortos
{

namespace devices
{

class SpiMasterTransfer;

/**
 * \brief SpiMasterTransfersRange is an alias for ContiguousRange of SpiMasterTransfer elements.
 *
 * \ingroup devices
 */

using SpiMasterTransfersRange = estd::ContiguousRange<const SpiMasterTransfer>;

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERTRANSFERSRANGE_HPP_
