/**
 * \file
 * \brief SemaphoreTryWaitFunctor class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SEMAPHORETRYWAITFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SEMAPHORETRYWAITFUNCTOR_HPP_

#include "distortos/internal/synchronization/SemaphoreFunctor.hpp"

namespace distortos
{

namespace internal
{

/// SemaphoreTryWaitFunctor class is a SemaphoreFunctor which calls Semaphore::tryWait()
class SemaphoreTryWaitFunctor : public SemaphoreFunctor
{
public:

	/**
	 * \brief Calls Semaphore::tryWait()
	 *
	 * \param [in] semaphore is a reference to Semaphore object for which Semaphore::tryWait() will be called
	 *
	 * \return value returned by Semaphore::tryWait()
	 */

	int operator()(Semaphore& semaphore) const override;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SEMAPHORETRYWAITFUNCTOR_HPP_
