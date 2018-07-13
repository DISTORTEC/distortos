/**
 * \file
 * \brief SignalsInterruptionTestCase class implementation
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SignalsInterruptionTestCase.hpp"

#include "distortos/distortosConfiguration.h"

/// configuration required by SignalsInterruptionTestCase
#define SIGNALS_INTERRUPTION_TEST_CASE_ENABLED CONFIG_SIGNALS_ENABLE == 1 && CONFIG_MAIN_THREAD_QUEUED_SIGNALS > 0 && \
		CONFIG_MAIN_THREAD_SIGNAL_ACTIONS > 0

#if SIGNALS_INTERRUPTION_TEST_CASE_ENABLED == 1

#include "SequenceAsserter.hpp"
#include "wasteTime.hpp"

#include "distortos/ConditionVariable.hpp"
#include "distortos/DynamicThread.hpp"
#include "distortos/Mutex.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/ThisThread-Signals.hpp"

#include <mutex>

#include <cerrno>

#endif	// SIGNALS_INTERRUPTION_TEST_CASE_ENABLED == 1

namespace distortos
{

namespace test
{

#if SIGNALS_INTERRUPTION_TEST_CASE_ENABLED == 1

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// long duration used in tests
constexpr auto longDuration = TickClock::duration{10};

/// size of stack for test threads, bytes
constexpr size_t testThreadStackSize {512};

/// original priority of main test thread
constexpr auto mainTestThreadPriority = SignalsInterruptionTestCase::getTestCasePriority();

/// priority just above \a mainTestThreadPriority
constexpr decltype(mainTestThreadPriority) highPriority {mainTestThreadPriority + 1};

/// priority just below \a mainTestThreadPriority
constexpr decltype(mainTestThreadPriority) lowPriority {mainTestThreadPriority - 1};

/// priority just below \a lowPriority
constexpr decltype(mainTestThreadPriority) veryLowPriority {lowPriority - 1};

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// base of test steps
class TestStep
{
public:

	/**
	 * \brief TestStep's constructor
	 *
	 * \param [in] totalSequencePoints is the total number of sequence points used in test step
	 * \param [in] expectedContextSwitchCount is the expected number of context switches during test step
	 * \param [in] signalingThreadSequencePoint1 is the first sequence point used in signalingThreadFunction()
	 * \param [in] signalHandlerSequencePoint is the sequence point used in SignalsInterruptionTestCase::signalHandler()
	 * \param [in] joinSequencePoint is the sequence point used before joining with the thread that executes
	 * signalingThreadFunction()
	 * \param [in] signalingThreadSequencePoint2 is the second sequence point used in signalingThreadFunction()
	 */

	constexpr TestStep(const uint8_t totalSequencePoints, const uint8_t expectedContextSwitchCount,
			const uint8_t signalingThreadSequencePoint1, const uint8_t signalHandlerSequencePoint,
			const uint8_t joinSequencePoint, const uint8_t signalingThreadSequencePoint2) :
			totalSequencePoints_{totalSequencePoints},
			expectedContextSwitchCount_{expectedContextSwitchCount},
			signalingThreadSequencePoint1_{signalingThreadSequencePoint1},
			signalHandlerSequencePoint_{signalHandlerSequencePoint},
			joinSequencePoint_{joinSequencePoint},
			signalingThreadSequencePoint2_{signalingThreadSequencePoint2}
	{

	}

	/**
	 * \brief TestStep's virtual destructor
	 */

	virtual ~TestStep() = default;

	/**
	 * \brief Runs test step.
	 *
	 * Creates and starts a thread that executes signalingThreadFunction(). Then pure virtual block() is called and its
	 * execution time is measured. After that the thread is joined, duration of block() execution is checked (must be 0)
	 * and pure virtual postiondition() is called.
	 *
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 *
	 * \return true if the test step succeeded, false otherwise
	 */

	bool run(SequenceAsserter& sequenceAsserter)
	{
		auto& currentThread = ThisThread::get();
		auto signalingThread = makeDynamicThread({testThreadStackSize, lowPriority}, &TestStep::signalingThreadFunction,
				std::ref(*this), std::ref(sequenceAsserter), std::ref(currentThread));
		signalingThread.start();
		wasteTime(TickClock::duration{});
		const auto start = TickClock::now();

		const auto blockResult = block();

		const auto duration = TickClock::now() - start;
		sequenceAsserter.sequencePoint(joinSequencePoint_);
		signalingThread.join();

		if (blockResult == false)
			return false;
		if (duration != decltype(duration){})	// execution of block() must finish within single tick
			return false;
		if (postcondition() == false)
			return false;

		return true;
	}

	/**
	 * \brief Gets execution parameters of test step.
	 *
	 * \return std::pair with total number of sequence points used in test step (first element) and expected number of
	 * context switches during test step (second element)
	 */

	std::pair<uint8_t, uint8_t> getParameters() const
	{
		return {totalSequencePoints_, expectedContextSwitchCount_};
	}

private:

	/**
	 * \brief Executes the action that will block current thread.
	 *
	 * \return true if execution of blocking action succeeded, false otherwise
	 */

	virtual bool block() = 0;

	/**
	 * \brief Checks postcondition of the test step.
	 *
	 * This should be overridden by derived classes if there is some postcondition to check.
	 *
	 * \return true if postcondition was satisfied, false otherwise
	 */

	virtual bool postcondition()
	{
		return true;
	}

	/**
	 * \brief Queues signal to provided thread.
	 *
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 * \param [in] thread is a reference to Thread to which the signal will be queued
	 */

	void signalingThreadFunction(SequenceAsserter& sequenceAsserter, Thread& thread) const
	{
		sequenceAsserter.sequencePoint(signalingThreadSequencePoint1_);
		thread.queueSignal(signalHandlerSequencePoint_, sigval{reinterpret_cast<int>(&sequenceAsserter)});
		sequenceAsserter.sequencePoint(signalingThreadSequencePoint2_);
	}

	/// total number of sequence points used in test step
	uint8_t totalSequencePoints_;

	/// expected number of context switches during test step
	uint8_t expectedContextSwitchCount_;

	/// first sequence point used in signalingThreadFunction()
	uint8_t signalingThreadSequencePoint1_;

	/// sequence point used in SignalsInterruptionTestCase::signalHandler()
	uint8_t signalHandlerSequencePoint_;

	/// sequence point used before joining with the thread that executes signalingThreadFunction()
	uint8_t joinSequencePoint_;

	/// second sequence point used in signalingThreadFunction()
	uint8_t signalingThreadSequencePoint2_;
};

/// test step for condition variables
class ConditionVariableTestStep : public TestStep
{
public:

	/// type of condition variable test step
	enum class Type : uint8_t
	{
		/// uses ConditionVariable::wait()
		wait,
		/// uses ConditionVariable::waitFor()
		waitFor,
		/// uses ConditionVariable::waitUntil()
		waitUntil,
	};

	/**
	 * \brief ConditionVariableTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 */

	explicit ConditionVariableTestStep(const Type type) :
			TestStep{4, 4, 0, 1, 2, 3},
			conditionVariable_{},
			mutex_{},
			type_{type}
	{
		mutex_.lock();
	}

private:

	/**
	 * \brief Executes the action that will block current thread.
	 *
	 * Calls ConditionVariable::wait(), ConditionVariable::waitFor() or ConditionVariable::waitUntil() - depending on
	 * the type of test step - and ensures that the function completes successfully.
	 *
	 * \return true if execution of blocking action succeeded, false otherwise
	 */

	bool block() override
	{
		const auto ret = type_ == Type::wait ? conditionVariable_.wait(mutex_) :
				type_ == Type::waitFor ? conditionVariable_.waitFor(mutex_, longDuration) :
				conditionVariable_.waitUntil(mutex_, TickClock::now() + longDuration);
		return ret == 0;
	}

	/**
	 * \brief Checks postcondition of the test step.
	 *
	 * Checks state of condition variable's mutex after test step.
	 *
	 * \return true if postcondition was satisfied, false otherwise
	 */

	bool postcondition() override
	{
		if (mutex_.tryLock() != EBUSY)		// mutex must be locked
			return false;
		if (mutex_.unlock() != 0)			// mutex may be unlocked without problems
			return false;

		return true;
	}

	/// tested object
	ConditionVariable conditionVariable_;

	/// mutex used with \a conditionVariable_
	Mutex mutex_;

	/// type of test step
	Type type_;
};

/// test step for thread join
class JoinTestStep : public TestStep
{
public:

	/**
	 * \brief JoinTestStep's constructor
	 *
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 */

	explicit JoinTestStep(SequenceAsserter& sequenceAsserter) :
			TestStep{5, 5, 0, 1, 4, 2},
			sequencePointThread_{{testThreadStackSize, veryLowPriority}, &SequenceAsserter::sequencePoint,
					std::ref(sequenceAsserter), 3}
	{

	}

private:

	/**
	 * \brief Executes the action that will block current thread.
	 *
	 * Starts internal thread and joins it, ensuring that the function completes successfully.
	 *
	 * \return true if execution of blocking action succeeded, false otherwise
	 */

	bool block() override
	{
		sequencePointThread_.start();
		return sequencePointThread_.join() == 0;
	}

	/// internal thread that marks sequence point and is joined in block()
	DynamicThread sequencePointThread_;
};

/// test step for mutexes
class MutexTestStep : public TestStep
{
public:

	/// type of mutex test step
	enum class Type : uint8_t
	{
		/// uses Mutex::lock()
		lock,
		/// uses Mutex::tryLockFor()
		tryLockFor,
		/// uses Mutex::tryLockUntil()
		tryLockUntil,
	};

	/**
	 * \brief MutexTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 */

	MutexTestStep(const Type type, SequenceAsserter& sequenceAsserter) :
			TestStep{8, 9, 2, 3, 6, 4},
			mutexLockerThread_{{testThreadStackSize, highPriority}, &MutexTestStep::mutexLockerThreadFunction,
					std::ref(*this)},
			mutex_{},
			sequenceAsserter_(sequenceAsserter),
			type_{type}
	{

	}

	/**
	 * \brief MutexTestStep's virtual destructor
	 *
	 * Joins the "mutex locker" thread.
	 */

	~MutexTestStep() override
	{
		mutexLockerThread_.join();
	}

private:

	/**
	 * \brief Executes the action that will block current thread.
	 *
	 * Starts internal "mutex locker" thread. Then calls Mutex::lock(), Mutex::tryLockFor() or Mutex::tryLockUntil() -
	 * depending on the type of test step - and ensures that the function completes successfully.
	 *
	 * \return true if execution of blocking action succeeded, false otherwise
	 */

	bool block() override
	{
		mutexLockerThread_.start();
		sequenceAsserter_.sequencePoint(1);
		const auto ret = type_ == Type::lock ? mutex_.lock() :
				type_ == Type::tryLockFor ? mutex_.tryLockFor(longDuration) :
				mutex_.tryLockUntil(TickClock::now() + longDuration);
		return ret == 0;
	}

	/**
	 * \brief Main function of internal "mutex locker" thread.
	 *
	 * Locks the mutex, changes its priority to \a veryLowPriority (causing preemption) and unlocks the mutex.
	 */

	void mutexLockerThreadFunction()
	{
		sequenceAsserter_.sequencePoint(0);
		{
			const std::lock_guard<Mutex> lockGuard {mutex_};

			ThisThread::setPriority(veryLowPriority);
			sequenceAsserter_.sequencePoint(5);
		}
		sequenceAsserter_.sequencePoint(7);
	}

	/**
	 * \brief Checks postcondition of the test step.
	 *
	 * Checks state of mutex after test step.
	 *
	 * \return true if postcondition was satisfied, false otherwise
	 */

	bool postcondition() override
	{
		if (mutex_.tryLock() != EBUSY)		// mutex must be locked
			return false;
		if (mutex_.unlock() != 0)			// mutex may be unlocked without problems
			return false;

		return true;
	}

	/// internal "mutex locker" thread
	DynamicThread mutexLockerThread_;

	/// tested object
	Mutex mutex_;

	/// reference to shared SequenceAsserter object
	SequenceAsserter& sequenceAsserter_;

	/// type of test step
	Type type_;
};

/// test step for semaphores
class SemaphoreTestStep : public TestStep
{
public:

	/// type of semaphore test step
	enum class Type : uint8_t
	{
		/// uses Semaphore::wait()
		wait,
		/// uses Semaphore::tryWaitFor()
		tryWaitFor,
		/// uses Semaphore::tryWaitUntil()
		tryWaitUntil,
	};

	/**
	 * \brief SemaphoreTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 */

	explicit SemaphoreTestStep(const Type type) :
			TestStep{4, 4, 0, 1, 2, 3},
			semaphore_{0},
			type_{type}
	{

	}

private:

	/**
	 * \brief Executes the action that will block current thread.
	 *
	 * Calls Semaphore::wait(), Semaphore::tryWaitFor() or Semaphore::tryWaitUntil() - depending on the type of test
	 * step - and ensures that the function returns with EINTR error code.
	 *
	 * \return true if execution of blocking action succeeded, false otherwise
	 */

	bool block() override
	{
		const auto ret = type_ == Type::wait ? semaphore_.wait() :
				type_ == Type::tryWaitFor ? semaphore_.tryWaitFor(longDuration) :
				semaphore_.tryWaitUntil(TickClock::now() + longDuration);
		return ret == EINTR;
	}

	/**
	 * \brief Checks postcondition of the test step.
	 *
	 * Checks state of semaphore after interruption.
	 *
	 * \return true if postcondition was satisfied, false otherwise
	 */

	bool postcondition() override
	{
		if (semaphore_.getValue() != 0)			// after interruption, semaphore's value must be 0
			return false;
		if (semaphore_.tryWait() != EAGAIN)		// after interruption, semaphore cannot be "taken"
			return false;
		if (semaphore_.post() != 0)
			return false;
		if (semaphore_.getValue() != 1)			// after single "post", semaphore's value must be 1
			return false;
		if (semaphore_.tryWait() != 0)			// after single "post", semaphore can be "taken" immediately
			return false;
		if (semaphore_.tryWait() != EAGAIN)		// after single "post", semaphore can be "taken" only once
			return false;

		return true;
	}

	/// tested object
	Semaphore semaphore_;

	/// type of test step
	Type type_;
};

/// test step for waiting for signals
class SignalsTestStep : public TestStep
{
public:

	/// type of signals test step
	enum class Type : uint8_t
	{
		/// uses ThisThread::Signals::wait()
		wait,
		/// uses ThisThread::Signals::tryWaitFor()
		tryWaitFor,
		/// uses ThisThread::Signals::tryWaitUntil()
		tryWaitUntil,
	};

	/**
	 * \brief SignalsTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 */

	constexpr explicit SignalsTestStep(const Type type) :
			TestStep{4, 4, 0, 1, 2, 3},
			type_{type}
	{

	}

private:

	/**
	 * \brief Executes the action that will block current thread.
	 *
	 * Calls ThisThread::Signals::wait(), ThisThread::Signals::tryWaitFor() or ThisThread::Signals::tryWaitUntil() -
	 * depending on the type of test step - and ensures that the function returns with EINTR error code.
	 *
	 * \return true if execution of blocking action succeeded, false otherwise
	 */

	bool block() override
	{
		const SignalSet fullSignalSet {SignalSet::full};
		const auto ret = type_ == Type::wait ? ThisThread::Signals::wait(fullSignalSet) :
				type_ == Type::tryWaitFor ? ThisThread::Signals::tryWaitFor(fullSignalSet, longDuration) :
				ThisThread::Signals::tryWaitUntil(fullSignalSet, TickClock::now() + longDuration);
		return ret.first == EINTR;
	}

	/// type of test step
	Type type_;
};

/// test step for thread sleep
class SleepTestStep : public TestStep
{
public:

	/// type of sleep test step
	enum class Type : uint8_t
	{
		/// uses ThisThread::sleepFor()
		sleepFor,
		/// uses ThisThread::sleepUntil()
		sleepUntil,
	};

	/**
	 * \brief SleepTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 */

	constexpr explicit SleepTestStep(const Type type) :
			TestStep{4, 4, 0, 1, 2, 3},
			type_{type}
	{

	}

private:

	/**
	 * \brief Executes the action that will block current thread.
	 *
	 * Calls ThisThread::sleepFor() or ThisThread::sleepUntil() - depending on the type of test step - and ensures that
	 * the function returns with EINTR error code.
	 *
	 * \return true if execution of blocking action succeeded, false otherwise
	 */

	bool block() override
	{
		const auto ret = type_ == Type::sleepFor ? ThisThread::sleepFor(longDuration) :
				ThisThread::sleepUntil(TickClock::now() + longDuration);
		return ret == EINTR;
	}

	/// type of test step
	Type type_;
};

/// type of test step
enum class TestStepType : uint8_t
{
	/// ConditionVariableTestStep, ConditionVariableTestStep::Type::wait
	conditionVariableWait,
	/// ConditionVariableTestStep, ConditionVariableTestStep::Type::waitFor
	conditionVariableWaitFor,
	/// ConditionVariableTestStep, ConditionVariableTestStep::Type::waitUntil
	conditionVariableWaitUntil,
	/// JoinTestStep
	join,
	/// MutexTestStep, MutexTestStep::Type::lock
	mutexLock,
	/// MutexTestStep, MutexTestStep::Type::tryLockFor
	mutexTryLockFor,
	/// MutexTestStep, MutexTestStep::Type::tryLockUntil
	mutexTryLockUntil,
	/// SemaphoreTestStep, SemaphoreTestStep::Type::wait
	semaphoreWait,
	/// SemaphoreTestStep, SemaphoreTestStep::Type::tryWaitFor
	semaphoreTryWaitFor,
	/// SemaphoreTestStep, SemaphoreTestStep::Type::tryWaitUntil
	semaphoreTryWaitUntil,
	/// SignalsTestStep, SignalsTestStep::Type::wait
	signalsWait,
	/// SignalsTestStep, SignalsTestStep::Type::tryWaitFor
	signalsTryWaitFor,
	/// SignalsTestStep, SignalsTestStep::Type::tryWaitUntil
	signalsTryWaitUntil,
	/// SleepTestStep, SleepTestStep::Type::sleepFor
	sleepFor,
	/// SleepTestStep, SleepTestStep::Type::sleepUntil
	sleepUntil,
};

/// uninitialized storage for any of test steps
union TestStepStorage
{
	/// storage for ConditionVariableTestStep
	std::aligned_storage<sizeof(ConditionVariableTestStep), alignof(ConditionVariableTestStep)>::type
			conditionVariableTestStepStorage;
	/// storage for JoinTestStep
	std::aligned_storage<sizeof(JoinTestStep), alignof(JoinTestStep)>::type joinTestStepStorage;
	/// storage for MutexTestStep
	std::aligned_storage<sizeof(MutexTestStep), alignof(MutexTestStep)>::type mutexTestStepStorage;
	/// storage for SemaphoreTestStep
	std::aligned_storage<sizeof(SemaphoreTestStep), alignof(SemaphoreTestStep)>::type semaphoreTestStepStorage;
	/// storage for SignalsTestStep
	std::aligned_storage<sizeof(SignalsTestStep), alignof(SignalsTestStep)>::type signalsTestStepStorage;
	/// storage for SleepTestStep
	std::aligned_storage<sizeof(SleepTestStep), alignof(SleepTestStep)>::type sleepTestStepStorage;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all test step types
const TestStepType testStepTypes[]
{
		TestStepType::conditionVariableWait,
		TestStepType::conditionVariableWaitFor,
		TestStepType::conditionVariableWaitUntil,
		TestStepType::join,
		TestStepType::mutexLock,
		TestStepType::mutexTryLockFor,
		TestStepType::mutexTryLockUntil,
		TestStepType::semaphoreWait,
		TestStepType::semaphoreTryWaitFor,
		TestStepType::semaphoreTryWaitUntil,
		TestStepType::signalsWait,
		TestStepType::signalsTryWaitFor,
		TestStepType::signalsTryWaitUntil,
		TestStepType::sleepFor,
		TestStepType::sleepUntil,
};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Builder of TestStep objects.
 *
 * \param [in] testStepType is the type of test step
 * \param [in] testStepStoragetestStepStorage is a reference to storage in which the object will be constructed
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 *
 * \return reference to TestStep object constructed in \a testStepStorage
 */

TestStep& makeTestStep(const TestStepType testStepType, TestStepStorage& testStepStorage,
		SequenceAsserter& sequenceAsserter)
{
	if (testStepType == TestStepType::conditionVariableWait)
		return *new (&testStepStorage) ConditionVariableTestStep {ConditionVariableTestStep::Type::wait};
	else if (testStepType == TestStepType::conditionVariableWaitFor)
		return *new (&testStepStorage) ConditionVariableTestStep {ConditionVariableTestStep::Type::waitFor};
	else if (testStepType == TestStepType::conditionVariableWaitUntil)
		return *new (&testStepStorage) ConditionVariableTestStep {ConditionVariableTestStep::Type::waitUntil};
	else if (testStepType == TestStepType::join)
		return *new (&testStepStorage) JoinTestStep {sequenceAsserter};
	else if (testStepType == TestStepType::mutexLock)
		return *new (&testStepStorage) MutexTestStep {MutexTestStep::Type::lock, sequenceAsserter};
	else if (testStepType == TestStepType::mutexTryLockFor)
		return *new (&testStepStorage) MutexTestStep {MutexTestStep::Type::tryLockFor, sequenceAsserter};
	else if (testStepType == TestStepType::mutexTryLockUntil)
		return *new (&testStepStorage) MutexTestStep {MutexTestStep::Type::tryLockUntil, sequenceAsserter};
	else if (testStepType == TestStepType::semaphoreWait)
		return *new (&testStepStorage) SemaphoreTestStep {SemaphoreTestStep::Type::wait};
	else if (testStepType == TestStepType::semaphoreTryWaitFor)
		return *new (&testStepStorage) SemaphoreTestStep {SemaphoreTestStep::Type::tryWaitFor};
	else if (testStepType == TestStepType::semaphoreTryWaitUntil)
		return *new (&testStepStorage) SemaphoreTestStep {SemaphoreTestStep::Type::tryWaitUntil};
	if (testStepType == TestStepType::signalsWait)
		return *new (&testStepStorage) SignalsTestStep {SignalsTestStep::Type::wait};
	else if (testStepType == TestStepType::signalsTryWaitFor)
		return *new (&testStepStorage) SignalsTestStep {SignalsTestStep::Type::tryWaitFor};
	else if (testStepType == TestStepType::signalsTryWaitUntil)
		return *new (&testStepStorage) SignalsTestStep {SignalsTestStep::Type::tryWaitUntil};
	else if (testStepType == TestStepType::sleepFor)
		return *new (&testStepStorage) SleepTestStep {SleepTestStep::Type::sleepFor};
	else // if (testStepType == TestStepType::sleepUntil)
		return *new (&testStepStorage) SleepTestStep {SleepTestStep::Type::sleepUntil};
}

}	// namespace

#endif	// SIGNALS_INTERRUPTION_TEST_CASE_ENABLED == 1

/*---------------------------------------------------------------------------------------------------------------------+
| private static functions
+---------------------------------------------------------------------------------------------------------------------*/

void SignalsInterruptionTestCase::signalHandler(const SignalInformation& signalInformation)
{
#if SIGNALS_INTERRUPTION_TEST_CASE_ENABLED == 1

	const auto sequenceAsserter = static_cast<SequenceAsserter*>(signalInformation.getValue().sival_ptr);
	if (sequenceAsserter != nullptr)
		sequenceAsserter->sequencePoint(signalInformation.getSignalNumber());

#else	// SIGNALS_INTERRUPTION_TEST_CASE_ENABLED != 1

	static_cast<void>(signalInformation);	// suppress warning

#endif	// SIGNALS_INTERRUPTION_TEST_CASE_ENABLED != 1
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsInterruptionTestCase::run_() const
{
#if SIGNALS_INTERRUPTION_TEST_CASE_ENABLED == 1

	const auto contextSwitchCount = statistics::getContextSwitchCount();
	auto expectedContextSwitchCount = decltype(contextSwitchCount){};

	for (const auto testStepType : testStepTypes)
	{
		SequenceAsserter sequenceAsserter;

		// initially no signals may be pending
		if (ThisThread::Signals::getPendingSignalSet().getBitset().any() == true)
			return false;

		TestStepStorage testStepStorage;
		auto& testStep = makeTestStep(testStepType, testStepStorage, sequenceAsserter);
		const auto testStepRunResult = testStep.run(sequenceAsserter);
		const auto testStepParameters = testStep.getParameters();
		testStep.~TestStep();

		if (testStepRunResult == false)
			return false;
		if (sequenceAsserter.assertSequence(testStepParameters.first) == false)
			return false;
		expectedContextSwitchCount += testStepParameters.second;
		if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
			return false;
	}

	// after the test no signals may be pending
	if (ThisThread::Signals::getPendingSignalSet().getBitset().any() == true)
		return false;

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

#endif	// SIGNALS_INTERRUPTION_TEST_CASE_ENABLED == 1

	return true;
}

}	// namespace test

}	// namespace distortos
