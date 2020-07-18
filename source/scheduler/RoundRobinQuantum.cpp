/**
 * \file
 * \brief RoundRobinQuantum class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/RoundRobinQuantum.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| private member variables
+---------------------------------------------------------------------------------------------------------------------*/

constexpr decltype(RoundRobinQuantum::quantumRawInitializer_) RoundRobinQuantum::quantumRawInitializer_;

}	// namespace internal

}	// namespace distortos
