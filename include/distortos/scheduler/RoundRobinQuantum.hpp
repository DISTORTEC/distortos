/**
 * \file
 * \brief RoundRobinQuantum class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-05
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_ROUNDROBINQUANTUM_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_ROUNDROBINQUANTUM_HPP_

#include <cstdint>

namespace distortos
{

namespace scheduler
{

/// RoundRobinQuantum class is a quantum of time for round-robin scheduling
class RoundRobinQuantum
{
public:

	/**
	 * \brief RoundRobinQuantum's constructor
	 *
	 * Initializes quantum value to 0.
	 */

	constexpr RoundRobinQuantum() :
			quantum_{}
	{}

	/**
	 * \brief Decrements round-robin's quantum.
	 *
	 * This function should be called from tick interrupt for the currently running thread. Underflow of quantum after
	 * this decrement is not possible.
	 *
	 * \note this function must be called with enabled interrupt masking
	 */

	void decrement();

	/**
	 * \brief Gets current value of round-robin's quantum.
	 *
	 * \return current value of round-robin's quantum of the thread
	 */

	uint8_t get() const { return quantum_; }

	/**
	 * \brief Resets value of round-robin's quantum.
	 *
	 * This function should be called from context switcher after selecting new task that will be run.
	 */

	void reset();

private:

	/// round-robin quantum
	uint8_t quantum_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_ROUNDROBINQUANTUM_HPP_
