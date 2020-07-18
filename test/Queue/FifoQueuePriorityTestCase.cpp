/**
 * \file
 * \brief FifoQueuePriorityTestCase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "FifoQueuePriorityTestCase.hpp"

#include "QueueWrappers.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/statistics.hpp"

#include <malloc.h>

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// array with "false" and "true" bool values
constexpr bool falseTrue[] {false, true};

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// pair of sequence points
using SequencePoints = std::pair<unsigned int, unsigned int>;

/// type of test thread function
using TestThreadFunction = void(SequenceAsserter&, SequencePoints, const QueueWrapper&);

/// function executed to prepare queue for test
using Prepare = void(const QueueWrapper&);

/// function executed on queue to trigger unblocking of test thread
using Trigger = bool(const QueueWrapper&, size_t);

/// function with final check of queue's contents after all test threads are terminated
using FinalCheck = bool(const QueueWrapper&);

/// tuple with functions for one stage, Prepare and FinalCheck may be nullptr
using Stage = std::tuple<const TestThreadFunction&, const Prepare* const, const Trigger* const,
		const FinalCheck* const>;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Final check for "pop" stage.
 *
 * The queue should contain "second" sequence points of test threads in the same order as expected by SequenceAsserter.
 *
 * \param [in] queueWrapper is a reference to shared QueueWrapper object
 *
 * \return true if final check succeeded, false otherwise
 */

bool popFinalCheck(const QueueWrapper& queueWrapper)
{
	for (size_t i = 0; i < totalThreads; ++i)
	{
		uint8_t priority {};
		OperationCountingType testValue {};
		const auto ret = queueWrapper.tryPop(priority, testValue);
		if (ret != 0 || testValue.getValue() != i + totalThreads)
			return false;
	}

	return true;
}

/**
 * \brief Prepares queue for "pop" stage - just fills it completely with increasing values.
 *
 * \param [in] queueWrapper is a reference to shared QueueWrapper object
 */

void popPrepare(const QueueWrapper& queueWrapper)
{
	for (size_t i = 0; i < totalThreads; ++i)
		queueWrapper.tryPush(uint8_t{}, OperationCountingType{i});
}

/**
 * \brief QueueWrapper::pop() test thread
 *
 * Marks the first sequence point in SequenceAsserter, waits for the last sequence point from FIFO queue and marks it in
 * SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance (second one is ignored)
 * \param [in] queueWrapper is a reference to shared QueueWrapper object
 */

void popThread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints,
		const QueueWrapper& queueWrapper)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	uint8_t priority {};
	OperationCountingType lastSequencePoint {};
	queueWrapper.pop(priority, lastSequencePoint);
	sequenceAsserter.sequencePoint(lastSequencePoint.getValue());
}

/**
 * \brief Trigger action with QueueWrapper::pop().
 *
 * \param [in] queueWrapper is a reference to shared QueueWrapper object
 * \param [in] i is the iteration counter
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool popTrigger(const QueueWrapper& queueWrapper, const size_t i)
{
	uint8_t priority {};
	OperationCountingType testValue {};
	queueWrapper.pop(priority, testValue);
	return testValue.getValue() == i;
}

/**
 * \brief QueueWrapper::push() test thread
 *
 * Marks the first sequence point in SequenceAsserter, waits for free space in FIFO queue and marks last sequence point
 * in SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 * \param [in] queueWrapper is a reference to shared QueueWrapper object
 */

void pushThread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints,
		const QueueWrapper& queueWrapper)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	queueWrapper.push(uint8_t{}, OperationCountingType{sequencePoints.second});
	sequenceAsserter.sequencePoint(sequencePoints.second);
}

/**
 * \brief Trigger action with QueueWrapper::push().
 *
 * \param [in] queueWrapper is a reference to shared QueueWrapper object
 * \param [in] i is the iteration counter
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool pushTrigger(const QueueWrapper& queueWrapper, const size_t i)
{
	queueWrapper.push(uint8_t{}, OperationCountingType{i + totalThreads});
	return true;
}

/**
 * \brief Builder of test threads
 *
 * \param [in] testThreadFunction is a reference to function that will be used in test thread
 * \param [in] firstSequencePoint is the first sequence point for this instance - equal to the order in which this
 * thread will be started
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] queueWrapper is a reference to shared QueueWrapper object
 *
 * \return constructed DynamicThread object
 */

DynamicThread makeTestThread(const TestThreadFunction& testThreadFunction, const unsigned int firstSequencePoint,
		const ThreadParameters& threadParameters, SequenceAsserter& sequenceAsserter, const QueueWrapper& queueWrapper)
{
	return makeDynamicThread({testThreadStackSize, threadParameters.first}, testThreadFunction,
			std::ref(sequenceAsserter), SequencePoints{firstSequencePoint, threadParameters.second + totalThreads},
			std::cref(queueWrapper));
}

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// test stages
const std::array<Stage, 2> stages
{{
		Stage{popThread, nullptr, pushTrigger, nullptr},
		Stage{pushThread, popPrepare, popTrigger, popFinalCheck},
}};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FifoQueuePriorityTestCase::run_() const
{
	const auto allocatedMemory = mallinfo().uordblks;
	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};
	constexpr size_t fifoQueueTypes {4};

	for (const auto& stage : stages)
		for (const auto& phase : priorityTestPhases)
			for (const auto dynamic : falseTrue)
				for (const auto raw : falseTrue)
				{
					{
						const auto queueWrapper = makeQueueWrapper<totalThreads>(dynamic, raw, true);
						SequenceAsserter sequenceAsserter;

						const auto& threadFunction = std::get<0>(stage);
						std::array<DynamicThread, totalThreads> threads
						{{
								makeTestThread(threadFunction, 0, phase.first[phase.second[0]], sequenceAsserter,
										*queueWrapper),
								makeTestThread(threadFunction, 1, phase.first[phase.second[1]], sequenceAsserter,
										*queueWrapper),
								makeTestThread(threadFunction, 2, phase.first[phase.second[2]], sequenceAsserter,
										*queueWrapper),
								makeTestThread(threadFunction, 3, phase.first[phase.second[3]], sequenceAsserter,
										*queueWrapper),
								makeTestThread(threadFunction, 4, phase.first[phase.second[4]], sequenceAsserter,
										*queueWrapper),
								makeTestThread(threadFunction, 5, phase.first[phase.second[5]], sequenceAsserter,
										*queueWrapper),
								makeTestThread(threadFunction, 6, phase.first[phase.second[6]], sequenceAsserter,
										*queueWrapper),
								makeTestThread(threadFunction, 7, phase.first[phase.second[7]], sequenceAsserter,
										*queueWrapper),
						}};

						// execute Prepare
						if (std::get<1>(stage) != nullptr)
							std::get<1>(stage)(*queueWrapper);

						bool result {true};

						for (auto& thread : threads)
						{
							thread.start();
							// 2 context switches: "into" the thread and "back" to main thread when test thread blocks
							// on FIFO queue
							expectedContextSwitchCount += 2;
							if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
								result = false;
						}

						if (sequenceAsserter.assertSequence(totalThreads) == false)
							result = false;

						for (size_t i = 0; i < threads.size(); ++i)
						{
							const auto triggerResult = std::get<2>(stage)(*queueWrapper, i);
							if (triggerResult == false)
								result = false;
							// 2 context switches: into" the unblocked thread and "back" to main thread when test thread
							// terminates
							expectedContextSwitchCount += 2;
							if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
								result = false;
						}

						for (auto& thread : threads)
							thread.join();

						// execute FinalCheck
						if (std::get<3>(stage) != nullptr && std::get<3>(stage)(*queueWrapper) == false)
							result = false;

						if (result == false || sequenceAsserter.assertSequence(totalThreads * 2) == false)
							return false;
					}

					// dynamic memory must be deallocated after each test phase
					if (mallinfo().uordblks != allocatedMemory)
						return false;
				}

	if (statistics::getContextSwitchCount() - contextSwitchCount != 2 * 4 * totalThreads * priorityTestPhases.size() *
			fifoQueueTypes)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
