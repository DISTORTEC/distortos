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
 * \date 2015-12-05
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERCONTROLBLOCK_HPP_

#include "distortos/internal/scheduler/SoftwareTimerListNode.hpp"
#include "distortos/internal/scheduler/SoftwareTimerList-types.hpp"

#include <array>

namespace distortos
{

class SoftwareTimer;

namespace internal
{

class SoftwareTimerList;

/// SoftwareTimerControlBlock class is a control block of software timer
class SoftwareTimerControlBlock : public SoftwareTimerListNode
{
public:

	/// type of object used as storage for SoftwareTimerList elements - 3 pointers
	using Link = std::array<std::aligned_storage<sizeof(void*), alignof(void*)>::type, 3>;

	/// type of runner for software timer's function
	using FunctionRunner = void(SoftwareTimer&);

	/**
	 * \brief SoftwareTimerControlBlock's constructor
	 *
	 * \param [in] functionRunner is a reference to runner for software timer's function
	 * \param [in] owner is a reference to SoftwareTimer object that owns this SoftwareTimerControlBlock
	 */

	SoftwareTimerControlBlock(FunctionRunner& functionRunner, SoftwareTimer& owner);

	/**
	 * \brief SoftwareTimerControlBlock's destructor
	 *
	 * If the timer is running it is stopped.
	 */

	~SoftwareTimerControlBlock();

	/**
	 * \return reference to internal storage for list link
	 */

	Link& getLink()
	{
		return link_;
	}

	/**
	 * \return true if the timer is running, false otherwise
	 */

	bool isRunning() const
	{
		return list_ != nullptr;
	}

	/**
	 * \brief Runs software timer's function.
	 *
	 * \note this should only be called by SoftwareTimerSupervisor::tickInterruptHandler()
	 */

	void run() const;

	/**
	 * \brief Sets the list that has this object.
	 *
	 * \param [in] list is a pointer to list that has this object
	 */

	void setList(SoftwareTimerList* const list)
	{
		list_ = list;
	}

	/**
	 * \brief Starts the timer.
	 *
	 * \param [in] timePoint is the time point at which the function will be executed
	 */

	void start(TickClock::time_point timePoint);

	/**
	 * \brief Stops the timer.
	 */

	void stop();

private:

	/// storage for list link
	Link link_;

	/// reference to runner for software timer's function
	FunctionRunner& functionRunner_;

	/// reference to SoftwareTimer object that owns this SoftwareTimerControlBlock
	SoftwareTimer& owner_;

	/// pointer to list that has this object
	SoftwareTimerList* volatile list_;

	/// iterator of this object on the list, valid after it has been added to some list
	SoftwareTimerListIterator iterator_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERCONTROLBLOCK_HPP_
