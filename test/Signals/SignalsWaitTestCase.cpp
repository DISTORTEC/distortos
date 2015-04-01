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
 * Marks the first sequence point in SequenceAsserter, waits for any possible signal. The signal number of first
 * accepted signal is used to calculate "sequence point offset". This value is then used to mark sequence points for all
 * following signals that will be accepted.
 *
 * It is assumed that \a totalThreads signals will be generated for each test thread.
 *
 * First \a totalThreads sequence points will be marked before test threads block waiting for signal. Then each test
 * thread must mark sequence points in the range <em>[totalThreads * (i + 1); totalThreads * (i + 2))</em>, where \a i
 * is the index of unblocked thread in <em>[0; totalThreads)</em> range. Because it is not possible to fit all required
 * sequence points into signal number values, these are "encoded". The ranges of sequence points mentioned earlier are
 * obtained from ranges of received signal numbers in the following form <em>[0 + 1; totalThreads + i)</em>.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] firstSequencePoint is the first sequence point of this instance
 */

void thread(SequenceAsserter& sequenceAsserter, const unsigned int firstSequencePoint)
{
	sequenceAsserter.sequencePoint(firstSequencePoint);
	const SignalSet signalSet {SignalSet::full};
	auto waitResult = ThisThread::Signals::wait(signalSet);
	if (waitResult.first != 0)
		return;
	const auto& signalInformation = waitResult.second;
	if (signalInformation.getCode() != SignalInformation::Code::Generated)
		return;

	const auto signalNumber = signalInformation.getSignalNumber();
	const auto sequencePointOffset = totalThreads + totalThreads * signalNumber - signalNumber;
	sequenceAsserter.sequencePoint(signalNumber + sequencePointOffset);

	while (waitResult = ThisThread::Signals::tryWait(signalSet), waitResult.first == 0 &&
			signalInformation.getCode() == SignalInformation::Code::Generated)
		sequenceAsserter.sequencePoint(signalInformation.getSignalNumber() + sequencePointOffset);
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

			for (size_t j = 0; j < phase.second.size(); ++j)
			{
				const auto ret = threads[threadIndex].generateSignal(i + phase.second[j]);
				if (ret != 0)
					result = false;
			}

			ThisThread::setPriority(testCasePriority_);
			// 2 context switches: into" the unblocked thread and "back" to main thread when test thread terminates
			expectedContextSwitchCount += 2;
			if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
				result = false;
		}

		for (auto& thread : threads)
			thread.join();

		if (result == false || sequenceAsserter.assertSequence(totalThreads * (totalThreads + 1)) == false)
			return false;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != 4 * totalThreads * priorityTestPhases.size())
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
