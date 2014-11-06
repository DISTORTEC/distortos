/**
 * \file
 * \brief MutexPriorityProtocolTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-06
 */

#include "MutexPriorityProtocolTestCase.hpp"

#include "SequenceAsserter.hpp"
#include "wasteTime.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/Mutex.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/statistics.hpp"

#include "distortos/estd/ContiguousRange.hpp"

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
constexpr size_t testThreadStackSize {384};

/// total number of test threads
constexpr size_t totalThreads {5};

/// duration unit used in test
constexpr TickClock::duration durationUnit {8};

// "12" is the length (in durationUnit) of the longest test thread
/// \todo remove when FIFO scheduling for threads is implemented
static_assert((durationUnit * 12).count() < CONFIG_TICK_RATE_HZ / CONFIG_ROUND_ROBIN_RATE_HZ,
		"Invalid configuration for test case - there may be no preemptions due to round-robin scheduling.");

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread(TickClock::time_point sleepUntil, SequenceAsserter& sequenceAsserter, const TestStepRange& steps);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, thread, std::declval<TickClock::time_point>(),
		std::ref(std::declval<SequenceAsserter&>()), std::ref(std::declval<const TestStepRange&>())));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
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
		wasteTime(step.duration);
	}
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] priority is the priority of thread
 * \param [in] sleepUntil is the time_point at which test steps will start to be executed by thread
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] steps is a reference to TestStepRange with test steps for this instance
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const uint8_t priority, const TickClock::time_point sleepUntil,
		SequenceAsserter& sequenceAsserter, const TestStepRange& steps)
{
	return makeStaticThread<testThreadStackSize>(priority, thread, static_cast<TickClock::time_point>(sleepUntil),
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

	std::array<TestThread, totalThreads> threads
	{{
			makeTestThread(1, now + durationUnit * delays[0], sequenceAsserter, stepsRanges[0]),
			makeTestThread(2, now + durationUnit * delays[1], sequenceAsserter, stepsRanges[1]),
			makeTestThread(3, now + durationUnit * delays[2], sequenceAsserter, stepsRanges[2]),
			makeTestThread(4, now + durationUnit * delays[3], sequenceAsserter, stepsRanges[3]),
			makeTestThread(5, now + durationUnit * delays[4], sequenceAsserter, stepsRanges[4]),
	}};

	for (auto& thread : threads)
		thread.start();

	for (auto& thread : threads)
		thread.join();

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchesCount)
		return false;

	const auto totalSequencePoints = 2 * (stepsRanges[0].size() + stepsRanges[1].size() + stepsRanges[2].size() +
			stepsRanges[3].size() + stepsRanges[4].size());
	return sequenceAsserter.assertSequence(totalSequencePoints);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool MutexPriorityProtocolTestCase::run_() const
{
	Mutex mutex12 {Mutex::Type::Normal, Mutex::Protocol::PriorityProtect, 2};	// shared by thread 1 and 2
	Mutex mutex13 {Mutex::Type::Normal, Mutex::Protocol::PriorityProtect, 3};	// shared by thread 1 and 3
	Mutex mutex23 {Mutex::Type::Normal, Mutex::Protocol::PriorityProtect, 3};	// shared by thread 2 and 3
	Mutex mutex35 {Mutex::Type::Normal, Mutex::Protocol::PriorityProtect, 5};	// shared by thread 3 and 5

	const TestStep steps1[]
	{
			{nullptr, nullptr, 0, 1, durationUnit * 1},
			{&mutex13, &Mutex::lock, 2, 3, durationUnit * 1},
			{&mutex12, &Mutex::lock, 4, 5, durationUnit * 5},
			{&mutex13, &Mutex::unlock, 6, 29, durationUnit * 1},
			{&mutex13, &Mutex::lock, 30, 31, durationUnit * 2},
			{&mutex12, &Mutex::unlock, 32, 33, durationUnit * 1},
			{&mutex13, &Mutex::unlock, 34, 45, durationUnit * 1},
	};
	const TestStep steps2[]
	{
			{nullptr, nullptr, 35, 36, durationUnit * 1},
			{&mutex23, &Mutex::lock, 37, 38, durationUnit * 1},
			{&mutex12, &Mutex::lock, 39, 40, durationUnit * 2},
			{&mutex23, &Mutex::unlock, 41, 42, durationUnit * 1},
			{&mutex12, &Mutex::unlock, 43, 44, durationUnit * 1},
	};
	const TestStep steps3[]
	{
			{nullptr, nullptr, 7, 8, durationUnit * 1},
			{&mutex35, &Mutex::lock, 9, 10, durationUnit * 1},
			{&mutex23, &Mutex::lock, 11, 12, durationUnit * 1},
			{&mutex13, &Mutex::lock, 13, 14, durationUnit * 2},
			{&mutex35, &Mutex::unlock, 15, 24, durationUnit * 1},
			{&mutex23, &Mutex::unlock, 25, 26, durationUnit * 1},
			{&mutex13, &Mutex::unlock, 27, 28, durationUnit * 1},
	};
	const TestStep steps4[]
	{
			{nullptr, nullptr, 22, 23, durationUnit * 5},
	};
	const TestStep steps5[]
	{
			{nullptr, nullptr, 16, 17, durationUnit * 1},
			{&mutex35, &Mutex::lock, 18, 19, durationUnit * 2},
			{&mutex35, &Mutex::unlock, 20, 21, durationUnit * 1},
	};
	const std::array<TestStepRange, totalThreads> stepsRanges
	{{
			TestStepRange{steps1},
			TestStepRange{steps2},
			TestStepRange{steps3},
			TestStepRange{steps4},
			TestStepRange{steps5},
	}};
	const std::array<int, totalThreads> delays
	{{
			0, 3, 6, 10, 11,
	}};

	// 9 context switches for to the test scenario itself, 6 context switches for delayed start of each test thread
	return testRunner(delays, stepsRanges, 15);
}

}	// namespace test

}	// namespace distortos
