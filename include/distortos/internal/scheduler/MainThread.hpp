/**
 * \file
 * \brief MainThread class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-27
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_MAINTHREAD_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_MAINTHREAD_HPP_

#include "distortos/ThreadCommon.hpp"

namespace distortos
{

namespace internal
{

/// MainThread class is a Thread for main()
class MainThread : public ThreadCommon
{
public:

	/**
	 * \brief MainThread's constructor.
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] threadGroupControlBlock is a reference to ThreadGroupControlBlock to which this object will be added
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for main thread, nullptr to disable reception
	 * of signals for main thread
	 */

	MainThread(uint8_t priority, ThreadGroupControlBlock& threadGroupControlBlock, SignalsReceiver* signalsReceiver);

	using ThreadCommon::getThreadControlBlock;

private:

	/**
	 * \brief "Run" function of thread
	 *
	 * Just a dummy which never gets used.
	 */

	virtual void run() override;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_MAINTHREAD_HPP_
