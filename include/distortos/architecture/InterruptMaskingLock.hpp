/**
 * \file
 * \brief InterruptMaskingLock class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASKINGLOCK_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASKINGLOCK_HPP_

#include "distortos/architecture/InterruptMaskingUnmaskingLock.hpp"
#include "distortos/architecture/enableInterruptMasking.hpp"

namespace distortos
{

namespace architecture
{

/// InterruptMaskingLock class is a RAII wrapper for enableInterruptMasking() / restoreInterruptMasking()
class InterruptMaskingLock : private InterruptMaskingUnmaskingLock<enableInterruptMasking>
{

};

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASKINGLOCK_HPP_
