/**
 * \file
 * \brief SignalsWaitTestCase class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SignalsWaitTestCase.hpp"

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread-Signals.hpp"
#include "distortos/ThisThread.hpp"

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

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/// bit shift of signal number encoded in signal value
constexpr size_t signalNumberShift {(sizeof(decltype(sigval{}.sival_int)) / 2) * 8};

/// mask used to obtain sequence point from signal value (by removing encoded signal number)
constexpr decltype(sigval{}.sival_int) signalValueMask {(1 << signalNumberShift) - 1};

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread function
using TestThreadFunction = void(SequenceAsserter&, unsigned int);

/// function executed to trigger unblocking of test thread
using Trigger = bool(Thread&, size_t, const TestPhase&);

/// pair with functions for one stage
using Stage = std::pair<const TestThreadFunction&, const Trigger&>;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread for signals that were "generated"
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

void generatedSignalsThread(SequenceAsserter& sequenceAsserter, const unsigned int firstSequencePoint)
{
	sequenceAsserter.sequencePoint(firstSequencePoint);
	const SignalSet signalSet {SignalSet::full};
	auto waitResult = ThisThread::Signals::wait(signalSet);
	if (waitResult.first != 0)
		return;
	const auto& signalInformation = waitResult.second;
	if (signalInformation.getCode() != SignalInformation::Code::generated)
		return;

	const auto signalNumber = signalInformation.getSignalNumber();
	const auto sequencePointOffset = totalThreads + totalThreads * signalNumber - signalNumber;
	sequenceAsserter.sequencePoint(signalNumber + sequencePointOffset);

	while (waitResult = ThisThread::Signals::tryWait(signalSet), waitResult.first == 0 &&
			signalInformation.getCode() == SignalInformation::Code::generated)
		sequenceAsserter.sequencePoint(signalInformation.getSignalNumber() + sequencePointOffset);
}

/**
 * \brief Trigger function that "generates" signals.
 *
 * \param [in] thread is a reference to test thread that will be triggered
 * \param [in] index is the index of currently triggered thread - equal to the order in which this function is called
 * during single phase
 * \param [in] phase is a reference to current TestPhase
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool generatedSignalsTrigger(Thread& thread, const size_t index, const TestPhase& phase)
{
	for (size_t i = 0; i < phase.second.size(); ++i)
	{
		const auto ret = thread.generateSignal(index + phase.second[i]);
		if (ret != 0)
			return false;
	}

	return true;
}

/**
 * \brief Test thread for signals that were "queued"
 *
 * Marks the first sequence point in SequenceAsserter, waits for any possible signal. Value of each accepted signal
 * is split into two parts:
 * - sequence point which is obtained by masking with \a signalValueMask,
 * - signal number which is obtained by shifting right by \a signalNumberShift.
 * Signal number taken from SignalInformation object must be equal to signal number encoded in signal value.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] firstSequencePoint is the first sequence point of this instance
 */

void queuedSignalsThread(SequenceAsserter& sequenceAsserter, const unsigned int firstSequencePoint)
{
	sequenceAsserter.sequencePoint(firstSequencePoint);
	const SignalSet signalSet {SignalSet::full};
	auto waitResult = ThisThread::Signals::wait(signalSet);
	if (waitResult.first != 0)
		return;
	const auto& signalInformation = waitResult.second;
	if (signalInformation.getCode() != SignalInformation::Code::queued)
		return;

	const auto signalNumber = signalInformation.getSignalNumber();
	const auto signalValue = signalInformation.getValue().sival_int;
	if (signalNumber != signalValue >> signalNumberShift)
		return;
	sequenceAsserter.sequencePoint(signalValue & signalValueMask);

	while (waitResult = ThisThread::Signals::tryWait(signalSet), waitResult.first == 0 &&
			signalInformation.getCode() == SignalInformation::Code::queued &&
			signalInformation.getSignalNumber() == signalInformation.getValue().sival_int >> signalNumberShift)
		sequenceAsserter.sequencePoint(signalInformation.getValue().sival_int & signalValueMask);
}

/**
 * \brief Trigger function that "queues" signals.
 *
 * \param [in] thread is a reference to test thread that will be triggered
 * \param [in] index is the index of currently triggered thread - equal to the order in which this function is called
 * during single phase
 * \param [in] phase is a reference to current TestPhase
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool queuedSignalsTrigger(Thread& thread, const size_t index, const TestPhase& phase)
{
	for (size_t i = 0; i < phase.second.size(); ++i)
	{
		const auto signalNumber = maxPhasePriority - phase.first[phase.second[i]].first;
		const decltype(sigval{}.sival_int) signalValue =
				(totalThreads * (index + 1) + phase.first[phase.second[i]].second) |
				signalNumber << signalNumberShift;
		const auto ret = thread.queueSignal(signalNumber, sigval{signalValue});
		if (ret != 0)
			return false;
	}

	return true;
}

/**
 * \brief Builder of test threads
 *
 * \param [in] testThreadFunction is a reference to test thread function that will be used in test thread
 * \param [in] priority is the thread's priority
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] firstSequencePoint is the first sequence point for this instance - equal to the order in which this
 * thread will be started
 *
 * \return constructed DynamicThread object
 */

DynamicThread makeTestThread(const TestThreadFunction& testThreadFunction, const uint8_t priority,
		SequenceAsserter& sequenceAsserter, const unsigned int firstSequencePoint)
{
	return makeDynamicThread({testThreadStackSize, true, totalThreads, 0, priority}, testThreadFunction,
			std::ref(sequenceAsserter), firstSequencePoint);
}

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// test stages
const std::array<Stage, 2> stages
{{
		{generatedSignalsThread, generatedSignalsTrigger},
		{queuedSignalsThread, queuedSignalsTrigger},
}};

}	// namespace

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsWaitTestCase::run_() const
{
#if DISTORTOS_SIGNALS_ENABLE == 1

	// priority required for this whole test to work
	static_assert(testCasePriority_ + 2 <= UINT8_MAX, "Invalid test case priority");
	constexpr decltype(testCasePriority_) testThreadPriority {testCasePriority_ + 1};
	constexpr decltype(testCasePriority_) aboveTestThreadPriority {testThreadPriority + 1};

	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};

	for (const auto& stage : stages)
		for (const auto& phase : priorityTestPhases)
		{
			SequenceAsserter sequenceAsserter;

			auto& threadFunction = stage.first;
			std::array<DynamicThread, totalThreads> threads
			{{
					makeTestThread(threadFunction, testThreadPriority, sequenceAsserter, 0),
					makeTestThread(threadFunction, testThreadPriority, sequenceAsserter, 1),
					makeTestThread(threadFunction, testThreadPriority, sequenceAsserter, 2),
					makeTestThread(threadFunction, testThreadPriority, sequenceAsserter, 3),
					makeTestThread(threadFunction, testThreadPriority, sequenceAsserter, 4),
					makeTestThread(threadFunction, testThreadPriority, sequenceAsserter, 5),
					makeTestThread(threadFunction, testThreadPriority, sequenceAsserter, 6),
					makeTestThread(threadFunction, testThreadPriority, sequenceAsserter, 7),
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

			for (const auto& thread : threads)
				if (thread.getState() != ThreadState::waitingForSignal)
					result = false;

			if (sequenceAsserter.assertSequence(totalThreads) == false)
				result = false;

			for (size_t i = 0; i < phase.second.size(); ++i)
			{
				auto& thread = threads[phase.second[i]];
				ThisThread::setPriority(aboveTestThreadPriority);

				const auto triggerResult = stage.second(thread, i, phase);	// execute "trigger"
				if (triggerResult == false)
					result = false;

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

	constexpr auto totalContextSwitches = 4 * totalThreads * priorityTestPhases.size() * stages.size();
	if (statistics::getContextSwitchCount() - contextSwitchCount != totalContextSwitches)
		return false;

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

	return true;
}

}	// namespace test

}	// namespace distortos
