/**
 * \file
 * \brief DynamicSoftwareTimer class header
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICSOFTWARETIMER_HPP_
#define INCLUDE_DISTORTOS_DYNAMICSOFTWARETIMER_HPP_

#include "distortos/SoftwareTimerCommon.hpp"

#include <functional>

namespace distortos
{

/// \addtogroup softwareTimers
/// \{

/**
 * \brief DynamicSoftwareTimer class is a type-erased interface for software timer that has dynamic storage for bound
 * function.
 */

class DynamicSoftwareTimer : public SoftwareTimerCommon
{
public:

	/**
	 * \brief DynamicSoftwareTimer's constructor
	 *
	 * \tparam Function is the function that will be executed
	 * \tparam Args are the arguments for function
	 *
	 * \param [in] function is a function that will be executed from interrupt context at a later time
	 * \param [in] args are arguments for function
	 */

	template<typename Function, typename... Args>
	DynamicSoftwareTimer(Function&& function, Args&&... args);

private:

	/**
	 * \brief "Run" function of software timer
	 *
	 * Executes bound function object.
	 */

	void run() override;

	/// bound function object
	std::function<void()> boundFunction_;
};

/**
 * \brief Helper factory function to make DynamicSoftwareTimer object
 *
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for function
 *
 * \param [in] function is a function that will be executed from interrupt context at a later time
 * \param [in] args are arguments for function
 *
 * \return DynamicSoftwareTimer object
 */

template<typename Function, typename... Args>
DynamicSoftwareTimer makeDynamicSoftwareTimer(Function&& function, Args&&... args)
{
	return {std::forward<Function>(function), std::forward<Args>(args)...};
}

/// \}

template<typename Function, typename... Args>
DynamicSoftwareTimer::DynamicSoftwareTimer(Function&& function, Args&&... args) :
		SoftwareTimerCommon{},
		boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
{

}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICSOFTWARETIMER_HPP_
