/**
 * \file
 * \brief ThreadOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ThreadOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/InterruptMaskingLock.hpp"
#include "distortos/StaticThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"

#include <malloc.h>

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
	{
		auto testThread = makeDynamicThread({0, 1}, emptyFunction);
		const auto ret = testThread.start();	// attempting to start thread with too small stack must fail with ENOSPC
		if (ret != ENOSPC)
		{
			if (ret == 0)
				testThread.join();
			return false;
		}
	}

	auto testThread = makeDynamicThread({192, 1}, emptyFunction);

	// state of created (but not yet started) thread must be "ThreadState::created"
	if (testThread.getState() != ThreadState::created)
		return false;

	bool result {true};

	{
		const InterruptMaskingLock interruptMaskingLock;	// make sure the thread doesn't get to run yet

		{
			const auto ret = testThread.start();
			if (ret != 0)
				return false;
		}

		if (testThread.getState() != ThreadState::runnable)	// started thread's state must be "ThreadState::runnable"
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

	// terminated thread's state must be "ThreadState::terminated"
	if (testThread.getState() != ThreadState::terminated)
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
 * Tests behavior of joining current thread.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
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

	const auto allocatedMemory = mallinfo().uordblks;
	const auto lambda =
			[](int& sharedRet)
			{
				sharedRet = ThisThread::detach();
			};

	// attempt to detach static thread must fail with ENOTSUP
	{
		int sharedRet {};
		auto staticThread = makeAndStartStaticThread<192>(1, lambda, std::ref(sharedRet));
		bool result {true};
		if (staticThread.detach() != ENOTSUP)	// static thread cannot be detached
			result = false;
		if (staticThread.getState() != ThreadState::runnable)
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
		if (dynamicThread.getState() != ThreadState::detached)
			result = false;
		if (dynamicThread.join() != EINVAL || result == false || sharedRet != 0x2e981e48 ||
				dynamicThread.detach() != EINVAL)	// detached thread cannot be joined, self-detach must fail
			return false;
	}

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
		return false;

	// detaching dynamic thread that is started, but not yet terminated, must succeed
	{
		int sharedRet {0x0f0dad58};
		auto dynamicThread = makeAndStartDynamicThread({192, 1}, lambda, std::ref(sharedRet));
		bool result {true};
		if (dynamicThread.detach() != 0)
			result = false;
		if (dynamicThread.getState() != ThreadState::detached)
			result = false;
		if (dynamicThread.join() != EINVAL)	// detached thread cannot be joined
			result = false;
		if (sharedRet != 0x0f0dad58)
			result = false;
		waitForNextTick();
		if (result == false || sharedRet != EINVAL || dynamicThread.detach() != EINVAL)	// self-detach must fail
			return false;
	}

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
		return false;

	// self-detach of dynamic thread must succeed
	{
		int sharedRet {0x5d3c799b};
		auto dynamicThread = makeAndStartDynamicThread({192, UINT8_MAX}, lambda, std::ref(sharedRet));
		bool result {true};
		if (dynamicThread.getState() != ThreadState::detached)
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

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
		return false;

	// detaching dynamic thread that is already terminated must succeed, the thread is just deleted
	{
		auto dynamicThread = makeAndStartDynamicThread({192, UINT8_MAX}, emptyFunction);
		bool result {true};
		if (dynamicThread.getState() != ThreadState::terminated)
			result = false;
		if (dynamicThread.detach() != 0)
			result = false;
		if (dynamicThread.getState() != ThreadState::detached)
			result = false;
		// detached thread cannot be joined
		if (dynamicThread.join() != EINVAL || result == false || dynamicThread.detach() != EINVAL)
			return false;
	}

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
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

	const auto allocatedMemory = mallinfo().uordblks;
	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (const auto& function : {phase1, phase2, phase3})
	{
		const auto ret = function();
		if (ret != true)
			return ret;

		if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
			return false;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
