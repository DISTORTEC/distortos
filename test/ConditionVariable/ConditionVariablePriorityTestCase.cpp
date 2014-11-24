/**
 * \file
 * \brief ConditionVariablePriorityTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-24
 */

#include "ConditionVariablePriorityTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/ConditionVariable.hpp"
#include "distortos/Mutex.hpp"
#include "distortos/statistics.hpp"

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
constexpr size_t testThreadStackSize {256};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread(SequenceAsserter& sequenceAsserter, unsigned int sequencePoint, ConditionVariable& conditionVariable,
		Mutex& mutex);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, thread,
		std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>(),
		std::ref(std::declval<ConditionVariable&>()), std::ref(std::declval<Mutex&>())));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
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
	mutex.lock();
	conditionVariable.wait(mutex);
	sequenceAsserter.sequencePoint(sequencePoint);
	mutex.unlock();
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] conditionVariable is a reference to shared condition variable
 * \param [in] mutex is a reference to shared mutex
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const ThreadParameters& threadParameters, SequenceAsserter& sequenceAsserter,
		ConditionVariable& conditionVariable, Mutex& mutex)
{
	return makeStaticThread<testThreadStackSize>(threadParameters.first, thread, std::ref(sequenceAsserter),
			static_cast<unsigned int>(threadParameters.second), std::ref(conditionVariable), std::ref(mutex));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ConditionVariablePriorityTestCase::Implementation::run_() const
{
	using Parameters = std::tuple<Mutex::Type, Mutex::Protocol, uint8_t>;
	static const std::array<Parameters, 9> parametersArray
	{{
			Parameters{Mutex::Type::Normal, Mutex::Protocol::None, {}},
			Parameters{Mutex::Type::Normal, Mutex::Protocol::PriorityProtect, UINT8_MAX},
			Parameters{Mutex::Type::Normal, Mutex::Protocol::PriorityInheritance, {}},
			Parameters{Mutex::Type::ErrorChecking, Mutex::Protocol::None, {}},
			Parameters{Mutex::Type::ErrorChecking, Mutex::Protocol::PriorityProtect, UINT8_MAX},
			Parameters{Mutex::Type::ErrorChecking, Mutex::Protocol::PriorityInheritance, {}},
			Parameters{Mutex::Type::Recursive, Mutex::Protocol::None, {}},
			Parameters{Mutex::Type::Recursive, Mutex::Protocol::PriorityProtect, UINT8_MAX},
			Parameters{Mutex::Type::Recursive, Mutex::Protocol::PriorityInheritance, {}},
	}};

	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};

	for (const auto& parameters : parametersArray)
		for (const auto& phase : priorityTestPhases)
		{
			SequenceAsserter sequenceAsserter;
			ConditionVariable conditionVariable;
			Mutex mutex {std::get<0>(parameters), std::get<1>(parameters), std::get<2>(parameters)};

			std::array<TestThread, totalThreads> threads
			{{
					makeTestThread(phase.first[phase.second[0]], sequenceAsserter, conditionVariable, mutex),
					makeTestThread(phase.first[phase.second[1]], sequenceAsserter, conditionVariable, mutex),
					makeTestThread(phase.first[phase.second[2]], sequenceAsserter, conditionVariable, mutex),
					makeTestThread(phase.first[phase.second[3]], sequenceAsserter, conditionVariable, mutex),
					makeTestThread(phase.first[phase.second[4]], sequenceAsserter, conditionVariable, mutex),
					makeTestThread(phase.first[phase.second[5]], sequenceAsserter, conditionVariable, mutex),
					makeTestThread(phase.first[phase.second[6]], sequenceAsserter, conditionVariable, mutex),
					makeTestThread(phase.first[phase.second[7]], sequenceAsserter, conditionVariable, mutex),
					makeTestThread(phase.first[phase.second[8]], sequenceAsserter, conditionVariable, mutex),
					makeTestThread(phase.first[phase.second[9]], sequenceAsserter, conditionVariable, mutex),
			}};

			bool result {true};

			for (auto& thread : threads)
			{
				thread.start();
				// 2 context switches: "into" the thread and "back" to main thread when test thread blocks on condition
				// variable
				expectedContextSwitchCount += 2;
				if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
					result = false;
			}

			for (size_t i = 0; i < threads.size(); ++i)
				conditionVariable.notifyOne();

			// for each test thread there are 2 context switches: into" the unblocked thread and "back" to main thread
			// when test thread terminates
			expectedContextSwitchCount += 2 * threads.size();
			if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
				result = false;

			for (auto& thread : threads)
				thread.join();

			if (result == false || sequenceAsserter.assertSequence(totalThreads) == false)
				return false;
		}

	if (statistics::getContextSwitchCount() - contextSwitchCount != 4 * totalThreads * priorityTestPhases.size() *
			parametersArray.size())
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
