/**
 * \file
 * \brief MutexPriorityInheritanceOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#include "MutexPriorityInheritanceOperationsTestCase.hpp"

#include "distortos/Mutex.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/StaticThread.hpp"

#include "distortos/estd/ReferenceHolder.hpp"
#include "distortos/estd/ReverseAdaptor.hpp"

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

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests basic priority inheritance mechanism of mutexes with PriorityInheritance protocol.
 *
 * 10 threads are created and "connected" into a tree-like hierarchy using mutexes. This structure is presented on the
 * diagram below. Mutexes are marked with "Mx" boxes and threads with "Tx" ellipses. The higher the thread is drawn, the
 * higher its priority is (drawn on the side).
 *
 * \dot
 * digraph G
 * {
 * 		{
 * 			node [shape=plaintext];
 * 			"+10" -> "+9" -> "+8" -> "+7" -> "+6" -> "+5" -> "+4" -> "+3" -> "+2" -> "+1" -> "+0";
 * 		}
 *
 * 		{rank = same; "+10"; "T111";}
 * 		{rank = same; "+9"; "T110";}
 * 		{rank = same; "+8"; "T101";}
 * 		{rank = same; "+7"; "T100";}
 * 		{rank = same; "+6"; "T11";}
 * 		{rank = same; "+5"; "T10";}
 * 		{rank = same; "+4"; "T01";}
 * 		{rank = same; "+3"; "T00";}
 * 		{rank = same; "+2"; "T1";}
 * 		{rank = same; "+1"; "T0";}
 * 		{rank = same; "+0"; "main";}
 *
 * 		{
 * 			node [shape=box];
 * 			"M0"; "M1"; "M00"; "M01"; "M10"; "M11"; "M100"; "M101"; "M110"; "M111";
 * 		}
 *
 * 		"T111" -> "M111" -> "T11" -> "M11" -> "T1" -> "M1" -> "main";
 * 		"T110" -> "M110" -> "T11";
 * 		"T101" -> "M101" -> "T10" -> "M10" -> "T1";
 * 		"T100" -> "M100" -> "T10";
 * 		"T01" -> "M01" -> "T0" -> "M0" -> "main";
 * 		"T00" -> "M00" -> "T0";
 * }
 * \enddot
 *
 * Main thread is expected to inherit priority of each started test thread when this thread blocks on the mutex. After
 * the last step main thread will inherit priority of thread T111 through a chain of 3 mutexes blocking 3 threads. After
 * the test (when all links are broken) all priorities are expected to return to their previous values.
 *
 * \param [in] type is the Mutex::Type that will be tested
 *
 * \return true if the test case succeeded, false otherwise
 */

bool testBasicPriorityInheritance(const Mutex::Type type)
{
	constexpr size_t testThreadStackSize {384};
	constexpr size_t totalThreads {10};

	// effective priority (relative to testThreadPriority) for each test thread in each test step
	static const std::array<std::array<uint8_t, totalThreads>, totalThreads> priorityBoosts
	{{
			{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
			{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
			{3, 2, 3, 4, 5, 6, 7, 8, 9, 10},
			{4, 2, 3, 4, 5, 6, 7, 8, 9, 10},
			{4, 5, 3, 4, 5, 6, 7, 8, 9, 10},
			{4, 6, 3, 4, 5, 6, 7, 8, 9, 10},
			{4, 7, 3, 4, 7, 6, 7, 8, 9, 10},
			{4, 8, 3, 4, 8, 6, 7, 8, 9, 10},
			{4, 9, 3, 4, 8, 9, 7, 8, 9, 10},
			{4, 10, 3, 4, 8, 10, 7, 8, 9, 10},
	}};

	Mutex mutex0 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex1 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex00 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex01 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex10 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex11 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex100 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex101 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex110 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex111 {type, Mutex::Protocol::PriorityInheritance};

	LockThread threadObject0 {&mutex00, &mutex01, &mutex0};
	LockThread threadObject1 {&mutex10, &mutex11, &mutex1};
	LockThread threadObject00 {&mutex00, nullptr, nullptr};
	LockThread threadObject01 {&mutex01, nullptr, nullptr};
	LockThread threadObject10 {&mutex100, &mutex101, &mutex10};
	LockThread threadObject11 {&mutex110, &mutex111, &mutex11};
	LockThread threadObject100 {&mutex100, nullptr, nullptr};
	LockThread threadObject101 {&mutex101, nullptr, nullptr};
	LockThread threadObject110 {&mutex110, nullptr, nullptr};
	LockThread threadObject111 {&mutex111, nullptr, nullptr};

	auto thread0 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][0], std::ref(threadObject0));
	auto thread1 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][1], std::ref(threadObject1));
	auto thread00 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][2], std::ref(threadObject00));
	auto thread01 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][3], std::ref(threadObject01));
	auto thread10 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][4], std::ref(threadObject10));
	auto thread11 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][5], std::ref(threadObject11));
	auto thread100 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][6], std::ref(threadObject100));
	auto thread101 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][7], std::ref(threadObject101));
	auto thread110 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][8], std::ref(threadObject110));
	auto thread111 =
			makeStaticThread<testThreadStackSize>(testThreadPriority + priorityBoosts[0][9], std::ref(threadObject111));

	using TestThreadHolder = estd::ReferenceHolder<decltype(thread0)>;
	std::array<TestThreadHolder, totalThreads> threads
	{{
			TestThreadHolder{thread0},
			TestThreadHolder{thread1},
			TestThreadHolder{thread00},
			TestThreadHolder{thread01},
			TestThreadHolder{thread10},
			TestThreadHolder{thread11},
			TestThreadHolder{thread100},
			TestThreadHolder{thread101},
			TestThreadHolder{thread110},
			TestThreadHolder{thread111},
	}};

	bool result {true};

	{
		const auto ret = mutex0.lock();
		if (ret != 0)
			result = false;
	}
	{
		const auto ret = mutex1.lock();
		if (ret != 0)
			result = false;
	}

	for (size_t i = 0; i < threads.size(); ++i)
	{
		auto& thread = threads[i].get();
		thread.start();
		if (ThisThread::getEffectivePriority() != thread.getEffectivePriority())
			result = false;

		for (size_t j = 0; j < threads.size(); ++j)
			if (threads[j].get().getEffectivePriority() != testThreadPriority + priorityBoosts[i][j])
				result = false;
	}

	{
		const auto ret = mutex1.unlock();
		if (ret != 0)
			result = false;
	}

	if (ThisThread::getEffectivePriority() != thread0.getEffectivePriority())
		result = false;

	{
		const auto ret = mutex0.unlock();
		if (ret != 0)
			result = false;
	}

	for (const auto& thread : threads)
		thread.get().join();

	if (ThisThread::getEffectivePriority() != testThreadPriority)
		result = false;

	for (size_t i = 0; i < threads.size(); ++i)
		if (threads[i].get().getEffectivePriority() != testThreadPriority + priorityBoosts[0][i])
			result = false;

	for (const auto& threadObject : {threadObject0, threadObject1, threadObject00, threadObject01, threadObject10,
			threadObject11, threadObject100, threadObject101, threadObject110, threadObject111})
		if (threadObject.getRet() != 0)
			result = false;

	return result;
}

/**
 * \brief Tests behavior of priority inheritance mechanism of mutexes in the event of canceled (timed-out) lock attempt.
 *
 * 10 threads are created and "connected" into a "vertical" hierarchy with current thread using mutexes (2 for each
 * thread, except the last one). Each mutex "connects" two adjacent threads. Each thread locks the first mutex
 * "normally" (with no timeout) and the second one with timeout, with exception of main thread - which locks its only
 * mutex with no timeout - and last thread - which locks its only mutex with timeout. Timeouts of each thread are
 * selected so that the highest priority thread time-outs first, and the lowest priority thread - last.
 *
 * Main thread is expected to inherit priority of each started test thread when this thread blocks on the mutex. After
 * last thread is started main thread will inherit priority of thread T9 through a chain of 10 mutexes blocking 10
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
	constexpr size_t testThreadStackSize {512};
	constexpr size_t totalThreads {10};

	Mutex mutex0 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex1 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex2 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex3 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex4 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex5 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex6 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex7 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex8 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex9 {type, Mutex::Protocol::PriorityInheritance};

	TryLockForThread threadObject0 {&mutex1, mutex0, durationUnit * 10};
	TryLockForThread threadObject1 {&mutex2, mutex1, durationUnit * 9};
	TryLockForThread threadObject2 {&mutex3, mutex2, durationUnit * 8};
	TryLockForThread threadObject3 {&mutex4, mutex3, durationUnit * 7};
	TryLockForThread threadObject4 {&mutex5, mutex4, durationUnit * 6};
	TryLockForThread threadObject5 {&mutex6, mutex5, durationUnit * 5};
	TryLockForThread threadObject6 {&mutex7, mutex6, durationUnit * 4};
	TryLockForThread threadObject7 {&mutex8, mutex7, durationUnit * 3};
	TryLockForThread threadObject8 {&mutex9, mutex8, durationUnit * 2};
	TryLockForThread threadObject9 {nullptr, mutex9, durationUnit * 1};

	using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, std::ref(std::declval<TryLockForThread&>())));
	std::array<TestThread, totalThreads> threads
	{{
			makeStaticThread<testThreadStackSize>(testThreadPriority + 1, std::ref(threadObject0)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 2, std::ref(threadObject1)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 3, std::ref(threadObject2)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 4, std::ref(threadObject3)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 5, std::ref(threadObject4)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 6, std::ref(threadObject5)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 7, std::ref(threadObject6)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 8, std::ref(threadObject7)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 9, std::ref(threadObject8)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 10, std::ref(threadObject9)),
	}};

	bool result {true};

	{
		const auto ret = mutex0.lock();
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
		const auto ret = mutex0.unlock();
		if (ret != 0)
			result = false;
	}

	for (const auto& threadObject : {threadObject0, threadObject1, threadObject2, threadObject3, threadObject4,
			threadObject5, threadObject6, threadObject7, threadObject8, threadObject9})
		if (threadObject.getRet() != ETIMEDOUT)
			result = false;

	return result;
}

/**
 * \brief Tests behavior of priority inheritance mechanism of mutexes in the event of priority change.
 *
 * 10 threads are created and "connected" into a "vertical" hierarchy with current thread using mutexes (2 for each
 * thread, except the last one). Each mutex "connects" two adjacent threads.
 *
 * Main thread is expected to inherit priority of each started test thread when this thread blocks on the mutex. After
 * last thread is started main thread will inherit priority of thread T9 through a chain of 10 mutexes blocking 10
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
	constexpr size_t testThreadStackSize {512};
	constexpr size_t totalThreads {10};

	// index of thread ([0;4] only!), new priority
	static const std::pair<uint8_t, uint8_t> priorityChanges[]
	{
			// set all to testThreadPriority (minimal value that is not idle priority)
			{9, testThreadPriority}, {8, testThreadPriority}, {7, testThreadPriority}, {6, testThreadPriority},
			{5, testThreadPriority}, {4, testThreadPriority}, {3, testThreadPriority}, {2, testThreadPriority},
			{1, testThreadPriority}, {0, testThreadPriority},

			// restore what was set previously, in reverse order
			{0, testThreadPriority + 1}, {1, testThreadPriority + 2}, {2, testThreadPriority + 3},
			{3, testThreadPriority + 4}, {4, testThreadPriority + 5}, {5, testThreadPriority + 6},
			{6, testThreadPriority + 7}, {7, testThreadPriority + 8}, {8, testThreadPriority + 9},
			{9, testThreadPriority + 10},

			// max priority for each thread, restore previous value after each change
			{0, UINT8_MAX}, {0, testThreadPriority + 1}, {1, UINT8_MAX}, {1, testThreadPriority + 2},
			{2, UINT8_MAX}, {2, testThreadPriority + 3}, {3, UINT8_MAX}, {3, testThreadPriority + 4},
			{4, UINT8_MAX}, {4, testThreadPriority + 5}, {5, UINT8_MAX}, {5, testThreadPriority + 6},
			{6, UINT8_MAX}, {6, testThreadPriority + 7}, {7, UINT8_MAX}, {7, testThreadPriority + 8},
			{8, UINT8_MAX}, {8, testThreadPriority + 9}, {9, UINT8_MAX}, {9, testThreadPriority + 10},
	};

	Mutex mutex0 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex1 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex2 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex3 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex4 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex5 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex6 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex7 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex8 {type, Mutex::Protocol::PriorityInheritance};
	Mutex mutex9 {type, Mutex::Protocol::PriorityInheritance};

	LockThread threadObject0 {&mutex1, &mutex0, nullptr};
	LockThread threadObject1 {&mutex2, &mutex1, nullptr};
	LockThread threadObject2 {&mutex3, &mutex2, nullptr};
	LockThread threadObject3 {&mutex4, &mutex3, nullptr};
	LockThread threadObject4 {&mutex5, &mutex4, nullptr};
	LockThread threadObject5 {&mutex6, &mutex5, nullptr};
	LockThread threadObject6 {&mutex7, &mutex6, nullptr};
	LockThread threadObject7 {&mutex8, &mutex7, nullptr};
	LockThread threadObject8 {&mutex9, &mutex8, nullptr};
	LockThread threadObject9 {&mutex9, nullptr, nullptr};

	using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, std::ref(std::declval<LockThread&>())));
	std::array<TestThread, totalThreads> threads
	{{
			makeStaticThread<testThreadStackSize>(testThreadPriority + 1, std::ref(threadObject0)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 2, std::ref(threadObject1)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 3, std::ref(threadObject2)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 4, std::ref(threadObject3)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 5, std::ref(threadObject4)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 6, std::ref(threadObject5)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 7, std::ref(threadObject6)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 8, std::ref(threadObject7)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 9, std::ref(threadObject8)),
			makeStaticThread<testThreadStackSize>(testThreadPriority + 10, std::ref(threadObject9)),
	}};

	bool result {true};

	{
		const auto ret = mutex0.lock();
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
		threads[priorityChange.first].setPriority(priorityChange.second);	/// \todo check index of thread

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
		const auto ret = mutex0.unlock();
		if (ret != 0)
			result = false;
	}

	for (auto& thread : threads)
		thread.join();

	for (const auto& threadObject : {threadObject0, threadObject1, threadObject2, threadObject3, threadObject4,
			threadObject5, threadObject6, threadObject7, threadObject8, threadObject9})
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
			Mutex::Type::Normal,
			Mutex::Type::ErrorChecking,
			Mutex::Type::Recursive,
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
