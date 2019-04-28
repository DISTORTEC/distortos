/**
 * \file
 * \brief SpiMasterOperationRange type alias header
 * \deprecated scheduled to be removed after v0.7.0, use distortos/devices/communication/SpiMasterTransfersRange.hpp
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATIONRANGE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATIONRANGE_HPP_

#warning "'distortos/devices/communication/SpiMasterOperationRange.hpp' is deprecated: Use SpiMasterTransfersRange"

#include "distortos/devices/communication/SpiMasterTransfersRange.hpp"

namespace distortos
{

namespace devices
{

/**
 * SpiMasterOperationRange is an alias for ContiguousRange of SpiMasterOperation elements
 *
 * \deprecated scheduled to be removed after v0.7.0, use distortos::devices::SpiMasterTransfersRange
 */

using SpiMasterOperationRange __attribute__ ((deprecated("Use SpiMasterTransfersRange"))) = SpiMasterTransfersRange;

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATIONRANGE_HPP_
