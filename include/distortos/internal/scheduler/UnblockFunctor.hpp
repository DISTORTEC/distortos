/**
 * \file
 * \brief UnblockFunctor class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_UNBLOCKFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_UNBLOCKFUNCTOR_HPP_

#include "distortos/internal/scheduler/UnblockReason.hpp"

#include "estd/TypeErasedFunctor.hpp"

namespace distortos
{

namespace internal
{

class ThreadControlBlock;

/// UnblockFunctor is a functor executed when unblocking the thread, it receives two parameter - a reference to
/// ThreadControlBlock that is being unblocked and the reason of thread unblocking
class UnblockFunctor : public estd::TypeErasedFunctor<void(ThreadControlBlock&, UnblockReason)>
{

};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_UNBLOCKFUNCTOR_HPP_
