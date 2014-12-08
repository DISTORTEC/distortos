/**
 * \file
 * \brief SemaphoreWaitFunctor class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-12-10
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SEMAPHOREWAITFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SEMAPHOREWAITFUNCTOR_HPP_

#include "distortos/scheduler/SemaphoreFunctor.hpp"

namespace distortos
{

namespace scheduler
{

/// SemaphoreWaitFunctor class is a SemaphoreFunctor which calls Semaphore::wait()
class SemaphoreWaitFunctor : public SemaphoreFunctor
{
public:

	/**
	 * \brief Calls Semaphore::wait()
	 *
	 * \param [in] semaphore is a reference to Semaphore object for which Semaphore::wait() will be called
	 *
	 * \return value returned by Semaphore::wait()
	 */

	virtual int operator()(Semaphore& semaphore) const override;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SEMAPHOREWAITFUNCTOR_HPP_
