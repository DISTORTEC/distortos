/**
 * \file
 * \brief SignalCatchingOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SignalCatchingOperationsTestCase.hpp"

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "abortSignalHandler.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/ThisThread-Signals.hpp"

#include <cerrno>

/// configuration required by first and second phase of SignalCatchingOperationsTestCase
#define SIGNAL_CATCHING_OPERATIONS_TEST_CASE_PHASE_1_2_ENABLED DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS >= 1 && \
		DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS <= 31

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

namespace distortos
{

namespace test
{

#if DISTORTOS_SIGNALS_ENABLE == 1

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test threads, bytes
constexpr size_t testThreadStackSize {512};

#if SIGNAL_CATCHING_OPERATIONS_TEST_CASE_PHASE_1_2_ENABLED == 1

/// expected number of context switches in phase2() block involving thread: 1 - main thread is preempted by test thread
/// (main -> test), 2 - test thread terminates (test -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase2ThreadContextSwitchCount {2};

#endif	// SIGNAL_CATCHING_OPERATIONS_TEST_CASE_PHASE_1_2_ENABLED == 1

/// expected number of context switches in phase3() block involving thread: 1 - main thread is preempted by test thread
/// (main -> test), 2 - test thread is preempted after lowering its own priority (test -> main), 3 - main thread blocks
/// by attempting to join() test thread (main -> test), 4 - test thread terminates (test -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase3ThreadContextSwitchCount {4};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

#if SIGNAL_CATCHING_OPERATIONS_TEST_CASE_PHASE_1_2_ENABLED == 1

/**
 * \brief Phase 1 of test case.
 *
 * Tests various aspects of setting signal action, aiming for full coverage of execution paths through
 * SignalsCatcherControlBlock::setAssociation(). Following cases are tested:
 * - setting identical signal actions for multiple signal numbers must succeed as long as no more than
 * \a DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS different signal actions are used at the same time;
 * - setting the same signal action as the one that is currently set must always succeed;
 * - clearing signal action must always succeed;
 * - trying to change signal action for signal number must fail with EAGAIN if \a DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS
 * different signal actions are already used and the one that is changed is used by multiple signal numbers;
 * - trying to set new signal action for signal number must fail with EAGAIN when
 * \a DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS different signal actions are already used;
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	constexpr size_t mainThreadSignalActions {DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS};

	// the second to last iteration is used to set the same signal action as in the first iteration
	// the last iteration is used to set the same signal action as the one that is currently set
	for (size_t mask {}; mask <= mainThreadSignalActions + 1; ++mask)
		for (uint8_t signalNumber {}; signalNumber < SignalSet::Bitset{}.size(); ++signalNumber)
		{
			// last iteration? clip the value so that it is identical to the one from previous iteration
			const auto realMask = mask <= mainThreadSignalActions ? mask : mainThreadSignalActions;
			const SignalSet signalMask {((realMask + signalNumber) % mainThreadSignalActions)};
			const auto setSignalActionResult = ThisThread::Signals::setSignalAction(signalNumber,
					{abortSignalHandler, signalMask});
			if (setSignalActionResult.first != 0)
				return false;
			if (mask == 0)	// first iteration? previous signal action is equal to default SignalAction
			{
				if (setSignalActionResult.second.getHandler() != SignalAction{}.getHandler())
					return false;
			}
			else	// compare returned signal action with the expected one
			{
				const SignalSet previousSignalMask {((mask - 1 + signalNumber) % mainThreadSignalActions)};
				if (setSignalActionResult.second.getHandler() != abortSignalHandler ||
						setSignalActionResult.second.getSignalMask().getBitset() != previousSignalMask.getBitset())
					return false;
			}
		}

	constexpr uint8_t testSignalNumber {0};
	int ret;
	std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(testSignalNumber,
			{abortSignalHandler, SignalSet{SignalSet::full}});
	if (ret != EAGAIN)
		return false;
	std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(testSignalNumber, {});
	if (ret != 0)
		return false;
	std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(testSignalNumber,
			{abortSignalHandler, SignalSet{SignalSet::full}});
	if (ret != EAGAIN)
		return false;

	for (uint8_t signalNumber {}; signalNumber < SignalSet::Bitset{}.size(); ++signalNumber)
	{
		std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(signalNumber, {});
		if (ret != 0)
			return false;
	}

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests various error cases:
 * - attempt to get/set signal action for invalid signal number must fail with EINVAL;
 * - attempt to get/set signal action or to set signal mask in a thread that either has disabled reception of signals or
 * has disabled catching/handling of signals must fail with ENOTSUP;
 * - signal mask of a thread that either has disabled reception of signals or has disabled catching/handling of signals
 * must be equal to "full" SignalSet;
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	for (auto signalNumber = SignalSet::Bitset{}.size(); signalNumber <= UINT8_MAX; ++signalNumber)
	{
		int ret;
		std::tie(ret, std::ignore) = ThisThread::Signals::getSignalAction(signalNumber);
		if (ret != EINVAL)
			return false;
		std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(signalNumber, SignalAction{});
		if (ret != EINVAL)
			return false;
	}

	auto testThreadLambda =
			[](int& getSignalActionRet, SignalSet& signalMask, int& setSignalActionRet, int& setSignalMaskRet)
			{
				std::tie(getSignalActionRet, std::ignore) = ThisThread::Signals::getSignalAction({});
				signalMask = ThisThread::Signals::getSignalMask();
				std::tie(setSignalActionRet, std::ignore) = ThisThread::Signals::setSignalAction({}, SignalAction{});
				setSignalMaskRet = ThisThread::Signals::setSignalMask(SignalSet{SignalSet::full});
			};

	{
		int getSignalActionRet {};
		SignalSet signalMask {SignalSet::empty};
		int setSignalActionRet {};
		int setSignalMaskRet {};
		auto testThread = makeAndStartDynamicThread({testThreadStackSize, false, 0, 0, UINT8_MAX}, testThreadLambda,
				std::ref(getSignalActionRet), std::ref(signalMask), std::ref(setSignalActionRet),
				std::ref(setSignalMaskRet));
		testThread.join();
		if (getSignalActionRet != ENOTSUP)
			return false;
		if (signalMask.getBitset() != SignalSet{SignalSet::full}.getBitset())
			return false;
		if (setSignalActionRet != ENOTSUP)
			return false;
		if (setSignalMaskRet != ENOTSUP)
			return false;
	}

	{
		int getSignalActionRet {};
		SignalSet signalMask {SignalSet::empty};
		int setSignalActionRet {};
		int setSignalMaskRet {};
		auto testThread = makeAndStartDynamicThread({testThreadStackSize, true, 0, 0, UINT8_MAX}, testThreadLambda,
				std::ref(getSignalActionRet), std::ref(signalMask), std::ref(setSignalActionRet),
				std::ref(setSignalMaskRet));
		testThread.join();
		if (getSignalActionRet != ENOTSUP)
			return false;
		if (signalMask.getBitset() != SignalSet{SignalSet::full}.getBitset())
			return false;
		if (setSignalActionRet != ENOTSUP)
			return false;
		if (setSignalMaskRet != ENOTSUP)
			return false;
	}

	return true;
}

#endif	// SIGNAL_CATCHING_OPERATIONS_TEST_CASE_PHASE_1_2_ENABLED == 1

/**
 * \brief Phase 3 of test case.
 *
 * Tests whether generation/queuing of signal fails with ENOSPC if the amount of target thread's free stack is too small
 * to request signal delivery.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3()
{
	static_assert(SignalCatchingOperationsTestCase::getTestCasePriority() < UINT8_MAX &&
			SignalCatchingOperationsTestCase::getTestCasePriority() > 1, "Invalid test case priority");

	constexpr uint8_t testSignalNumber {16};
	const auto testThreadLambda = []()
	{
		ThisThread::Signals::setSignalAction(testSignalNumber, {abortSignalHandler, SignalSet{SignalSet::empty}});
		// This non-inline "inner" lambda with big array is meant to ensure that the biggest stack usage is when the
		// context is switched, leaving no room to request signal delivery.
		[]() __attribute__ ((noinline))
		{
			volatile uint8_t array[testThreadStackSize / 2] {};
			ThisThread::setPriority(1);
			(void)array[0];	// make sure the array is not removed and prevent tail-call optimization
		}();
	};

	size_t stackSize;
	{
		auto testThread = makeAndStartDynamicThread({testThreadStackSize, true, 1, 1, UINT8_MAX}, testThreadLambda);
		testThread.join();
		stackSize = testThread.getStackHighWaterMark();
	}
	{
		auto testThread = makeAndStartDynamicThread({stackSize, true, 1, 1, UINT8_MAX}, testThreadLambda);
		const auto ret1 = testThread.generateSignal(testSignalNumber);
		const auto ret2 = testThread.queueSignal(testSignalNumber, sigval{});
		testThread.join();
		if (ret1 != ENOSPC || ret2 != ENOSPC)	// signal generation/queuing must fail with ENOSPC
			return false;
		if (testThread.getPendingSignalSet().getBitset().none() == false)	// no signals may be pending
			return false;
	}

	return true;
}

}	// namespace

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalCatchingOperationsTestCase::run_() const
{
#if DISTORTOS_SIGNALS_ENABLE == 1

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	constexpr auto phase3ExpectedContextSwitchCount = 2 * phase3ThreadContextSwitchCount;

#if SIGNAL_CATCHING_OPERATIONS_TEST_CASE_PHASE_1_2_ENABLED == 1
	constexpr auto phase2ExpectedContextSwitchCount = 2 * phase2ThreadContextSwitchCount;
	constexpr auto expectedContextSwitchCount = phase2ExpectedContextSwitchCount + phase3ExpectedContextSwitchCount;
	for (const auto& function : {phase1, phase2, phase3})
#else	// SIGNAL_CATCHING_OPERATIONS_TEST_CASE_PHASE_1_2_ENABLED != 1
	constexpr auto expectedContextSwitchCount = phase3ExpectedContextSwitchCount;
	for (const auto& function : {phase3})
#endif	// SIGNAL_CATCHING_OPERATIONS_TEST_CASE_PHASE_1_2_ENABLED != 1
	{
		// initially no signals may be pending
		if (ThisThread::Signals::getPendingSignalSet().getBitset().none() == false)
			return false;

		if (function() == false)
			return false;
	}

	// after the test no signals may be pending
	if (ThisThread::Signals::getPendingSignalSet().getBitset().none() == false)
		return false;

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

	return true;
}

}	// namespace test

}	// namespace distortos
