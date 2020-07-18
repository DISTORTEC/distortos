/**
 * \file
 * \brief InterruptUnmaskingLock class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_INTERRUPTUNMASKINGLOCK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_INTERRUPTUNMASKINGLOCK_HPP_

#include "distortos/architecture/disableInterruptMasking.hpp"

#include "distortos/internal/synchronization/InterruptMaskingUnmaskingLock.hpp"

namespace distortos
{

namespace internal
{

/// InterruptUnmaskingLock class is a RAII wrapper for
/// architecture::disableInterruptMasking() / architecture::restoreInterruptMasking()
class InterruptUnmaskingLock : private InterruptMaskingUnmaskingLock<architecture::disableInterruptMasking>
{

};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_INTERRUPTUNMASKINGLOCK_HPP_
