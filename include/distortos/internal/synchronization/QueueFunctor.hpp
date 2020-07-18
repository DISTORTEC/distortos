/**
 * \file
 * \brief QueueFunctor class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_QUEUEFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_QUEUEFUNCTOR_HPP_

#include "estd/TypeErasedFunctor.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief QueueFunctor is a type-erased interface for functors which execute some action on queue's storage (like
 * copy-constructing, swapping, destroying, emplacing, ...).
 *
 * The functor will be called by queue internals with one argument - \a storage - which is a pointer to storage with/for
 * element
 */

class QueueFunctor : public estd::TypeErasedFunctor<void(void*)>
{

};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_QUEUEFUNCTOR_HPP_
