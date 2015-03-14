/**
 * \file
 * \brief SignalsReceiverControlBlock class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-14
 */

#ifndef INCLUDE_DISTORTOS_SIGNALS_SIGNALSRECEIVERCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SIGNALS_SIGNALSRECEIVERCONTROLBLOCK_HPP_

namespace distortos
{

namespace scheduler
{

class ThreadControlBlock;

}	// namespace scheduler

namespace synchronization
{

/// SignalsReceiverControlBlock class is a structure required by threads for "receiving" of signals
class SignalsReceiverControlBlock
{
public:

	/**
	 * \brief SignalsReceiverControlBlock's constructor
	 */

	constexpr SignalsReceiverControlBlock() :
			threadControlBlock_{}
	{

	}

	/**
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 */

	void setThreadControlBlock(scheduler::ThreadControlBlock& threadControlBlock)
	{
		threadControlBlock_ = &threadControlBlock;
	}

private:

	/// pointer to associated ThreadControlBlock
	scheduler::ThreadControlBlock* threadControlBlock_;
};

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALS_SIGNALSRECEIVERCONTROLBLOCK_HPP_
