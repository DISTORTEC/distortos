/**
 * \file
 * \brief SoftwareTimer class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-14
 */

#ifndef INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_
#define INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_

#include "distortos/scheduler/SoftwareTimerControlBlock.hpp"

namespace distortos
{

/// SoftwareTimer is a base for software timers
class SoftwareTimer : private scheduler::SoftwareTimerControlBlock
{
public:

	/**
	 * \brief SoftwareTimer's constructor
	 */

	SoftwareTimer() :
			SoftwareTimerControlBlock{*this}
	{

	}

	using SoftwareTimerControlBlock::isRunning;

	using SoftwareTimerControlBlock::start;

	using SoftwareTimerControlBlock::stop;

protected:

	/**
	 * \brief SoftwareTimer's destructor
	 *
	 * \note Polymorphic objects of SoftwareTimer type must not be deleted via pointer/reference
	 */

	~SoftwareTimer()
	{

	}
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_
