/**
 * \file
 * \brief InterruptMaskingUnmaskingLock class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASKINGUNMASKINGLOCK_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASKINGUNMASKINGLOCK_HPP_

#include "distortos/architecture/restoreInterruptMasking.hpp"

namespace distortos
{

namespace architecture
{

/**
 * \brief InterruptMaskingUnmaskingLock class is a RAII wrapper for interrupt mask manipulation.
 *
 * \tparam Function is a reference to function which modifies interrupt mask and returns InterruptMask;
 * enableInterruptMasking() or disableInterruptMasking() should be used
 */

template<InterruptMask (& Function)()>
class InterruptMaskingUnmaskingLock
{
public:

	/**
	 * \brief InterruptMaskingUnmaskingLock's constructor
	 *
	 * Enables/disables interrupt masking, saving current interrupt mask for use in destructor.
	 */

	InterruptMaskingUnmaskingLock() :
			interruptMask_{Function()}
	{

	}

	/**
	 * \brief InterruptMaskingUnmaskingLock's destructor
	 *
	 * Restores previous interrupt masking state by restoring interrupt mask saved in constructor.
	 */

	~InterruptMaskingUnmaskingLock()
	{
		restoreInterruptMasking(interruptMask_);
	}

	InterruptMaskingUnmaskingLock(const InterruptMaskingUnmaskingLock&) = delete;
	InterruptMaskingUnmaskingLock(InterruptMaskingUnmaskingLock&&) = delete;
	InterruptMaskingUnmaskingLock& operator=(const InterruptMaskingUnmaskingLock&) = delete;
	InterruptMaskingUnmaskingLock& operator=(InterruptMaskingUnmaskingLock&&) = delete;

private:

	/// interrupt mask
	const InterruptMask interruptMask_;
};

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASKINGUNMASKINGLOCK_HPP_
