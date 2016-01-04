/**
 * \file
 * \brief SignalCatchingOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2016-01-04
 */

#include "SignalCatchingOperationsTestCase.hpp"

#include "abortSignalHandler.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread-Signals.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test threads, bytes
constexpr size_t testThreadStackSize {512};

/// expected number of context switches in phase2() block involving thread: 1 - main thread is preempted by test thread
/// (main -> test), 2 - test thread terminates (test -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase2ThreadContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Phase 1 of test case.
 *
 * Tests various aspects of setting signal action, aiming for full coverage of execution paths through
 * SignalsCatcherControlBlock::setAssociation(). Following cases are tested:
 * - setting identical signal actions for multiple signal numbers must succeed as long as no more than
 * \a CONFIG_MAIN_THREAD_SIGNAL_ACTIONS different signal actions are used at the same time;
 * - setting the same signal action as the one that is currently set must always succeed;
 * - clearing signal action must always succeed;
 * - trying to change signal action for signal number must fail with EAGAIN if \a CONFIG_MAIN_THREAD_SIGNAL_ACTIONS
 * different signal actions are already used and the one that is changed is used by multiple signal numbers;
 * - trying to set new signal action for signal number must fail with EAGAIN when \a CONFIG_MAIN_THREAD_SIGNAL_ACTIONS
 * different signal actions are already used;
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	constexpr size_t mainThreadSignalActions {CONFIG_MAIN_THREAD_SIGNAL_ACTIONS};

	// the second to last iteration is used to set the same signal action as in the first iteration
	// the last iteration is used to set the same signal action as the one that is currently set
	for (size_t shift {}; shift <= mainThreadSignalActions + 1; ++shift)
		for (uint8_t signalNumber {}; signalNumber < SignalSet::Bitset{}.size(); ++signalNumber)
		{
			// last iteration? clip the value so that it is identical to the one from previous iteration
			const auto realShift = shift <= mainThreadSignalActions ? shift : mainThreadSignalActions;
			const SignalSet signalMask {1u << ((realShift + signalNumber) % mainThreadSignalActions)};
			const auto setSignalActionResult = ThisThread::Signals::setSignalAction(signalNumber,
					{abortSignalHandler, signalMask});
			if (setSignalActionResult.first != 0)
				return false;
			if (shift == 0)	// first itertion? previous signal action is equal to default SignalAction
			{
				if (setSignalActionResult.second.getHandler() != SignalAction{}.getHandler())
					return false;
			}
			else	// compare returned signal action with the expected one
			{
				const SignalSet previousSignalMask {1u << ((shift - 1 + signalNumber) % mainThreadSignalActions)};
				if (setSignalActionResult.second.getHandler() != abortSignalHandler ||
						setSignalActionResult.second.getSignalMask().getBitset() != previousSignalMask.getBitset())
					return false;
			}
		}

	constexpr uint8_t testSignalNumber {2};
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
		auto testThread = makeDynamicThread({testThreadStackSize, false, 0, 0, UINT8_MAX}, testThreadLambda,
				std::ref(getSignalActionRet), std::ref(signalMask), std::ref(setSignalActionRet),
				std::ref(setSignalMaskRet));
		testThread.start();
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
		auto testThread = makeDynamicThread({testThreadStackSize, true, 0, 0, UINT8_MAX}, testThreadLambda,
				std::ref(getSignalActionRet), std::ref(signalMask), std::ref(setSignalActionRet),
				std::ref(setSignalMaskRet));
		testThread.start();
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

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalCatchingOperationsTestCase::run_() const
{
	constexpr auto phase2ExpectedContextSwitchCount = 2 * phase2ThreadContextSwitchCount;
	constexpr auto expectedContextSwitchCount = phase2ExpectedContextSwitchCount;

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (const auto& function : {phase1, phase2})
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

	return true;
}

}	// namespace test

}	// namespace distortos
