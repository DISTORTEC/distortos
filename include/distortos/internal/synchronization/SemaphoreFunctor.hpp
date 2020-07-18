/**
 * \file
 * \brief SemaphoreFunctor class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SEMAPHOREFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SEMAPHOREFUNCTOR_HPP_

#include "estd/TypeErasedFunctor.hpp"

namespace distortos
{

class Semaphore;

namespace internal
{

/**
 * \brief SemaphoreFunctor is a type-erased interface for functors which execute some action on semaphore (wait(),
 * tryWait(), tryWaitFor(), tryWaitUntil(), ...).
 *
 * The functor will be called with one argument - \a semaphore - which is a reference to Semaphore object on which the
 * action will be executed. Functor's operator should return 0 if the action was executed successfully, error code
 * otherwise.
 */

class SemaphoreFunctor : public estd::TypeErasedFunctor<int(Semaphore&)>
{

};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SEMAPHOREFUNCTOR_HPP_
