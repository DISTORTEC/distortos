/**
 * \file
 * \brief ThreadRoundRobinSchedulingTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-27
 */

#include "ThreadRoundRobinSchedulingTestCase.hpp"

#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/ThisThread.hpp"

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

/// priority of test thread
constexpr uint8_t testThreadPriority {1};

/// number of test threads
constexpr size_t totalThreads {10};

/// number of loops in test thread
constexpr size_t loops {5};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread(SequenceAsserter& sequenceAsserter, unsigned int sequencePoint, unsigned int sequenceStep);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(scheduler::makeStaticThread<testThreadStackSize>({}, thread,
		std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>(), std::declval<unsigned int>()));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Marks the sequence point in SequenceAsserter and waits for context switch. This is repeated several times.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 * \param [in] sequenceStep is the step added to sequence point in each iteration of the loop
 */

void thread(SequenceAsserter& sequenceAsserter, const unsigned int sequencePoint, const unsigned int sequenceStep)
{
	for (size_t i = 0; i < loops; ++i)
	{
		sequenceAsserter.sequencePoint(sequencePoint + i * sequenceStep);

		// this loop waits for a context switch, which is detected by checking the time between subsequent reads of
		// TickClock - if the thread is not preempted, max difference is 1 tick
		auto previous = TickClock::now();
		decltype(previous) now;
		while ((now = TickClock::now()) - previous <= TickClock::duration{1})
			previous = now;
	}
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 * \param [in] sequenceStep is the step added to sequence point in each iteration of the loop
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(SequenceAsserter& sequenceAsserter, const unsigned int sequencePoint,
		const unsigned int sequenceStep)
{
	return scheduler::makeStaticThread<testThreadStackSize>(testThreadPriority, thread, std::ref(sequenceAsserter),
			static_cast<unsigned int>(sequencePoint), static_cast<unsigned int>(sequenceStep));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadRoundRobinSchedulingTestCase::run_() const
{
	SequenceAsserter sequenceAsserter;

	std::array<TestThread, totalThreads> threads
	{{
			makeTestThread(sequenceAsserter, 0, totalThreads),
			makeTestThread(sequenceAsserter, 1, totalThreads),
			makeTestThread(sequenceAsserter, 2, totalThreads),
			makeTestThread(sequenceAsserter, 3, totalThreads),
			makeTestThread(sequenceAsserter, 4, totalThreads),
			makeTestThread(sequenceAsserter, 5, totalThreads),
			makeTestThread(sequenceAsserter, 6, totalThreads),
			makeTestThread(sequenceAsserter, 7, totalThreads),
			makeTestThread(sequenceAsserter, 8, totalThreads),
			makeTestThread(sequenceAsserter, 9, totalThreads),
	}};

	volatile bool dummyThreadTerminate {};

	// this thread is needed, because test threads need at least one other thread (which would preempt them) to
	// exit their wait loop, so there is a problem with ending the very last iteration of the very last thread...
	auto dummyThread = scheduler::makeStaticThread<testThreadStackSize>(testThreadPriority,
			[&dummyThreadTerminate]()
			{
				while (dummyThreadTerminate != true);
			});

	decltype(TickClock::now()) testStart;

	{
		architecture::InterruptMaskingLock interruptMaskingLock;

		// wait for beginning of next tick - test threads should be started in the same tick
		scheduler::ThisThread::sleepFor({});

		for (auto& thread : threads)
			thread.start();

		dummyThread.start();

		testStart = TickClock::now();
	}

	for (auto& thread : threads)
		thread.join();

	const auto testDuration = TickClock::now() - testStart;

	dummyThreadTerminate = true;
	dummyThread.join();

	if (sequenceAsserter.assertSequence(totalThreads * loops) == false)
		return false;

	// calculations need to take dummyThread into account - this is the "+ 1"
	if (testDuration != scheduler::RoundRobinQuantum::getInitial() * (totalThreads + 1) * loops)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
