/**
 * \file
 * \brief ConditionVariablePriorityTestCase class implementation
 *
 * \author Copyright (C) 2014-2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ConditionVariablePriorityTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/ConditionVariable.hpp"
#include "distortos/DynamicThread.hpp"
#include "distortos/Mutex.hpp"
#include "distortos/statistics.hpp"

#include <mutex>

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
constexpr size_t testThreadStackSize {512};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread
 *
 * Locks the mutex, waits for condition variable, marks the sequence point in SequenceAsserter and unlocks the mutex.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 * \param [in] conditionVariable is a reference to shared condition variable
 * \param [in] mutex is a reference to shared mutex
 */

void thread(SequenceAsserter& sequenceAsserter, const unsigned int sequencePoint, ConditionVariable& conditionVariable,
		Mutex& mutex)
{
	const std::lock_guard<Mutex> lockGuard {mutex};

	conditionVariable.wait(mutex);
	sequenceAsserter.sequencePoint(sequencePoint);
}

/**
 * \brief Builder of test threads
 *
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] conditionVariable is a reference to shared condition variable
 * \param [in] mutex is a reference to shared mutex
 *
 * \return constructed DynamicThread object
 */

DynamicThread makeTestThread(const ThreadParameters& threadParameters, SequenceAsserter& sequenceAsserter,
		ConditionVariable& conditionVariable, Mutex& mutex)
{
	return makeDynamicThread({testThreadStackSize, threadParameters.first}, thread, std::ref(sequenceAsserter),
			threadParameters.second, std::ref(conditionVariable), std::ref(mutex));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ConditionVariablePriorityTestCase::run_() const
{
	using Parameters = std::tuple<Mutex::Type, Mutex::Protocol, uint8_t>;
	static const std::array<Parameters, 9> parametersArray
	{{
			Parameters{Mutex::Type::normal, Mutex::Protocol::none, {}},
			Parameters{Mutex::Type::normal, Mutex::Protocol::priorityProtect, UINT8_MAX},
			Parameters{Mutex::Type::normal, Mutex::Protocol::priorityInheritance, {}},
			Parameters{Mutex::Type::errorChecking, Mutex::Protocol::none, {}},
			Parameters{Mutex::Type::errorChecking, Mutex::Protocol::priorityProtect, UINT8_MAX},
			Parameters{Mutex::Type::errorChecking, Mutex::Protocol::priorityInheritance, {}},
			Parameters{Mutex::Type::recursive, Mutex::Protocol::none, {}},
			Parameters{Mutex::Type::recursive, Mutex::Protocol::priorityProtect, UINT8_MAX},
			Parameters{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance, {}},
	}};

	const auto notifyOne = [](ConditionVariable& conditionVariable)
			{
				for (size_t i = 0; i < totalThreads; ++i)
					conditionVariable.notifyOne();
			};
	// notifyOne() unblocks one thread at a time, for each test thread there are 2 context switches: into" the unblocked
	// thread and "back" to main thread when test thread terminates
	constexpr uint8_t notifyOneContextSwitches {2 * totalThreads};

	const auto notifyAll = [](ConditionVariable& conditionVariable)
			{
				conditionVariable.notifyAll();
			};
	// notifyAll() unblocks all threads simultaneously from a lock zone, so there is 1 context switch for each unblocked
	// thread and one final context switch when all test threads terminate
	constexpr uint8_t notifyAllContextSwitches {totalThreads + 1};

	using Notifier = void(ConditionVariable&);
	const std::array<std::pair<const Notifier* const, uint8_t>, 2> notifiers
	{{
		{notifyOne, notifyOneContextSwitches},
		{notifyAll, notifyAllContextSwitches},
	}};

	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};

	for (const auto& notifier : notifiers)
		for (const auto& parameters : parametersArray)
			for (const auto& phase : priorityTestPhases)
			{
				SequenceAsserter sequenceAsserter;
				ConditionVariable conditionVariable;
				Mutex mutex {std::get<0>(parameters), std::get<1>(parameters), std::get<2>(parameters)};

				std::array<DynamicThread, totalThreads> threads
				{{
						makeTestThread(phase.first[phase.second[0]], sequenceAsserter, conditionVariable, mutex),
						makeTestThread(phase.first[phase.second[1]], sequenceAsserter, conditionVariable, mutex),
						makeTestThread(phase.first[phase.second[2]], sequenceAsserter, conditionVariable, mutex),
						makeTestThread(phase.first[phase.second[3]], sequenceAsserter, conditionVariable, mutex),
						makeTestThread(phase.first[phase.second[4]], sequenceAsserter, conditionVariable, mutex),
						makeTestThread(phase.first[phase.second[5]], sequenceAsserter, conditionVariable, mutex),
						makeTestThread(phase.first[phase.second[6]], sequenceAsserter, conditionVariable, mutex),
						makeTestThread(phase.first[phase.second[7]], sequenceAsserter, conditionVariable, mutex),
				}};

				bool result {true};

				for (auto& thread : threads)
				{
					thread.start();
					// 2 context switches: "into" the thread and "back" to main thread when test thread blocks on
					// condition variable
					expectedContextSwitchCount += 2;
					if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
						result = false;
				}

				for (const auto& thread : threads)
					if (thread.getState() != ThreadState::blockedOnConditionVariable)
						result = false;

				notifier.first(conditionVariable);

				expectedContextSwitchCount += notifier.second;
				if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
					result = false;

				for (auto& thread : threads)
					thread.join();

				if (result == false || sequenceAsserter.assertSequence(totalThreads) == false)
					return false;
			}

	if (statistics::getContextSwitchCount() - contextSwitchCount != priorityTestPhases.size() * parametersArray.size() *
			(4 * totalThreads + notifyOneContextSwitches + notifyAllContextSwitches))
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
