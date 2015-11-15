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
 * \date 2015-11-15
 */

#ifndef INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_
#define INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_

#include "distortos/scheduler/SoftwareTimerControlBlock.hpp"

namespace distortos
{

/// SoftwareTimer is a base for software timers
class SoftwareTimer
{
public:

	/**
	 * \brief SoftwareTimer's constructor
	 */

	SoftwareTimer() :
			softwareTimerControlBlock_{softwareTimerRunner, *this}
	{

	}

	/**
	 * \return true if the timer is running, false otherwise
	 */

	bool isRunning() const;

	/**
	 * \brief Starts the timer.
	 *
	 * \note The duration will never be shorter, so one additional tick is always added to the duration.
	 *
	 * \param [in] duration is the duration after which the function will be executed
	 */

	void start(const TickClock::duration duration)
	{
		softwareTimerControlBlock_.start(duration);
	}

	/**
	 * \brief Starts the timer.
	 *
	 * \note The duration must not be shorter, so one additional tick is always added to the duration.
	 *
	 * \param Rep is type of tick counter
	 * \param Period is std::ratio type representing the tick period of the clock, in seconds
	 *
	 * \param [in] duration is the duration after which the function will be executed
	 */

	template<typename Rep, typename Period>
	void start(const std::chrono::duration<Rep, Period> duration)
	{
		start(std::chrono::duration_cast<TickClock::duration>(duration));
	}

	/**
	 * \brief Starts the timer.
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 */

	void start(const TickClock::time_point timePoint)
	{
		softwareTimerControlBlock_.start(timePoint);
	}

	/**
	 * \brief Starts the timer.
	 *
	 * \param Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 */

	template<typename Duration>
	void start(const std::chrono::time_point<TickClock, Duration> timePoint)
	{
		start(std::chrono::time_point_cast<TickClock::duration>(timePoint));
	}

	/**
	 * \brief Stops the timer.
	 */

	void stop()
	{
		softwareTimerControlBlock_.stop();
	}

protected:

	/**
	 * \brief SoftwareTimer's destructor
	 *
	 * \note Polymorphic objects of SoftwareTimer type must not be deleted via pointer/reference
	 */

	~SoftwareTimer()
	{

	}

private:

	/**
	 * \brief "Run" function of software timer
	 *
	 * This should be overridden by derived classes.
	 */

	virtual void run() = 0;

	/**
	 * \brief Software timer's function runner
	 *
	 * \param [in] softwareTimer is a reference to SoftwareTimer object that is being run
	 */

	static void softwareTimerRunner(SoftwareTimer& softwareTimer);

	/// internal SoftwareTimerControlBlock object
	scheduler::SoftwareTimerControlBlock softwareTimerControlBlock_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_
