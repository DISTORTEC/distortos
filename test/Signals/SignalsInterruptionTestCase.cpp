/**
 * \file
 * \brief SignalsInterruptionTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-07-16
 */

#include "SignalsInterruptionTestCase.hpp"

#include "SequenceAsserter.hpp"
#include "waitForNextTick.hpp"

#include "distortos/ConditionVariable.hpp"
#include "distortos/Mutex.hpp"
#include "distortos/StaticThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/ThisThread-Signals.hpp"

namespace distortos
{

namespace test
{

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

	virtual ~TestStep()
	{

	}

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
		auto signalingThread = makeStaticThread<testThreadStackSize>(lowPriority, &TestStep::signalingThreadFunction,
				std::ref(*this), std::ref(sequenceAsserter), std::ref(currentThread));
		waitForNextTick();
		signalingThread.start();
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
	 * \param [in] thread is a reference to ThreadBase to which the signal will be queued
	 */

	void signalingThreadFunction(SequenceAsserter& sequenceAsserter, ThreadBase& thread) const
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
		Wait,
		/// uses ConditionVariable::waitFor()
		WaitFor,
		/// uses ConditionVariable::waitUntil()
		WaitUntil,
	};

	/**
	 * \brief ConditionVariableTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 */

	explicit ConditionVariableTestStep(const Type type) :
			TestStep{4, 6, 0, 1, 2, 3},
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

	virtual bool block() override
	{
		const auto ret = type_ == Type::Wait ? conditionVariable_.wait(mutex_) :
				type_ == Type::WaitFor ? conditionVariable_.waitFor(mutex_, longDuration) :
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

	virtual bool postcondition() override
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
			TestStep{5, 7, 0, 1, 4, 2},
			sequencePointThread_{veryLowPriority, &SequenceAsserter::sequencePoint, std::ref(sequenceAsserter), 3}
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

	virtual bool block() override
	{
		sequencePointThread_.start();
		return sequencePointThread_.join() == 0;
	}

	/// type of internal thread
	using SequencePointThread = decltype(makeStaticThread<testThreadStackSize>({}, &SequenceAsserter::sequencePoint,
			std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>()));

	/// internal thread that marks sequence point and is joined in block()
	SequencePointThread sequencePointThread_;
};

/// test step for mutexes
class MutexTestStep : public TestStep
{
public:

	/// type of mutex test step
	enum class Type : uint8_t
	{
		/// uses Mutex::lock()
		Lock,
		/// uses Mutex::tryLockFor()
		TryLockFor,
		/// uses Mutex::tryLockUntil()
		TryLockUntil,
	};

	/**
	 * \brief MutexTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 */

	MutexTestStep(const Type type, SequenceAsserter& sequenceAsserter) :
			TestStep{8, 11, 2, 3, 6, 4},
			mutexLockerThread_{highPriority, &MutexTestStep::mutexLockerThreadFunction, std::ref(*this)},
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

	virtual ~MutexTestStep() override
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

	virtual bool block() override
	{
		mutexLockerThread_.start();
		sequenceAsserter_.sequencePoint(1);
		const auto ret = type_ == Type::Lock ? mutex_.lock() :
				type_ == Type::TryLockFor ? mutex_.tryLockFor(longDuration) :
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
		mutex_.lock();
		ThisThread::setPriority(veryLowPriority);
		sequenceAsserter_.sequencePoint(5);
		mutex_.unlock();
		sequenceAsserter_.sequencePoint(7);
	}

	/**
	 * \brief Checks postcondition of the test step.
	 *
	 * Checks state of mutex after test step.
	 *
	 * \return true if postcondition was satisfied, false otherwise
	 */

	virtual bool postcondition() override
	{
		if (mutex_.tryLock() != EBUSY)		// mutex must be locked
			return false;
		if (mutex_.unlock() != 0)			// mutex may be unlocked without problems
			return false;

		return true;
	}

	/// type of internal "mutex locker" thread
	using MutexLockerThread = decltype(makeStaticThread<testThreadStackSize>({},
			&MutexTestStep::mutexLockerThreadFunction, std::ref(std::declval<MutexTestStep&>())));

	/// internal "mutex locker" thread
	MutexLockerThread mutexLockerThread_;

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
		Wait,
		/// uses Semaphore::tryWaitFor()
		TryWaitFor,
		/// uses Semaphore::tryWaitUntil()
		TryWaitUntil,
	};

	/**
	 * \brief SemaphoreTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 */

	explicit SemaphoreTestStep(const Type type) :
			TestStep{4, 6, 0, 1, 2, 3},
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

	virtual bool block() override
	{
		const auto ret = type_ == Type::Wait ? semaphore_.wait() :
				type_ == Type::TryWaitFor ? semaphore_.tryWaitFor(longDuration) :
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

	virtual bool postcondition() override
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
		Wait,
		/// uses ThisThread::Signals::tryWaitFor()
		TryWaitFor,
		/// uses ThisThread::Signals::tryWaitUntil()
		TryWaitUntil,
	};

	/**
	 * \brief SignalsTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 */

	constexpr explicit SignalsTestStep(const Type type) :
			TestStep{4, 6, 0, 1, 2, 3},
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

	virtual bool block() override
	{
		const SignalSet fullSignalSet {SignalSet::full};
		const auto ret = type_ == Type::Wait ? ThisThread::Signals::wait(fullSignalSet) :
				type_ == Type::TryWaitFor ? ThisThread::Signals::tryWaitFor(fullSignalSet, longDuration) :
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
		For,
		/// uses ThisThread::sleepUntil()
		Until,
	};

	/**
	 * \brief SleepTestStep's constructor
	 *
	 * \param [in] type is the type of test step
	 */

	constexpr explicit SleepTestStep(const Type type) :
			TestStep{4, 6, 0, 1, 2, 3},
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

	virtual bool block() override
	{
		const auto ret = type_ == Type::For ? ThisThread::sleepFor(longDuration) :
				ThisThread::sleepUntil(TickClock::now() + longDuration);
		return ret == EINTR;
	}

	/// type of test step
	Type type_;
};

/// type of test step
enum class TestStepType : uint8_t
{
	/// ConditionVariableTestStep, ConditionVariableTestStep::Type::Wait
	ConditionVariableWait,
	/// ConditionVariableTestStep, ConditionVariableTestStep::Type::WaitFor
	ConditionVariableWaitFor,
	/// ConditionVariableTestStep, ConditionVariableTestStep::Type::WaitUntil
	ConditionVariableWaitUntil,
	/// JoinTestStep
	Join,
	/// MutexTestStep, MutexTestStep::Type::Lock
	MutexLock,
	/// MutexTestStep, MutexTestStep::Type::TryLockFor
	MutexTryLockFor,
	/// MutexTestStep, MutexTestStep::Type::TryLockUntil
	MutexTryLockUntil,
	/// SemaphoreTestStep, SemaphoreTestStep::Type::Wait
	SemaphoreWait,
	/// SemaphoreTestStep, SemaphoreTestStep::Type::TryWaitFor
	SemaphoreTryWaitFor,
	/// SemaphoreTestStep, SemaphoreTestStep::Type::TryWaitUntil
	SemaphoreTryWaitUntil,
	/// SignalsTestStep, SignalsTestStep::Type::Wait
	SignalsWait,
	/// SignalsTestStep, SignalsTestStep::Type::TryWaitFor
	SignalsTryWaitFor,
	/// SignalsTestStep, SignalsTestStep::Type::TryWaitUntil
	SignalsTryWaitUntil,
	/// SleepTestStep, SleepTestStep::Type::For
	SleepFor,
	/// SleepTestStep, SleepTestStep::Type::Until
	SleepUntil,
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
		TestStepType::ConditionVariableWait,
		TestStepType::ConditionVariableWaitFor,
		TestStepType::ConditionVariableWaitUntil,
		TestStepType::Join,
		TestStepType::MutexLock,
		TestStepType::MutexTryLockFor,
		TestStepType::MutexTryLockUntil,
		TestStepType::SemaphoreWait,
		TestStepType::SemaphoreTryWaitFor,
		TestStepType::SemaphoreTryWaitUntil,
		TestStepType::SignalsWait,
		TestStepType::SignalsTryWaitFor,
		TestStepType::SignalsTryWaitUntil,
		TestStepType::SleepFor,
		TestStepType::SleepUntil,
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
	if (testStepType == TestStepType::ConditionVariableWait)
		return *new (&testStepStorage) ConditionVariableTestStep {ConditionVariableTestStep::Type::Wait};
	else if (testStepType == TestStepType::ConditionVariableWaitFor)
		return *new (&testStepStorage) ConditionVariableTestStep {ConditionVariableTestStep::Type::WaitFor};
	else if (testStepType == TestStepType::ConditionVariableWaitUntil)
		return *new (&testStepStorage) ConditionVariableTestStep {ConditionVariableTestStep::Type::WaitUntil};
	else if (testStepType == TestStepType::Join)
		return *new (&testStepStorage) JoinTestStep {sequenceAsserter};
	else if (testStepType == TestStepType::MutexLock)
		return *new (&testStepStorage) MutexTestStep {MutexTestStep::Type::Lock, sequenceAsserter};
	else if (testStepType == TestStepType::MutexTryLockFor)
		return *new (&testStepStorage) MutexTestStep {MutexTestStep::Type::TryLockFor, sequenceAsserter};
	else if (testStepType == TestStepType::MutexTryLockUntil)
		return *new (&testStepStorage) MutexTestStep {MutexTestStep::Type::TryLockUntil, sequenceAsserter};
	else if (testStepType == TestStepType::SemaphoreWait)
		return *new (&testStepStorage) SemaphoreTestStep {SemaphoreTestStep::Type::Wait};
	else if (testStepType == TestStepType::SemaphoreTryWaitFor)
		return *new (&testStepStorage) SemaphoreTestStep {SemaphoreTestStep::Type::TryWaitFor};
	else if (testStepType == TestStepType::SemaphoreTryWaitUntil)
		return *new (&testStepStorage) SemaphoreTestStep {SemaphoreTestStep::Type::TryWaitUntil};
	if (testStepType == TestStepType::SignalsWait)
		return *new (&testStepStorage) SignalsTestStep {SignalsTestStep::Type::Wait};
	else if (testStepType == TestStepType::SignalsTryWaitFor)
		return *new (&testStepStorage) SignalsTestStep {SignalsTestStep::Type::TryWaitFor};
	else if (testStepType == TestStepType::SignalsTryWaitUntil)
		return *new (&testStepStorage) SignalsTestStep {SignalsTestStep::Type::TryWaitUntil};
	else if (testStepType == TestStepType::SleepFor)
		return *new (&testStepStorage) SleepTestStep {SleepTestStep::Type::For};
	else // if (testStepType == TestStepType::SleepUntil)
		return *new (&testStepStorage) SleepTestStep {SleepTestStep::Type::Until};
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private static functions
+---------------------------------------------------------------------------------------------------------------------*/

void SignalsInterruptionTestCase::signalHandler(const SignalInformation& signalInformation)
{
	const auto sequenceAsserter = static_cast<SequenceAsserter*>(signalInformation.getValue().sival_ptr);
	if (sequenceAsserter != nullptr)
		sequenceAsserter->sequencePoint(signalInformation.getSignalNumber());
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsInterruptionTestCase::run_() const
{
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

	return true;
}

}	// namespace test

}	// namespace distortos
