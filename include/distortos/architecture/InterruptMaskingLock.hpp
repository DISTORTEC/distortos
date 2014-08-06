/**
 * \file
 * \brief InterruptMaskingLock class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-06
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASKINGLOCK_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASKINGLOCK_HPP_

#include "distortos/architecture.hpp"

namespace distortos
{

namespace architecture
{

/// InterruptMaskingLock class is a RAII wrapper for enableInterruptMasking() / restoreInterruptMasking()
class InterruptMaskingLock
{
public:

	/**
	 * \brief InterruptMaskingLock's constructor
	 *
	 * Enables interrupt masking, saving current interrupt mask for use in destructor.
	 */

	InterruptMaskingLock() :
			interruptMask_{enableInterruptMasking()}
	{}

	/**
	 * \brief InterruptMaskingLock's destructor
	 *
	 * Disables interrupt masking, restoring interrupt mask saved in constructor.
	 */

	~InterruptMaskingLock()
	{
		restoreInterruptMasking(interruptMask_);
	}

	InterruptMaskingLock(const InterruptMaskingLock &) = delete;
	InterruptMaskingLock(InterruptMaskingLock &&) = delete;
	InterruptMaskingLock & operator= (const InterruptMaskingLock &) = delete;
	InterruptMaskingLock & operator= (InterruptMaskingLock &&) = delete;

private:

	/// interrupt mask
	const InterruptMask interruptMask_;
};

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTMASKINGLOCK_HPP_
