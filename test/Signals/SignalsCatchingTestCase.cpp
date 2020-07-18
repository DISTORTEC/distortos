/**
 * \file
 * \brief SignalsCatchingTestCase class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SignalsCatchingTestCase.hpp"

#include "distortos/distortosConfiguration.h"

/// configuration required by SignalsCatchingTestCase
#define SIGNALS_CATCHING_TEST_CASE_ENABLED DISTORTOS_SIGNALS_ENABLE == 1 && \
		DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS >= 8 && DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS >= 8

#if SIGNALS_CATCHING_TEST_CASE_ENABLED == 1

#include "SequenceAsserter.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/StaticSoftwareTimer.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/ThisThread-Signals.hpp"

#include "estd/ContiguousRange.hpp"

#include <cerrno>

#endif	// SIGNALS_CATCHING_TEST_CASE_ENABLED == 1

namespace distortos
{

namespace test
{

#if SIGNALS_CATCHING_TEST_CASE_ENABLED == 1

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

class TestStep;

/// range of test steps
using TestStepsRange = estd::ContiguousRange<const TestStep>;

/// single test stage
struct Stage
{
	/// range of test steps for main test thread
	TestStepsRange mainTestThreadStepsRange;

	/// range of test steps for signal handler
	TestStepsRange signalHandlerStepsRange;

	/// range of test steps for second test thread
	TestStepsRange secondTestThreadStepsRange;

	/// expected number of context switches for this stage
	decltype(statistics::getContextSwitchCount()) contextSwitchCount;
};

/// pair of sequence points
using SequencePoints = std::pair<unsigned int, unsigned int>;

/// basic type of test step executed in signal handler
class BasicHandlerStep
{
public:

	/**
	 * \brief BasicHandlerStep's constructor
	 *
	 * \param [in] pendingSignalSet is the expected set of currently pending signals for current thread
	 * \param [in] signalMask is the expected signal mask for current thread
	 * \param [in] code is the expected signal code in received SignalInformation object
	 * \param [in] signalNumber is the expected signal number in received SignalInformation object
	 * \param [in] value is the expected signal value in received SignalInformation object, default - default
	 * constructed int value
	 */

	constexpr BasicHandlerStep(const SignalSet pendingSignalSet, const SignalSet signalMask,
			const SignalInformation::Code code, const uint8_t signalNumber, const int value = {}) :
			pendingSignalSet_{pendingSignalSet},
			signalMask_{signalMask},
			value_{value},
			code_{code},
			signalNumber_{signalNumber}
	{

	}

	/**
	 * \brief BasicHandlerStep's function call operator
	 *
	 * Compares set of pending signals and signal mask of current thread with expected values. Verifies received
	 * SignalInformation object.
	 *
	 * \param [in] signalInformation is a reference to received SignalInformation object
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(const SignalInformation& signalInformation) const;

private:

	/// expected set of currently pending signals for current thread
	SignalSet pendingSignalSet_;

	/// expected signal mask for current thread
	SignalSet signalMask_;

	/// expected signal value in received SignalInformation object
	int value_;

	/// expected signal code in received SignalInformation object
	SignalInformation::Code code_;

	/// expected signal number in received SignalInformation object
	uint8_t signalNumber_;
};

/// test step that generates or queues a signal
class GenerateQueueSignalStep
{
public:

	/**
	 * \brief GenerateQueueSignalStep's constructor
	 *
	 * \param [in] code selects whether the signal will be generated (SignalInformation::Code::generated) or queued
	 * (SignalInformation::Code::queued)
	 * \param [in] signalNumber is the signal number that will be generated or queued
	 * \param [in] value is the value that will be queued with the signal, relevant only if \a code is
	 * SignalInformation::Code::queued, default - default constructed int value
	 */

	constexpr GenerateQueueSignalStep(const SignalInformation::Code code, const uint8_t signalNumber,
			const int value = {}) :
			value_{value},
			code_{code},
			signalNumber_{signalNumber}
	{

	}

	/**
	 * \brief GenerateQueueSignalStep's function call operator
	 *
	 * Generates or queues selected signal.
	 *
	 * \param [in] thread is a reference to Thread to which the signal will be generated/queued
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(Thread& thread) const;

private:

	/// value that will be queued with the signal, relevant only if \a code_ is SignalInformation::Code::generated
	int value_;

	/// selects whether the signal will be generated (SignalInformation::Code::generated) or queued
	/// (SignalInformation::Code::queued)
	SignalInformation::Code code_;

	/// signal number that will be generated or queued
	uint8_t signalNumber_;
};

/// test step that changes thread's priority
class ThreadPriorityStep
{
public:

	/**
	 * \brief ThreadPriorityStep's constructor
	 *
	 * \param [in] priority is the priority that will be set for thread
	 */

	constexpr explicit ThreadPriorityStep(const uint8_t priority) :
			priority_{priority}
	{

	}

	/**
	 * \brief ThreadPriorityStep's function call operator
	 *
	 * Changes priority of \a thread.
	 *
	 * \param [in] thread is a reference to Thread of which the priority will be changed
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(Thread& thread) const;

private:

	/// priority that will be set for thread
	uint8_t priority_;
};

/// test step that sets signal mask
class SignalMaskStep
{
public:

	/**
	 * \brief SignalMaskStep's constructor
	 *
	 * \param [in] signalMask is the signal mask that will be set for current thread
	 */

	constexpr explicit SignalMaskStep(const SignalSet signalMask) :
			signalMask_{signalMask}
	{

	}

	/**
	 * \brief SignalMaskStep's function call operator
	 *
	 * Sets configured signal mask for current thread.
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()() const;

private:

	/// signal mask that will be set for current thread
	SignalSet signalMask_;
};

/// test step that creates software timer, starts it and waits until it stops
class SoftwareTimerStep
{
public:

	/// type of function for software timer
	using Function = void(TestStepsRange& testStepsRange, Thread&, const SignalInformation*);

	/**
	 * \brief SoftwareTimerStep's constructor
	 *
	 * \param [in] function is a reference to function executed by software timer
	 */

	constexpr SoftwareTimerStep(Function& function) :
			function_{function}
	{

	}

	/**
	 * \brief SoftwareTimerStep's function call operator
	 *
	 * Creates software timer with provided range of test steps, starts it and waits until it stops.
	 *
	 * \param [in] testStepsRange is a reference to range of test steps
	 * \param [in] thread is a reference to Thread passed to software timer
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(TestStepsRange& testStepsRange, Thread& thread) const;

private:

	/// reference to function executed by software timer
	Function& function_;
};

/// single test step
class TestStep
{
public:

	/// type of test step
	enum class Type : uint8_t
	{
		/// BasicHandlerStep
		basicHandler,
		/// GenerateQueueSignalStep
		generateQueueSignal,
		/// ThreadPriorityStep
		threadPriority,
		/// SignalMaskStep
		signalMask,
		/// SoftwareTimerStep
		softwareTimer,
	};

	/**
	 * \brief TestStep's constructor for BasicHandler type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available TestStep should be executed in the same iteration (true) or
	 * not (false)
	 * \param [in] basicHandlerStep is the BasicHandlerStep that will be executed in test step
	 */

	constexpr TestStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint, const bool more,
			const BasicHandlerStep basicHandlerStep) :
			basicHandlerStep_{basicHandlerStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::basicHandler}
	{

	}

	/**
	 * \brief TestStep's constructor for GenerateQueueSignal type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available TestStep should be executed in the same iteration (true) or
	 * not (false)
	 * \param [in] generateQueueSignalStep is the GenerateQueueSignalStep that will be executed in test step
	 */

	constexpr TestStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint, const bool more,
			const GenerateQueueSignalStep generateQueueSignalStep) :
			generateQueueSignalStep_{generateQueueSignalStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::generateQueueSignal}
	{

	}

	/**
	 * \brief TestStep's constructor for ThreadPriority type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available TestStep should be executed in the same iteration (true) or
	 * not (false)
	 * \param [in] threadPriorityStep is the ThreadPriorityStep that will be executed in test step
	 */

	constexpr TestStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint, const bool more,
			const ThreadPriorityStep threadPriorityStep) :
			threadPriorityStep_{threadPriorityStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::threadPriority}
	{

	}

	/**
	 * \brief TestStep's constructor for SignalMask type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available TestStep should be executed in the same iteration (true) or
	 * not (false)
	 * \param [in] signalMaskStep is the SignalMaskStep that will be executed in test step
	 */

	constexpr TestStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint, const bool more,
			const SignalMaskStep signalMaskStep) :
			signalMaskStep_{signalMaskStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::signalMask}
	{

	}

	/**
	 * \brief TestStep's constructor for SoftwareTimer type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available TestStep should be executed in the same iteration (true) or
	 * not (false)
	 * \param [in] softwareTimerStep is the SoftwareTimerStep that will be executed in test step
	 */

	constexpr TestStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint, const bool more,
			const SoftwareTimerStep softwareTimerStep) :
			softwareTimerStep_{softwareTimerStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::softwareTimer}
	{

	}

	/**
	 * \brief TestStep's function call operator
	 *
	 * Marks first sequence point, executes internal test step and marks last sequence point.
	 *
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 * \param [in] testStepsRange is a reference to range of test steps
	 * \param [in] thread is a reference to Thread passed to internal test step
	 * \param [in] signalInformation is a pointer to received SignalInformation object, required only for
	 * BasicHandlerStep
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(SequenceAsserter& sequenceAsserter, TestStepsRange& testStepsRange, Thread& thread,
			const SignalInformation* signalInformation) const;

	/**
	 * \return true if another available TestStep should be executed in the same iteration, false otherwise
	 */

	bool shouldExecuteMore() const
	{
		return more_;
	}

private:

	/// internal test step that will be executed
	union
	{
		/// BasicHandlerStep test step - valid only if type_ == Type::basicHandler
		BasicHandlerStep basicHandlerStep_;

		/// GenerateQueueSignalStep test step - valid only if type_ == Type::generateQueueSignal
		GenerateQueueSignalStep generateQueueSignalStep_;

		/// ThreadPriorityStep test step - valid only if type_ == Type::threadPriority
		ThreadPriorityStep threadPriorityStep_;

		/// SignalMaskStep test step - valid only if type_ == Type::signalMask
		SignalMaskStep signalMaskStep_;

		/// SoftwareTimerStep test step - valid only if type_ == Type::softwareTimer
		SoftwareTimerStep softwareTimerStep_;
	};

	/// sequence points of test step
	SequencePoints sequencePoints_;

	/// selects whether another available TestStep should be executed in the same iteration (true) or not (false)
	bool more_;

	/// type of test step
	Type type_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// original priority of main test thread
constexpr auto mainTestThreadPriority = SignalsCatchingTestCase::getTestCasePriority();

/// priority of second test thread - just below \a mainTestThreadPriority
constexpr decltype(mainTestThreadPriority) secondTestThreadPriority {mainTestThreadPriority - 1};

/// low priority of main test thread - just below \a secondTestThreadPriority
constexpr decltype(mainTestThreadPriority) lowPriority {secondTestThreadPriority - 1};

/// size of stack for second test thread, bytes
constexpr size_t secondTestThreadStackSize {512};

/// total number of signals that are tested
constexpr size_t totalSignals {8};

/// range of test steps for signal handler
TestStepsRange handlerStepsRange;

/// shared return code of signal handler, is is reset before each test phase
sig_atomic_t sharedSigAtomic;

/// shared SequenceAsserter object, is is reset before each test phase
SequenceAsserter sharedSequenceAsserter;

/*---------------------------------------------------------------------------------------------------------------------+
| BasicHandlerStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int BasicHandlerStep::operator()(const SignalInformation& signalInformation) const
{
	return ThisThread::Signals::getPendingSignalSet().getBitset() == pendingSignalSet_.getBitset() &&
			ThisThread::Signals::getSignalMask().getBitset() == signalMask_.getBitset() &&
			signalInformation.getCode() == code_ && signalInformation.getSignalNumber() == signalNumber_ &&
			signalInformation.getValue().sival_int == value_ ? 0 : EINVAL;
}

/*---------------------------------------------------------------------------------------------------------------------+
| GenerateQueueSignalStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int GenerateQueueSignalStep::operator()(Thread& thread) const
{
	return code_ == SignalInformation::Code::generated ? thread.generateSignal(signalNumber_) :
			thread.queueSignal(signalNumber_, sigval{value_});
}

/*---------------------------------------------------------------------------------------------------------------------+
| TestStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int TestStep::operator()(SequenceAsserter& sequenceAsserter, TestStepsRange& testStepsRange, Thread& thread,
		const SignalInformation* const signalInformation) const
{
	sequenceAsserter.sequencePoint(sequencePoints_.first);

	const auto ret =
			type_ == Type::basicHandler && signalInformation != nullptr ? basicHandlerStep_(*signalInformation) :
			type_ == Type::generateQueueSignal ? generateQueueSignalStep_(thread) :
			type_ == Type::threadPriority ? threadPriorityStep_(thread) :
			type_ == Type::signalMask ? signalMaskStep_() :
			type_ == Type::softwareTimer ? softwareTimerStep_(testStepsRange, thread) : EINVAL;

	sequenceAsserter.sequencePoint(sequencePoints_.second);

	return ret;
}

/*---------------------------------------------------------------------------------------------------------------------+
| ThreadPriorityStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int ThreadPriorityStep::operator()(Thread& thread) const
{
	thread.setPriority(priority_);
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| SignalMaskStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int SignalMaskStep::operator()() const
{
	return ThisThread::Signals::setSignalMask(signalMask_);
}

/*---------------------------------------------------------------------------------------------------------------------+
| SoftwareTimerStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int SoftwareTimerStep::operator()(TestStepsRange& testStepsRange, Thread& thread) const
{
	auto softwareTimer = makeStaticSoftwareTimer(function_, std::ref(testStepsRange), std::ref(thread), nullptr);
	softwareTimer.start(TickClock::duration{});
	while (softwareTimer.isRunning() == true);
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Generates special test pattern of signal mask.
 *
 * Test pattern masks following signals,
 * - selected signal,
 * - selected signal + totalSignals,
 * - all signals above (totalSignals * 2).
 *
 * \param [in] signalNumber is the signal number that will be masked
 *
 * \return signal mask with special test pattern
 */

constexpr SignalSet getSignalMask(const uint8_t signalNumber)
{
	return SignalSet{static_cast<uint32_t>(1 << signalNumber | 1 << (signalNumber + totalSignals) |
			UINT32_MAX << (totalSignals * 2))};
}

/**
 * \brief Runner of test steps.
 *
 * If \a testStepsRange is empty, error is marked (function call is unexpected). Otherwise first element if removed from
 * the range and executed. Whole sequence is repeated if TestStep::shouldExecuteMore() of the test step that was just
 * executed returns true.
 *
 * \param [in] testStepsRange is a reference to range of test steps
 * \param [in] thread is a reference to Thread passed to test steps
 * \param [in] signalInformation is a pointer to received SignalInformation object, nullptr if function is not executed
 * from signal handler
 */

void testStepsRunner(TestStepsRange& testStepsRange, Thread& thread, const SignalInformation* const signalInformation)
{
	bool more {true};
	while (more == true && testStepsRange.size() != 0)
	{
		// remove the first element from the range
		auto& testStep = *testStepsRange.begin();
		testStepsRange = {testStepsRange.begin() + 1, testStepsRange.end()};

		const auto ret = testStep(sharedSequenceAsserter, testStepsRange, thread, signalInformation);
		if (ret != 0)
			sharedSigAtomic = ret;

		more = testStep.shouldExecuteMore();
	}

	if (more == true)
		sharedSigAtomic = EINVAL;	// function call was not expected
}

/**
 * \brief Signal handler
 *
 * Executes testStepsRunner() with \a handlerStepsRange and received SignalInformation object.
 *
 * \param [in] signalInformation is a reference to received SignalInformation object
 */

void handler(const SignalInformation& signalInformation)
{
	testStepsRunner(handlerStepsRange, ThisThread::get(), &signalInformation);
}

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// test steps for first phase, for main test thread
const TestStep phase1MainTestThreadSteps[]
{
		// part 1 - normal generated signals
		{0, 3, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{4, 7, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{8, 11, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{12, 15, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{16, 19, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{20, 23, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{24, 27, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{28, 31, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		// part 2 - normal queued signals
		{32, 35, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x6c3d9ebc}},
		{36, 39, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x52e04282}},
		{40, 43, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x29f9fc86}},
		{44, 47, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x19677883}},
		{48, 51, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x7f2d693b}},
		{52, 55, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x1a98ab78}},
		{56, 59, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x6b96c96b}},
		{60, 63, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x463445cc}},
		// part 3 - unmasking of pending generated signals
		{64, 65, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{66, 67, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{68, 69, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{70, 71, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{72, 73, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{74, 75, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{76, 77, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{78, 79, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{80, 81, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{82, 99, true, SignalMaskStep{SignalSet{SignalSet::empty}}},
		// part 4 - unmasking of pending queued signals
		{100, 101, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{102, 103, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x08055dbe}},
		{104, 105, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x2c9530e7}},
		{106, 107, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x79b6c040}},
		{108, 109, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x620f2acf}},
		{110, 111, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x7537d600}},
		{112, 113, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x605724fd}},
		{114, 115, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x4f8d74b5}},
		{116, 117, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x7f2e4b25}},
		{118, 135, true, SignalMaskStep{SignalSet{SignalSet::empty}}},
		// part 5 - unmasking of pending signals
		{136, 137, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{138, 139, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{140, 141, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{142, 143, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{144, 145, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{146, 147, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{148, 149, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{150, 151, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{152, 153, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{154, 155, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x3e37ad5}},
		{156, 157, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x6c9f1f2}},
		{158, 159, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x5b12eca3}},
		{160, 161, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x48b947e9}},
		{162, 163, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x381cacad}},
		{164, 165, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x44bcee3c}},
		{166, 167, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x1cfc75b5}},
		{168, 169, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x15c508ba}},
		{170, 203, true, SignalMaskStep{SignalSet{SignalSet::empty}}},
		// part 6 - unmasking of pending signals - one at a time
		{204, 205, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{206, 207, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{208, 209, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{210, 211, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{212, 213, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{214, 215, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{216, 217, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{218, 219, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{220, 221, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{222, 223, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x3d982f37}},
		{224, 225, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x75143ba1}},
		{226, 227, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x4cf013f5}},
		{228, 229, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x8894ac1}},
		{230, 231, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x399dbe15}},
		{232, 233, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x7abf0fb2}},
		{234, 235, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x587c305e}},
		{236, 237, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x3f6fa768}},
		{238, 243, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b11111011}}},
		{244, 249, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b11011011}}},
		{250, 255, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b11010011}}},
		{256, 261, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b01010011}}},
		{262, 267, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b00010011}}},
		{268, 273, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b00010001}}},
		{274, 279, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b00000001}}},
		{280, 285, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b00000000}}},
		// part 7 - nested signal handlers - generate signal from within signal handler
		{286, 349, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		// part 8 - nested signal handlers - queue signal from within signal handler
		{350, 413, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x39a52149}},
		// part 9 - nested signal handlers - unmasking of pending signals (one at a time) from within signal handler
		{414, 415, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{416, 417, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{418, 419, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{420, 421, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{422, 423, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{424, 425, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{426, 427, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{428, 429, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{430, 431, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{432, 433, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x2e02ef41}},
		{434, 435, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x406f37db}},
		{436, 437, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x1e14b4d4}},
		{438, 439, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x4cd69c9e}},
		{440, 441, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x248f49de}},
		{442, 443, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x116dd593}},
		{444, 445, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x5d8d88c9}},
		{446, 447, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x0a8b8d2c}},
		{448, 495, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b11101111}}},
		{496, 497, false, SignalMaskStep{SignalSet{SignalSet::empty}}},
};

/// test steps for first phase, for signal handler
const TestStep phase1SignalHandlerSteps[]
{
		// part 1 - normal generated signals
		{1, 2, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::generated, 0}},
		{5, 6, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::generated, 1}},
		{9, 10, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::generated, 2}},
		{13, 14, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::generated, 3}},
		{17, 18, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::generated, 4}},
		{21, 22, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::generated, 5}},
		{25, 26, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::generated, 6}},
		{29, 30, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::generated, 7}},
		// part 2 - normal queued signals
		{33, 34, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::queued, 0, 0x6c3d9ebc}},
		{37, 38, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::queued, 1, 0x52e04282}},
		{41, 42, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::queued, 2, 0x29f9fc86}},
		{45, 46, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::queued, 3, 0x19677883}},
		{49, 50, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::queued, 4, 0x7f2d693b}},
		{53, 54, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::queued, 5, 0x1a98ab78}},
		{57, 58, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::queued, 6, 0x6b96c96b}},
		{61, 62, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::queued, 7, 0x463445cc}},
		// part 3 - unmasking of pending generated signals
		{83, 84, false, BasicHandlerStep{SignalSet{0b11111110}, getSignalMask(0), SignalInformation::Code::generated,
				0}},
		{85, 86, false, BasicHandlerStep{SignalSet{0b11111100}, getSignalMask(1), SignalInformation::Code::generated,
				1}},
		{87, 88, false, BasicHandlerStep{SignalSet{0b11111000}, getSignalMask(2), SignalInformation::Code::generated,
				2}},
		{89, 90, false, BasicHandlerStep{SignalSet{0b11110000}, getSignalMask(3), SignalInformation::Code::generated,
				3}},
		{91, 92, false, BasicHandlerStep{SignalSet{0b11100000}, getSignalMask(4), SignalInformation::Code::generated,
				4}},
		{93, 94, false, BasicHandlerStep{SignalSet{0b11000000}, getSignalMask(5), SignalInformation::Code::generated,
				5}},
		{95, 96, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(6), SignalInformation::Code::generated,
				6}},
		{97, 98, false, BasicHandlerStep{SignalSet{0b00000000}, getSignalMask(7), SignalInformation::Code::generated,
				7}},
		// part 4 - unmasking of pending queued signals
		{119, 120, false, BasicHandlerStep{SignalSet{0b10000100}, getSignalMask(2), SignalInformation::Code::queued, 2,
				0x2c9530e7}},
		{121, 122, false, BasicHandlerStep{SignalSet{0b10000100}, getSignalMask(2), SignalInformation::Code::queued, 2,
				0x620f2acf}},
		{123, 124, false, BasicHandlerStep{SignalSet{0b10000100}, getSignalMask(2), SignalInformation::Code::queued, 2,
				0x605724fd}},
		{125, 126, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(2), SignalInformation::Code::queued, 2,
				0x7f2e4b25}},
		{127, 128, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x08055dbe}},
		{129, 130, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x79b6c040}},
		{131, 132, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x7537d600}},
		{133, 134, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::queued, 7, 0x4f8d74b5}},
		// part 5 - unmasking of pending signals
		{171, 172, false, BasicHandlerStep{SignalSet{0b11111111}, getSignalMask(0), SignalInformation::Code::queued, 0,
				0x44bcee3c}},
		{173, 174, false, BasicHandlerStep{SignalSet{0b11111110}, getSignalMask(0), SignalInformation::Code::generated,
				0}},
		{175, 176, false, BasicHandlerStep{SignalSet{0b11111110}, getSignalMask(1), SignalInformation::Code::queued, 1,
				0x1cfc75b5}},
		{177, 178, false, BasicHandlerStep{SignalSet{0b11111100}, getSignalMask(1), SignalInformation::Code::generated,
				1}},
		{179, 180, false, BasicHandlerStep{SignalSet{0b11111100}, getSignalMask(2), SignalInformation::Code::queued, 2,
				0x15c508ba}},
		{181, 182, false, BasicHandlerStep{SignalSet{0b11111000}, getSignalMask(2), SignalInformation::Code::generated,
				2}},
		{183, 184, false, BasicHandlerStep{SignalSet{0b11111000}, getSignalMask(3), SignalInformation::Code::queued, 3,
				0x5b12eca3}},
		{185, 186, false, BasicHandlerStep{SignalSet{0b11110000}, getSignalMask(3), SignalInformation::Code::generated,
				3}},
		{187, 188, false, BasicHandlerStep{SignalSet{0b11110000}, getSignalMask(4), SignalInformation::Code::queued, 4,
				0x381cacad}},
		{189, 190, false, BasicHandlerStep{SignalSet{0b11100000}, getSignalMask(4), SignalInformation::Code::generated,
				4}},
		{191, 192, false, BasicHandlerStep{SignalSet{0b11100000}, getSignalMask(5), SignalInformation::Code::queued, 5,
				0x48b947e9}},
		{193, 194, false, BasicHandlerStep{SignalSet{0b11000000}, getSignalMask(5), SignalInformation::Code::generated,
				5}},
		{195, 196, false, BasicHandlerStep{SignalSet{0b11000000}, getSignalMask(6), SignalInformation::Code::queued, 6,
				0x6c9f1f2}},
		{197, 198, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(6), SignalInformation::Code::generated,
				6}},
		{199, 200, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x3e37ad5}},
		{201, 202, false, BasicHandlerStep{SignalSet{0b00000000}, getSignalMask(7), SignalInformation::Code::generated,
				7}},
		// part 6 - unmasking of pending signals - one at a time
		{239, 240, false, BasicHandlerStep{SignalSet{0b11111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::queued, 2, 0x7abf0fb2}},
		{241, 242, false, BasicHandlerStep{SignalSet{0b11111011}, SignalSet{SignalSet::full},
				SignalInformation::Code::generated, 2}},
		{245, 246, false, BasicHandlerStep{SignalSet{0b11111011}, SignalSet{UINT32_MAX << totalSignals | 0b11111011},
				SignalInformation::Code::queued, 5, 0x4cf013f5}},
		{247, 248, false, BasicHandlerStep{SignalSet{0b11011011}, SignalSet{UINT32_MAX << totalSignals | 0b11111011},
				SignalInformation::Code::generated, 5}},
		{251, 252, false, BasicHandlerStep{SignalSet{0b11011011}, SignalSet{UINT32_MAX << totalSignals | 0b11011011},
				SignalInformation::Code::queued, 3, 0x399dbe15}},
		{253, 254, false, BasicHandlerStep{SignalSet{0b11010011}, SignalSet{UINT32_MAX << totalSignals | 0b11011011},
				SignalInformation::Code::generated, 3}},
		{257, 258, false, BasicHandlerStep{SignalSet{0b11010011}, SignalSet{UINT32_MAX << totalSignals | 0b11010011},
				SignalInformation::Code::queued, 7, 0x75143ba1}},
		{259, 260, false, BasicHandlerStep{SignalSet{0b01010011}, SignalSet{UINT32_MAX << totalSignals | 0b11010011},
				SignalInformation::Code::generated, 7}},
		{263, 264, false, BasicHandlerStep{SignalSet{0b01010011}, SignalSet{UINT32_MAX << totalSignals | 0b01010011},
				SignalInformation::Code::queued, 6, 0x587c305e}},
		{265, 266, false, BasicHandlerStep{SignalSet{0b00010011}, SignalSet{UINT32_MAX << totalSignals | 0b01010011},
				SignalInformation::Code::generated, 6}},
		{269, 270, false, BasicHandlerStep{SignalSet{0b00010011}, SignalSet{UINT32_MAX << totalSignals | 0b00010011},
				SignalInformation::Code::queued, 1, 0x8894ac1}},
		{271, 272, false, BasicHandlerStep{SignalSet{0b00010001}, SignalSet{UINT32_MAX << totalSignals | 0b00010011},
				SignalInformation::Code::generated, 1}},
		{275, 276, false, BasicHandlerStep{SignalSet{0b00010001}, SignalSet{UINT32_MAX << totalSignals | 0b00010001},
				SignalInformation::Code::queued, 4, 0x3f6fa768}},
		{277, 278, false, BasicHandlerStep{SignalSet{0b00000001}, SignalSet{UINT32_MAX << totalSignals | 0b00010001},
				SignalInformation::Code::generated, 4}},
		{281, 282, false, BasicHandlerStep{SignalSet{0b00000001}, SignalSet{UINT32_MAX << totalSignals | 0b00000001},
				SignalInformation::Code::queued, 0, 0x3d982f37}},
		{283, 284, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b00000001}, SignalInformation::Code::generated, 0}},
		// part 7 - nested signal handlers - generate signal from within signal handler
		{287, 288, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b00000001}, SignalInformation::Code::generated, 0}},
		{289, 290, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{291, 346, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{292, 293, true, BasicHandlerStep{SignalSet{0b00000001}, SignalSet{UINT32_MAX << totalSignals | 0b00000011},
				SignalInformation::Code::generated, 1}},
		{294, 295, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{296, 343, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{297, 298, true, BasicHandlerStep{SignalSet{0b00000011}, SignalSet{UINT32_MAX << totalSignals | 0b00000111},
				SignalInformation::Code::generated, 2}},
		{299, 300, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{301, 340, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{302, 303, true, BasicHandlerStep{SignalSet{0b00000111}, SignalSet{UINT32_MAX << totalSignals | 0b00001111},
				SignalInformation::Code::generated, 3}},
		{304, 305, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{306, 337, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{307, 308, true, BasicHandlerStep{SignalSet{0b00001111}, SignalSet{UINT32_MAX << totalSignals | 0b00011111},
				SignalInformation::Code::generated, 4}},
		{309, 310, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{311, 334, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{312, 313, true, BasicHandlerStep{SignalSet{0b00011111}, SignalSet{UINT32_MAX << totalSignals | 0b00111111},
				SignalInformation::Code::generated, 5}},
		{314, 315, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{316, 331, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{317, 318, true, BasicHandlerStep{SignalSet{0b00111111}, SignalSet{UINT32_MAX << totalSignals | 0b01111111},
				SignalInformation::Code::generated, 6}},
		{319, 320, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{321, 328, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{322, 323, true, BasicHandlerStep{SignalSet{0b01111111}, SignalSet{UINT32_MAX << totalSignals | 0b11111111},
				SignalInformation::Code::generated, 7}},
		{324, 325, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{326, 327, false, BasicHandlerStep{SignalSet{0b01111111}, SignalSet{UINT32_MAX << totalSignals | 0b11111111},
				SignalInformation::Code::generated, 7}},
		{329, 330, false, BasicHandlerStep{SignalSet{0b00111111}, SignalSet{UINT32_MAX << totalSignals | 0b01111111},
				SignalInformation::Code::generated, 6}},
		{332, 333, false, BasicHandlerStep{SignalSet{0b00011111}, SignalSet{UINT32_MAX << totalSignals | 0b00111111},
				SignalInformation::Code::generated, 5}},
		{335, 336, false, BasicHandlerStep{SignalSet{0b00001111}, SignalSet{UINT32_MAX << totalSignals | 0b00011111},
				SignalInformation::Code::generated, 4}},
		{338, 339, false, BasicHandlerStep{SignalSet{0b00000111}, SignalSet{UINT32_MAX << totalSignals | 0b00001111},
				SignalInformation::Code::generated, 3}},
		{341, 342, false, BasicHandlerStep{SignalSet{0b00000011}, SignalSet{UINT32_MAX << totalSignals | 0b00000111},
				SignalInformation::Code::generated, 2}},
		{344, 345, false, BasicHandlerStep{SignalSet{0b00000001}, SignalSet{UINT32_MAX << totalSignals | 0b00000011},
				SignalInformation::Code::generated, 1}},
		{347, 348, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b00000001}, SignalInformation::Code::generated, 0}},
		// part 8 - nested signal handlers - queue signal from within signal handler
		{351, 352, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b00000001}, SignalInformation::Code::queued, 0, 0x39a52149}},
		{353, 354, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x69eb4368}},
		{355, 410, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x712a84cd}},
		{356, 357, true, BasicHandlerStep{SignalSet{0b00000001}, SignalSet{UINT32_MAX << totalSignals | 0b00000011},
				SignalInformation::Code::queued, 1, 0x712a84cd}},
		{358, 359, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x59a40114}},
		{360, 407, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x0b3731c5}},
		{361, 362, true, BasicHandlerStep{SignalSet{0b00000011}, SignalSet{UINT32_MAX << totalSignals | 0b00000111},
				SignalInformation::Code::queued, 2, 0x0b3731c5}},
		{363, 364, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x1b265400}},
		{365, 404, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x5cd75dbe}},
		{366, 367, true, BasicHandlerStep{SignalSet{0b00000111}, SignalSet{UINT32_MAX << totalSignals | 0b00001111},
				SignalInformation::Code::queued, 3, 0x5cd75dbe}},
		{368, 369, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x58ad8ba0}},
		{370, 401, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x257ce662}},
		{371, 372, true, BasicHandlerStep{SignalSet{0b00001111}, SignalSet{UINT32_MAX << totalSignals | 0b00011111},
				SignalInformation::Code::queued, 4, 0x257ce662}},
		{373, 374, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x00d0f4f2}},
		{375, 398, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x360d7c66}},
		{376, 377, true, BasicHandlerStep{SignalSet{0b00011111}, SignalSet{UINT32_MAX << totalSignals | 0b00111111},
				SignalInformation::Code::queued, 5, 0x360d7c66}},
		{378, 379, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x20eb8156}},
		{380, 395, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x3df6dd36}},
		{381, 382, true, BasicHandlerStep{SignalSet{0b00111111}, SignalSet{UINT32_MAX << totalSignals | 0b01111111},
				SignalInformation::Code::queued, 6, 0x3df6dd36}},
		{383, 384, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x5f3de0fd}},
		{385, 392, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x7a478fc2}},
		{386, 387, true, BasicHandlerStep{SignalSet{0b01111111}, SignalSet{UINT32_MAX << totalSignals | 0b11111111},
				SignalInformation::Code::queued, 7, 0x7a478fc2}},
		{388, 389, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x687a1290}},
		{390, 391, false, BasicHandlerStep{SignalSet{0b01111111}, SignalSet{UINT32_MAX << totalSignals | 0b11111111},
				SignalInformation::Code::queued, 7, 0x687a1290}},
		{393, 394, false, BasicHandlerStep{SignalSet{0b00111111}, SignalSet{UINT32_MAX << totalSignals | 0b01111111},
				SignalInformation::Code::queued, 6, 0x5f3de0fd}},
		{396, 397, false, BasicHandlerStep{SignalSet{0b00011111}, SignalSet{UINT32_MAX << totalSignals | 0b00111111},
				SignalInformation::Code::queued, 5, 0x20eb8156}},
		{399, 400, false, BasicHandlerStep{SignalSet{0b00001111}, SignalSet{UINT32_MAX << totalSignals | 0b00011111},
				SignalInformation::Code::queued, 4, 0x00d0f4f2}},
		{402, 403, false, BasicHandlerStep{SignalSet{0b00000111}, SignalSet{UINT32_MAX << totalSignals | 0b00001111},
				SignalInformation::Code::queued, 3, 0x58ad8ba0}},
		{405, 406, false, BasicHandlerStep{SignalSet{0b00000011}, SignalSet{UINT32_MAX << totalSignals | 0b00000111},
				SignalInformation::Code::queued, 2, 0x1b265400}},
		{408, 409, false, BasicHandlerStep{SignalSet{0b00000001}, SignalSet{UINT32_MAX << totalSignals | 0b00000011},
				SignalInformation::Code::queued, 1, 0x59a40114}},
		{411, 412, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b00000001}, SignalInformation::Code::queued, 0, 0x69eb4368}},
		// part 9 - nested signal handlers - unmasking of pending signals (one at a time) from within signal handler
		{449, 450, true, BasicHandlerStep{SignalSet{0b11111111}, SignalSet{SignalSet::full},
			SignalInformation::Code::queued, 4, 0x248f49de}},
		{451, 492, false, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b01111111}}},
		{452, 453, true, BasicHandlerStep{SignalSet{0b11111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::queued, 7, 0x0a8b8d2c}},
		{454, 489, false, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b11111011}}},
		{455, 456, true, BasicHandlerStep{SignalSet{0b11111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::queued, 2, 0x2e02ef41}},
		{457, 486, false, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b11110111}}},
		{458, 459, true, BasicHandlerStep{SignalSet{0b11111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::queued, 3, 0x116dd593}},
		{460, 483, false, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b11111101}}},
		{461, 462, true, BasicHandlerStep{SignalSet{0b11111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::queued, 1, 0x4cd69c9e}},
		{463, 480, false, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b11011111}}},
		{464, 465, true, BasicHandlerStep{SignalSet{0b11111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::queued, 5, 0x1e14b4d4}},
		{466, 477, false, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b11111110}}},
		{467, 468, true, BasicHandlerStep{SignalSet{0b11111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::queued, 0, 0x5d8d88c9}},
		{469, 474, false, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals | 0b10111111}}},
		{470, 471, false, BasicHandlerStep{SignalSet{0b11111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::queued, 6, 0x406f37db}},
		{472, 473, false, BasicHandlerStep{SignalSet{0b10111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::generated, 6}},
		{475, 476, false, BasicHandlerStep{SignalSet{0b10111110}, SignalSet{SignalSet::full},
				SignalInformation::Code::generated, 0}},
		{478, 479, false, BasicHandlerStep{SignalSet{0b10011110}, SignalSet{SignalSet::full},
				SignalInformation::Code::generated, 5}},
		{481, 482, false, BasicHandlerStep{SignalSet{0b10011100}, SignalSet{SignalSet::full},
				SignalInformation::Code::generated, 1}},
		{484, 485, false, BasicHandlerStep{SignalSet{0b10010100}, SignalSet{SignalSet::full},
				SignalInformation::Code::generated, 3}},
		{487, 488, false, BasicHandlerStep{SignalSet{0b10010000}, SignalSet{SignalSet::full},
				SignalInformation::Code::generated, 2}},
		{490, 491, false, BasicHandlerStep{SignalSet{0b00010000}, SignalSet{SignalSet::full},
				SignalInformation::Code::generated, 7}},
		{493, 494, false, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{SignalSet::full},
				SignalInformation::Code::generated, 4}},
};

/// test steps for second phase, for main test thread
const TestStep phase2MainTestThreadSteps[]
{
		// part 1 - normal generated signals
		{0, 5, true, SoftwareTimerStep{testStepsRunner}},
		{1, 2, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{6, 11, true, SoftwareTimerStep{testStepsRunner}},
		{7, 8, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{12, 17, true, SoftwareTimerStep{testStepsRunner}},
		{13, 14, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{18, 23, true, SoftwareTimerStep{testStepsRunner}},
		{19, 20, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{24, 29, true, SoftwareTimerStep{testStepsRunner}},
		{25, 26, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{30, 35, true, SoftwareTimerStep{testStepsRunner}},
		{31, 32, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{36, 41, true, SoftwareTimerStep{testStepsRunner}},
		{37, 38, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{42, 47, true, SoftwareTimerStep{testStepsRunner}},
		{43, 44, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		// part 2 - normal queued signals
		{48, 53, true, SoftwareTimerStep{testStepsRunner}},
		{49, 50, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x7bbee7c7}},
		{54, 59, true, SoftwareTimerStep{testStepsRunner}},
		{55, 56, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x144006e1}},
		{60, 65, true, SoftwareTimerStep{testStepsRunner}},
		{61, 62, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x3c2f74c3}},
		{66, 71, true, SoftwareTimerStep{testStepsRunner}},
		{67, 68, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x6842c269}},
		{72, 77, true, SoftwareTimerStep{testStepsRunner}},
		{73, 74, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x282ea423}},
		{78, 83, true, SoftwareTimerStep{testStepsRunner}},
		{79, 80, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x4ac997a6}},
		{84, 89, true, SoftwareTimerStep{testStepsRunner}},
		{85, 86, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x60375e15}},
		{90, 95, true, SoftwareTimerStep{testStepsRunner}},
		{91, 92, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x5025b208}},
		// part 3 - multiple generated signals
		{96, 129, true, SoftwareTimerStep{testStepsRunner}},
		{97, 98, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{99, 100, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{101, 102, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{103, 104, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{105, 106, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{107, 108, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{109, 110, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{111, 112, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		// part 4 - multiple queued signals
		{130, 163, true, SoftwareTimerStep{testStepsRunner}},
		{131, 132, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x7230a2cc}},
		{133, 134, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x183c1811}},
		{135, 136, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x4be507b0}},
		{137, 138, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x249cd4e7}},
		{139, 140, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x44e1258a}},
		{141, 142, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x4ea5f291}},
		{143, 144, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x7739addf}},
		{145, 146, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x66583e56}},
		// part 5 - multiple signals
		{164, 229, true, SoftwareTimerStep{testStepsRunner}},
		{165, 166, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{167, 168, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{169, 170, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{171, 172, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{173, 174, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{175, 176, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{177, 178, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{179, 180, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{181, 182, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x3e9c82fe}},
		{183, 184, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x011c51eb}},
		{185, 186, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x7eb45b48}},
		{187, 188, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x6a4f49c6}},
		{189, 190, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x2961cf0c}},
		{191, 192, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x0ac99377}},
		{193, 194, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x6df7494e}},
		{195, 196, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x3ce8f20c}},
		// part 6 - nested signal handlers - generate signal from interrupt while signal handler is still running
		{230, 231, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals}}},
		{232, 279, true, SoftwareTimerStep{testStepsRunner}},
		{233, 234, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		// part 7 - nested signal handlers - queue signal from interrupt while signal handler is still running
		{280, 327, true, SoftwareTimerStep{testStepsRunner}},
		{281, 282, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x388c64c3}},
		{328, 329, false, SignalMaskStep{SignalSet{SignalSet::empty}}},
};

/// test steps for second phase, for signal handler
const TestStep phase2SignalHandlerSteps[]
{
		// part 1 - normal generated signals
		{3, 4, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::generated, 0}},
		{9, 10, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::generated, 1}},
		{15, 16, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::generated, 2}},
		{21, 22, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::generated, 3}},
		{27, 28, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::generated, 4}},
		{33, 34, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::generated, 5}},
		{39, 40, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::generated, 6}},
		{45, 46, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::generated, 7}},
		// part 2 - normal queued signals
		{51, 52, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0), SignalInformation::Code::queued,
				0, 0x7bbee7c7}},
		{57, 58, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1), SignalInformation::Code::queued,
				1, 0x144006e1}},
		{63, 64, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2), SignalInformation::Code::queued,
				2, 0x3c2f74c3}},
		{69, 70, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3), SignalInformation::Code::queued,
				3, 0x6842c269}},
		{75, 76, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4), SignalInformation::Code::queued,
				4, 0x282ea423}},
		{81, 82, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5), SignalInformation::Code::queued,
				5, 0x4ac997a6}},
		{87, 88, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6), SignalInformation::Code::queued,
				6, 0x60375e15}},
		{93, 94, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7), SignalInformation::Code::queued,
				7, 0x5025b208}},
		// part 3 - multiple generated signals
		{113, 114, false, BasicHandlerStep{SignalSet{0b11111110}, getSignalMask(0), SignalInformation::Code::generated,
				0}},
		{115, 116, false, BasicHandlerStep{SignalSet{0b11111100}, getSignalMask(1), SignalInformation::Code::generated,
				1}},
		{117, 118, false, BasicHandlerStep{SignalSet{0b11111000}, getSignalMask(2), SignalInformation::Code::generated,
				2}},
		{119, 120, false, BasicHandlerStep{SignalSet{0b11110000}, getSignalMask(3), SignalInformation::Code::generated,
				3}},
		{121, 122, false, BasicHandlerStep{SignalSet{0b11100000}, getSignalMask(4), SignalInformation::Code::generated,
				4}},
		{123, 124, false, BasicHandlerStep{SignalSet{0b11000000}, getSignalMask(5), SignalInformation::Code::generated,
				5}},
		{125, 126, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(6), SignalInformation::Code::generated,
				6}},
		{127, 128, false, BasicHandlerStep{SignalSet{0b00000000}, getSignalMask(7), SignalInformation::Code::generated,
				7}},
		// part 4 - multiple queued signals
		{147, 148, false, BasicHandlerStep{SignalSet{0b10001000}, getSignalMask(3), SignalInformation::Code::queued, 3,
				0x183c1811}},
		{149, 150, false, BasicHandlerStep{SignalSet{0b10001000}, getSignalMask(3), SignalInformation::Code::queued, 3,
				0x249cd4e7}},
		{151, 152, false, BasicHandlerStep{SignalSet{0b10001000}, getSignalMask(3), SignalInformation::Code::queued, 3,
				0x4ea5f291}},
		{153, 154, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(3), SignalInformation::Code::queued, 3,
				0x66583e56}},
		{155, 156, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x7230a2cc}},
		{157, 158, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x4be507b0}},
		{159, 160, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x44e1258a}},
		{161, 162, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::queued, 7, 0x7739addf}},
		// part 5 - multiple signals
		{197, 198, false, BasicHandlerStep{SignalSet{0b11111111}, getSignalMask(0), SignalInformation::Code::queued, 0,
				0x3ce8f20c}},
		{199, 200, false, BasicHandlerStep{SignalSet{0b11111110}, getSignalMask(0), SignalInformation::Code::generated,
				0}},
		{201, 202, false, BasicHandlerStep{SignalSet{0b11111110}, getSignalMask(1), SignalInformation::Code::queued, 1,
				0x2961cf0c}},
		{203, 204, false, BasicHandlerStep{SignalSet{0b11111100}, getSignalMask(1), SignalInformation::Code::generated,
				1}},
		{205, 206, false, BasicHandlerStep{SignalSet{0b11111100}, getSignalMask(2), SignalInformation::Code::queued, 2,
				0x0ac99377}},
		{207, 208, false, BasicHandlerStep{SignalSet{0b11111000}, getSignalMask(2), SignalInformation::Code::generated,
				2}},
		{209, 210, false, BasicHandlerStep{SignalSet{0b11111000}, getSignalMask(3), SignalInformation::Code::queued, 3,
				0x6a4f49c6}},
		{211, 212, false, BasicHandlerStep{SignalSet{0b11110000}, getSignalMask(3), SignalInformation::Code::generated,
				3}},
		{213, 214, false, BasicHandlerStep{SignalSet{0b11110000}, getSignalMask(4), SignalInformation::Code::queued, 4,
				0x3e9c82fe}},
		{215, 216, false, BasicHandlerStep{SignalSet{0b11100000}, getSignalMask(4), SignalInformation::Code::generated,
				4}},
		{217, 218, false, BasicHandlerStep{SignalSet{0b11100000}, getSignalMask(5), SignalInformation::Code::queued, 5,
				0x7eb45b48}},
		{219, 220, false, BasicHandlerStep{SignalSet{0b11000000}, getSignalMask(5), SignalInformation::Code::generated,
				5}},
		{221, 222, false, BasicHandlerStep{SignalSet{0b11000000}, getSignalMask(6), SignalInformation::Code::queued, 6,
				0x011c51eb}},
		{223, 224, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(6), SignalInformation::Code::generated,
				6}},
		{225, 226, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x6df7494e}},
		{227, 228, false, BasicHandlerStep{SignalSet{0b00000000}, getSignalMask(7), SignalInformation::Code::generated,
				7}},
		// part 6 - nested signal handlers - generate signal from interrupt while signal handler is still running
		{235, 236, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b00100000}, SignalInformation::Code::generated, 5}},
		{237, 278, false, SoftwareTimerStep{testStepsRunner}},
		{238, 239, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{240, 241, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b00100100}, SignalInformation::Code::generated, 2}},
		{242, 277, false, SoftwareTimerStep{testStepsRunner}},
		{243, 244, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{245, 246, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b00100110}, SignalInformation::Code::generated, 1}},
		{247, 276, false, SoftwareTimerStep{testStepsRunner}},
		{248, 249, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{250, 251, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b10100110}, SignalInformation::Code::generated, 7}},
		{252, 275, false, SoftwareTimerStep{testStepsRunner}},
		{253, 254, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{255, 256, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11100110}, SignalInformation::Code::generated, 6}},
		{257, 274, false, SoftwareTimerStep{testStepsRunner}},
		{258, 259, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{260, 261, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11101110}, SignalInformation::Code::generated, 3}},
		{262, 273, false, SoftwareTimerStep{testStepsRunner}},
		{263, 264, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{265, 266, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11101111}, SignalInformation::Code::generated, 0}},
		{267, 272, false, SoftwareTimerStep{testStepsRunner}},
		{268, 269, false, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{270, 271, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11111111}, SignalInformation::Code::generated, 4}},
		// part 7 - nested signal handlers - queue signal from interrupt while signal handler is still running
		{283, 284, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b01000000}, SignalInformation::Code::queued, 6, 0x388c64c3}},
		{285, 326, false, SoftwareTimerStep{testStepsRunner}},
		{286, 287, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x49020a59}},
		{288, 289, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b01001000}, SignalInformation::Code::queued, 3, 0x49020a59}},
		{290, 325, false, SoftwareTimerStep{testStepsRunner}},
		{291, 292, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x7e8842b9}},
		{293, 294, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b01001001}, SignalInformation::Code::queued, 0, 0x7e8842b9}},
		{295, 324, false, SoftwareTimerStep{testStepsRunner}},
		{296, 297, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x19ccf80b}},
		{298, 299, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11001001}, SignalInformation::Code::queued, 7, 0x19ccf80b}},
		{300, 323, false, SoftwareTimerStep{testStepsRunner}},
		{301, 302, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x68e46a0f}},
		{303, 304, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11011001}, SignalInformation::Code::queued, 4, 0x68e46a0f}},
		{305, 322, false, SoftwareTimerStep{testStepsRunner}},
		{306, 307, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x3bf7a491}},
		{308, 309, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11011101}, SignalInformation::Code::queued, 2, 0x3bf7a491}},
		{310, 321, false, SoftwareTimerStep{testStepsRunner}},
		{311, 312, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x5ff85ef6}},
		{313, 314, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11111101}, SignalInformation::Code::queued, 5, 0x5ff85ef6}},
		{315, 320, false, SoftwareTimerStep{testStepsRunner}},
		{316, 317, false, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x4f8adea0}},
		{318, 319, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11111111}, SignalInformation::Code::queued, 1, 0x4f8adea0}},
};

/// test steps for third phase, for main test thread
const TestStep phase3MainTestThreadSteps[]
{
		// part 1 - normal generated signals
		{0, 6, true, ThreadPriorityStep{lowPriority}},
		{7, 14, true, ThreadPriorityStep{lowPriority}},
		{15, 22, true, ThreadPriorityStep{lowPriority}},
		{23, 30, true, ThreadPriorityStep{lowPriority}},
		{31, 38, true, ThreadPriorityStep{lowPriority}},
		{39, 46, true, ThreadPriorityStep{lowPriority}},
		{47, 54, true, ThreadPriorityStep{lowPriority}},
		{55, 62, true, ThreadPriorityStep{lowPriority}},
		// part 2 - normal queued signals
		{63, 70, true, ThreadPriorityStep{lowPriority}},
		{71, 78, true, ThreadPriorityStep{lowPriority}},
		{79, 86, true, ThreadPriorityStep{lowPriority}},
		{87, 94, true, ThreadPriorityStep{lowPriority}},
		{95, 102, true, ThreadPriorityStep{lowPriority}},
		{103, 110, true, ThreadPriorityStep{lowPriority}},
		{111, 118, true, ThreadPriorityStep{lowPriority}},
		{119, 126, true, ThreadPriorityStep{lowPriority}},
		// part 3 - multiple generated signals
		{127, 162, true, ThreadPriorityStep{lowPriority}},
		// part 4 - multiple queued signals
		{163, 198, true, ThreadPriorityStep{lowPriority}},
		// part 5 - multiple signals
		{199, 266, true, ThreadPriorityStep{lowPriority}},
		// part 6 - nested signal handlers - generate signal from second test thread while signal handler is still
		// running
		{267, 268, true, SignalMaskStep{SignalSet{UINT32_MAX << totalSignals}}},
		{269, 332, true, ThreadPriorityStep{lowPriority}},
		// part 7 - nested signal handlers - queue signal from second test thread while signal handler is still running
		{333, 396, true, ThreadPriorityStep{lowPriority}},
		{397, 398, false, SignalMaskStep{SignalSet{SignalSet::empty}}},
};

/// test steps for third phase, for signal handler
const TestStep phase3SignalHandlerSteps[]
{
		// part 1 - normal generated signals
		{4, 5, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::generated, 0}},
		{12, 13, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::generated, 1}},
		{20, 21, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::generated, 2}},
		{28, 29, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::generated, 3}},
		{36, 37, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::generated, 4}},
		{44, 45, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::generated, 5}},
		{52, 53, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::generated, 6}},
		{60, 61, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::generated, 7}},
		// part 2 - normal queued signals
		{68, 69, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::queued, 0, 0x1d804a34}},
		{76, 77, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::queued, 1, 0x27bb29af}},
		{84, 85, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::queued, 2, 0x7006f7ce}},
		{92, 93, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::queued, 3, 0x10f09338}},
		{100, 101, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::queued, 4, 0x3d657640}},
		{108, 109, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::queued, 5, 0x2ebf9a5f}},
		{116, 117, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::queued, 6, 0x1b914049}},
		{124, 125, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::queued, 7, 0x1989b75c}},
		// part 3 - multiple generated signals
		{146, 147, false, BasicHandlerStep{SignalSet{0b11111110}, getSignalMask(0), SignalInformation::Code::generated,
				0}},
		{148, 149, false, BasicHandlerStep{SignalSet{0b11111100}, getSignalMask(1), SignalInformation::Code::generated,
				1}},
		{150, 151, false, BasicHandlerStep{SignalSet{0b11111000}, getSignalMask(2), SignalInformation::Code::generated,
				2}},
		{152, 153, false, BasicHandlerStep{SignalSet{0b11110000}, getSignalMask(3), SignalInformation::Code::generated,
				3}},
		{154, 155, false, BasicHandlerStep{SignalSet{0b11100000}, getSignalMask(4), SignalInformation::Code::generated,
				4}},
		{156, 157, false, BasicHandlerStep{SignalSet{0b11000000}, getSignalMask(5), SignalInformation::Code::generated,
				5}},
		{158, 159, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(6), SignalInformation::Code::generated,
				6}},
		{160, 161, false, BasicHandlerStep{SignalSet{0b00000000}, getSignalMask(7), SignalInformation::Code::generated,
				7}},
		// part 4 - multiple queued signals
		{182, 183, false, BasicHandlerStep{SignalSet{0b10010000}, getSignalMask(4), SignalInformation::Code::queued, 4,
				0x4ab84596}},
		{184, 185, false, BasicHandlerStep{SignalSet{0b10010000}, getSignalMask(4), SignalInformation::Code::queued, 4,
				0x59e3f309}},
		{186, 187, false, BasicHandlerStep{SignalSet{0b10010000}, getSignalMask(4), SignalInformation::Code::queued, 4,
				0x1ab5ff9e}},
		{188, 189, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(4), SignalInformation::Code::queued, 4,
				0x1e09b682}},
		{190, 191, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x29bd9d85}},
		{192, 193, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x4fa58f4f}},
		{194, 195, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x48b7be56}},
		{196, 197, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::queued, 7, 0x6f8dc16b}},
		// part 5 - multiple signals
		{234, 235, false, BasicHandlerStep{SignalSet{0b11111111}, getSignalMask(0), SignalInformation::Code::queued, 0,
				0x37921ff7}},
		{236, 237, false, BasicHandlerStep{SignalSet{0b11111110}, getSignalMask(0), SignalInformation::Code::generated,
				0}},
		{238, 239, false, BasicHandlerStep{SignalSet{0b11111110}, getSignalMask(1), SignalInformation::Code::queued, 1,
				0x2a7fc8fc}},
		{240, 241, false, BasicHandlerStep{SignalSet{0b11111100}, getSignalMask(1), SignalInformation::Code::generated,
				1}},
		{242, 243, false, BasicHandlerStep{SignalSet{0b11111100}, getSignalMask(2), SignalInformation::Code::queued, 2,
				0x4fab0990}},
		{244, 245, false, BasicHandlerStep{SignalSet{0b11111000}, getSignalMask(2), SignalInformation::Code::generated,
				2}},
		{246, 247, false, BasicHandlerStep{SignalSet{0b11111000}, getSignalMask(3), SignalInformation::Code::queued, 3,
				0x2bc11d59}},
		{248, 249, false, BasicHandlerStep{SignalSet{0b11110000}, getSignalMask(3), SignalInformation::Code::generated,
				3}},
		{250, 251, false, BasicHandlerStep{SignalSet{0b11110000}, getSignalMask(4), SignalInformation::Code::queued, 4,
				0x6fd4db4a}},
		{252, 253, false, BasicHandlerStep{SignalSet{0b11100000}, getSignalMask(4), SignalInformation::Code::generated,
				4}},
		{254, 255, false, BasicHandlerStep{SignalSet{0b11100000}, getSignalMask(5), SignalInformation::Code::queued, 5,
				0x37f2577b}},
		{256, 257, false, BasicHandlerStep{SignalSet{0b11000000}, getSignalMask(5), SignalInformation::Code::generated,
				5}},
		{258, 259, false, BasicHandlerStep{SignalSet{0b11000000}, getSignalMask(6), SignalInformation::Code::queued, 6,
				0x01c636ac}},
		{260, 261, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(6), SignalInformation::Code::generated,
				6}},
		{262, 263, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::queued, 7,
				0x6bd662a6}},
		{264, 265, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::generated, 7}},
		// part 6 - nested signal handlers - generate signal from second test thread while signal handler is still
		// running
		{274, 275, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b10000000}, SignalInformation::Code::generated, 7}},
		{276, 331, false, ThreadPriorityStep{lowPriority}},
		{281, 282, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11000000}, SignalInformation::Code::generated, 6}},
		{283, 330, false, ThreadPriorityStep{lowPriority}},
		{288, 289, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11000100}, SignalInformation::Code::generated, 2}},
		{290, 329, false, ThreadPriorityStep{lowPriority}},
		{295, 296, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11000110}, SignalInformation::Code::generated, 1}},
		{297, 328, false, ThreadPriorityStep{lowPriority}},
		{302, 303, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11000111}, SignalInformation::Code::generated, 0}},
		{304, 327, false, ThreadPriorityStep{lowPriority}},
		{309, 310, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11100111}, SignalInformation::Code::generated, 5}},
		{311, 326, false, ThreadPriorityStep{lowPriority}},
		{316, 317, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11101111}, SignalInformation::Code::generated, 3}},
		{318, 325, false, ThreadPriorityStep{lowPriority}},
		{323, 324, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11111111}, SignalInformation::Code::generated, 4}},
		// part 7 - nested signal handlers - queue signal from second test thread while signal handler is still running
		{338, 339, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b00000010}, SignalInformation::Code::queued, 1, 0x4d937c16}},
		{340, 395, false, ThreadPriorityStep{lowPriority}},
		{345, 346, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b01000010}, SignalInformation::Code::queued, 6, 0x1fb6d90e}},
		{347, 394, false, ThreadPriorityStep{lowPriority}},
		{352, 353, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b01000011}, SignalInformation::Code::queued, 0, 0x666795c4}},
		{354, 393, false, ThreadPriorityStep{lowPriority}},
		{359, 360, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b01100011}, SignalInformation::Code::queued, 5, 0x6637a3ac}},
		{361, 392, false, ThreadPriorityStep{lowPriority}},
		{366, 367, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b01101011}, SignalInformation::Code::queued, 3, 0x7d9b8bf5}},
		{368, 391, false, ThreadPriorityStep{lowPriority}},
		{373, 374, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b01101111}, SignalInformation::Code::queued, 2, 0x6e2bb980}},
		{375, 390, false, ThreadPriorityStep{lowPriority}},
		{380, 381, true, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11101111}, SignalInformation::Code::queued, 7, 0x2b680b53}},
		{382, 389, false, ThreadPriorityStep{lowPriority}},
		{387, 388, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{UINT32_MAX << totalSignals | 0b11111111}, SignalInformation::Code::queued, 4, 0x63897fe9}},
};

/// test steps for third phase, for second test thread
const TestStep phase3SecondTestThreadStepsRange[]
{
		// part 1 - normal generated signals
		{1, 2, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{3, 8, true, ThreadPriorityStep{mainTestThreadPriority}},
		{9, 10, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{11, 16, true, ThreadPriorityStep{mainTestThreadPriority}},
		{17, 18, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{19, 24, true, ThreadPriorityStep{mainTestThreadPriority}},
		{25, 26, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{27, 32, true, ThreadPriorityStep{mainTestThreadPriority}},
		{33, 34, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{35, 40, true, ThreadPriorityStep{mainTestThreadPriority}},
		{41, 42, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{43, 48, true, ThreadPriorityStep{mainTestThreadPriority}},
		{49, 50, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{51, 56, true, ThreadPriorityStep{mainTestThreadPriority}},
		{57, 58, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{59, 64, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 2 - normal queued signals
		{65, 66, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x1d804a34}},
		{67, 72, true, ThreadPriorityStep{mainTestThreadPriority}},
		{73, 74, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x27bb29af}},
		{75, 80, true, ThreadPriorityStep{mainTestThreadPriority}},
		{81, 82, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x7006f7ce}},
		{83, 88, true, ThreadPriorityStep{mainTestThreadPriority}},
		{89, 90, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x10f09338}},
		{91, 96, true, ThreadPriorityStep{mainTestThreadPriority}},
		{97, 98, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x3d657640}},
		{99, 104, true, ThreadPriorityStep{mainTestThreadPriority}},
		{105, 106, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x2ebf9a5f}},
		{107, 112, true, ThreadPriorityStep{mainTestThreadPriority}},
		{113, 114, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x1b914049}},
		{115, 120, true, ThreadPriorityStep{mainTestThreadPriority}},
		{121, 122, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x1989b75c}},
		{123, 128, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 3 - multiple generated signals
		{129, 130, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{131, 132, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{133, 134, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{135, 136, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{137, 138, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{139, 140, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{141, 142, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{143, 144, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{145, 164, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 4 - multiple queued signals
		{165, 166, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x29bd9d85}},
		{167, 168, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x4ab84596}},
		{169, 170, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x4fa58f4f}},
		{171, 172, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x59e3f309}},
		{173, 174, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x48b7be56}},
		{175, 176, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x1ab5ff9e}},
		{177, 178, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x6f8dc16b}},
		{179, 180, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x1e09b682}},
		{181, 200, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 5 - multiple signals
		{201, 202, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{203, 204, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{205, 206, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{207, 208, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{209, 210, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{211, 212, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{213, 214, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{215, 216, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{217, 218, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x37f2577b}},
		{219, 220, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x2bc11d59}},
		{221, 222, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x37921ff7}},
		{223, 224, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x6bd662a6}},
		{225, 226, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x4fab0990}},
		{227, 228, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x2a7fc8fc}},
		{229, 230, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x6fd4db4a}},
		{231, 232, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x01c636ac}},
		{233, 270, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 6 - nested signal handlers - generate signal from second test thread while signal handler is still
		// running
		{271, 272, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 7}},
		{273, 277, true, ThreadPriorityStep{mainTestThreadPriority}},
		{278, 279, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 6}},
		{280, 284, true, ThreadPriorityStep{mainTestThreadPriority}},
		{285, 286, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 2}},
		{287, 291, true, ThreadPriorityStep{mainTestThreadPriority}},
		{292, 293, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 1}},
		{294, 298, true, ThreadPriorityStep{mainTestThreadPriority}},
		{299, 300, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 0}},
		{301, 305, true, ThreadPriorityStep{mainTestThreadPriority}},
		{306, 307, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 5}},
		{308, 312, true, ThreadPriorityStep{mainTestThreadPriority}},
		{313, 314, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 3}},
		{315, 319, true, ThreadPriorityStep{mainTestThreadPriority}},
		{320, 321, true, GenerateQueueSignalStep{SignalInformation::Code::generated, 4}},
		{322, 334, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 7 - nested signal handlers - queue signal from second test thread while signal handler is still running
		{335, 336, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 1, 0x4d937c16}},
		{337, 341, true, ThreadPriorityStep{mainTestThreadPriority}},
		{342, 343, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 6, 0x1fb6d90e}},
		{344, 348, true, ThreadPriorityStep{mainTestThreadPriority}},
		{349, 350, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 0, 0x666795c4}},
		{351, 355, true, ThreadPriorityStep{mainTestThreadPriority}},
		{356, 357, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 5, 0x6637a3ac}},
		{358, 362, true, ThreadPriorityStep{mainTestThreadPriority}},
		{363, 364, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 3, 0x7d9b8bf5}},
		{365, 369, true, ThreadPriorityStep{mainTestThreadPriority}},
		{370, 371, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 2, 0x6e2bb980}},
		{372, 376, true, ThreadPriorityStep{mainTestThreadPriority}},
		{377, 378, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 7, 0x2b680b53}},
		{379, 383, true, ThreadPriorityStep{mainTestThreadPriority}},
		{384, 385, true, GenerateQueueSignalStep{SignalInformation::Code::queued, 4, 0x63897fe9}},
		{386, 399, false, ThreadPriorityStep{mainTestThreadPriority}},
};

/// test stages
const Stage stages[]
{
		// tests catching of signals generated/queued by current thread for itself
		{TestStepsRange{phase1MainTestThreadSteps}, TestStepsRange{phase1SignalHandlerSteps}, TestStepsRange{}, 0},
		// tests catching of signals generated/queued by interrupt (via software timer) for current thread
		{TestStepsRange{phase2MainTestThreadSteps}, TestStepsRange{phase2SignalHandlerSteps}, TestStepsRange{}, 0},
		// tests catching of signals generated/queued by second test thread for main test thread
		{TestStepsRange{phase3MainTestThreadSteps}, TestStepsRange{phase3SignalHandlerSteps},
				TestStepsRange{phase3SecondTestThreadStepsRange}, 70 + 2},
};

}	// namespace

#endif	// SIGNALS_CATCHING_TEST_CASE_ENABLED == 1

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsCatchingTestCase::run_() const
{
#if SIGNALS_CATCHING_TEST_CASE_ENABLED == 1

	const auto contextSwitchCount = statistics::getContextSwitchCount();
	auto expectedContextSwitchCount = decltype(contextSwitchCount){};

	for (uint8_t signalNumber {}; signalNumber < totalSignals; ++signalNumber)
	{
		const auto setSignalActionResult = ThisThread::Signals::setSignalAction(signalNumber,
				{handler, getSignalMask(totalSignals + signalNumber)});
		if (setSignalActionResult.first != 0)
			return false;
	}

	auto& currentThread = ThisThread::get();

	for (auto& stage : stages)
	{
		// initially no signals may be pending
		if (ThisThread::Signals::getPendingSignalSet().getBitset().any() == true)
			return false;

		sharedSequenceAsserter = {};	// reset object
		sharedSigAtomic = {};			// reset object

		handlerStepsRange = stage.signalHandlerStepsRange;
		auto currentThreadStepsRange = stage.mainTestThreadStepsRange;
		auto secondTestThreadStepsRange = stage.secondTestThreadStepsRange;

		auto secondThread = makeDynamicThread({secondTestThreadStackSize, secondTestThreadPriority}, testStepsRunner,
				std::ref(secondTestThreadStepsRange), std::ref(currentThread), nullptr);

		const auto isSecondThreadNeeded = secondTestThreadStepsRange.size() != 0;
		if (isSecondThreadNeeded == true)
			secondThread.start();

		testStepsRunner(currentThreadStepsRange, currentThread, nullptr);

		if (isSecondThreadNeeded == true)
			secondThread.join();

		const auto mainTestThreadStepsRangeSize = stage.mainTestThreadStepsRange.size();
		const auto signalHandlerStepsRangeSize = stage.signalHandlerStepsRange.size();
		const auto secondTestThreadStepsRangeSize = stage.secondTestThreadStepsRange.size();
		const auto totalTestStepsRangeSize = mainTestThreadStepsRangeSize + signalHandlerStepsRangeSize +
				secondTestThreadStepsRangeSize;
		if (sharedSequenceAsserter.assertSequence(2 * totalTestStepsRangeSize) == false)
			return false;

		if (sharedSigAtomic != 0)
			return false;

		expectedContextSwitchCount += stage.contextSwitchCount;
		if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
			return false;
	}

	// after the test no signals may be pending
	if (ThisThread::Signals::getPendingSignalSet().getBitset().any() == true)
		return false;

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

#endif	// SIGNALS_CATCHING_TEST_CASE_ENABLED == 1

	return true;
}

}	// namespace test

}	// namespace distortos
