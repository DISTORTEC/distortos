/**
 * \file
 * \brief statistics namespace header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATISTICS_HPP_
#define INCLUDE_DISTORTOS_STATISTICS_HPP_

#include <cstdint>

namespace distortos
{

namespace statistics
{

/// \addtogroup statistics
/// \{

/**
 * \return number of context switches
 */

uint64_t getContextSwitchCount();

/// \}

}	// namespace statistics

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATISTICS_HPP_
