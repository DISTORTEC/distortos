/**
 * \file
 * \brief ThreadPriorityChangeTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-19
 */

#include "ThreadPriorityChangeTestCase.hpp"

#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
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
constexpr size_t testThreadStackSize {192};

/// number of test threads
constexpr size_t totalThreads {10};

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
 * \param [in] priority is the thread's priority
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const uint8_t priority, SequenceAsserter& sequenceAsserter, const unsigned int sequencePoint)
{
	return makeStaticThread<testThreadStackSize>(priority, thread, std::ref(sequenceAsserter),
			static_cast<unsigned int>(sequencePoint));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadPriorityChangeTestCase::Implementation::run_() const
{
	const auto thisThreadPriority = ThisThread::getPriority();
	const decltype(thisThreadPriority) testThreadPriority = thisThreadPriority / 2;

	if (thisThreadPriority - testThreadPriority < 3)	// difference required for this whole test to work
		return false;

	SequenceAsserter sequenceAsserter;

	std::array<TestThread, totalThreads> threads
	{{
			makeTestThread(testThreadPriority, sequenceAsserter, 4),	// change to same priority
			makeTestThread(testThreadPriority, sequenceAsserter, 0),	// rise
			makeTestThread(testThreadPriority, sequenceAsserter, 7),	// lower
			makeTestThread(testThreadPriority, sequenceAsserter, 8),	// lower with "always behind" mode
			makeTestThread(testThreadPriority, sequenceAsserter, 1),	// rise
			makeTestThread(testThreadPriority, sequenceAsserter, 5),	// change to same priority
			makeTestThread(testThreadPriority, sequenceAsserter, 2),	// rise
			makeTestThread(testThreadPriority, sequenceAsserter, 6),	// lower
			makeTestThread(testThreadPriority, sequenceAsserter, 9),	// lower with "always behind" mode
			makeTestThread(testThreadPriority, sequenceAsserter, 3),	// rise
	}};

	for (auto& thread : threads)
		thread.start();

	threads[0].setPriority(threads[0].getPriority());
	threads[1].setPriority(threads[1].getPriority() + 2);
	threads[2].setPriority(threads[2].getPriority() - 2);
	threads[3].setPriority(threads[3].getPriority() - 2, true);
	threads[4].setPriority(threads[4].getPriority() + 2);
	threads[5].setPriority(threads[5].getPriority(), true);			// useless "always behind" mode
	threads[6].setPriority(threads[6].getPriority() + 2, true);		// useless "always behind" mode
	threads[7].setPriority(threads[7].getPriority() - 2, false);	// explicit "always before" mode
	threads[8].setPriority(threads[8].getPriority() - 2, true);
	threads[9].setPriority(threads[9].getPriority() + 2, true);		// useless "always behind" mode

	const auto result1 = sequenceAsserter.assertSequence(0);	// no thread finished
	ThisThread::setPriority(threads[1].getPriority() - 1);
	const auto result2 = sequenceAsserter.assertSequence(4);	// high priority threads finished
	ThisThread::setPriority(threads[0].getPriority() - 1);
	const auto result3 = sequenceAsserter.assertSequence(6);	// middle priority threads finished
	ThisThread::setPriority(threads[2].getPriority() - 1);

	for (auto& thread : threads)
		thread.join();

	ThisThread::setPriority(thisThreadPriority);

	if (result1 == false || result2 == false || result3 == false ||
			sequenceAsserter.assertSequence(totalThreads) == false)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
