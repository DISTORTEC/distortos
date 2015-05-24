/**
 * \file
 * \brief SoftwareTimerBase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-24
 */

#ifndef INCLUDE_DISTORTOS_SOFTWARETIMERBASE_HPP_
#define INCLUDE_DISTORTOS_SOFTWARETIMERBASE_HPP_

#include "distortos/scheduler/SoftwareTimerControlBlock.hpp"

namespace distortos
{

/// SoftwareTimerBase is a base for software timers
class SoftwareTimerBase : private scheduler::SoftwareTimerControlBlock
{
public:

	/**
	 * \brief SoftwareTimerBase's constructor
	 */

	SoftwareTimerBase() :
			SoftwareTimerControlBlock{}
	{

	}

	using SoftwareTimerControlBlock::isRunning;

	using SoftwareTimerControlBlock::start;

	using SoftwareTimerControlBlock::stop;

protected:

	/**
	 * \brief SoftwareTimerBase's destructor
	 *
	 * \note Polymorphic objects of SoftwareTimerBase type must not be deleted via pointer/reference
	 */

	~SoftwareTimerBase()
	{

	}
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SOFTWARETIMERBASE_HPP_
