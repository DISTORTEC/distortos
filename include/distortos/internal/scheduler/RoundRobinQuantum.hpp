/**
 * \file
 * \brief RoundRobinQuantum class header
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_ROUNDROBINQUANTUM_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_ROUNDROBINQUANTUM_HPP_

#include "distortos/TickClock.hpp"

namespace distortos
{

namespace internal
{

/// RoundRobinQuantum class is a quantum of time for round-robin scheduling
class RoundRobinQuantum
{
public:

	/// type of quantum counter
	using Representation = uint8_t;

	/// duration type used for quantum
	using Duration = std::chrono::duration<Representation, TickClock::period>;

	/**
	 * \return initial value for round-robin quantum
	 */

	constexpr static Duration getInitial()
	{
		return Duration{quantumRawInitializer_};
	}

	/**
	 * \brief RoundRobinQuantum's constructor
	 *
	 * Initializes quantum value to initial value - just like after call to reset().
	 */

	constexpr RoundRobinQuantum() :
			quantum_{getInitial()}
	{

	}

	/**
	 * \brief Decrements round-robin's quantum.
	 *
	 * This function should be called from tick interrupt for the currently running thread. Underflow of quantum after
	 * this decrement is not possible.
	 *
	 * \note this function must be called with enabled interrupt masking
	 */

	void decrement()
	{
		if (isZero() == false)
			--quantum_;
	}

	/**
	 * \brief Gets current value of round-robin's quantum.
	 *
	 * \return current value of round-robin's quantum of the thread
	 */

	Duration get() const
	{
		return quantum_;
	}

	/**
	 * \brief Convenience function to test whether the quantum is already at 0.
	 *
	 * \return true if quantum is zero, false otherwise
	 */

	bool isZero() const
	{
		return quantum_ == Duration{0};
	}

	/**
	 * \brief Resets value of round-robin's quantum.
	 *
	 * This function should be called from context switcher after selecting new task that will be run.
	 */

	void reset()
	{
		quantum_ = getInitial();
	}

private:

	static_assert(DISTORTOS_TICK_FREQUENCY > 0, "DISTORTOS_TICK_FREQUENCY must be positive and non-zero!");
	static_assert(DISTORTOS_ROUND_ROBIN_FREQUENCY > 0,
			"DISTORTOS_ROUND_ROBIN_FREQUENCY must be positive and non-zero!");

	/// raw initializer value for round-robin quantum, calculated with rounding to nearest
	constexpr static auto quantumRawInitializer_ = (DISTORTOS_TICK_FREQUENCY + DISTORTOS_ROUND_ROBIN_FREQUENCY / 2) /
			DISTORTOS_ROUND_ROBIN_FREQUENCY;

	static_assert(quantumRawInitializer_ > 0 || quantumRawInitializer_ <= UINT8_MAX,
			"DISTORTOS_TICK_FREQUENCY and DISTORTOS_ROUND_ROBIN_FREQUENCY values produce invalid round-robin quantum!");

	/// round-robin quantum
	Duration quantum_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_ROUNDROBINQUANTUM_HPP_
