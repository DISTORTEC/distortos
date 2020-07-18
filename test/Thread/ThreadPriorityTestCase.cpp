/**
 * \file
 * \brief ThreadPriorityTestCase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ThreadPriorityTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/InterruptMaskingLock.hpp"

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

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread
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
 * \brief Builder of test threads
 *
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 *
 * \return constructed DynamicThread object
 */

DynamicThread makeTestThread(const ThreadParameters& threadParameters, SequenceAsserter& sequenceAsserter)
{
	return makeDynamicThread({testThreadStackSize, threadParameters.first}, thread, std::ref(sequenceAsserter),
			threadParameters.second);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadPriorityTestCase::run_() const
{
	const auto allocatedMemory = mallinfo().uordblks;

	for (const auto& phase : priorityTestPhases)
	{
		{
			SequenceAsserter sequenceAsserter;

			std::array<DynamicThread, totalThreads> threads
			{{
					makeTestThread(phase.first[phase.second[0]], sequenceAsserter),
					makeTestThread(phase.first[phase.second[1]], sequenceAsserter),
					makeTestThread(phase.first[phase.second[2]], sequenceAsserter),
					makeTestThread(phase.first[phase.second[3]], sequenceAsserter),
					makeTestThread(phase.first[phase.second[4]], sequenceAsserter),
					makeTestThread(phase.first[phase.second[5]], sequenceAsserter),
					makeTestThread(phase.first[phase.second[6]], sequenceAsserter),
					makeTestThread(phase.first[phase.second[7]], sequenceAsserter),
			}};

			{
				const InterruptMaskingLock interruptMaskingLock;

				for (auto& thread : threads)
					thread.start();
			}

			for (auto& thread : threads)
				thread.join();

			if (sequenceAsserter.assertSequence(totalThreads) == false)
				return false;
		}

		if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
