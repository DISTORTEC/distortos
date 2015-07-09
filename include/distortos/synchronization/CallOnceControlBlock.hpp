/**
 * \file
 * \brief CallOnceControlBlock class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-07-09
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_CALLONCECONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_CALLONCECONTROLBLOCK_HPP_

namespace distortos
{

namespace scheduler
{

class ThreadControlBlockList;

}	// namespace scheduler

namespace synchronization
{

/// CallOnceControlBlock class implements functionality of OnceFlag class and callOnce()
class CallOnceControlBlock
{
public:

	/**
	 * \brief CallOnceControlBlock's constructor
	 */

	constexpr CallOnceControlBlock() :
			blockedList_{},
			done_{}
	{

	}

private:

	/// pointer to stack-allocated list of ThreadControlBlock objects blocked on associated OnceFlag
	scheduler::ThreadControlBlockList* blockedList_;

	/// tells whether any function was already called for this object (true) or not (false)
	bool done_;
};

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_CALLONCECONTROLBLOCK_HPP_
