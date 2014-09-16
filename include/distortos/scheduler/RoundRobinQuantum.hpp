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
 * \date 2014-09-16
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_ROUNDROBINQUANTUM_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_ROUNDROBINQUANTUM_HPP_

#include "distortos/scheduler/TickClock.hpp"

namespace distortos
{

namespace scheduler
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
			quantum_{quantumRawInitializer_}
	{}

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

	Duration get() const { return quantum_; }

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
		quantum_ = Duration{quantumRawInitializer_};
	}

private:

	static_assert(CONFIG_TICK_RATE_HZ > 0, "CONFIG_TICK_RATE_HZ must be positive and non-zero!");
	static_assert(CONFIG_ROUND_ROBIN_RATE_HZ > 0, "CONFIG_ROUND_ROBIN_RATE_HZ must be positive and non-zero!");

	/// raw initializer value for round-robin quantum, calculated with rounding to nearest
	constexpr static auto quantumRawInitializer_ = (CONFIG_TICK_RATE_HZ + CONFIG_ROUND_ROBIN_RATE_HZ / 2) /
			CONFIG_ROUND_ROBIN_RATE_HZ;

	static_assert(quantumRawInitializer_ > 0 || quantumRawInitializer_ <= UINT8_MAX,
			"CONFIG_TICK_RATE_HZ and CONFIG_ROUND_ROBIN_RATE_HZ values produce invalid round-robin quantum!");

	/// round-robin quantum
	Duration quantum_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_ROUNDROBINQUANTUM_HPP_
