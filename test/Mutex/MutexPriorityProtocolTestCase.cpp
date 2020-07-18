/**
 * \file
 * \brief MutexPriorityProtocolTestCase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "MutexPriorityProtocolTestCase.hpp"

#include "SequenceAsserter.hpp"
#include "wasteTime.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/Mutex.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"

#include "estd/ContiguousRange.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// single test step for test thread
struct TestStep
{
	/// mutex that will be used by function, nullptr to do nothing in this step
	Mutex* mutex;

	/// function executed on mutex, nullptr to do nothing in this step
	int (Mutex::*function)();

	/// sequence point right before calling function
	int sequencePointBefore;

	/// sequence point right after return from function
	int sequencePointAfter;

	/// duration wasted after executing function
	TickClock::duration duration;
};

/// range with test steps
using TestStepRange = estd::ContiguousRange<const TestStep>;

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/// total number of test threads
constexpr size_t totalThreads {5};

/// duration unit used in test
constexpr TickClock::duration durationUnit {10};

/// array with delays of test threads (in durationUnit)
const std::array<int, totalThreads> threadDelays
{{
		0, 3, 6, 10, 11,
}};

/// number of context switches caused by delayed start of test threads
constexpr size_t delayedStartContextSwitchesCount {4};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread
 *
 * First thing done is going to sleep for requested amount of time. Then all steps from provided TestStepRange are
 * executed:
 * - mark "before" sequence point,
 * - if provided, execute function on mutex,
 * - mark "after" sequence point,
 * - waste time.
 *
 * \param [in] sleepUntil is the time_point at which test steps will start to be executed
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] steps is a reference to TestStepRange with test steps for this instance
 */

void thread(const TickClock::time_point sleepUntil, SequenceAsserter& sequenceAsserter, const TestStepRange& steps)
{
	ThisThread::sleepUntil(sleepUntil);

	for (const auto& step : steps)
	{
		sequenceAsserter.sequencePoint(step.sequencePointBefore);
		if (step.mutex != nullptr && step.function != nullptr)
			(step.mutex->*step.function)();
		sequenceAsserter.sequencePoint(step.sequencePointAfter);
		wasteTime(TickClock::now() + step.duration);
	}
}

/**
 * \brief Makes and starts test thread
 *
 * \param [in] priority is the priority of thread
 * \param [in] sleepUntil is the time_point at which test steps will start to be executed by thread
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] steps is a reference to TestStepRange with test steps for this instance
 *
 * \return constructed and started DynamicThread object
 */

DynamicThread makeAndStartTestThread(const uint8_t priority, const TickClock::time_point sleepUntil,
		SequenceAsserter& sequenceAsserter, const TestStepRange& steps)
{
	return makeAndStartDynamicThread({testThreadStackSize, priority, SchedulingPolicy::fifo}, thread, sleepUntil,
			std::ref(sequenceAsserter), std::ref(steps));
}

/**
 * \brief Common code to run the test
 *
 * \param [in] delays is an array with delays for each test thread
 * \param [in] stepsRanges is an array with TestStepRange elements for each test thread
 * \param [in] expectedContextSwitchesCount is the expected count of context switches for the whole test
 *
 * \return true if the test case succeeded, false otherwise
 */

bool testRunner(const std::array<int, totalThreads>& delays, const std::array<TestStepRange, totalThreads>& stepsRanges,
		const size_t expectedContextSwitchesCount)
{
	const auto contextSwitchCount = statistics::getContextSwitchCount();

	SequenceAsserter sequenceAsserter;
	const auto now = TickClock::now();

	std::array<DynamicThread, totalThreads> threads
	{{
			makeAndStartTestThread(1, now + durationUnit * delays[0], sequenceAsserter, stepsRanges[0]),
			makeAndStartTestThread(2, now + durationUnit * delays[1], sequenceAsserter, stepsRanges[1]),
			makeAndStartTestThread(3, now + durationUnit * delays[2], sequenceAsserter, stepsRanges[2]),
			makeAndStartTestThread(4, now + durationUnit * delays[3], sequenceAsserter, stepsRanges[3]),
			makeAndStartTestThread(5, now + durationUnit * delays[4], sequenceAsserter, stepsRanges[4]),
	}};

	for (auto& thread : threads)
		thread.join();

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchesCount)
		return false;

	const auto totalSequencePoints = 2 * (stepsRanges[0].size() + stepsRanges[1].size() + stepsRanges[2].size() +
			stepsRanges[3].size() + stepsRanges[4].size());
	return sequenceAsserter.assertSequence(totalSequencePoints);
}

/**
 * \brief Test phase of priorityInheritance protocol.
 *
 * \return true if the test phase succeeded, false otherwise
 */

bool priorityInheritancePhase()
{
	Mutex mutex12 {Mutex::Protocol::priorityInheritance};	// shared by thread 1 and 2
	Mutex mutex123 {Mutex::Protocol::priorityInheritance};	// shared by thread 1, 2 and 3
	Mutex mutex13 {Mutex::Protocol::priorityInheritance};	// shared by thread 1 and 3
	Mutex mutex23 {Mutex::Protocol::priorityInheritance};	// shared by thread 2 and 3
	Mutex mutex35 {Mutex::Protocol::priorityInheritance};	// shared by thread 3 and 5

	const TestStep steps1[]
	{
			{nullptr, nullptr, 0, 1, durationUnit * 1},
			{&mutex13, &Mutex::lock, 2, 3, durationUnit * 1},
			{&mutex123, &Mutex::lock, 4, 5, durationUnit * 1},
			{&mutex12, &Mutex::lock, 11, 12, durationUnit * 2},
			{nullptr, nullptr, 18, 19, durationUnit * 2},
			{&mutex13, &Mutex::unlock, 25, 28, durationUnit * 1},
			{&mutex123, &Mutex::unlock, 29, 32, durationUnit * 1},
			{&mutex12, &Mutex::unlock, 33, 57, durationUnit * 1},
	};
	const TestStep steps2[]
	{
			{nullptr, nullptr, 6, 7, durationUnit * 1},
			{&mutex23, &Mutex::lock, 8, 9, durationUnit * 1},
			{&mutex123, &Mutex::lock, 10, 30, durationUnit * 1},
			{&mutex12, &Mutex::lock, 31, 34, durationUnit * 2},
			{&mutex23, &Mutex::unlock, 35, 38, durationUnit * 1},
			{&mutex123, &Mutex::unlock, 39, 54, durationUnit * 1},
			{&mutex12, &Mutex::unlock, 55, 56, durationUnit * 1},
	};
	const TestStep steps3[]
	{
			{nullptr, nullptr, 13, 14, durationUnit * 1},
			{&mutex35, &Mutex::lock, 15, 16, durationUnit * 1},
			{&mutex13, &Mutex::lock, 17, 26, durationUnit * 1},
			{&mutex23, &Mutex::lock, 27, 36, durationUnit * 1},
			{&mutex123, &Mutex::lock, 37, 40, durationUnit * 2},
			{&mutex35, &Mutex::unlock, 41, 47, durationUnit * 1},
			{&mutex123, &Mutex::unlock, 48, 49, durationUnit * 1},
			{&mutex23, &Mutex::unlock, 50, 51, durationUnit * 1},
			{&mutex13, &Mutex::unlock, 52, 53, durationUnit * 1},
	};
	const TestStep steps4[]
	{
			{nullptr, nullptr, 20, 21, durationUnit * 2},
			{nullptr, nullptr, 45, 46, durationUnit * 3},
	};
	const TestStep steps5[]
	{
			{nullptr, nullptr, 22, 23, durationUnit * 1},
			{&mutex35, &Mutex::lock, 24, 42, durationUnit * 2},
			{&mutex35, &Mutex::unlock, 43, 44, durationUnit * 1},
	};
	const std::array<TestStepRange, totalThreads> stepsRanges
	{{
			TestStepRange{steps1},
			TestStepRange{steps2},
			TestStepRange{steps3},
			TestStepRange{steps4},
			TestStepRange{steps5},
	}};

	return testRunner(threadDelays, stepsRanges, 22 + delayedStartContextSwitchesCount);
}

/**
 * \brief Test phase of priorityProtect protocol.
 *
 * \return true if the test phase succeeded, false otherwise
 */

bool priorityProtectPhase()
{
	Mutex mutex12 {Mutex::Protocol::priorityProtect, 2};	// shared by thread 1 and 2
	Mutex mutex123 {Mutex::Protocol::priorityProtect, 3};	// shared by thread 1, 2 and 3
	Mutex mutex13 {Mutex::Protocol::priorityProtect, 3};	// shared by thread 1 and 3
	Mutex mutex23 {Mutex::Protocol::priorityProtect, 3};	// shared by thread 2 and 3
	Mutex mutex35 {Mutex::Protocol::priorityProtect, 5};	// shared by thread 3 and 5

	const TestStep steps1[]
	{
			{nullptr, nullptr, 0, 1, durationUnit * 1},
			{&mutex13, &Mutex::lock, 2, 3, durationUnit * 1},
			{&mutex123, &Mutex::lock, 4, 5, durationUnit * 1},
			{&mutex12, &Mutex::lock, 6, 7, durationUnit * 2},
			{nullptr, nullptr, 8, 9, durationUnit * 2},
			{&mutex13, &Mutex::unlock, 10, 11, durationUnit * 1},
			{&mutex123, &Mutex::unlock, 12, 41, durationUnit * 1},
			{&mutex12, &Mutex::unlock, 42, 57, durationUnit * 1},
	};
	const TestStep steps2[]
	{
			{nullptr, nullptr, 43, 44, durationUnit * 1},
			{&mutex23, &Mutex::lock, 45, 46, durationUnit * 1},
			{&mutex123, &Mutex::lock, 47, 48, durationUnit * 1},
			{&mutex12, &Mutex::lock, 49, 50, durationUnit * 2},
			{&mutex23, &Mutex::unlock, 51, 52, durationUnit * 1},
			{&mutex123, &Mutex::unlock, 53, 54, durationUnit * 1},
			{&mutex12, &Mutex::unlock, 55, 56, durationUnit * 1},
	};
	const TestStep steps3[]
	{
			{nullptr, nullptr, 13, 14, durationUnit * 1},
			{&mutex35, &Mutex::lock, 15, 16, durationUnit * 1},
			{&mutex13, &Mutex::lock, 17, 18, durationUnit * 1},
			{&mutex23, &Mutex::lock, 19, 20, durationUnit * 1},
			{&mutex123, &Mutex::lock, 21, 22, durationUnit * 2},
			{&mutex35, &Mutex::unlock, 23, 34, durationUnit * 1},
			{&mutex123, &Mutex::unlock, 35, 36, durationUnit * 1},
			{&mutex23, &Mutex::unlock, 37, 38, durationUnit * 1},
			{&mutex13, &Mutex::unlock, 39, 40, durationUnit * 1},
	};
	const TestStep steps4[]
	{
			{nullptr, nullptr, 30, 31, durationUnit * 2},
			{nullptr, nullptr, 32, 33, durationUnit * 3},
	};
	const TestStep steps5[]
	{
			{nullptr, nullptr, 24, 25, durationUnit * 1},
			{&mutex35, &Mutex::lock, 26, 27, durationUnit * 2},
			{&mutex35, &Mutex::unlock, 28, 29, durationUnit * 1},
	};
	const std::array<TestStepRange, totalThreads> stepsRanges
	{{
			TestStepRange{steps1},
			TestStepRange{steps2},
			TestStepRange{steps3},
			TestStepRange{steps4},
			TestStepRange{steps5},
	}};

	return testRunner(threadDelays, stepsRanges, 9 + delayedStartContextSwitchesCount);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool MutexPriorityProtocolTestCase::run_() const
{
	const auto priorityInheritancePhaseResult = priorityInheritancePhase();
	if (priorityInheritancePhaseResult == false)
		return priorityInheritancePhaseResult;

	return priorityProtectPhase();
}

}	// namespace test

}	// namespace distortos
