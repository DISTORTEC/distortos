/**
 * \file
 * \brief SoftwareTimer class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-29
 */

#ifndef INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_
#define INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_

#include "distortos/scheduler/SoftwareTimerControlBlock.hpp"

#include <functional>

namespace distortos
{

/**
 * \brief SoftwareTimer class is a templated interface for software timer
 *
 * \param Function is the function that will be executed
 * \param Args are the arguments for function
 */

template<typename Function, typename... Args>
class SoftwareTimer : private scheduler::SoftwareTimerControlBlock
{
public:

	/**
	 * \brief SoftwareTimer's constructor
	 *
	 * \param [in] function is a function that will be executed from interrupt context at a later time
	 * \param [in] args are arguments for function
	 */

	SoftwareTimer(Function&& function, Args&&... args) :
			SoftwareTimerControlBlock{},
			boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
	{

	}

	using SoftwareTimerControlBlock::isRunning;

	using SoftwareTimerControlBlock::start;

	using SoftwareTimerControlBlock::stop;

private:

	/**
	 * \brief Software timer's internal function.
	 *
	 * Executes bound function object.
	 */

	virtual void execute_() const override { boundFunction_(); }

	/// bound function object
	decltype(std::bind(std::declval<Function>(), std::declval<Args>()...)) boundFunction_;
};

/**
 * \brief Helper factory function to make SoftwareTimer object with deduced template arguments
 *
 * \param Function is the function that will be executed
 * \param Args are the arguments for function
 *
 * \param [in] function is a function that will be executed from interrupt context at a later time
 * \param [in] args are arguments for function
 *
 * \return SoftwareTimer object with deduced template arguments
 */

template<typename Function, typename... Args>
SoftwareTimer<Function, Args...> makeSoftwareTimer(Function&& function, Args&&... args)
{
	return {std::forward<Function>(function), std::forward<Args>(args)...};
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SOFTWARETIMER_HPP_
