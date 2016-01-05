/**
 * \file
 * \brief SoftwareTimer class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_
#define INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_

#include "distortos/TickClock.hpp"

namespace distortos
{

/**
 * \brief SoftwareTimer class is an abstract interface for software timers
 *
 * \ingroup softwareTimers
 */

class SoftwareTimer
{
public:

	/**
	 * \brief SoftwareTimer's destructor
	 */

	virtual ~SoftwareTimer() = 0;

	/**
	 * \return true if the timer is running, false otherwise
	 */

	virtual bool isRunning() const = 0;

	/**
	 * \brief Starts the timer.
	 *
	 * \note The duration will never be shorter, so one additional tick is always added to the duration.
	 *
	 * \param [in] duration is the duration after which the function will be executed
	 *
	 * \return 0 on success, error code otherwise
	 */

	int start(TickClock::duration duration);

	/**
	 * \brief Starts the timer.
	 *
	 * \note The duration must not be shorter, so one additional tick is always added to the duration.
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, in seconds
	 *
	 * \param [in] duration is the duration after which the function will be executed
	 *
	 * \return 0 on success, error code otherwise
	 */

	template<typename Rep, typename Period>
	int start(const std::chrono::duration<Rep, Period> duration)
	{
		return start(std::chrono::duration_cast<TickClock::duration>(duration));
	}

	/**
	 * \brief Starts the timer.
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int start(TickClock::time_point timePoint) = 0;

	/**
	 * \brief Starts the timer.
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 *
	 * \return 0 on success, error code otherwise
	 */

	template<typename Duration>
	int start(const std::chrono::time_point<TickClock, Duration> timePoint)
	{
		return start(std::chrono::time_point_cast<TickClock::duration>(timePoint));
	}

	/**
	 * \brief Stops the timer.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int stop() = 0;

protected:

	/**
	 * \brief Software timer's function runner
	 *
	 * \param [in] softwareTimer is a reference to SoftwareTimer object that is being run
	 */

	static void softwareTimerRunner(SoftwareTimer& softwareTimer);

private:

	/**
	 * \brief "Run" function of software timer
	 *
	 * This should be overridden by derived classes.
	 */

	virtual void run() = 0;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_
