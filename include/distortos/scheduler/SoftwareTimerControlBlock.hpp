/**
 * \file
 * \brief SoftwareTimerControlBlock class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-24
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCK_HPP_

#include "distortos/scheduler/SoftwareTimerControlBlockList-types.hpp"

#include "distortos/TickClock.hpp"

#include <array>

namespace distortos
{

namespace scheduler
{

class SoftwareTimerControlBlockList;

/// SoftwareTimerControlBlock class is a control block of software timer
class SoftwareTimerControlBlock
{
public:

	/// type of object used as storage for SoftwareTimerControlBlockList elements - 3 pointers
	using Link = std::array<std::aligned_storage<sizeof(void*), alignof(void*)>::type, 3>;

	/**
	 * \brief SoftwareTimerControlBlock's constructor
	 */

	SoftwareTimerControlBlock();

	/**
	 * \brief Execute software timer's function.
	 *
	 * Calls internal pure virtual execute_(), which should be provided by derived classes.
	 *
	 * \note this should only be called by SoftwareTimerSupervisor::tickInterruptHandler()
	 */

	void execute() const
	{
		execute_();
	}

	/**
	 * \return reference to internal storage for list link
	 */

	Link& getLink()
	{
		return link_;
	}

	/**
	 * \return const reference to expiration time point
	 */

	const TickClock::time_point& getTimePoint() const
	{
		return timePoint_;
	}

	/**
	 * \return true if the timer is running, false otherwise
	 */

	bool isRunning() const
	{
		return list_ != nullptr;
	}

	/**
	 * \brief Sets the list that has this object.
	 *
	 * \param [in] list is a pointer to list that has this object
	 */

	void setList(SoftwareTimerControlBlockList* const list)
	{
		list_ = list;
	}

	/**
	 * \brief Starts the timer.
	 *
	 * \note The duration will never be shorter, so one additional tick is always added to the duration.
	 *
	 * \param [in] duration is the duration after which the function will be executed
	 */

	void start(TickClock::duration duration);

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

	void start(TickClock::time_point timePoint);

	/**
	 * \brief Starts the timer.
	 *
	 * \param Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 */

	template<typename Duration>
	void start(std::chrono::time_point<TickClock, Duration> timePoint)
	{
		start(std::chrono::time_point_cast<TickClock::duration>(timePoint));
	}

	/**
	 * \brief Stops the timer.
	 */

	void stop();

protected:

	/**
	 * \brief SoftwareTimerControlBlock's destructor
	 *
	 * If the timer is running it is stopped.
	 */

	~SoftwareTimerControlBlock();

private:

	/**
	 * \brief Software timer's internal function.
	 *
	 * \note this should be provided by derived classes
	 */

	virtual void execute_() const = 0;

	///time point of expiration
	TickClock::time_point timePoint_;

	/// storage for list link
	Link link_;

	/// pointer to list that has this object
	SoftwareTimerControlBlockList* volatile list_;

	/// iterator of this object on the list, valid after it has been added to some list
	SoftwareTimerControlBlockListIterator iterator_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCK_HPP_
