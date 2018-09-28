/**
 * \file
 * \brief SpiMasterOperation type alias header
 * \deprecated scheduled to be removed after v0.7.0, use SpiMasterTransfer
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATION_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATION_HPP_

#warning "'distortos/devices/communication/SpiMasterOperation.hpp' is deprecated: Use SpiMasterTransfer"

#include "distortos/devices/communication/SpiMasterTransfer.hpp"

namespace distortos
{

namespace devices
{

/**
 * SpiMasterOperation is an alias for SpiMasterTransfer.
 *
 * \deprecated scheduled to be removed after v0.7.0, use SpiMasterTransfer
 */

using SpiMasterOperation __attribute__ ((deprecated("Use SpiMasterTransfer"))) = SpiMasterTransfer;

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATION_HPP_
