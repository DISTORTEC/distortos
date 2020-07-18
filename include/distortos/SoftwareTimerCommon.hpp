/**
 * \file
 * \brief SoftwareTimerCommon class header
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

	constexpr SoftwareTimerCommon() :
			softwareTimerControlBlock_{softwareTimerRunner, *this}
	{

	}

	/**
	 * \brief SoftwareTimerCommon's destructor
	 */

	~SoftwareTimerCommon() override;

	/**
	 * \return true if the timer is running, false otherwise
	 */

	bool isRunning() const override;

	/**
	 * \brief Starts the timer.
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 * \param [in] period is the period used to restart repetitive software timer, 0 for one-shot software timers,
	 * default - 0
	 *
	 * \return 0 on success, error code otherwise
	 */

	int start(TickClock::time_point timePoint, TickClock::duration period = {}) override;

	using SoftwareTimer::start;

	/**
	 * \brief Stops the timer.
	 *
	 * \return 0 on success, error code otherwise
	 */

	int stop() override;

	SoftwareTimerCommon(const SoftwareTimerCommon&) = delete;
	SoftwareTimerCommon(SoftwareTimerCommon&&) = default;
	const SoftwareTimerCommon& operator=(const SoftwareTimerCommon&) = delete;
	SoftwareTimerCommon& operator=(SoftwareTimerCommon&&) = delete;

private:

	/// internal SoftwareTimerControlBlock object
	internal::SoftwareTimerControlBlock softwareTimerControlBlock_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SOFTWARETIMERCOMMON_HPP_
