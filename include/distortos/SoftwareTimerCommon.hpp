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
 * \date 2015-11-28
 */

#ifndef INCLUDE_DISTORTOS_SOFTWARETIMERCOMMON_HPP_
#define INCLUDE_DISTORTOS_SOFTWARETIMERCOMMON_HPP_

#include "distortos/SoftwareTimer.hpp"

#include "distortos/internal/scheduler/SoftwareTimerControlBlock.hpp"

namespace distortos
{

/**
 * \brief SoftwareTimerCommon class implements common functionality of software timers
 *
 * \ingroup softwareTimers
 */

class SoftwareTimerCommon : public SoftwareTimer
{
public:

	/**
	 * \brief SoftwareTimerCommon's constructor
	 */

	SoftwareTimerCommon();

	/**
	 * \brief SoftwareTimerCommon's destructor
	 */

	virtual ~SoftwareTimerCommon() override;

	/**
	 * \return true if the timer is running, false otherwise
	 */

	virtual bool isRunning() const override;

	/**
	 * \brief Starts the timer.
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int start(TickClock::time_point timePoint) override;

	using SoftwareTimer::start;

	/**
	 * \brief Stops the timer.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int stop() override;

private:

	/// internal SoftwareTimerControlBlock object
	internal::SoftwareTimerControlBlock softwareTimerControlBlock_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SOFTWARETIMERCOMMON_HPP_
