/**
 * \file
 * \brief SemaphoreTryWaitUntilFunctor class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-12
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_SEMAPHORETRYWAITUNTILFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_SEMAPHORETRYWAITUNTILFUNCTOR_HPP_

#include "distortos/synchronization/SemaphoreFunctor.hpp"

#include "distortos/TickClock.hpp"

namespace distortos
{

namespace scheduler
{

/// SemaphoreTryWaitUntilFunctor class is a SemaphoreFunctor which calls Semaphore::tryWaitUntil() with bounded time
/// point
class SemaphoreTryWaitUntilFunctor : public synchronization::SemaphoreFunctor
{
public:

	/**
	 * \brief SemaphoreTryWaitUntilFunctor's constructor
	 *
	 * \param [in] timePoint is the bounded time point for Semaphore::tryWaitUntil() call
	 */

	constexpr explicit SemaphoreTryWaitUntilFunctor(const TickClock::time_point timePoint) :
			timePoint_{timePoint}
	{

	}

	/**
	 * \brief Calls Semaphore::tryWaitUntil() with bounded time point.
	 *
	 * \param [in] semaphore is a reference to Semaphore object for which Semaphore::tryWaitUntil() will be called
	 *
	 * \return value returned by Semaphore::tryWaitUntil()
	 */

	virtual int operator()(Semaphore& semaphore) const override;

private:

	/// bounded time point for Semaphore::tryWaitUntil() call
	const TickClock::time_point timePoint_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_SEMAPHORETRYWAITUNTILFUNCTOR_HPP_
