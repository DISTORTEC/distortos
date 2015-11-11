/**
 * \file
 * \brief SignalsCatchingTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-11
 */

#include "SignalsCatchingTestCase.hpp"

#include "SequenceAsserter.hpp"

#include "distortos/SoftwareTimer.hpp"
#include "distortos/StaticThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/ThisThread-Signals.hpp"

#include "estd/ContiguousRange.hpp"

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

/// test step that generates of queues a signal
class GenerateQueueSignalStep
{
public:

	/**
	 * \brief GenerateQueueSignalStep's constructor
	 *
	 * \param [in] code selects whether the signal will be generated (SignalInformation::Code::Generated) or queued
	 * (SignalInformation::Code::Queued)
	 * \param [in] signalNumber is the signal number that will be generated or queued
	 * \param [in] value is the value that will be queued with the signal, relevant only if \a code is
	 * SignalInformation::Code::Generated, default - default constructed int value
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

	/// value that will be queued with the signal, relevant only if \a code_ is SignalInformation::Code::Generated
	int value_;

	/// selects whether the signal will be generated (SignalInformation::Code::Generated) or queued
	/// (SignalInformation::Code::Queued)
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
		BasicHandler,
		/// GenerateQueueSignalStep
		GenerateQueueSignal,
		/// ThreadPriorityStep
		ThreadPriority,
		/// SignalMaskStep
		SignalMask,
		/// SoftwareTimerStep
		SoftwareTimer,
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
			type_{Type::BasicHandler}
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
			type_{Type::GenerateQueueSignal}
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
			type_{Type::ThreadPriority}
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
			type_{Type::SignalMask}
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
			type_{Type::SoftwareTimer}
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
		/// BasicHandlerStep test step - valid only if type_ == Type::BasicHandler
		BasicHandlerStep basicHandlerStep_;

		/// GenerateQueueSignalStep test step - valid only if type_ == Type::GenerateQueueSignal
		GenerateQueueSignalStep generateQueueSignalStep_;

		/// ThreadPriorityStep test step - valid only if type_ == Type::ThreadPriority
		ThreadPriorityStep threadPriorityStep_;

		/// SignalMaskStep test step - valid only if type_ == Type::SignalMask
		SignalMaskStep signalMaskStep_;

		/// SoftwareTimerStep test step - valid only if type_ == Type::SoftwareTimer
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
constexpr size_t totalSignals {10};

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
	return code_ == SignalInformation::Code::Generated ? thread.generateSignal(signalNumber_) :
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
			type_ == Type::BasicHandler && signalInformation != nullptr ? basicHandlerStep_(*signalInformation) :
			type_ == Type::GenerateQueueSignal ? generateQueueSignalStep_(thread) :
			type_ == Type::ThreadPriority ? threadPriorityStep_(thread) :
			type_ == Type::SignalMask ? signalMaskStep_() :
			type_ == Type::SoftwareTimer ? softwareTimerStep_(testStepsRange, thread) : EINVAL;

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
	return SignalSet{static_cast<uint32_t>((1 << signalNumber) | (1 << (signalNumber + totalSignals)) |
			(UINT32_MAX << (totalSignals * 2)))};
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
		{0, 3, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{4, 7, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{8, 11, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{12, 15, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{16, 19, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{20, 23, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{24, 27, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{28, 31, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{32, 35, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{36, 39, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		// part 2 - normal queued signals
		{40, 43, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x6c3d9ebc}},
		{44, 47, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x52e04282}},
		{48, 51, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x29f9fc86}},
		{52, 55, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x19677883}},
		{56, 59, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x7f2d693b}},
		{60, 63, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x1a98ab78}},
		{64, 67, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x6b96c96b}},
		{68, 71, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x463445cc}},
		{72, 75, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x38dccfd2}},
		{76, 79, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x1e8ac134}},
		// part 3 - unmasking of pending generated signals
		{80, 81, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{82, 83, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{84, 85, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{86, 87, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{88, 89, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{90, 91, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{92, 93, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{94, 95, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{96, 97, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{98, 99, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{100, 101, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{102, 123, true, SignalMaskStep{SignalSet{SignalSet::empty}}},
		// part 4 - unmasking of pending queued signals
		{124, 125, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{126, 127, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x08055dbe}},
		{128, 129, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x2c9530e7}},
		{130, 131, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x79b6c040}},
		{132, 133, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x620f2acf}},
		{134, 135, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x7537d600}},
		{136, 137, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x605724fd}},
		{138, 139, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x4f8d74b5}},
		{140, 141, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x7f2e4b25}},
		{142, 143, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x2b56f970}},
		{144, 145, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x3898dc9e}},
		{146, 167, true, SignalMaskStep{SignalSet{SignalSet::empty}}},
		// part 5 - unmasking of pending signals
		{168, 169, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{170, 171, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{172, 173, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{174, 175, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{176, 177, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{178, 179, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{180, 181, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{182, 183, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{184, 185, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{186, 187, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{188, 189, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{190, 191, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x3e37ad5}},
		{192, 193, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x6c9f1f2}},
		{194, 195, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x5b12eca3}},
		{196, 197, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x48b947e9}},
		{198, 199, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x381cacad}},
		{200, 201, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x299d82f6}},
		{202, 203, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x44bcee3c}},
		{204, 205, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x6b98cc3d}},
		{206, 207, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x1cfc75b5}},
		{208, 209, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x15c508ba}},
		{210, 251, true, SignalMaskStep{SignalSet{SignalSet::empty}}},
		// part 6 - unmasking of pending signals - one at a time
		{252, 253, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{254, 255, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{256, 257, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{258, 259, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{260, 261, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{262, 263, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{264, 265, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{266, 267, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{268, 269, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{270, 271, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{272, 273, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{274, 275, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x3d982f37}},
		{276, 277, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x75143ba1}},
		{278, 279, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x4cf013f5}},
		{280, 281, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x1c010698}},
		{282, 283, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0xd4dc81a}},
		{284, 285, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x8894ac1}},
		{286, 287, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x399dbe15}},
		{288, 289, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x7abf0fb2}},
		{290, 291, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x587c305e}},
		{292, 293, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x3f6fa768}},
		{294, 299, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111111011}}},
		{300, 305, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111011011}}},
		{306, 311, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111010011}}},
		{312, 317, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1011010011}}},
		{318, 323, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1001010011}}},
		{324, 329, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1000010011}}},
		{330, 335, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b0000010011}}},
		{336, 341, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b0000010001}}},
		{342, 347, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b0000000001}}},
		{348, 353, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b0000000000}}},
		// part 7 - nested signal handlers - generate signal from within signal handler
		{354, 433, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		// part 8 - nested signal handlers - queue signal from within signal handler
		{434, 513, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x39a52149}},
		// part 9 - nested signal handlers - unmasking of pending signals (one at a time) from within signal handler
		{514, 515, true, SignalMaskStep{SignalSet{SignalSet::full}}},
		{516, 517, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{518, 519, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{520, 521, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{522, 523, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{524, 525, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{526, 527, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{528, 529, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{530, 531, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{532, 533, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{534, 535, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{536, 537, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x2e02ef41}},
		{538, 539, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x406f37db}},
		{540, 541, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x1e14b4d4}},
		{542, 543, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x579cdee3}},
		{544, 545, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x4cd69c9e}},
		{546, 547, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x49ad2898}},
		{548, 549, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x248f49de}},
		{550, 551, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x116dd593}},
		{552, 553, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x5d8d88c9}},
		{554, 555, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x0a8b8d2c}},
		{556, 615, true, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111101111}}},
		{616, 617, false, SignalMaskStep{SignalSet{SignalSet::empty}}},
};

/// test steps for first phase, for signal handler
const TestStep phase1SignalHandlerSteps[]
{
		// part 1 - normal generated signals
		{1, 2, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::Generated, 0}},
		{5, 6, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::Generated, 1}},
		{9, 10, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::Generated, 2}},
		{13, 14, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::Generated, 3}},
		{17, 18, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::Generated, 4}},
		{21, 22, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::Generated, 5}},
		{25, 26, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::Generated, 6}},
		{29, 30, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::Generated, 7}},
		{33, 34, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(8),
				SignalInformation::Code::Generated, 8}},
		{37, 38, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Generated, 9}},
		// part 2 - normal queued signals
		{41, 42, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::Queued, 0, 0x6c3d9ebc}},
		{45, 46, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::Queued, 1, 0x52e04282}},
		{49, 50, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::Queued, 2, 0x29f9fc86}},
		{53, 54, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x19677883}},
		{57, 58, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x7f2d693b}},
		{61, 62, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::Queued, 5, 0x1a98ab78}},
		{65, 66, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::Queued, 6, 0x6b96c96b}},
		{69, 70, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x463445cc}},
		{73, 74, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(8),
				SignalInformation::Code::Queued, 8, 0x38dccfd2}},
		{77, 78, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x1e8ac134}},
		// part 3 - unmasking of pending generated signals
		{103, 104, false, BasicHandlerStep{SignalSet{0b1111111110}, getSignalMask(0),
				SignalInformation::Code::Generated, 0}},
		{105, 106, false, BasicHandlerStep{SignalSet{0b1111111100}, getSignalMask(1),
				SignalInformation::Code::Generated, 1}},
		{107, 108, false, BasicHandlerStep{SignalSet{0b1111111000}, getSignalMask(2),
				SignalInformation::Code::Generated, 2}},
		{109, 110, false, BasicHandlerStep{SignalSet{0b1111110000}, getSignalMask(3),
				SignalInformation::Code::Generated, 3}},
		{111, 112, false, BasicHandlerStep{SignalSet{0b1111100000}, getSignalMask(4),
				SignalInformation::Code::Generated, 4}},
		{113, 114, false, BasicHandlerStep{SignalSet{0b1111000000}, getSignalMask(5),
				SignalInformation::Code::Generated, 5}},
		{115, 116, false, BasicHandlerStep{SignalSet{0b1110000000}, getSignalMask(6),
				SignalInformation::Code::Generated, 6}},
		{117, 118, false, BasicHandlerStep{SignalSet{0b1100000000}, getSignalMask(7),
				SignalInformation::Code::Generated, 7}},
		{119, 120, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(8),
				SignalInformation::Code::Generated, 8}},
		{121, 122, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Generated, 9}},
		// part 4 - unmasking of pending queued signals
		{147, 148, false, BasicHandlerStep{SignalSet{0b10000100}, getSignalMask(2), SignalInformation::Code::Queued,
				2, 0x2c9530e7}},
		{149, 150, false, BasicHandlerStep{SignalSet{0b10000100}, getSignalMask(2), SignalInformation::Code::Queued,
				2, 0x620f2acf}},
		{151, 152, false, BasicHandlerStep{SignalSet{0b10000100}, getSignalMask(2), SignalInformation::Code::Queued,
				2, 0x605724fd}},
		{153, 154, false, BasicHandlerStep{SignalSet{0b10000100}, getSignalMask(2), SignalInformation::Code::Queued,
				2, 0x7f2e4b25}},
		{155, 156, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(2), SignalInformation::Code::Queued,
				2, 0x3898dc9e}},
		{157, 158, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::Queued,
				7, 0x08055dbe}},
		{159, 160, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::Queued,
				7, 0x79b6c040}},
		{161, 162, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::Queued,
				7, 0x7537d600}},
		{163, 164, false, BasicHandlerStep{SignalSet{0b10000000}, getSignalMask(7), SignalInformation::Code::Queued,
				7, 0x4f8d74b5}},
		{165, 166, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x2b56f970}},
		// part 5 - unmasking of pending signals
		{211, 212, false, BasicHandlerStep{SignalSet{0b1111111111}, getSignalMask(0),
				SignalInformation::Code::Queued, 0, 0x44bcee3c}},
		{213, 214, false, BasicHandlerStep{SignalSet{0b1111111110}, getSignalMask(0),
				SignalInformation::Code::Generated, 0}},
		{215, 216, false, BasicHandlerStep{SignalSet{0b1111111110}, getSignalMask(1),
				SignalInformation::Code::Queued, 1, 0x1cfc75b5}},
		{217, 218, false, BasicHandlerStep{SignalSet{0b1111111100}, getSignalMask(1),
				SignalInformation::Code::Generated, 1}},
		{219, 220, false, BasicHandlerStep{SignalSet{0b1111111100}, getSignalMask(2),
				SignalInformation::Code::Queued, 2, 0x15c508ba}},
		{221, 222, false, BasicHandlerStep{SignalSet{0b1111111000}, getSignalMask(2),
				SignalInformation::Code::Generated, 2}},
		{223, 224, false, BasicHandlerStep{SignalSet{0b1111111000}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x5b12eca3}},
		{225, 226, false, BasicHandlerStep{SignalSet{0b1111110000}, getSignalMask(3),
				SignalInformation::Code::Generated, 3}},
		{227, 228, false, BasicHandlerStep{SignalSet{0b1111110000}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x381cacad}},
		{229, 230, false, BasicHandlerStep{SignalSet{0b1111100000}, getSignalMask(4),
				SignalInformation::Code::Generated, 4}},
		{231, 232, false, BasicHandlerStep{SignalSet{0b1111100000}, getSignalMask(5),
				SignalInformation::Code::Queued, 5, 0x48b947e9}},
		{233, 234, false, BasicHandlerStep{SignalSet{0b1111000000}, getSignalMask(5),
				SignalInformation::Code::Generated, 5}},
		{235, 236, false, BasicHandlerStep{SignalSet{0b1111000000}, getSignalMask(6),
				SignalInformation::Code::Queued, 6, 0x6c9f1f2}},
		{237, 238, false, BasicHandlerStep{SignalSet{0b1110000000}, getSignalMask(6),
				SignalInformation::Code::Generated, 6}},
		{239, 240, false, BasicHandlerStep{SignalSet{0b1110000000}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x3e37ad5}},
		{241, 242, false, BasicHandlerStep{SignalSet{0b1100000000}, getSignalMask(7),
				SignalInformation::Code::Generated, 7}},
		{243, 244, false, BasicHandlerStep{SignalSet{0b1100000000}, getSignalMask(8),
				SignalInformation::Code::Queued, 8, 0x6b98cc3d}},
		{245, 246, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(8),
				SignalInformation::Code::Generated, 8}},
		{247, 248, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x299d82f6}},
		{249, 250, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Generated, 9}},
		// part 6 - unmasking of pending signals - one at a time
		{295, 296, false, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 2, 0x7abf0fb2}},
		{297, 298, false, BasicHandlerStep{SignalSet{0b1111111011}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 2}},
		{301, 302, false, BasicHandlerStep{SignalSet{0b1111111011}, SignalSet{(UINT32_MAX << 10) | 0b1111111011},
				SignalInformation::Code::Queued, 5, 0x4cf013f5}},
		{303, 304, false, BasicHandlerStep{SignalSet{0b1111011011}, SignalSet{(UINT32_MAX << 10) | 0b1111111011},
				SignalInformation::Code::Generated, 5}},
		{307, 308, false, BasicHandlerStep{SignalSet{0b1111011011}, SignalSet{(UINT32_MAX << 10) | 0b1111011011},
				SignalInformation::Code::Queued, 3, 0x399dbe15}},
		{309, 310, false, BasicHandlerStep{SignalSet{0b1111010011}, SignalSet{(UINT32_MAX << 10) | 0b1111011011},
				SignalInformation::Code::Generated, 3}},
		{313, 314, false, BasicHandlerStep{SignalSet{0b1111010011}, SignalSet{(UINT32_MAX << 10) | 0b1111010011},
				SignalInformation::Code::Queued, 8, 0x1c010698}},
		{315, 316, false, BasicHandlerStep{SignalSet{0b1011010011}, SignalSet{(UINT32_MAX << 10) | 0b1111010011},
				SignalInformation::Code::Generated, 8}},
		{319, 320, false, BasicHandlerStep{SignalSet{0b1011010011}, SignalSet{(UINT32_MAX << 10) | 0b1011010011},
				SignalInformation::Code::Queued, 7, 0x75143ba1}},
		{321, 322, false, BasicHandlerStep{SignalSet{0b1001010011}, SignalSet{(UINT32_MAX << 10) | 0b1011010011},
				SignalInformation::Code::Generated, 7}},
		{325, 326, false, BasicHandlerStep{SignalSet{0b1001010011}, SignalSet{(UINT32_MAX << 10) | 0b1001010011},
				SignalInformation::Code::Queued, 6, 0x587c305e}},
		{327, 328, false, BasicHandlerStep{SignalSet{0b1000010011}, SignalSet{(UINT32_MAX << 10) | 0b1001010011},
				SignalInformation::Code::Generated, 6}},
		{331, 332, false, BasicHandlerStep{SignalSet{0b1000010011}, SignalSet{(UINT32_MAX << 10) | 0b1000010011},
				SignalInformation::Code::Queued, 9, 0xd4dc81a}},
		{333, 334, false, BasicHandlerStep{SignalSet{0b0000010011}, SignalSet{(UINT32_MAX << 10) | 0b1000010011},
				SignalInformation::Code::Generated, 9}},
		{337, 338, false, BasicHandlerStep{SignalSet{0b0000010011}, SignalSet{(UINT32_MAX << 10) | 0b0000010011},
				SignalInformation::Code::Queued, 1, 0x8894ac1}},
		{339, 340, false, BasicHandlerStep{SignalSet{0b0000010001}, SignalSet{(UINT32_MAX << 10) | 0b0000010011},
				SignalInformation::Code::Generated, 1}},
		{343, 344, false, BasicHandlerStep{SignalSet{0b0000010001}, SignalSet{(UINT32_MAX << 10) | 0b0000010001},
				SignalInformation::Code::Queued, 4, 0x3f6fa768}},
		{345, 346, false, BasicHandlerStep{SignalSet{0b0000000001}, SignalSet{(UINT32_MAX << 10) | 0b0000010001},
				SignalInformation::Code::Generated, 4}},
		{349, 350, false, BasicHandlerStep{SignalSet{0b0000000001}, SignalSet{(UINT32_MAX << 10) | 0b0000000001},
				SignalInformation::Code::Queued, 0, 0x3d982f37}},
		{351, 352, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{(UINT32_MAX << 10) | 0b0000000001}, SignalInformation::Code::Generated, 0}},
		// part 7 - nested signal handlers - generate signal from within signal handler
		{355, 356, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0000000001},
				SignalInformation::Code::Generated, 0}},
		{357, 358, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{359, 430, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{360, 361, true, BasicHandlerStep{SignalSet{0b0000000001}, SignalSet{(UINT32_MAX << 10) | 0b0000000011},
				SignalInformation::Code::Generated, 1}},
		{362, 363, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{364, 427, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{365, 366, true, BasicHandlerStep{SignalSet{0b0000000011}, SignalSet{(UINT32_MAX << 10) | 0b0000000111},
				SignalInformation::Code::Generated, 2}},
		{367, 368, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{369, 424, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{370, 371, true, BasicHandlerStep{SignalSet{0b0000000111}, SignalSet{(UINT32_MAX << 10) | 0b0000001111},
				SignalInformation::Code::Generated, 3}},
		{372, 373, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{374, 421, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{375, 376, true, BasicHandlerStep{SignalSet{0b0000001111}, SignalSet{(UINT32_MAX << 10) | 0b0000011111},
				SignalInformation::Code::Generated, 4}},
		{377, 378, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{379, 418, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{380, 381, true, BasicHandlerStep{SignalSet{0b0000011111}, SignalSet{(UINT32_MAX << 10) | 0b0000111111},
				SignalInformation::Code::Generated, 5}},
		{382, 383, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{384, 415, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{385, 386, true, BasicHandlerStep{SignalSet{0b0000111111}, SignalSet{(UINT32_MAX << 10) | 0b0001111111},
				SignalInformation::Code::Generated, 6}},
		{387, 388, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{389, 412, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{390, 391, true, BasicHandlerStep{SignalSet{0b0001111111}, SignalSet{(UINT32_MAX << 10) | 0b0011111111},
				SignalInformation::Code::Generated, 7}},
		{392, 393, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{394, 409, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{395, 396, true, BasicHandlerStep{SignalSet{0b0011111111}, SignalSet{(UINT32_MAX << 10) | 0b0111111111},
				SignalInformation::Code::Generated, 8}},
		{397, 398, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{399, 406, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{400, 401, true, BasicHandlerStep{SignalSet{0b0111111111}, SignalSet{(UINT32_MAX << 10) | 0b1111111111},
				SignalInformation::Code::Generated, 9}},
		{402, 403, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{404, 405, false, BasicHandlerStep{SignalSet{0b0111111111}, SignalSet{(UINT32_MAX << 10) | 0b1111111111},
				SignalInformation::Code::Generated, 9}},
		{407, 408, false, BasicHandlerStep{SignalSet{0b0011111111}, SignalSet{(UINT32_MAX << 10) | 0b0111111111},
				SignalInformation::Code::Generated, 8}},
		{410, 411, false, BasicHandlerStep{SignalSet{0b0001111111}, SignalSet{(UINT32_MAX << 10) | 0b0011111111},
				SignalInformation::Code::Generated, 7}},
		{413, 414, false, BasicHandlerStep{SignalSet{0b0000111111}, SignalSet{(UINT32_MAX << 10) | 0b0001111111},
				SignalInformation::Code::Generated, 6}},
		{416, 417, false, BasicHandlerStep{SignalSet{0b0000011111}, SignalSet{(UINT32_MAX << 10) | 0b0000111111},
				SignalInformation::Code::Generated, 5}},
		{419, 420, false, BasicHandlerStep{SignalSet{0b0000001111}, SignalSet{(UINT32_MAX << 10) | 0b0000011111},
				SignalInformation::Code::Generated, 4}},
		{422, 423, false, BasicHandlerStep{SignalSet{0b0000000111}, SignalSet{(UINT32_MAX << 10) | 0b0000001111},
				SignalInformation::Code::Generated, 3}},
		{425, 426, false, BasicHandlerStep{SignalSet{0b0000000011}, SignalSet{(UINT32_MAX << 10) | 0b0000000111},
				SignalInformation::Code::Generated, 2}},
		{428, 429, false, BasicHandlerStep{SignalSet{0b0000000001}, SignalSet{(UINT32_MAX << 10) | 0b0000000011},
				SignalInformation::Code::Generated, 1}},
		{431, 432, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{(UINT32_MAX << 10) | 0b0000000001}, SignalInformation::Code::Generated, 0}},
		// part 8 - nested signal handlers - queue signal from within signal handler
		{435, 436, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0000000001},
				SignalInformation::Code::Queued, 0, 0x39a52149}},
		{437, 438, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x69eb4368}},
		{439, 510, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x712a84cd}},
		{440, 441, true, BasicHandlerStep{SignalSet{0b0000000001}, SignalSet{(UINT32_MAX << 10) | 0b0000000011},
				SignalInformation::Code::Queued, 1, 0x712a84cd}},
		{442, 443, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x59a40114}},
		{444, 507, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x0b3731c5}},
		{445, 446, true, BasicHandlerStep{SignalSet{0b0000000011}, SignalSet{(UINT32_MAX << 10) | 0b0000000111},
				SignalInformation::Code::Queued, 2, 0x0b3731c5}},
		{447, 448, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x1b265400}},
		{449, 504, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x5cd75dbe}},
		{450, 451, true, BasicHandlerStep{SignalSet{0b0000000111}, SignalSet{(UINT32_MAX << 10) | 0b0000001111},
				SignalInformation::Code::Queued, 3, 0x5cd75dbe}},
		{452, 453, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x58ad8ba0}},
		{454, 501, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x257ce662}},
		{455, 456, true, BasicHandlerStep{SignalSet{0b0000001111}, SignalSet{(UINT32_MAX << 10) | 0b0000011111},
				SignalInformation::Code::Queued, 4, 0x257ce662}},
		{457, 458, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x00d0f4f2}},
		{459, 498, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x360d7c66}},
		{460, 461, true, BasicHandlerStep{SignalSet{0b0000011111}, SignalSet{(UINT32_MAX << 10) | 0b0000111111},
				SignalInformation::Code::Queued, 5, 0x360d7c66}},
		{462, 463, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x20eb8156}},
		{464, 495, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x3df6dd36}},
		{465, 466, true, BasicHandlerStep{SignalSet{0b0000111111}, SignalSet{(UINT32_MAX << 10) | 0b0001111111},
				SignalInformation::Code::Queued, 6, 0x3df6dd36}},
		{467, 468, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x5f3de0fd}},
		{469, 492, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x7a478fc2}},
		{470, 471, true, BasicHandlerStep{SignalSet{0b0001111111}, SignalSet{(UINT32_MAX << 10) | 0b0011111111},
				SignalInformation::Code::Queued, 7, 0x7a478fc2}},
		{472, 473, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x687a1290}},
		{474, 489, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x5391b8a8}},
		{475, 476, true, BasicHandlerStep{SignalSet{0b0011111111}, SignalSet{(UINT32_MAX << 10) | 0b0111111111},
				SignalInformation::Code::Queued, 8, 0x5391b8a8}},
		{477, 478, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x0099d78f}},
		{479, 486, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x3deaad39}},
		{480, 481, true, BasicHandlerStep{SignalSet{0b0111111111}, SignalSet{(UINT32_MAX << 10) | 0b1111111111},
				SignalInformation::Code::Queued, 9, 0x3deaad39}},
		{482, 483, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x27a3d686}},
		{484, 485, false, BasicHandlerStep{SignalSet{0b0111111111}, SignalSet{(UINT32_MAX << 10) | 0b1111111111},
				SignalInformation::Code::Queued, 9, 0x27a3d686}},
		{487, 488, false, BasicHandlerStep{SignalSet{0b0011111111}, SignalSet{(UINT32_MAX << 10) | 0b0111111111},
				SignalInformation::Code::Queued, 8, 0x0099d78f}},
		{490, 491, false, BasicHandlerStep{SignalSet{0b0001111111}, SignalSet{(UINT32_MAX << 10) | 0b0011111111},
				SignalInformation::Code::Queued, 7, 0x687a1290}},
		{493, 494, false, BasicHandlerStep{SignalSet{0b0000111111}, SignalSet{(UINT32_MAX << 10) | 0b0001111111},
				SignalInformation::Code::Queued, 6, 0x5f3de0fd}},
		{496, 497, false, BasicHandlerStep{SignalSet{0b0000011111}, SignalSet{(UINT32_MAX << 10) | 0b0000111111},
				SignalInformation::Code::Queued, 5, 0x20eb8156}},
		{499, 500, false, BasicHandlerStep{SignalSet{0b0000001111}, SignalSet{(UINT32_MAX << 10) | 0b0000011111},
				SignalInformation::Code::Queued, 4, 0x00d0f4f2}},
		{502, 503, false, BasicHandlerStep{SignalSet{0b0000000111}, SignalSet{(UINT32_MAX << 10) | 0b0000001111},
				SignalInformation::Code::Queued, 3, 0x58ad8ba0}},
		{505, 506, false, BasicHandlerStep{SignalSet{0b0000000011}, SignalSet{(UINT32_MAX << 10) | 0b0000000111},
				SignalInformation::Code::Queued, 2, 0x1b265400}},
		{508, 509, false, BasicHandlerStep{SignalSet{0b0000000001}, SignalSet{(UINT32_MAX << 10) | 0b0000000011},
				SignalInformation::Code::Queued, 1, 0x59a40114}},
		{511, 512, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{(UINT32_MAX << 10) | 0b0000000001}, SignalInformation::Code::Queued, 0, 0x69eb4368}},
		// part 9 - nested signal handlers - unmasking of pending signals (one at a time) from within signal handler
		{557, 558, true, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 4, 0x248f49de}},
		{559, 612, false, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1101111111}}},
		{560, 561, true, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 7, 0x0a8b8d2c}},
		{562, 609, false, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111111011}}},
		{563, 564, true, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 2, 0x2e02ef41}},
		{565, 606, false, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111110111}}},
		{566, 567, true, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 3, 0x116dd593}},
		{568, 603, false, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1011111111}}},
		{569, 570, true, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 8, 0x49ad2898}},
		{571, 600, false, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111111101}}},
		{572, 573, true, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 1, 0x4cd69c9e}},
		{574, 597, false, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111011111}}},
		{575, 576, true, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 5, 0x1e14b4d4}},
		{577, 594, false, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111111110}}},
		{578, 579, true, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 0, 0x5d8d88c9}},
		{580, 591, false, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b0111111111}}},
		{581, 582, true, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 9, 0x579cdee3}},
		{583, 588, false, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1110111111}}},
		{584, 585, false, BasicHandlerStep{SignalSet{0b1111111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Queued, 6, 0x406f37db}},
		{586, 587, false, BasicHandlerStep{SignalSet{0b1110111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 6}},
		{589, 590, false, BasicHandlerStep{SignalSet{0b0110111111}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 9}},
		{592, 593, false, BasicHandlerStep{SignalSet{0b0110111110}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 0}},
		{595, 596, false, BasicHandlerStep{SignalSet{0b0110011110}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 5}},
		{598, 599, false, BasicHandlerStep{SignalSet{0b0110011100}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 1}},
		{601, 602, false, BasicHandlerStep{SignalSet{0b0010011100}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 8}},
		{604, 605, false, BasicHandlerStep{SignalSet{0b0010010100}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 3}},
		{607, 608, false, BasicHandlerStep{SignalSet{0b0010010000}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 2}},
		{610, 611, false, BasicHandlerStep{SignalSet{0b0000010000}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 7}},
		{613, 614, false, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{SignalSet::full},
				SignalInformation::Code::Generated, 4}},
};

/// test steps for second phase, for main test thread
const TestStep phase2MainTestThreadSteps[]
{
		// part 1 - normal generated signals
		{0, 5, true, SoftwareTimerStep{testStepsRunner}},
		{1, 2, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{6, 11, true, SoftwareTimerStep{testStepsRunner}},
		{7, 8, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{12, 17, true, SoftwareTimerStep{testStepsRunner}},
		{13, 14, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{18, 23, true, SoftwareTimerStep{testStepsRunner}},
		{19, 20, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{24, 29, true, SoftwareTimerStep{testStepsRunner}},
		{25, 26, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{30, 35, true, SoftwareTimerStep{testStepsRunner}},
		{31, 32, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{36, 41, true, SoftwareTimerStep{testStepsRunner}},
		{37, 38, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{42, 47, true, SoftwareTimerStep{testStepsRunner}},
		{43, 44, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{48, 53, true, SoftwareTimerStep{testStepsRunner}},
		{49, 50, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{54, 59, true, SoftwareTimerStep{testStepsRunner}},
		{55, 56, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		// part 2 - normal queued signals
		{60, 65, true, SoftwareTimerStep{testStepsRunner}},
		{61, 62, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x7bbee7c7}},
		{66, 71, true, SoftwareTimerStep{testStepsRunner}},
		{67, 68, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x144006e1}},
		{72, 77, true, SoftwareTimerStep{testStepsRunner}},
		{73, 74, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x3c2f74c3}},
		{78, 83, true, SoftwareTimerStep{testStepsRunner}},
		{79, 80, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x6842c269}},
		{84, 89, true, SoftwareTimerStep{testStepsRunner}},
		{85, 86, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x282ea423}},
		{90, 95, true, SoftwareTimerStep{testStepsRunner}},
		{91, 92, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x4ac997a6}},
		{96, 101, true, SoftwareTimerStep{testStepsRunner}},
		{97, 98, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x60375e15}},
		{102, 107, true, SoftwareTimerStep{testStepsRunner}},
		{103, 104, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x5025b208}},
		{108, 113, true, SoftwareTimerStep{testStepsRunner}},
		{109, 110, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x0ff3e7c1}},
		{114, 119, true, SoftwareTimerStep{testStepsRunner}},
		{115, 116, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x6fc765af}},
		// part 3 - multiple generated signals
		{120, 161, true, SoftwareTimerStep{testStepsRunner}},
		{121, 122, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{123, 124, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{125, 126, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{127, 128, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{129, 130, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{131, 132, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{133, 134, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{135, 136, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{137, 138, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{139, 140, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		// part 4 - multiple queued signals
		{162, 203, true, SoftwareTimerStep{testStepsRunner}},
		{163, 164, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x7230a2cc}},
		{165, 166, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x183c1811}},
		{167, 168, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x4be507b0}},
		{169, 170, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x249cd4e7}},
		{171, 172, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x44e1258a}},
		{173, 174, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x4ea5f291}},
		{175, 176, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x2a31564c}},
		{177, 178, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x7a15bc72}},
		{179, 180, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x7739addf}},
		{181, 182, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x66583e56}},
		// part 5 - multiple signals
		{204, 285, true, SoftwareTimerStep{testStepsRunner}},
		{205, 206, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{207, 208, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{209, 210, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{211, 212, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{213, 214, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{215, 216, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{217, 218, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{219, 220, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{221, 222, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{223, 224, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{225, 226, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x3e9c82fe}},
		{227, 228, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x011c51eb}},
		{229, 230, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x7eb45b48}},
		{231, 232, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x6a4f49c6}},
		{233, 234, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x2961cf0c}},
		{235, 236, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x4eac6e95}},
		{237, 238, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x0ac99377}},
		{239, 240, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x7fd0446b}},
		{241, 242, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x6df7494e}},
		{243, 244, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x3ce8f20c}},
		// part 6 - nested signal handlers - generate signal from interrupt while signal handler is still running
		{286, 287, true, SignalMaskStep{SignalSet{UINT32_MAX << 10}}},
		{288, 347, true, SoftwareTimerStep{testStepsRunner}},
		{289, 290, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		// part 7 - nested signal handlers - queue signal from interrupt while signal handler is still running
		{348, 407, true, SoftwareTimerStep{testStepsRunner}},
		{349, 350, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x388c64c3}},
		{408, 409, false, SignalMaskStep{SignalSet{SignalSet::empty}}},
};

/// test steps for second phase, for signal handler
const TestStep phase2SignalHandlerSteps[]
{
		// part 1 - normal generated signals
		{3, 4, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::Generated, 0}},
		{9, 10, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::Generated, 1}},
		{15, 16, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::Generated, 2}},
		{21, 22, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::Generated, 3}},
		{27, 28, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::Generated, 4}},
		{33, 34, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::Generated, 5}},
		{39, 40, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::Generated, 6}},
		{45, 46, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::Generated, 7}},
		{51, 52, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(8),
				SignalInformation::Code::Generated, 8}},
		{57, 58, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Generated, 9}},
		// part 2 - normal queued signals
		{63, 64, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::Queued, 0, 0x7bbee7c7}},
		{69, 70, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::Queued, 1, 0x144006e1}},
		{75, 76, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::Queued, 2, 0x3c2f74c3}},
		{81, 82, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x6842c269}},
		{87, 88, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x282ea423}},
		{93, 94, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::Queued, 5, 0x4ac997a6}},
		{99, 100, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::Queued, 6, 0x60375e15}},
		{105, 106, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x5025b208}},
		{111, 112, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(8),
				SignalInformation::Code::Queued, 8, 0x0ff3e7c1}},
		{117, 118, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x6fc765af}},
		// part 3 - multiple generated signals
		{141, 142, false, BasicHandlerStep{SignalSet{0b1111111110}, getSignalMask(0),
				SignalInformation::Code::Generated, 0}},
		{143, 144, false, BasicHandlerStep{SignalSet{0b1111111100}, getSignalMask(1),
				SignalInformation::Code::Generated, 1}},
		{145, 146, false, BasicHandlerStep{SignalSet{0b1111111000}, getSignalMask(2),
				SignalInformation::Code::Generated, 2}},
		{147, 148, false, BasicHandlerStep{SignalSet{0b1111110000}, getSignalMask(3),
				SignalInformation::Code::Generated, 3}},
		{149, 150, false, BasicHandlerStep{SignalSet{0b1111100000}, getSignalMask(4),
				SignalInformation::Code::Generated, 4}},
		{151, 152, false, BasicHandlerStep{SignalSet{0b1111000000}, getSignalMask(5),
				SignalInformation::Code::Generated, 5}},
		{153, 154, false, BasicHandlerStep{SignalSet{0b1110000000}, getSignalMask(6),
				SignalInformation::Code::Generated, 6}},
		{155, 156, false, BasicHandlerStep{SignalSet{0b1100000000}, getSignalMask(7),
				SignalInformation::Code::Generated, 7}},
		{157, 158, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(8),
				SignalInformation::Code::Generated, 8}},
		{159, 160, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Generated, 9}},
		// part 4 - multiple queued signals
		{183, 184, false, BasicHandlerStep{SignalSet{0b1000001000}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x183c1811}},
		{185, 186, false, BasicHandlerStep{SignalSet{0b1000001000}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x249cd4e7}},
		{187, 188, false, BasicHandlerStep{SignalSet{0b1000001000}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x4ea5f291}},
		{189, 190, false, BasicHandlerStep{SignalSet{0b1000001000}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x7a15bc72}},
		{191, 192, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x66583e56}},
		{193, 194, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x7230a2cc}},
		{195, 196, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x4be507b0}},
		{197, 198, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x44e1258a}},
		{199, 200, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x2a31564c}},
		{201, 202, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x7739addf}},
		// part 5 - multiple signals
		{245, 246, false, BasicHandlerStep{SignalSet{0b1111111111}, getSignalMask(0),
				SignalInformation::Code::Queued, 0, 0x3ce8f20c}},
		{247, 248, false, BasicHandlerStep{SignalSet{0b1111111110}, getSignalMask(0),
				SignalInformation::Code::Generated, 0}},
		{249, 250, false, BasicHandlerStep{SignalSet{0b1111111110}, getSignalMask(1),
				SignalInformation::Code::Queued, 1, 0x2961cf0c}},
		{251, 252, false, BasicHandlerStep{SignalSet{0b1111111100}, getSignalMask(1),
				SignalInformation::Code::Generated, 1}},
		{253, 254, false, BasicHandlerStep{SignalSet{0b1111111100}, getSignalMask(2),
				SignalInformation::Code::Queued, 2, 0x0ac99377}},
		{255, 256, false, BasicHandlerStep{SignalSet{0b1111111000}, getSignalMask(2),
				SignalInformation::Code::Generated, 2}},
		{257, 258, false, BasicHandlerStep{SignalSet{0b1111111000}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x6a4f49c6}},
		{259, 260, false, BasicHandlerStep{SignalSet{0b1111110000}, getSignalMask(3),
				SignalInformation::Code::Generated, 3}},
		{261, 262, false, BasicHandlerStep{SignalSet{0b1111110000}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x3e9c82fe}},
		{263, 264, false, BasicHandlerStep{SignalSet{0b1111100000}, getSignalMask(4),
				SignalInformation::Code::Generated, 4}},
		{265, 266, false, BasicHandlerStep{SignalSet{0b1111100000}, getSignalMask(5),
				SignalInformation::Code::Queued, 5, 0x7eb45b48}},
		{267, 268, false, BasicHandlerStep{SignalSet{0b1111000000}, getSignalMask(5),
				SignalInformation::Code::Generated, 5}},
		{269, 270, false, BasicHandlerStep{SignalSet{0b1111000000}, getSignalMask(6),
				SignalInformation::Code::Queued, 6, 0x011c51eb}},
		{271, 272, false, BasicHandlerStep{SignalSet{0b1110000000}, getSignalMask(6),
				SignalInformation::Code::Generated, 6}},
		{273, 274, false, BasicHandlerStep{SignalSet{0b1110000000}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x6df7494e}},
		{275, 276, false, BasicHandlerStep{SignalSet{0b1100000000}, getSignalMask(7),
				SignalInformation::Code::Generated, 7}},
		{277, 278, false, BasicHandlerStep{SignalSet{0b1100000000}, getSignalMask(8),
				SignalInformation::Code::Queued, 8, 0x4eac6e95}},
		{279, 280, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(8),
				SignalInformation::Code::Generated, 8}},
		{281, 282, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x7fd0446b}},
		{283, 284, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Generated, 9}},
		// part 6 - nested signal handlers - generate signal from interrupt while signal handler is still running
		{291, 292, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0000100000},
				SignalInformation::Code::Generated, 5}},
		{293, 346, false, SoftwareTimerStep{testStepsRunner}},
		{294, 295, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{296, 297, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0000100100},
				SignalInformation::Code::Generated, 2}},
		{298, 345, false, SoftwareTimerStep{testStepsRunner}},
		{299, 300, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{301, 302, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0000100110},
				SignalInformation::Code::Generated, 1}},
		{303, 344, false, SoftwareTimerStep{testStepsRunner}},
		{304, 305, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{306, 307, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0010100110},
				SignalInformation::Code::Generated, 7}},
		{308, 343, false, SoftwareTimerStep{testStepsRunner}},
		{309, 310, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{311, 312, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0011100110},
				SignalInformation::Code::Generated, 6}},
		{313, 342, false, SoftwareTimerStep{testStepsRunner}},
		{314, 315, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{316, 317, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011100110},
				SignalInformation::Code::Generated, 9}},
		{318, 341, false, SoftwareTimerStep{testStepsRunner}},
		{319, 320, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{321, 322, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011101110},
				SignalInformation::Code::Generated, 3}},
		{323, 340, false, SoftwareTimerStep{testStepsRunner}},
		{324, 325, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{326, 327, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011101111},
				SignalInformation::Code::Generated, 0}},
		{328, 339, false, SoftwareTimerStep{testStepsRunner}},
		{329, 330, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{331, 332, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011111111},
				SignalInformation::Code::Generated, 4}},
		{333, 338, false, SoftwareTimerStep{testStepsRunner}},
		{334, 335, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{336, 337, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{(UINT32_MAX << 10) | 0b1111111111}, SignalInformation::Code::Generated, 8}},
		// part 7 - nested signal handlers - queue signal from interrupt while signal handler is still running
		{351, 352, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0001000000},
				SignalInformation::Code::Queued, 6, 0x388c64c3}},
		{353, 406, false, SoftwareTimerStep{testStepsRunner}},
		{354, 355, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x49020a59}},
		{356, 357, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0001001000},
				SignalInformation::Code::Queued, 3, 0x49020a59}},
		{358, 405, false, SoftwareTimerStep{testStepsRunner}},
		{359, 360, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x7e8842b9}},
		{361, 362, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0001001001},
				SignalInformation::Code::Queued, 0, 0x7e8842b9}},
		{363, 404, false, SoftwareTimerStep{testStepsRunner}},
		{364, 365, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x19ccf80b}},
		{366, 367, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0011001001},
				SignalInformation::Code::Queued, 7, 0x19ccf80b}},
		{368, 403, false, SoftwareTimerStep{testStepsRunner}},
		{369, 370, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x207e6915}},
		{371, 372, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011001001},
				SignalInformation::Code::Queued, 9, 0x207e6915}},
		{373, 402, false, SoftwareTimerStep{testStepsRunner}},
		{374, 375, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x68e46a0f}},
		{376, 377, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011011001},
				SignalInformation::Code::Queued, 4, 0x68e46a0f}},
		{378, 401, false, SoftwareTimerStep{testStepsRunner}},
		{379, 380, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x3bf7a491}},
		{381, 382, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011011101},
				SignalInformation::Code::Queued, 2, 0x3bf7a491}},
		{383, 400, false, SoftwareTimerStep{testStepsRunner}},
		{384, 385, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x5ff85ef6}},
		{386, 387, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011111101},
				SignalInformation::Code::Queued, 5, 0x5ff85ef6}},
		{388, 399, false, SoftwareTimerStep{testStepsRunner}},
		{389, 390, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x7a66bbc6}},
		{391, 392, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111111101},
				SignalInformation::Code::Queued, 8, 0x7a66bbc6}},
		{393, 398, false, SoftwareTimerStep{testStepsRunner}},
		{394, 395, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x4f8adea0}},
		{396, 397, false, BasicHandlerStep{SignalSet{SignalSet::empty},
				SignalSet{(UINT32_MAX << 10) | 0b1111111111}, SignalInformation::Code::Queued, 1, 0x4f8adea0}},
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
		{63, 70, true, ThreadPriorityStep{lowPriority}},
		{71, 78, true, ThreadPriorityStep{lowPriority}},
		// part 2 - normal queued signals
		{79, 86, true, ThreadPriorityStep{lowPriority}},
		{87, 94, true, ThreadPriorityStep{lowPriority}},
		{95, 102, true, ThreadPriorityStep{lowPriority}},
		{103, 110, true, ThreadPriorityStep{lowPriority}},
		{111, 118, true, ThreadPriorityStep{lowPriority}},
		{119, 126, true, ThreadPriorityStep{lowPriority}},
		{127, 134, true, ThreadPriorityStep{lowPriority}},
		{135, 142, true, ThreadPriorityStep{lowPriority}},
		{143, 150, true, ThreadPriorityStep{lowPriority}},
		{151, 158, true, ThreadPriorityStep{lowPriority}},
		// part 3 - multiple generated signals
		{159, 202, true, ThreadPriorityStep{lowPriority}},
		// part 4 - multiple queued signals
		{203, 246, true, ThreadPriorityStep{lowPriority}},
		// part 5 - multiple signals
		{247, 330, true, ThreadPriorityStep{lowPriority}},
		// part 6 - nested signal handlers - generate signal from second test thread while signal handler is still
		// running
		{331, 332, true, SignalMaskStep{SignalSet{UINT32_MAX << 10}}},
		{333, 412, true, ThreadPriorityStep{lowPriority}},
		// part 7 - nested signal handlers - queue signal from second test thread while signal handler is still running
		{413, 492, true, ThreadPriorityStep{lowPriority}},
		{493, 494, false, SignalMaskStep{SignalSet{SignalSet::empty}}},
};

/// test steps for third phase, for signal handler
const TestStep phase3SignalHandlerSteps[]
{
		// part 1 - normal generated signals
		{4, 5, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::Generated, 0}},
		{12, 13, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::Generated, 1}},
		{20, 21, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::Generated, 2}},
		{28, 29, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::Generated, 3}},
		{36, 37, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::Generated, 4}},
		{44, 45, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::Generated, 5}},
		{52, 53, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::Generated, 6}},
		{60, 61, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::Generated, 7}},
		{68, 69, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(8),
				SignalInformation::Code::Generated, 8}},
		{76, 77, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Generated, 9}},
		// part 2 - normal queued signals
		{84, 85, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(0),
				SignalInformation::Code::Queued, 0, 0x1d804a34}},
		{92, 93, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(1),
				SignalInformation::Code::Queued, 1, 0x27bb29af}},
		{100, 101, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(2),
				SignalInformation::Code::Queued, 2, 0x7006f7ce}},
		{108, 109, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x10f09338}},
		{116, 117, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x3d657640}},
		{124, 125, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(5),
				SignalInformation::Code::Queued, 5, 0x2ebf9a5f}},
		{132, 133, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(6),
				SignalInformation::Code::Queued, 6, 0x1b914049}},
		{140, 141, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x1989b75c}},
		{148, 149, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(8),
				SignalInformation::Code::Queued, 8, 0x6ab930be}},
		{156, 157, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x7cbf056b}},
		// part 3 - multiple generated signals
		{182, 183, false, BasicHandlerStep{SignalSet{0b1111111110}, getSignalMask(0),
				SignalInformation::Code::Generated, 0}},
		{184, 185, false, BasicHandlerStep{SignalSet{0b1111111100}, getSignalMask(1),
				SignalInformation::Code::Generated, 1}},
		{186, 187, false, BasicHandlerStep{SignalSet{0b1111111000}, getSignalMask(2),
				SignalInformation::Code::Generated, 2}},
		{188, 189, false, BasicHandlerStep{SignalSet{0b1111110000}, getSignalMask(3),
				SignalInformation::Code::Generated, 3}},
		{190, 191, false, BasicHandlerStep{SignalSet{0b1111100000}, getSignalMask(4),
				SignalInformation::Code::Generated, 4}},
		{192, 193, false, BasicHandlerStep{SignalSet{0b1111000000}, getSignalMask(5),
				SignalInformation::Code::Generated, 5}},
		{194, 195, false, BasicHandlerStep{SignalSet{0b1110000000}, getSignalMask(6),
				SignalInformation::Code::Generated, 6}},
		{196, 197, false, BasicHandlerStep{SignalSet{0b1100000000}, getSignalMask(7),
				SignalInformation::Code::Generated, 7}},
		{198, 199, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(8),
				SignalInformation::Code::Generated, 8}},
		{200, 201, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Generated, 9}},
		// part 4 - multiple queued signals
		{226, 227, false, BasicHandlerStep{SignalSet{0b0010010000}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x4ab84596}},
		{228, 229, false, BasicHandlerStep{SignalSet{0b0010010000}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x59e3f309}},
		{230, 231, false, BasicHandlerStep{SignalSet{0b0010010000}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x1ab5ff9e}},
		{232, 233, false, BasicHandlerStep{SignalSet{0b0010010000}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x7ca05af6}},
		{234, 235, false, BasicHandlerStep{SignalSet{0b0010000000}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x1e09b682}},
		{236, 237, false, BasicHandlerStep{SignalSet{0b0010000000}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x29bd9d85}},
		{238, 239, false, BasicHandlerStep{SignalSet{0b0010000000}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x4fa58f4f}},
		{240, 241, false, BasicHandlerStep{SignalSet{0b0010000000}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x48b7be56}},
		{242, 243, false, BasicHandlerStep{SignalSet{0b0010000000}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x0af1d37f}},
		{244, 245, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x6f8dc16b}},
		// part 5 - multiple signals
		{290, 291, false, BasicHandlerStep{SignalSet{0b1111111111}, getSignalMask(0),
				SignalInformation::Code::Queued, 0, 0x37921ff7}},
		{292, 293, false, BasicHandlerStep{SignalSet{0b1111111110}, getSignalMask(0),
				SignalInformation::Code::Generated, 0}},
		{294, 295, false, BasicHandlerStep{SignalSet{0b1111111110}, getSignalMask(1),
				SignalInformation::Code::Queued, 1, 0x2a7fc8fc}},
		{296, 297, false, BasicHandlerStep{SignalSet{0b1111111100}, getSignalMask(1),
				SignalInformation::Code::Generated, 1}},
		{298, 299, false, BasicHandlerStep{SignalSet{0b1111111100}, getSignalMask(2),
				SignalInformation::Code::Queued, 2, 0x4fab0990}},
		{300, 301, false, BasicHandlerStep{SignalSet{0b1111111000}, getSignalMask(2),
				SignalInformation::Code::Generated, 2}},
		{302, 303, false, BasicHandlerStep{SignalSet{0b1111111000}, getSignalMask(3),
				SignalInformation::Code::Queued, 3, 0x2bc11d59}},
		{304, 305, false, BasicHandlerStep{SignalSet{0b1111110000}, getSignalMask(3),
				SignalInformation::Code::Generated, 3}},
		{306, 307, false, BasicHandlerStep{SignalSet{0b1111110000}, getSignalMask(4),
				SignalInformation::Code::Queued, 4, 0x6fd4db4a}},
		{308, 309, false, BasicHandlerStep{SignalSet{0b1111100000}, getSignalMask(4),
				SignalInformation::Code::Generated, 4}},
		{310, 311, false, BasicHandlerStep{SignalSet{0b1111100000}, getSignalMask(5),
				SignalInformation::Code::Queued, 5, 0x37f2577b}},
		{312, 313, false, BasicHandlerStep{SignalSet{0b1111000000}, getSignalMask(5),
				SignalInformation::Code::Generated, 5}},
		{314, 315, false, BasicHandlerStep{SignalSet{0b1111000000}, getSignalMask(6),
				SignalInformation::Code::Queued, 6, 0x01c636ac}},
		{316, 317, false, BasicHandlerStep{SignalSet{0b1110000000}, getSignalMask(6),
				SignalInformation::Code::Generated, 6}},
		{318, 319, false, BasicHandlerStep{SignalSet{0b1110000000}, getSignalMask(7),
				SignalInformation::Code::Queued, 7, 0x6bd662a6}},
		{320, 321, false, BasicHandlerStep{SignalSet{0b1100000000}, getSignalMask(7),
				SignalInformation::Code::Generated, 7}},
		{322, 323, false, BasicHandlerStep{SignalSet{0b1100000000}, getSignalMask(8),
				SignalInformation::Code::Queued, 8, 0x0e04dff2}},
		{324, 325, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(8),
				SignalInformation::Code::Generated, 8}},
		{326, 327, false, BasicHandlerStep{SignalSet{0b1000000000}, getSignalMask(9),
				SignalInformation::Code::Queued, 9, 0x741f5e69}},
		{328, 329, false, BasicHandlerStep{SignalSet{SignalSet::empty}, getSignalMask(9),
				SignalInformation::Code::Generated, 9}},
		// part 6 - nested signal handlers - generate signal from second test thread while signal handler is still
		// running
		{338, 339, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0010000000},
				SignalInformation::Code::Generated, 7}},
		{340, 411, false, ThreadPriorityStep{lowPriority}},
		{345, 346, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0110000000},
				SignalInformation::Code::Generated, 8}},
		{347, 410, false, ThreadPriorityStep{lowPriority}},
		{352, 353, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1110000000},
				SignalInformation::Code::Generated, 9}},
		{354, 409, false, ThreadPriorityStep{lowPriority}},
		{359, 360, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111000000},
				SignalInformation::Code::Generated, 6}},
		{361, 408, false, ThreadPriorityStep{lowPriority}},
		{366, 367, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111000100},
				SignalInformation::Code::Generated, 2}},
		{368, 407, false, ThreadPriorityStep{lowPriority}},
		{373, 374, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111000110},
				SignalInformation::Code::Generated, 1}},
		{375, 406, false, ThreadPriorityStep{lowPriority}},
		{380, 381, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111000111},
				SignalInformation::Code::Generated, 0}},
		{382, 405, false, ThreadPriorityStep{lowPriority}},
		{387, 388, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111100111},
				SignalInformation::Code::Generated, 5}},
		{389, 404, false, ThreadPriorityStep{lowPriority}},
		{394, 395, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111101111},
				SignalInformation::Code::Generated, 3}},
		{396, 403, false, ThreadPriorityStep{lowPriority}},
		{401, 402, false, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111111111},
				SignalInformation::Code::Generated, 4}},
		// part 7 - nested signal handlers - queue signal from second test thread while signal handler is still running
		{418, 419, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0000000010},
				SignalInformation::Code::Queued, 1, 0x4d937c16}},
		{420, 491, false, ThreadPriorityStep{lowPriority}},
		{425, 426, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0001000010},
				SignalInformation::Code::Queued, 6, 0x1fb6d90e}},
		{427, 490, false, ThreadPriorityStep{lowPriority}},
		{432, 433, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0001000011},
				SignalInformation::Code::Queued, 0, 0x666795c4}},
		{434, 489, false, ThreadPriorityStep{lowPriority}},
		{439, 440, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1001000011},
				SignalInformation::Code::Queued, 9, 0x6e9a50d2}},
		{441, 488, false, ThreadPriorityStep{lowPriority}},
		{446, 447, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1001100011},
				SignalInformation::Code::Queued, 5, 0x6637a3ac}},
		{448, 487, false, ThreadPriorityStep{lowPriority}},
		{453, 454, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1001101011},
				SignalInformation::Code::Queued, 3, 0x7d9b8bf5}},
		{455, 486, false, ThreadPriorityStep{lowPriority}},
		{460, 461, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1001101111},
				SignalInformation::Code::Queued, 2, 0x6e2bb980}},
		{462, 485, false, ThreadPriorityStep{lowPriority}},
		{467, 468, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1101101111},
				SignalInformation::Code::Queued, 8, 0x5dceaad0}},
		{469, 484, false, ThreadPriorityStep{lowPriority}},
		{474, 475, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111101111},
				SignalInformation::Code::Queued, 7, 0x2b680b53}},
		{476, 483, false, ThreadPriorityStep{lowPriority}},
		{481, 482, false, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111111111},
				SignalInformation::Code::Queued, 4, 0x63897fe9}},
};

/// test steps for third phase, for second test thread
const TestStep phase3SecondTestThreadStepsRange[]
{
		// part 1 - normal generated signals
		{1, 2, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{3, 8, true, ThreadPriorityStep{mainTestThreadPriority}},
		{9, 10, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{11, 16, true, ThreadPriorityStep{mainTestThreadPriority}},
		{17, 18, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{19, 24, true, ThreadPriorityStep{mainTestThreadPriority}},
		{25, 26, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{27, 32, true, ThreadPriorityStep{mainTestThreadPriority}},
		{33, 34, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{35, 40, true, ThreadPriorityStep{mainTestThreadPriority}},
		{41, 42, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{43, 48, true, ThreadPriorityStep{mainTestThreadPriority}},
		{49, 50, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{51, 56, true, ThreadPriorityStep{mainTestThreadPriority}},
		{57, 58, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{59, 64, true, ThreadPriorityStep{mainTestThreadPriority}},
		{65, 66, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{67, 72, true, ThreadPriorityStep{mainTestThreadPriority}},
		{73, 74, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{75, 80, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 2 - normal queued signals
		{81, 82, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x1d804a34}},
		{83, 88, true, ThreadPriorityStep{mainTestThreadPriority}},
		{89, 90, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x27bb29af}},
		{91, 96, true, ThreadPriorityStep{mainTestThreadPriority}},
		{97, 98, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x7006f7ce}},
		{99, 104, true, ThreadPriorityStep{mainTestThreadPriority}},
		{105, 106, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x10f09338}},
		{107, 112, true, ThreadPriorityStep{mainTestThreadPriority}},
		{113, 114, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x3d657640}},
		{115, 120, true, ThreadPriorityStep{mainTestThreadPriority}},
		{121, 122, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x2ebf9a5f}},
		{123, 128, true, ThreadPriorityStep{mainTestThreadPriority}},
		{129, 130, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x1b914049}},
		{131, 136, true, ThreadPriorityStep{mainTestThreadPriority}},
		{137, 138, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x1989b75c}},
		{139, 144, true, ThreadPriorityStep{mainTestThreadPriority}},
		{145, 146, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x6ab930be}},
		{147, 152, true, ThreadPriorityStep{mainTestThreadPriority}},
		{153, 154, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x7cbf056b}},
		{155, 160, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 3 - multiple generated signals
		{161, 162, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{163, 164, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{165, 166, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{167, 168, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{169, 170, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{171, 172, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{173, 174, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{175, 176, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{177, 178, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{179, 180, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{181, 204, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 4 - multiple queued signals
		{205, 206, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x29bd9d85}},
		{207, 208, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x4ab84596}},
		{209, 210, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x4fa58f4f}},
		{211, 212, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x59e3f309}},
		{213, 214, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x48b7be56}},
		{215, 216, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x1ab5ff9e}},
		{217, 218, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x0af1d37f}},
		{219, 220, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x7ca05af6}},
		{221, 222, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x6f8dc16b}},
		{223, 224, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x1e09b682}},
		{225, 248, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 5 - multiple signals
		{249, 250, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{251, 252, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{253, 254, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{255, 256, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{257, 258, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{259, 260, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{261, 262, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{263, 264, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{265, 266, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{267, 268, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{269, 270, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x741f5e69}},
		{271, 272, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x37f2577b}},
		{273, 274, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x2bc11d59}},
		{275, 276, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x37921ff7}},
		{277, 278, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x0e04dff2}},
		{279, 280, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x6bd662a6}},
		{281, 282, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x4fab0990}},
		{283, 284, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x2a7fc8fc}},
		{285, 286, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x6fd4db4a}},
		{287, 288, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x01c636ac}},
		{289, 334, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 6 - nested signal handlers - generate signal from second test thread while signal handler is still
		// running
		{335, 336, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
		{337, 341, true, ThreadPriorityStep{mainTestThreadPriority}},
		{342, 343, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
		{344, 348, true, ThreadPriorityStep{mainTestThreadPriority}},
		{349, 350, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
		{351, 355, true, ThreadPriorityStep{mainTestThreadPriority}},
		{356, 357, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
		{358, 362, true, ThreadPriorityStep{mainTestThreadPriority}},
		{363, 364, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
		{365, 369, true, ThreadPriorityStep{mainTestThreadPriority}},
		{370, 371, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
		{372, 376, true, ThreadPriorityStep{mainTestThreadPriority}},
		{377, 378, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
		{379, 383, true, ThreadPriorityStep{mainTestThreadPriority}},
		{384, 385, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
		{386, 390, true, ThreadPriorityStep{mainTestThreadPriority}},
		{391, 392, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
		{393, 397, true, ThreadPriorityStep{mainTestThreadPriority}},
		{398, 399, true, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
		{400, 414, true, ThreadPriorityStep{mainTestThreadPriority}},
		// part 7 - nested signal handlers - queue signal from second test thread while signal handler is still running
		{415, 416, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x4d937c16}},
		{417, 421, true, ThreadPriorityStep{mainTestThreadPriority}},
		{422, 423, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x1fb6d90e}},
		{424, 428, true, ThreadPriorityStep{mainTestThreadPriority}},
		{429, 430, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x666795c4}},
		{431, 435, true, ThreadPriorityStep{mainTestThreadPriority}},
		{436, 437, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x6e9a50d2}},
		{438, 442, true, ThreadPriorityStep{mainTestThreadPriority}},
		{443, 444, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x6637a3ac}},
		{445, 449, true, ThreadPriorityStep{mainTestThreadPriority}},
		{450, 451, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x7d9b8bf5}},
		{452, 456, true, ThreadPriorityStep{mainTestThreadPriority}},
		{457, 458, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x6e2bb980}},
		{459, 463, true, ThreadPriorityStep{mainTestThreadPriority}},
		{464, 465, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x5dceaad0}},
		{466, 470, true, ThreadPriorityStep{mainTestThreadPriority}},
		{471, 472, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x2b680b53}},
		{473, 477, true, ThreadPriorityStep{mainTestThreadPriority}},
		{478, 479, true, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x63897fe9}},
		{480, 495, false, ThreadPriorityStep{mainTestThreadPriority}},
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
				TestStepsRange{phase3SecondTestThreadStepsRange}, 43 * 2 + 2},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsCatchingTestCase::run_() const
{
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

		auto secondThread = makeStaticThread<secondTestThreadStackSize>(secondTestThreadPriority, testStepsRunner,
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

	return true;
}

}	// namespace test

}	// namespace distortos
