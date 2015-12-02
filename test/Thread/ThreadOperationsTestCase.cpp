/**
 * \file
 * \brief ThreadOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-02
 */

#include "ThreadOperationsTestCase.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Phase 1 of test case.
 *
 * Tests behavior of creating, starting and joining the test thread.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	auto testThread = makeStaticThread<192>(1, [](){});

	if (testThread.getState() != ThreadState::New)	// state of created (but not yet started) thread must be "New"
		return false;

	bool result {true};

	{
		architecture::InterruptMaskingLock interruptMaskingLock;	// make sure the thread doesn't get to run yet

		{
			const auto ret = testThread.start();
			if (ret != 0)
				return false;
		}

		if (testThread.getState() != ThreadState::Runnable)	// started thread's state must be "Runnable"
			result = false;

		{
			const auto ret = testThread.start();	// attempting to start a thread that is already started must fail
			if (ret != EINVAL)
				result = false;
		}
	}

	{
		const auto ret = testThread.join();
		if (ret != 0)
			result = false;
	}

	if (testThread.getState() != ThreadState::Terminated)	// terminated thread's state must be "Terminated"
		result = false;

	{
		const auto ret = testThread.start();	// attempting to start a thread that is already started must fail
		if (ret != EINVAL)
			result = false;
	}

	return result;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests behavior Thread::start() and Thread::join() when used with current thread.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	{
		const auto ret = ThisThread::get().start();	// attempting to start current thread must fail
		if (ret != EINVAL)
			return false;
	}

	{
		const auto ret = ThisThread::get().join();	// attempting to join current thread must fail
		if (ret != EDEADLK)
			return false;
	}

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadOperationsTestCase::run_() const
{
	constexpr auto phase1ExpectedContextSwitchCount = 2;
	constexpr auto phase2ExpectedContextSwitchCount = 0;
	constexpr auto expectedContextSwitchCount = phase1ExpectedContextSwitchCount + phase2ExpectedContextSwitchCount;

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (const auto& function : {phase1, phase2})
	{
		const auto ret = function();
		if (ret != true)
			return ret;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
