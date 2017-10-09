/**
 * \file
 * \brief DynamicSoftwareTimer class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
 * \brief DynamicSoftwareTimer class is a Timer allowing a (void) function
 * to run at a later time or at specified intervals.
 * In contrast to StaticSoftwareTimer, DynamicSoftwareTimer is not a class template.
 */
class DynamicSoftwareTimer : public SoftwareTimerCommon
{
public:
	/**
	 * \brief DynamicSoftwareTimer's constructor
	 *
	 * \param [in] function is a function that will be executed from interrupt context at a later time
	 * \param [in] args are arguments for function
	 * \tparam Function is the function that will be executed
	 * \tparam Args are the arguments for function
	 */
	template<typename Function, typename... Args>
	DynamicSoftwareTimer(Function&& function, Args&&... args) :
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
	std::function<void(void)>	boundFunction_;
};

/// \}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICSOFTWARETIMER_HPP_
