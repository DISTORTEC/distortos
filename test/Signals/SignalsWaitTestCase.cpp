/**
 * \file
 * \brief SignalsWaitTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-01
 */

#include "SignalsWaitTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread-Signals.hpp"
#include "distortos/ThisThread.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {384};

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread function
using TestThreadFunction = void(SequenceAsserter&, unsigned int);

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize, true>({}, std::declval<TestThreadFunction>(),
		std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>()));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Marks the first sequence point in SequenceAsserter, waits for any possible signal and uses the accepted signal as the
 * last sequence point - marking it in SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] firstSequencePoint is the first sequence point of this instance
 */

void thread(SequenceAsserter& sequenceAsserter, const unsigned int firstSequencePoint)
{
	sequenceAsserter.sequencePoint(firstSequencePoint);
	SignalSet signalSet {SignalSet::full};
	const auto waitResult = ThisThread::Signals::wait(signalSet);
	if (waitResult.first != 0)
		return;
	auto& signalInformation = waitResult.second;
	if (signalInformation.getCode() != SignalInformation::Code::Generated)
		return;
	sequenceAsserter.sequencePoint(signalInformation.getSignalNumber());
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] priority is the thread's priority
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] firstSequencePoint is the first sequence point for this instance - equal to the order in which this
 * thread will be started
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const uint8_t priority, SequenceAsserter& sequenceAsserter,
		const unsigned int firstSequencePoint)
{
	return makeStaticThread<testThreadStackSize, true>(priority, thread, std::ref(sequenceAsserter),
			static_cast<unsigned int>(firstSequencePoint));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsWaitTestCase::Implementation::run_() const
{
	// priority required for this whole test to work
	static_assert(testCasePriority_ + 2 <= UINT8_MAX, "Invalid test case priority");
	constexpr decltype(testCasePriority_) testThreadPriority {testCasePriority_ + 1};
	constexpr decltype(testCasePriority_) aboveTestThreadPriority {testThreadPriority + 1};

	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};

	for (const auto& phase : priorityTestPhases)
	{
		SequenceAsserter sequenceAsserter;

		std::array<TestThread, totalThreads> threads
		{{
				makeTestThread(testThreadPriority, sequenceAsserter, 0),
				makeTestThread(testThreadPriority, sequenceAsserter, 1),
				makeTestThread(testThreadPriority, sequenceAsserter, 2),
				makeTestThread(testThreadPriority, sequenceAsserter, 3),
				makeTestThread(testThreadPriority, sequenceAsserter, 4),
				makeTestThread(testThreadPriority, sequenceAsserter, 5),
				makeTestThread(testThreadPriority, sequenceAsserter, 6),
				makeTestThread(testThreadPriority, sequenceAsserter, 7),
				makeTestThread(testThreadPriority, sequenceAsserter, 8),
				makeTestThread(testThreadPriority, sequenceAsserter, 9),
		}};

		bool result {true};

		for (auto& thread : threads)
		{
			thread.start();
			// 2 context switches: "into" the thread and "back" to main thread when test thread blocks
			expectedContextSwitchCount += 2;
			if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
				result = false;
		}

		if (sequenceAsserter.assertSequence(totalThreads) == false)
			result = false;

		for (size_t i = 0; i < phase.second.size(); ++i)
		{
			const auto threadIndex = phase.second[i];
			ThisThread::setPriority(aboveTestThreadPriority);
			const auto ret = threads[threadIndex].generateSignal(totalThreads + i);
			if (ret != 0)
				result = false;
			ThisThread::setPriority(testCasePriority_);
			// 2 context switches: into" the unblocked thread and "back" to main thread when test thread terminates
			expectedContextSwitchCount += 2;
			if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
				result = false;
		}

		for (auto& thread : threads)
			thread.join();

		if (result == false || sequenceAsserter.assertSequence(totalThreads * 2) == false)
			return false;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != 4 * totalThreads * priorityTestPhases.size())
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
