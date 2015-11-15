/**
 * \file
 * \brief SoftwareTimerCommon class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-15
 */

#ifndef INCLUDE_DISTORTOS_SOFTWARETIMERCOMMON_HPP_
#define INCLUDE_DISTORTOS_SOFTWARETIMERCOMMON_HPP_

#include "distortos/SoftwareTimer.hpp"

namespace distortos
{

/// SoftwareTimerCommon class implements common functionality of software timers
class SoftwareTimerCommon : public SoftwareTimer
{
public:

	/**
	 * \brief SoftwareTimerCommon's constructor
	 */

	SoftwareTimerCommon();
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SOFTWARETIMERCOMMON_HPP_
