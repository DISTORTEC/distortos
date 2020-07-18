/**
 * \file
 * \brief MutexPriorityInheritanceOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "MutexPriorityInheritanceOperationsTestCase.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/Mutex.hpp"
#include "distortos/ThisThread.hpp"

#include "estd/ReverseAdaptor.hpp"

#include <cerrno>

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// functor class used in testBasicPriorityInheritance() and testPriorityChange() - it locks 0-3 mutexes and unlocks
/// them afterwards
class LockThread
{
public:

	/**
	 * \brief LockThread's constructor
	 *
	 * \param [in] mutex1 is a pointer to first mutex
	 * \param [in] mutex2 is a pointer to second mutex
	 * \param [in] mutex3 is a pointer to third mutex
	 */

	constexpr LockThread(Mutex* const mutex1, Mutex* const mutex2, Mutex* const mutex3) :
			mutexes_{mutex1, mutex2, mutex3},
			ret_{}
	{

	}

	/**
	 * \return combined return value of Mutex::lock() / Mutex::unlock()
	 */

	int getRet() const
	{
		return ret_;
	}

	/**
	 * \brief Main function of the thread.
	 *
	 * Locks all provided mutexes and then unlocks them in the same order.
	 */

	void operator()()
	{
		for (const auto mutex : mutexes_)
			if (mutex != nullptr)
			{
				const auto ret = mutex->lock();
				if (ret != 0)
					ret_ = ret;
			}

		for (const auto mutex : mutexes_)
			if (mutex != nullptr)
			{
				const auto ret = mutex->unlock();
				if (ret != 0)
					ret_ = ret;
			}
	}

private:

	/// array with pointers to mutexes
	std::array<Mutex*, 3> mutexes_;

	/// combined return value of Mutex::lock() / Mutex::unlock()
	int ret_;
};

/// functor class used in testCanceledLock() - it locks 1 or 2 mutexes (last one with timeout) and unlocks them
/// afterwards
class TryLockForThread
{
public:

	/**
	 * \brief TryLockForThread's constructor
	 *
	 * \param [in] unlockedMutex is a pointer to unlocked mutex which will be locked with no timeout, nullptr to skip
	 * this step
	 * \param [in] lockedMutex is a pointer to locked mutex on which lock attempt with timeout will be executed
	 * \param [in] duration is the duration used as argument for Mutex::tryLockFor()
	 */

	constexpr TryLockForThread(Mutex* const unlockedMutex, Mutex& lockedMutex, const TickClock::duration duration) :
			duration_{duration},
			unlockedMutex_{unlockedMutex},
			lockedMutex_(lockedMutex),
			ret_{}
	{

	}

	/**
	 * \return return value of Mutex::tryLockFor()
	 */

	int getRet() const
	{
		return ret_;
	}

	/**
	 * \brief Main function of the thread.
	 *
	 * Following steps are performed:
	 * 1. "unlocked mutex" is locked with no timeout (if it was provided)
	 * 2. attempt to lock "locked mutex" with given timeout is executed
	 * 3. if operation from step 2. succeeds (which should _NOT_ happen), this mutex is unlocked
	 * 4. "unlocked mutex" is unlocked (if it was provided)
	 *
	 * \note Values returned by operations in step 1, 3 and 4 are not checked to simplify this test.
	 */

	void operator()()
	{
		if (unlockedMutex_ != nullptr)
			unlockedMutex_->lock();

		ret_ = lockedMutex_.tryLockFor(duration_);

		// safety in case of problems with test - normally the mutex should _NOT_ be locked by this thread
		if (ret_ == 0)
			lockedMutex_.unlock();

		if (unlockedMutex_ != nullptr)
			unlockedMutex_->unlock();
	}

private:

	/// duration used as argument for Mutex::tryLockFor()
	TickClock::duration duration_;

	/// pointer to unlocked mutex which will be locked with no timeout, nullptr to skip this step
	Mutex* unlockedMutex_;

	/// reference to locked mutex on which lock attempt with timeout will be executed
	Mutex& lockedMutex_;

	/// return value of Mutex::tryLockFor()
	int ret_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// priority of current test thread
constexpr uint8_t testThreadPriority {MutexPriorityInheritanceOperationsTestCase::getTestCasePriority()};

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests basic priority inheritance mechanism of mutexes with priorityInheritance protocol.
 *
 * 8 threads are created and "connected" into a tree-like hierarchy using mutexes. This structure is presented on the
 * diagram below. Mutexes are marked with "Mx" boxes and threads with "Tx" ellipses. The higher the thread is drawn, the
 * higher its priority is (drawn on the side).
 *
 * \dot
 * digraph G
 * {
 *		{
 *			node [shape=plaintext];
 *			"+8" -> "+7" -> "+6" -> "+5" -> "+4" -> "+3" -> "+2" -> "+1" -> "+0";
 *		}
 *
 *		{rank = same; "+8"; "T7";}
 *		{rank = same; "+7"; "T6";}
 *		{rank = same; "+6"; "T5";}
 *		{rank = same; "+5"; "T4";}
 *		{rank = same; "+4"; "T3";}
 *		{rank = same; "+3"; "T2";}
 *		{rank = same; "+2"; "T1";}
 *		{rank = same; "+1"; "T0";}
 *		{rank = same; "+0"; "main";}
 *
 *		{
 *			node [shape=box];
 *			"M0"; "M1"; "M2"; "M3"; "M4"; "M5"; "M6"; "M7";
 *		}
 *
 *		"T6" -> "M6" -> "T0" -> "M0" -> "main";
 *		"T2" -> "M2" -> "T0";
 *		"T7" -> "M7" -> "T3" -> "M3" -> "T1" -> "M1" -> "main";
 *		"T4" -> "M4" -> "T2";
 *		"T5" -> "M5" -> "T1";
 * }
 * \enddot
 *
 * Main thread is expected to inherit priority of each started test thread when this thread blocks on the mutex. After
 * the last step main thread will inherit priority of thread T7 through a chain of 3 mutexes blocking 3 threads. After
 * the test (when all links are broken) all priorities are expected to return to their previous values.
 *
 * \param [in] type is the Mutex::Type that will be tested
 *
 * \return true if the test case succeeded, false otherwise
 */

bool testBasicPriorityInheritance(const Mutex::Type type)
{
	constexpr size_t totalThreads {8};

	// effective priority (relative to testThreadPriority) for each test thread in each test step
	static const std::array<std::array<uint8_t, totalThreads>, totalThreads> priorityBoosts
	{{
			{1, 2, 3, 4, 5, 6, 7, 8},
			{1, 2, 3, 4, 5, 6, 7, 8},
			{3, 2, 3, 4, 5, 6, 7, 8},
			{3, 4, 3, 4, 5, 6, 7, 8},
			{5, 4, 5, 4, 5, 6, 7, 8},
			{5, 6, 5, 4, 5, 6, 7, 8},
			{7, 6, 5, 4, 5, 6, 7, 8},
			{7, 8, 5, 8, 5, 6, 7, 8},
	}};

	std::array<Mutex, totalThreads> mutexes
	{{
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
	}};

	std::array<LockThread, totalThreads> threadObjects
	{{
			{&mutexes[6], &mutexes[2], &mutexes[0]},
			{&mutexes[5], &mutexes[3], &mutexes[1]},
			{&mutexes[4], &mutexes[2], nullptr},
			{&mutexes[7], &mutexes[3], nullptr},
			{&mutexes[4], nullptr, nullptr},
			{&mutexes[5], nullptr, nullptr},
			{&mutexes[6], nullptr, nullptr},
			{&mutexes[7], nullptr, nullptr},
	}};

	std::array<DynamicThread, totalThreads> threads
	{{
			makeDynamicThread({testThreadStackSize, static_cast<uint8_t>(testThreadPriority + priorityBoosts[0][0])},
					std::ref(threadObjects[0])),
			makeDynamicThread({testThreadStackSize, static_cast<uint8_t>(testThreadPriority + priorityBoosts[0][1])},
					std::ref(threadObjects[1])),
			makeDynamicThread({testThreadStackSize, static_cast<uint8_t>(testThreadPriority + priorityBoosts[0][2])},
					std::ref(threadObjects[2])),
			makeDynamicThread({testThreadStackSize, static_cast<uint8_t>(testThreadPriority + priorityBoosts[0][3])},
					std::ref(threadObjects[3])),
			makeDynamicThread({testThreadStackSize, static_cast<uint8_t>(testThreadPriority + priorityBoosts[0][4])},
					std::ref(threadObjects[4])),
			makeDynamicThread({testThreadStackSize, static_cast<uint8_t>(testThreadPriority + priorityBoosts[0][5])},
					std::ref(threadObjects[5])),
			makeDynamicThread({testThreadStackSize, static_cast<uint8_t>(testThreadPriority + priorityBoosts[0][6])},
					std::ref(threadObjects[6])),
			makeDynamicThread({testThreadStackSize, static_cast<uint8_t>(testThreadPriority + priorityBoosts[0][7])},
					std::ref(threadObjects[7])),
	}};

	bool result {true};

	{
		const auto ret = mutexes[0].lock();
		if (ret != 0)
			result = false;
	}
	{
		const auto ret = mutexes[1].lock();
		if (ret != 0)
			result = false;
	}

	for (size_t i = 0; i < threads.size(); ++i)
	{
		auto& thread = threads[i];
		thread.start();
		if (ThisThread::getEffectivePriority() != thread.getEffectivePriority())
			result = false;

		for (size_t j = 0; j < threads.size(); ++j)
			if (threads[j].getEffectivePriority() != testThreadPriority + priorityBoosts[i][j])
				result = false;
	}

	{
		const auto ret = mutexes[1].unlock();
		if (ret != 0)
			result = false;
	}

	if (ThisThread::getEffectivePriority() != threads[0].getEffectivePriority())
		result = false;

	{
		const auto ret = mutexes[0].unlock();
		if (ret != 0)
			result = false;
	}

	for (auto& thread : threads)
		thread.join();

	if (ThisThread::getEffectivePriority() != testThreadPriority)
		result = false;

	for (size_t i = 0; i < threads.size(); ++i)
		if (threads[i].getEffectivePriority() != testThreadPriority + priorityBoosts[0][i])
			result = false;

	for (const auto& threadObject : threadObjects)
		if (threadObject.getRet() != 0)
			result = false;

	return result;
}

/**
 * \brief Tests behavior of priority inheritance mechanism of mutexes in the event of canceled (timed-out) lock attempt.
 *
 * 8 threads are created and "connected" into a "vertical" hierarchy with current thread using mutexes (2 for each
 * thread, except the last one). Each mutex "connects" two adjacent threads. Each thread locks the first mutex
 * "normally" (with no timeout) and the second one with timeout, with exception of main thread - which locks its only
 * mutex with no timeout - and last thread - which locks its only mutex with timeout. Timeouts of each thread are
 * selected so that the highest priority thread time-outs first, and the lowest priority thread - last.
 *
 * Main thread is expected to inherit priority of each started test thread when this thread blocks on the mutex. After
 * last thread is started main thread will inherit priority of thread T7 through a chain of 8 mutexes blocking 8
 * threads. After each thread cancels block on its mutex (due to timeout) main thread's priority is expected to
 * decrease by one - to the value inherited from the new last thread in the chain.
 *
 * \param [in] type is the Mutex::Type that will be tested
 *
 * \return true if the test case succeeded, false otherwise
 */

bool testCanceledLock(const Mutex::Type type)
{
	constexpr TickClock::duration durationUnit {10};
	constexpr size_t totalThreads {8};

	std::array<Mutex, totalThreads> mutexes
	{{
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
	}};

	std::array<TryLockForThread, totalThreads> threadObjects
	{{
			{&mutexes[1], mutexes[0], durationUnit * 8},
			{&mutexes[2], mutexes[1], durationUnit * 7},
			{&mutexes[3], mutexes[2], durationUnit * 6},
			{&mutexes[4], mutexes[3], durationUnit * 5},
			{&mutexes[5], mutexes[4], durationUnit * 4},
			{&mutexes[6], mutexes[5], durationUnit * 3},
			{&mutexes[7], mutexes[6], durationUnit * 2},
			{nullptr, mutexes[7], durationUnit * 1},
	}};

	std::array<DynamicThread, totalThreads> threads
	{{
			makeDynamicThread({testThreadStackSize, testThreadPriority + 1}, std::ref(threadObjects[0])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 2}, std::ref(threadObjects[1])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 3}, std::ref(threadObjects[2])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 4}, std::ref(threadObjects[3])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 5}, std::ref(threadObjects[4])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 6}, std::ref(threadObjects[5])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 7}, std::ref(threadObjects[6])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 8}, std::ref(threadObjects[7])),
	}};

	bool result {true};

	{
		const auto ret = mutexes[0].lock();
		if (ret != 0)
			result = false;
	}

	for (auto& thread : threads)
	{
		thread.start();
		if (ThisThread::getEffectivePriority() != thread.getEffectivePriority())
			result = false;
	}

	for (auto& thread : estd::makeReverseAdaptor(threads))
	{
		thread.join();
		if (ThisThread::getEffectivePriority() != thread.getEffectivePriority() - 1)
			result = false;
	}

	{
		const auto ret = mutexes[0].unlock();
		if (ret != 0)
			result = false;
	}

	for (const auto& threadObject : threadObjects)
		if (threadObject.getRet() != ETIMEDOUT)
			result = false;

	return result;
}

/**
 * \brief Tests behavior of priority inheritance mechanism of mutexes in the event of priority change.
 *
 * 8 threads are created and "connected" into a "vertical" hierarchy with current thread using mutexes (2 for each
 * thread, except the last one). Each mutex "connects" two adjacent threads.
 *
 * Main thread is expected to inherit priority of each started test thread when this thread blocks on the mutex. After
 * last thread is started main thread will inherit priority of thread T7 through a chain of 8 mutexes blocking 8
 * threads.
 *
 * Change of priority applied to any of the threads in the chain is expected to propagate "up" this chain, up to main
 * thread.
 *
 * \param [in] type is the Mutex::Type that will be tested
 *
 * \return true if the test case succeeded, false otherwise
 */

bool testPriorityChange(const Mutex::Type type)
{
	constexpr size_t totalThreads {8};

	// index of thread, new priority
	static const std::pair<uint8_t, uint8_t> priorityChanges[]
	{
			// set all to testThreadPriority (minimal value that is not idle priority)
			{7, testThreadPriority}, {6, testThreadPriority}, {5, testThreadPriority}, {4, testThreadPriority},
			{3, testThreadPriority}, {2, testThreadPriority}, {1, testThreadPriority}, {0, testThreadPriority},

			// restore what was set previously, in reverse order
			{0, testThreadPriority + 1}, {1, testThreadPriority + 2}, {2, testThreadPriority + 3},
			{3, testThreadPriority + 4}, {4, testThreadPriority + 5}, {5, testThreadPriority + 6},
			{6, testThreadPriority + 7}, {7, testThreadPriority + 8},

			// max priority for each thread, restore previous value after each change
			{0, UINT8_MAX}, {0, testThreadPriority + 1}, {1, UINT8_MAX}, {1, testThreadPriority + 2},
			{2, UINT8_MAX}, {2, testThreadPriority + 3}, {3, UINT8_MAX}, {3, testThreadPriority + 4},
			{4, UINT8_MAX}, {4, testThreadPriority + 5}, {5, UINT8_MAX}, {5, testThreadPriority + 6},
			{6, UINT8_MAX}, {6, testThreadPriority + 7}, {7, UINT8_MAX}, {7, testThreadPriority + 8},
	};

	std::array<Mutex, totalThreads> mutexes
	{{
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
			Mutex{type, Mutex::Protocol::priorityInheritance},
	}};

	std::array<LockThread, totalThreads> threadObjects
	{{
			{&mutexes[1], &mutexes[0], nullptr},
			{&mutexes[2], &mutexes[1], nullptr},
			{&mutexes[3], &mutexes[2], nullptr},
			{&mutexes[4], &mutexes[3], nullptr},
			{&mutexes[5], &mutexes[4], nullptr},
			{&mutexes[6], &mutexes[5], nullptr},
			{&mutexes[7], &mutexes[6], nullptr},
			{&mutexes[7], nullptr, nullptr},
	}};

	std::array<DynamicThread, totalThreads> threads
	{{
			makeDynamicThread({testThreadStackSize, testThreadPriority + 1}, std::ref(threadObjects[0])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 2}, std::ref(threadObjects[1])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 3}, std::ref(threadObjects[2])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 4}, std::ref(threadObjects[3])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 5}, std::ref(threadObjects[4])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 6}, std::ref(threadObjects[5])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 7}, std::ref(threadObjects[6])),
			makeDynamicThread({testThreadStackSize, testThreadPriority + 8}, std::ref(threadObjects[7])),
	}};

	bool result {true};

	{
		const auto ret = mutexes[0].lock();
		if (ret != 0)
			result = false;
	}

	for (auto& thread : threads)
	{
		thread.start();
		if (ThisThread::getEffectivePriority() != thread.getEffectivePriority())
			result = false;
	}

	for (const auto& priorityChange : priorityChanges)
	{
		assert(priorityChange.first < threads.size());
		threads[priorityChange.first].setPriority(priorityChange.second);

		uint8_t inheritedPriority {};

		for (const auto& thread : estd::makeReverseAdaptor(threads))
		{
			const auto expectedEffectivePriority = std::max(inheritedPriority, thread.getPriority());
			const auto effectivePriority = thread.getEffectivePriority();
			if (expectedEffectivePriority != effectivePriority)
				result = false;
			inheritedPriority = effectivePriority;
		}

		const auto expectedEffectivePriority = std::max(inheritedPriority, ThisThread::getPriority());
		const auto effectivePriority = ThisThread::getEffectivePriority();
		if (expectedEffectivePriority != effectivePriority)
			result = false;
	}

	{
		const auto ret = mutexes[0].unlock();
		if (ret != 0)
			result = false;
	}

	for (auto& thread : threads)
		thread.join();

	for (const auto& threadObject : threadObjects)
		if (threadObject.getRet() != 0)
			result = false;

	return result;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool MutexPriorityInheritanceOperationsTestCase::run_() const
{
	static const Mutex::Type types[]
	{
			Mutex::Type::normal,
			Mutex::Type::errorChecking,
			Mutex::Type::recursive,
	};

	for (const auto type : types)
	{
		{
			const auto result = testBasicPriorityInheritance(type);
			if (result != true)
				return result;
		}

		{
			const auto result = testCanceledLock(type);
			if (result != true)
				return result;
		}

		{
			const auto result = testPriorityChange(type);
			if (result != true)
				return result;
		}
	}

	return true;
}

}	// namespace test

}	// namespace distortos
