/**
 * \file
 * \brief SoftwareTimer class header
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

	virtual ~SoftwareTimer() = default;

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
	 * \param [in] period is the period used to restart repetitive software timer, 0 for one-shot software timers,
	 * default - 0
	 *
	 * \return 0 on success, error code otherwise
	 */

	int start(TickClock::duration duration, TickClock::duration period = {});

	/**
	 * \brief Starts the timer.
	 *
	 * \note The duration must not be shorter, so one additional tick is always added to the duration.
	 *
	 * \tparam Rep1 is type of tick counter used in \a duration
	 * \tparam Period1 is std::ratio type representing the tick period of the clock used in \a duration, seconds
	 * \tparam Rep2 is type of tick counter used in \a period
	 * \tparam Period2 is std::ratio type representing the tick period of the clock used in \a period, seconds
	 *
	 * \param [in] duration is the duration after which the function will be executed
	 * \param [in] period is the period used to restart repetitive software timer, 0 for one-shot software timers,
	 * default - 0
	 *
	 * \return 0 on success, error code otherwise
	 */

	template<typename Rep1, typename Period1, typename Rep2 = TickClock::rep, typename Period2 = TickClock::period>
	int start(const std::chrono::duration<Rep1, Period1> duration,
			const std::chrono::duration<Rep2, Period2> period = {})
	{
		return start(std::chrono::duration_cast<TickClock::duration>(duration),
				std::chrono::duration_cast<TickClock::duration>(period));
	}

	/**
	 * \brief Starts the timer.
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 * \param [in] period is the period used to restart repetitive software timer, 0 for one-shot software timers,
	 * default - 0
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int start(TickClock::time_point timePoint, TickClock::duration period = {}) = 0;

	/**
	 * \brief Starts the timer.
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 * \tparam Rep is type of tick counter used in \a period
	 * \tparam Period is std::ratio type representing the tick period of the clock used in \a period, seconds
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 * \param [in] period is the period used to restart repetitive software timer, 0 for one-shot software timers,
	 * default - 0
	 *
	 * \return 0 on success, error code otherwise
	 */

	template<typename Duration, typename Rep = TickClock::rep, typename Period = TickClock::period>
	int start(const std::chrono::time_point<TickClock, Duration> timePoint,
			const std::chrono::duration<Rep, Period> period = {})
	{
		return start(std::chrono::time_point_cast<TickClock::duration>(timePoint),
				std::chrono::duration_cast<TickClock::duration>(period));
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
