/**
 * \file
 * \brief ThreadOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ThreadOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/StaticThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"

#include <malloc.h>

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
 * \brief Empty function used for test threads
 */

void emptyFunction()
{

}

/**
 * \brief Phase 1 of test case
 *
 * Tests behavior of creating, starting and joining the test thread.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	auto testThread = makeDynamicThread({192, 1}, emptyFunction);

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
 * \brief Phase 2 of test case
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

/**
 * \brief Phase 3 of test case
 *
 * Tests scenarios involving thread detachment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3()
{
#ifdef CONFIG_THREAD_DETACH_ENABLE

	const auto lambda =
			[](int& sharedRet)
			{
				sharedRet = ThisThread::detach();
			};

	// attempt to detach static thread must fail with ENOTSUP
	{
		int sharedRet {};
		auto staticThread = makeStaticThread<192>(1, lambda, std::ref(sharedRet));
		bool result {true};
		if (staticThread.start() != 0)
			result = false;
		if (staticThread.detach() != ENOTSUP)	// static thread cannot be detached
			result = false;
		if (staticThread.getState() != ThreadState::Runnable)
			result = false;
		if (staticThread.join() != 0 || result == false || sharedRet != ENOTSUP)	// self-detach must fail too
			return false;
	}

	// detaching dynamic thread that is not yet started must succeed, but the thread is just deleted and does not run
	{
		int sharedRet {0x2e981e48};
		auto dynamicThread = makeDynamicThread({192, 1}, lambda, std::ref(sharedRet));
		bool result {true};
		if (dynamicThread.detach() != 0)
			result = false;
		if (dynamicThread.start() != EINVAL)	// detached thread cannot be started
			result = false;
		if (dynamicThread.getState() != ThreadState::Detached)
			result = false;
		if (dynamicThread.join() != EINVAL || result == false || sharedRet != 0x2e981e48 ||
				dynamicThread.detach() != EINVAL)	// detached thread cannot be joined, self-detach must fail
			return false;
	}

	if (mallinfo().uordblks != 0)	// all dynamic memory must be deallocated after each test phase
		return false;

	// detaching dynamic thread that is started, but not yet terminated, must succeed
	{
		int sharedRet {0x0f0dad58};
		auto dynamicThread = makeDynamicThread({192, 1}, lambda, std::ref(sharedRet));
		bool result {true};
		if (dynamicThread.start() != 0)
			result = false;
		if (dynamicThread.detach() != 0)
			result = false;
		if (dynamicThread.getState() != ThreadState::Detached)
			result = false;
		if (dynamicThread.join() != EINVAL)	// detached thread cannot be joined
			result = false;
		if (sharedRet != 0x0f0dad58)
			result = false;
		waitForNextTick();
		if (result == false || sharedRet != EINVAL || dynamicThread.detach() != EINVAL)	// self-detach must fail
			return false;
	}

	if (mallinfo().uordblks != 0)	// all dynamic memory must be deallocated after each test phase
		return false;

	// self-detach of dynamic thread must succeed
	{
		int sharedRet {0x5d3c799b};
		auto dynamicThread = makeDynamicThread({192, UINT8_MAX}, lambda, std::ref(sharedRet));
		bool result {true};
		if (dynamicThread.start() != 0)
			result = false;
		if (dynamicThread.getState() != ThreadState::Detached)
			result = false;
		if (dynamicThread.detach() != EINVAL)	// second attempt to detach a thread must fail
			result = false;
		if (dynamicThread.join() != EINVAL)	// detached thread cannot be joined
			result = false;
		if (sharedRet != 0)	// self-detach must be successful
			result = false;
		waitForNextTick();
		if (result == false)
			return false;
	}

	if (mallinfo().uordblks != 0)	// all dynamic memory must be deallocated after each test phase
		return false;

	// detaching dynamic thread that is already terminated must succeed, the thread is just deleted
	{
		auto dynamicThread = makeDynamicThread({192, UINT8_MAX}, emptyFunction);
		bool result {true};
		if (dynamicThread.start() != 0)
			result = false;
		if (dynamicThread.getState() != ThreadState::Terminated)
			result = false;
		if (dynamicThread.detach() != 0)
			result = false;
		if (dynamicThread.getState() != ThreadState::Detached)
			result = false;
		// detached thread cannot be joined
		if (dynamicThread.join() != EINVAL || result == false || dynamicThread.detach() != EINVAL)
			return false;
	}

	if (mallinfo().uordblks != 0)	// all dynamic memory must be deallocated after each test phase
		return false;

#endif	// def CONFIG_THREAD_DETACH_ENABLE

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
#ifdef CONFIG_THREAD_DETACH_ENABLE
	constexpr auto phase3ExpectedContextSwitchCount = 11;
#else	// !def CONFIG_THREAD_DETACH_ENABLE
	constexpr auto phase3ExpectedContextSwitchCount = 0;
#endif	// !def CONFIG_THREAD_DETACH_ENABLE
	constexpr auto expectedContextSwitchCount = phase1ExpectedContextSwitchCount + phase2ExpectedContextSwitchCount +
			phase3ExpectedContextSwitchCount;

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (const auto& function : {phase1, phase2, phase3})
	{
		const auto ret = function();
		if (ret != true)
			return ret;

		if (mallinfo().uordblks != 0)	// all dynamic memory must be deallocated after each test phase
			return false;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
