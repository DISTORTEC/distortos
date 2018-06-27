/**
 * \file
 * \brief SpiMasterOperationsRange type alias header
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATIONSRANGE_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATIONSRANGE_HPP_

#include "estd/ContiguousRange.hpp"

namespace distortos
{

namespace devices
{

class SpiMasterOperation;

/**
 * SpiMasterOperationsRange is an alias for ContiguousRange of SpiMasterOperation elements
 *
 * \ingroup devices
 */

using SpiMasterOperationsRange = estd::ContiguousRange<SpiMasterOperation>;

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTEROPERATIONSRANGE_HPP_
