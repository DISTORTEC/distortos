/**
 * \file
 * \brief ThreadPriorityChangeTestCase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ThreadPriorityChangeTestCase.hpp"

#include "SequenceAsserter.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/ThisThread.hpp"

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

/// number of test threads
constexpr size_t totalThreads {8};

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
 * \brief Makes and starts test thread
 *
 * \param [in] priority is the thread's priority
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 *
 * \return constructed and started DynamicThread object
 */

DynamicThread makeAndStartTestThread(const uint8_t priority, SequenceAsserter& sequenceAsserter,
		const unsigned int sequencePoint)
{
	return makeAndStartDynamicThread({testThreadStackSize, priority}, thread, std::ref(sequenceAsserter),
			sequencePoint);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadPriorityChangeTestCase::run_() const
{
	const auto allocatedMemory = mallinfo().uordblks;

	{
		// difference required for this whole test to work
		static_assert(testCasePriority_ / 2 > 3, "Invalid test case priority");

		const auto thisThreadPriority = ThisThread::getPriority();
		const decltype(thisThreadPriority) testThreadPriority = thisThreadPriority / 2;

		SequenceAsserter sequenceAsserter;

		std::array<DynamicThread, totalThreads> threads
		{{
				makeAndStartTestThread(testThreadPriority, sequenceAsserter, 3),	// change to same priority
				makeAndStartTestThread(testThreadPriority, sequenceAsserter, 0),	// rise
				makeAndStartTestThread(testThreadPriority, sequenceAsserter, 6),	// lower
				makeAndStartTestThread(testThreadPriority, sequenceAsserter, 7),	// lower with "always behind" mode
				makeAndStartTestThread(testThreadPriority, sequenceAsserter, 4),	// change to same priority
				makeAndStartTestThread(testThreadPriority, sequenceAsserter, 1),	// rise
				makeAndStartTestThread(testThreadPriority, sequenceAsserter, 5),	// lower
				makeAndStartTestThread(testThreadPriority, sequenceAsserter, 2),	// rise
		}};

		threads[0].setPriority(threads[0].getPriority());
		threads[1].setPriority(threads[1].getPriority() + 2);
		threads[2].setPriority(threads[2].getPriority() - 2);
		threads[3].setPriority(threads[3].getPriority() - 2, true);
		threads[4].setPriority(threads[4].getPriority(), true);			// useless "always behind" mode
		threads[5].setPriority(threads[5].getPriority() + 2, true);		// useless "always behind" mode
		threads[6].setPriority(threads[6].getPriority() - 2, false);	// explicit "always before" mode
		threads[7].setPriority(threads[7].getPriority() + 2, true);		// useless "always behind" mode

		const auto result1 = sequenceAsserter.assertSequence(0);	// no thread finished
		ThisThread::setPriority(threads[1].getPriority() - 1);
		const auto result2 = sequenceAsserter.assertSequence(3);	// high priority threads finished
		ThisThread::setPriority(threads[0].getPriority() - 1);
		const auto result3 = sequenceAsserter.assertSequence(5);	// middle priority threads finished
		ThisThread::setPriority(threads[2].getPriority() - 1);

		for (auto& thread : threads)
			thread.join();

		ThisThread::setPriority(thisThreadPriority);

		if (result1 == false || result2 == false || result3 == false ||
				sequenceAsserter.assertSequence(totalThreads) == false)
			return false;
	}

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
