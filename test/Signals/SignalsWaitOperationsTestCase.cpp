/**
 * \file
 * \brief SignalsWaitOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SignalsWaitOperationsTestCase.hpp"

#include "distortos/distortosConfiguration.h"

/// configuration required by SignalsWaitOperationsTestCase
#define SIGNALS_WAIT_OPERATIONS_TEST_CASE_ENABLED \
		DISTORTOS_SIGNALS_ENABLE == 1 && DISTORTOS_MAIN_THREAD_CAN_RECEIVE_SIGNALS == 1

#if SIGNALS_WAIT_OPERATIONS_TEST_CASE_ENABLED == 1

#include "signalsTestSelfOneSignalPending.hpp"

#include "waitForNextTick.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/StaticSoftwareTimer.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/ThisThread-Signals.hpp"

#include <cerrno>

#endif	// #if SIGNALS_WAIT_OPERATIONS_TEST_CASE_ENABLED == 1

namespace distortos
{

namespace test
{

#if SIGNALS_WAIT_OPERATIONS_TEST_CASE_ENABLED == 1

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of function used to send signal to selected thread
using SendSignal = int(Thread&, uint8_t, int);

/// type of function used to test received SignalInformation object
using TestReceivedSignalInformation = bool(const SignalInformation&, uint8_t, int);

/// pair with functions for one stage
using Stage = std::pair<const SendSignal&, const TestReceivedSignalInformation&>;

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/// single duration used in tests
constexpr auto singleDuration = TickClock::duration{1};

/// long duration used in tests
constexpr auto longDuration = singleDuration * 10;

/// expected number of context switches in waitForNextTick(): main -> idle -> main
constexpr decltype(statistics::getContextSwitchCount()) waitForNextTickContextSwitchCount {2};

/// expected number of context switches in phase1 block involving timed-out tryWaitFor() or tryWaitUntil() (excluding
/// waitForNextTick()): 1 - main thread blocks waiting for signals (main -> idle), 2 - main thread wakes up
/// (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase1TimedOutWaitContextSwitchCount {2};

/// expected number of context switches in phase2 block involving software timer (excluding waitForNextTick()): 1 - main
/// thread blocks waiting for signals (main -> idle), 2 - main thread is unblocked by interrupt (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase2SoftwareTimerContextSwitchCount {2};

/// expected number of context switches in phase3 block involving thread: 1 - main thread is preempted by test thread
/// (main -> test), 2 - test thread terminates (test -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase3ThreadContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Wrapper for Thread::generateSignal().
 *
 * \param [in] thread is a reference to Thread for which the signal will be generated
 * \param [in] signalNumber is the signal that will be generated, [0; 31]
 * \param [in] value is the signal value (ignored)
 *
 * \return values returned by Thread::generateSignal()
 */

int generateSignalWrapper(Thread& thread, const uint8_t signalNumber, int)
{
	return thread.generateSignal(signalNumber);
}

#if defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0

/**
 * \brief Wrapper for Thread::queueSignal().
 *
 * \param [in] thread is a reference to Thread for which the signal will be queued
 * \param [in] signalNumber is the signal that will be queued, [0; 31]
 * \param [in] value is the signal value
 *
 * \return values returned by Thread::queueSignal()
 */

int queueSignalWrapper(Thread& thread, const uint8_t signalNumber, const int value)
{
	return thread.queueSignal(signalNumber, sigval{value});
}

#endif	// defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0

/**
 * \brief Tests whether received SignalInformation object matches the signal that was generated.
 *
 * \param [in] signalInformation is a reference to received SignalInformation object
 * \param [in] signalNumber is the signal number that was generated
 * \param [in] value is the signal value that was sent (ignored)
 *
 * \return true if test succeeded, false otherwise
 */

bool testReceivedGeneratedSignal(const SignalInformation& signalInformation, const uint8_t signalNumber, int)
{
	return signalInformation.getSignalNumber() == signalNumber &&
			signalInformation.getCode() == SignalInformation::Code::generated;
}

#if defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0

/**
 * \brief Tests whether received SignalInformation object matches the signal that was queued.
 *
 * \param [in] signalInformation is a reference to received SignalInformation object
 * \param [in] signalNumber is the signal number that was queued
 * \param [in] value is the signal value that was queued
 *
 * \return true if test succeeded, false otherwise
 */

bool testReceivedQueuedSignal(const SignalInformation& signalInformation, const uint8_t signalNumber, const int value)
{
	return signalInformation.getSignalNumber() == signalNumber &&
			signalInformation.getCode() == SignalInformation::Code::queued &&
			signalInformation.getValue().sival_int == value;
}

#endif	// defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0

/**
 * \brief Tests whether no signals are pending for current thread.
 *
 * \return true if test succeeded, false otherwise
 */

bool testSelfNoSignalsPending()
{
	const auto pendingSignalSet = ThisThread::Signals::getPendingSignalSet();
	const auto pendingBitset = pendingSignalSet.getBitset();
	return pendingBitset.none();
}

/**
 * \brief Tests sending of signal to current thread.
 *
 * Initially no signals may be pending for current thread. After call to \a sendSignal() exactly one signal -
 * \a signalNumber - must be pending.
 *
 * \param [in] sendSignal is a reference to function used to send signal to current thread
 * \param [in] signalNumber is the signal number that will be generated
 * \param [in] value is the signal value
 *
 * \return true if test succeeded, false otherwise
 */

bool testSelfSendSignal(const SendSignal& sendSignal, const uint8_t signalNumber, const int value)
{
	{
		const auto ret = testSelfNoSignalsPending();	// initially no signals may be pending
		if (ret != true)
			return ret;
	}

	{
		auto& mainThread = ThisThread::get();
		const auto ret = sendSignal(mainThread, signalNumber, value);
		if (ret != 0)
			return false;
	}

	return signalsTestSelfOneSignalPending(signalNumber);
}

/**
 * \brief Wrapper for ThisThread::Signals::tryWait() that can be used in phase3().
 *
 * \param [in] signalSet is a reference to SignalSet passed to ThisThread::Signals::tryWait()
 * \param [out] sharedRet is a reference to variable in which error code from ThisThread::Signals::tryWait() will be
 * saved
 */

void tryWaitWrapper(const SignalSet& signalSet, int& sharedRet)
{
	std::tie(sharedRet, std::ignore) = ThisThread::Signals::tryWait(signalSet);
}

/**
 * \brief Wrapper for ThisThread::Signals::tryWaitFor() that can be used in phase3().
 *
 * \param [in] signalSet is a reference to SignalSet passed to ThisThread::Signals::tryWaitFor()
 * \param [out] sharedRet is a reference to variable in which error code from ThisThread::Signals::tryWaitFor() will be
 * saved
 */

void tryWaitForWrapper(const SignalSet& signalSet, int& sharedRet)
{
	std::tie(sharedRet, std::ignore) = ThisThread::Signals::tryWaitFor(signalSet, TickClock::duration{});
}

/**
 * \brief Wrapper for ThisThread::Signals::tryWaitUntil() that can be used in phase3().
 *
 * \param [in] signalSet is a reference to SignalSet passed to ThisThread::Signals::tryWaitUntil()
 * \param [out] sharedRet is a reference to variable in which error code from ThisThread::Signals::tryWaitUntil() will
 * be saved
 */

void tryWaitUntilWrapper(const SignalSet& signalSet, int& sharedRet)
{
	std::tie(sharedRet, std::ignore) = ThisThread::Signals::tryWaitUntil(signalSet, TickClock::time_point{});
}

/**
 * \brief Wrapper for ThisThread::Signals::wait() that can be used in phase3().
 *
 * \param [in] signalSet is a reference to SignalSet passed to ThisThread::Signals::wait()
 * \param [out] sharedRet is a reference to variable in which error code from ThisThread::Signals::wait() will be saved
 */

void waitWrapper(const SignalSet& signalSet, int& sharedRet)
{
	std::tie(sharedRet, std::ignore) = ThisThread::Signals::wait(signalSet);
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests whether all ThisThread::Signals::tryWait*() functions properly ignore pending signal that is not included in
 * SignalSet and accept it otherwise.
 *
 * \param [in] sendSignal is a reference to function used to send signal to current thread
 * \param [in] testReceivedSignalInformation is a reference to TestReceivedSignalInformation function used to test
 * received SignalInformation object
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1(const SendSignal& sendSignal, const TestReceivedSignalInformation& testReceivedSignalInformation)
{
	const SignalSet fullSignalSet {SignalSet::full};

	{
		constexpr uint8_t testSignalNumber {19};
		constexpr int testValue {0x3bb0e679};

		{
			const auto ret = testSelfSendSignal(sendSignal, testSignalNumber, testValue);
			if (ret != true)
				return ret;
		}

		{
			SignalSet excludingSignalSet {SignalSet::full};
			const auto ret = excludingSignalSet.remove(testSignalNumber);
			if (ret != 0)
				return false;

			waitForNextTick();

			// no signals are pending, so ThisThread::Signals::tryWait() should fail immediately
			const auto start = TickClock::now();
			const auto tryWaitResult = ThisThread::Signals::tryWait(excludingSignalSet);
			if (tryWaitResult.first != EAGAIN || TickClock::now() != start)
				return false;
		}

		{
			// one signal is pending, so ThisThread::Signals::tryWait() must succeed immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto tryWaitResult = ThisThread::Signals::tryWait(fullSignalSet);
			auto& signalInformation = tryWaitResult.second;
			if (tryWaitResult.first != 0 ||
					testReceivedSignalInformation(signalInformation, testSignalNumber, testValue) != true ||
					start != TickClock::now())
				return false;
		}
	}

	{
		constexpr uint8_t testSignalNumber {8};
		constexpr int testValue {0x10514ecb};

		{
			const auto ret = testSelfSendSignal(sendSignal, testSignalNumber, testValue);
			if (ret != true)
				return ret;
		}

		{
			SignalSet excludingSignalSet {SignalSet::full};
			const auto ret = excludingSignalSet.remove(testSignalNumber);
			if (ret != 0)
				return false;

			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// no signals are pending, so ThisThread::Signals::tryWaitFor() should time-out at expected time
			const auto start = TickClock::now();
			const auto tryWaitResult = ThisThread::Signals::tryWaitFor(excludingSignalSet, singleDuration);
			const auto realDuration = TickClock::now() - start;
			if (tryWaitResult.first != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TimedOutWaitContextSwitchCount)
				return false;
		}

		{
			// one signal is pending, so ThisThread::Signals::tryWaitFor() must succeed immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto tryWaitResult = ThisThread::Signals::tryWaitFor(fullSignalSet, singleDuration);
			auto& signalInformation = tryWaitResult.second;
			if (tryWaitResult.first != 0 ||
					testReceivedSignalInformation(signalInformation, testSignalNumber, testValue) != true ||
					start != TickClock::now())
				return false;
		}
	}

	{
		constexpr uint8_t testSignalNumber {22};
		constexpr int testValue {0x1d931dc7};

		{
			const auto ret = testSelfSendSignal(sendSignal, testSignalNumber, testValue);
			if (ret != true)
				return ret;
		}

		{
			SignalSet excludingSignalSet {SignalSet::full};

			{
				const auto ret = excludingSignalSet.remove(testSignalNumber);
				if (ret != 0)
					return false;
			}

			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// no signals are pending, so ThisThread::Signals::tryWaitUntil() should time-out at exact expected time
			const auto requestedTimePoint = TickClock::now() + singleDuration;
			const auto tryWaitResult = ThisThread::Signals::tryWaitUntil(excludingSignalSet, requestedTimePoint);
			if (tryWaitResult.first != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TimedOutWaitContextSwitchCount)
				return false;
		}

		{
			// one signal is pending, so ThisThread::Signals::tryWaitUntil() must succeed immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto tryWaitResult = ThisThread::Signals::tryWaitUntil(fullSignalSet, start + singleDuration);
			auto& signalInformation = tryWaitResult.second;
			if (tryWaitResult.first != 0 ||
					testReceivedSignalInformation(signalInformation, testSignalNumber, testValue) != true ||
					start != TickClock::now())
				return false;
		}
	}

	return testSelfNoSignalsPending();
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests interrupt -> thread communication scenario. Main (current) thread waits for signals. Software timer generates
 * signal for main thread at specified time point from interrupt context, main thread is expected to accept this signal
 * (with ThisThread::Signals::wait(), ThisThread::Signals::tryWaitFor() and ThisThread::Signals::tryWaitUntil()) in the
 * same moment.
 *
 * \param [in] sendSignal is a reference to function used to send signal to current thread
 * \param [in] testReceivedSignalInformation is a reference to TestReceivedSignalInformation function used to test
 * received SignalInformation object
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2(const SendSignal& sendSignal, const TestReceivedSignalInformation& testReceivedSignalInformation)
{
	const SignalSet fullSignalSet {SignalSet::full};
	auto& mainThread = ThisThread::get();
	uint8_t sharedSignalNumber {};
	int sharedValue {};
	auto softwareTimer = makeStaticSoftwareTimer(
			[&sendSignal, &mainThread, &sharedSignalNumber, &sharedValue]()
			{
				sendSignal(mainThread, sharedSignalNumber, sharedValue);
			});

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedSignalNumber = 3;
		sharedValue = 0x5b518098;
		softwareTimer.start(wakeUpTimePoint);

		// no signals are currently pending, but ThisThread::Signals::wait() should succeed at expected time
		const auto waitResult = ThisThread::Signals::wait(fullSignalSet);
		const auto wokenUpTimePoint = TickClock::now();
		auto& signalInformation = waitResult.second;
		if (waitResult.first != 0 ||
				testReceivedSignalInformation(signalInformation, sharedSignalNumber, sharedValue) != true ||
				wakeUpTimePoint != wokenUpTimePoint ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase2SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testSelfNoSignalsPending();	// no signals may be pending
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedSignalNumber = 26;
		sharedValue = 0x7b3b5785;
		softwareTimer.start(wakeUpTimePoint);

		// no signals are currently pending, but ThisThread::Signals::tryWaitFor() should succeed at expected time
		const auto tryWaitResult = ThisThread::Signals::tryWaitFor(fullSignalSet, wakeUpTimePoint - TickClock::now() +
				longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		auto& signalInformation = tryWaitResult.second;
		if (tryWaitResult.first != 0 ||
				testReceivedSignalInformation(signalInformation, sharedSignalNumber, sharedValue) != true ||
				wakeUpTimePoint != wokenUpTimePoint ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase2SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testSelfNoSignalsPending();	// no signals may be pending
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedSignalNumber = 5;
		sharedValue = 0x7b352231;
		softwareTimer.start(wakeUpTimePoint);

		// no signals are currently pending, but ThisThread::Signals::tryWaitUntil() should succeed at expected time
		const auto tryWaitResult = ThisThread::Signals::tryWaitUntil(fullSignalSet, wakeUpTimePoint + longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		auto& signalInformation = tryWaitResult.second;
		if (tryWaitResult.first != 0 ||
				testReceivedSignalInformation(signalInformation, sharedSignalNumber, sharedValue) != true ||
				wakeUpTimePoint != wokenUpTimePoint ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase2SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testSelfNoSignalsPending();	// no signals may be pending
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 3 of test case.
 *
 * Tests various error cases:
 * - attempt to send a signal with invalid signal number must fail with EINVAL;
 * - attempt to send a signal to thread that has disabled reception of signals must fail with ENOTSUP;
 * - attempt to call ThisThread::Signals::wait(), ThisThread::Signals::tryWait(), ThisThread::Signals::tryWaitFor() or
 * ThisThread::Signals::tryWaitUntil() from thread that has disabled reception of signals must fail with ENOTSUP;
 *
 * \param [in] sendSignal is a reference to function used to send signal to selected thread
 * \param [in] testReceivedSignalInformation is a reference to TestReceivedSignalInformation function used to test
 * received SignalInformation object (ignored)
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3(const SendSignal& sendSignal, const TestReceivedSignalInformation&)
{
	auto& mainThread = ThisThread::get();
	for (auto signalNumber = SignalSet::Bitset{}.size(); signalNumber <= UINT8_MAX; ++signalNumber)
	{
		const auto ret = sendSignal(mainThread, signalNumber, {});
		if (ret != EINVAL)
			return false;
	}

	{
		auto testThread = makeDynamicThread({testThreadStackSize, false, 0, 0, 0}, [](){});
		const auto ret = sendSignal(testThread, {}, {});
		if (ret != ENOTSUP)
			return false;
	}

	const SignalSet signalSet {SignalSet::full};

	{
		int sharedRet {};
		auto testThread = makeAndStartDynamicThread({testThreadStackSize, false, 0, 0, UINT8_MAX}, waitWrapper,
				std::ref(signalSet), std::ref(sharedRet));
		testThread.join();
		if (sharedRet != ENOTSUP)
			return false;
	}

	{
		int sharedRet {};
		auto testThread = makeAndStartDynamicThread({testThreadStackSize, false, 0, 0, UINT8_MAX}, tryWaitWrapper,
				std::ref(signalSet), std::ref(sharedRet));
		testThread.join();
		if (sharedRet != ENOTSUP)
			return false;
	}

	{
		int sharedRet {};
		auto testThread = makeAndStartDynamicThread({testThreadStackSize, false, 0, 0, UINT8_MAX}, tryWaitForWrapper,
				std::ref(signalSet), std::ref(sharedRet));
		testThread.join();
		if (sharedRet != ENOTSUP)
			return false;
	}

	{
		int sharedRet {};
		auto testThread = makeAndStartDynamicThread({testThreadStackSize, false, 0, 0, UINT8_MAX}, tryWaitUntilWrapper,
				std::ref(signalSet), std::ref(sharedRet));
		testThread.join();
		if (sharedRet != ENOTSUP)
			return false;
	}

	return true;
}

/**
 * \brief Phase 4 of test case.
 *
 * Tests ignoring of pending signal and sending of ignored signal.
 *
 * \param [in] sendSignal is a reference to function used to send signal to selected thread
 * \param [in] testReceivedSignalInformation is a reference to TestReceivedSignalInformation function used to test
 * received SignalInformation object (ignored)
 *
 * \return true if test succeeded, false otherwise
 */

bool phase4(const SendSignal& sendSignal, const TestReceivedSignalInformation&)
{
#if defined(DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS) && DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS > 0

	constexpr uint8_t testSignalNumber {14};
	if (testSelfSendSignal(sendSignal, testSignalNumber, {}) == false)
		return false;

	// ignore pending signal
	const auto setSignalActionResult = ThisThread::Signals::setSignalAction(testSignalNumber, SignalAction{});
	if (setSignalActionResult.first != 0)
		return false;

	if (testSelfNoSignalsPending() == false)	// no signals may be pending
		return false;

	if (sendSignal(ThisThread::get(), testSignalNumber, {}) != 0)	// send ignored signal
		return false;

	if (testSelfNoSignalsPending() == false)	// no signals may be pending
		return false;

	int ret;
	// restore previous signal action
	std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(testSignalNumber, setSignalActionResult.second);
	return ret == 0;

#else	// !defined(DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS) || DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS <= 0

	static_cast<void>(sendSignal);	// suppress warning
	return true;

#endif	// !defined(DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS) || DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS <= 0
}

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of \a stages array
#if defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0
constexpr size_t stagesSize {2};
#else	// !defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) || DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS <= 0
constexpr size_t stagesSize {1};
#endif	// !defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) || DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS <= 0

/// test stages
const std::array<Stage, stagesSize> stages
{{
		{generateSignalWrapper, testReceivedGeneratedSignal},
#if defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0
		{queueSignalWrapper, testReceivedQueuedSignal},
#endif	// defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0
}};

}	// namespace

#endif	// #if SIGNALS_WAIT_OPERATIONS_TEST_CASE_ENABLED == 1

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsWaitOperationsTestCase::run_() const
{
#if SIGNALS_WAIT_OPERATIONS_TEST_CASE_ENABLED == 1

	constexpr auto phase1ExpectedContextSwitchCount = 6 * waitForNextTickContextSwitchCount +
			2 * phase1TimedOutWaitContextSwitchCount;
	constexpr auto phase2ExpectedContextSwitchCount = 3 * waitForNextTickContextSwitchCount +
			3 * phase2SoftwareTimerContextSwitchCount;
	constexpr auto phase3ExpectedContextSwitchCount = 4 * phase3ThreadContextSwitchCount;
	constexpr auto expectedContextSwitchCount = (phase1ExpectedContextSwitchCount + phase2ExpectedContextSwitchCount +
			phase3ExpectedContextSwitchCount) * stages.size();

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (auto& stage : stages)
		for (const auto& function : {phase1, phase2, phase3, phase4})
		{
			{
				const auto ret = testSelfNoSignalsPending();	// initially no signals may be pending
				if (ret != true)
					return ret;
			}

			const auto ret = function(stage.first, stage.second);
			if (ret != true)
				return ret;
		}

	const auto ret = testSelfNoSignalsPending();	// after the test no signals may be pending
	if (ret != true)
		return ret;

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

#endif	// #if SIGNALS_WAIT_OPERATIONS_TEST_CASE_ENABLED == 1

	return true;
}

}	// namespace test

}	// namespace distortos
