/**
 * \file
 * \brief ThreadPriorityTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-27
 */

#include "ThreadPriorityTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

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
constexpr size_t testThreadStackSize {192};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread(SequenceAsserter& sequenceAsserter, unsigned int sequencePoint);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, thread,
		std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>()));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Just marks the sequence point in SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 */

void thread(SequenceAsserter& sequenceAsserter, const unsigned int sequencePoint)
{
	sequenceAsserter.sequencePoint(sequencePoint);
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const ThreadParameters& threadParameters, SequenceAsserter& sequenceAsserter)
{
	return makeStaticThread<testThreadStackSize>(threadParameters.first, thread, std::ref(sequenceAsserter),
			static_cast<unsigned int>(threadParameters.second));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadPriorityTestCase::run_() const
{
	for (const auto& phase : priorityTestPhases)
	{
		SequenceAsserter sequenceAsserter;

		std::array<TestThread, totalThreads> threads
		{{
				makeTestThread(phase.first[phase.second[0]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[1]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[2]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[3]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[4]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[5]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[6]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[7]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[8]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[9]], sequenceAsserter),
		}};

		{
			architecture::InterruptMaskingLock interruptMaskingLock;

			for (auto& thread : threads)
				thread.start();
		}

		for (auto& thread : threads)
			thread.join();

		if (sequenceAsserter.assertSequence(totalThreads) == false)
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
