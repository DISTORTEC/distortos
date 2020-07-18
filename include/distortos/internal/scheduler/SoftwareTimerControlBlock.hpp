/**
 * \file
 * \brief SoftwareTimerControlBlock class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERCONTROLBLOCK_HPP_

#include "distortos/internal/scheduler/SoftwareTimerListNode.hpp"

namespace distortos
{

class SoftwareTimer;

namespace internal
{

class SoftwareTimerSupervisor;

/// SoftwareTimerControlBlock class is a control block of software timer
class SoftwareTimerControlBlock : public SoftwareTimerListNode
{
public:

	/// type of runner for software timer's function
	using FunctionRunner = void(SoftwareTimer&);

	/**
	 * \brief SoftwareTimerControlBlock's constructor
	 *
	 * \param [in] functionRunner is a reference to runner for software timer's function
	 * \param [in] owner is a reference to SoftwareTimer object that owns this SoftwareTimerControlBlock
	 */

	constexpr SoftwareTimerControlBlock(FunctionRunner& functionRunner, SoftwareTimer& owner) :
			SoftwareTimerListNode{},
			period_{},
			functionRunner_{functionRunner},
			owner_{owner}
	{

	}

	/**
	 * \brief SoftwareTimerControlBlock's destructor
	 *
	 * If the timer is running it is stopped.
	 */

	~SoftwareTimerControlBlock()
	{
		stop();
	}

	/**
	 * \return true if the timer is running, false otherwise
	 */

	bool isRunning() const
	{
		asm("" ::: "memory");	// required for LTO
		return node.isLinked() != false || period_ != decltype(period_){};
	}

	/**
	 * \brief Runs software timer's function.
	 *
	 * \note this should only be called by SoftwareTimerSupervisor::tickInterruptHandler()
	 *
	 * \param [in] supervisor is a reference to SoftwareTimerSupervisor that manages this object
	 */

	void run(SoftwareTimerSupervisor& supervisor);

	/**
	 * \brief Starts the timer.
	 *
	 * \param [in] supervisor is a reference to SoftwareTimerSupervisor to which this object will be added
	 * \param [in] timePoint is the time point at which the function will be executed
	 * \param [in] period is the period used to restart repetitive software timer, 0 for one-shot software timers
	 */

	void start(SoftwareTimerSupervisor& supervisor, TickClock::time_point timePoint, TickClock::duration period);

	/**
	 * \brief Stops the timer.
	 */

	void stop();

	SoftwareTimerControlBlock(const SoftwareTimerControlBlock&) = delete;
	SoftwareTimerControlBlock(SoftwareTimerControlBlock&&) = default;
	const SoftwareTimerControlBlock& operator=(const SoftwareTimerControlBlock&) = delete;
	SoftwareTimerControlBlock& operator=(SoftwareTimerControlBlock&&) = delete;

private:

	/**
	 * \brief Starts the timer - internal version, with no interrupt masking, no stopping and no configuration of
	 * period.
	 *
	 * \param [in] supervisor is a reference to SoftwareTimerSupervisor to which this object will be added
	 * \param [in] timePoint is the time point at which the function will be executed
	 */

	void startInternal(SoftwareTimerSupervisor& supervisor, TickClock::time_point timePoint);

	/**
	 * \brief Stops the timer - internal version, with no interrupt masking.
	 */

	void stopInternal();

	/// period used to restart repetitive software timer, 0 for one-shot software timers
	TickClock::duration period_;

	/// reference to runner for software timer's function
	FunctionRunner& functionRunner_;

	/// reference to SoftwareTimer object that owns this SoftwareTimerControlBlock
	SoftwareTimer& owner_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERCONTROLBLOCK_HPP_
