/**
 * \file
 * \brief RoundRobinQuantum class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-23
 */

#include "distortos/scheduler/RoundRobinQuantum.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| private member variables
+---------------------------------------------------------------------------------------------------------------------*/

constexpr decltype(RoundRobinQuantum::quantumRawInitializer_) RoundRobinQuantum::quantumRawInitializer_;

}	// namespace scheduler

}	// namespace distortos
