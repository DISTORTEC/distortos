/**
 * \file
 * \brief StaticSoftwareTimer class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATICSOFTWARETIMER_HPP_
#define INCLUDE_DISTORTOS_STATICSOFTWARETIMER_HPP_

#include "distortos/SoftwareTimerCommon.hpp"

#include <functional>

namespace distortos
{

/// \addtogroup softwareTimers
/// \{

/**
 * \brief StaticSoftwareTimer class is a templated interface for software timer
 *
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for function
 */

template<typename Function, typename... Args>
class StaticSoftwareTimer : public SoftwareTimerCommon
{
public:

	/**
	 * \brief StaticSoftwareTimer's constructor
	 *
	 * \param [in] function is a function that will be executed from interrupt context at a later time
	 * \param [in] args are arguments for function
	 */

	StaticSoftwareTimer(Function&& function, Args&&... args) :
			SoftwareTimerCommon{},
			boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
	{

	}

private:

	/**
	 * \brief "Run" function of software timer
	 *
	 * Executes bound function object.
	 */

	void run() override
	{
		boundFunction_();
	}

	/// bound function object
	decltype(std::bind(std::declval<Function>(), std::declval<Args>()...)) boundFunction_;
};

/**
 * \brief Helper factory function to make StaticSoftwareTimer object with deduced template arguments
 *
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for function
 *
 * \param [in] function is a function that will be executed from interrupt context at a later time
 * \param [in] args are arguments for function
 *
 * \return StaticSoftwareTimer object with deduced template arguments
 */

template<typename Function, typename... Args>
StaticSoftwareTimer<Function, Args...> makeStaticSoftwareTimer(Function&& function, Args&&... args)
{
	return {std::forward<Function>(function), std::forward<Args>(args)...};
}

/// \}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICSOFTWARETIMER_HPP_
