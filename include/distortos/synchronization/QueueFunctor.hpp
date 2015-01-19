/**
 * \file
 * \brief QueueFunctor class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-19
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_QUEUEFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_QUEUEFUNCTOR_HPP_

#include "distortos/estd/TypeErasedFunctor.hpp"

namespace distortos
{

namespace synchronization
{

/**
 * \brief QueueFunctor is a type-erased interface for functors which execute some action on queue's storage (like
 * copy-constructing, swapping, destroying, emplacing, ...).
 *
 * The functor will be called by queue internals with one argument - \a storage - which is a pointer to storage with/for
 * element
 */

using QueueFunctor = estd::TypeErasedFunctor<void(void*)>;

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_QUEUEFUNCTOR_HPP_
