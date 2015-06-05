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
 * \date 2015-06-05
 */

#include "SignalsCatchingTestCase.hpp"

#include "SequenceAsserter.hpp"

#include "distortos/SignalAction.hpp"
#include "distortos/SoftwareTimer.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread-Signals.hpp"

#include "distortos/estd/ContiguousRange.hpp"

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
	 * \return 0 on success, error code otherwise
	 */

	int operator()() const;

private:

	/// value that will be queued with the signal, relevant only if \a code_ is SignalInformation::Code::Generated
	int value_;

	/// selects whether the signal will be generated (SignalInformation::Code::Generated) or queued
	/// (SignalInformation::Code::Queued)
	SignalInformation::Code code_;

	/// signal number that will be generated or queued
	uint8_t signalNumber_;
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

/// test step that starts software timer and waits until it stops
class SoftwareTimerStep
{
public:

	/**
	 * \brief SoftwareTimerStep's constructor
	 */

	constexpr SoftwareTimerStep()
	{

	}

	/**
	 * \brief SoftwareTimerStep's function call operator
	 *
	 * Starts \a softwareTimer and waits until it stops.
	 *
	 * \param [in] softwareTimer is a reference to software timer
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(SoftwareTimerBase& softwareTimer) const;
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
	 * \param [in] softwareTimer is a pointer to software timer, required only for SoftwareTimerStep
	 * \param [in] signalInformation is a pointer to received SignalInformation object, required only for
	 * BasicHandlerStep
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(SequenceAsserter& sequenceAsserter, SoftwareTimerBase* softwareTimer,
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

/// range of test steps
using TestStepsRange = estd::ContiguousRange<const TestStep>;

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// total number of signals that are tested
constexpr size_t totalSignals {10};

/// range of test steps for signal handler
TestStepsRange handlerStepsRange;

/// shared return code of signal handler, is is reset before each test phase
sig_atomic_t sharedSigAtomic;

/// shared SequenceAsserter object, is is reset before each test phase
SequenceAsserter sharedSequenceAsserter;

/// pointer to current instance of software timer
SoftwareTimerBase* softwareTimerPointer;

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

int GenerateQueueSignalStep::operator()() const
{
	return code_ == SignalInformation::Code::Generated ?  ThisThread::Signals::generateSignal(signalNumber_) :
			ThisThread::Signals::queueSignal(signalNumber_, sigval{value_});
}

/*---------------------------------------------------------------------------------------------------------------------+
| TestStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int TestStep::operator()(SequenceAsserter& sequenceAsserter, SoftwareTimerBase* const softwareTimer,
		const SignalInformation* const signalInformation) const
{
	sequenceAsserter.sequencePoint(sequencePoints_.first);

	const auto ret =
			type_ == Type::BasicHandler && signalInformation != nullptr ? basicHandlerStep_(*signalInformation) :
			type_ == Type::GenerateQueueSignal ? generateQueueSignalStep_() :
			type_ == Type::SignalMask ? signalMaskStep_() :
			type_ == Type::SoftwareTimer && softwareTimer != nullptr ? softwareTimerStep_(*softwareTimer) : EINVAL;

	sequenceAsserter.sequencePoint(sequencePoints_.second);

	return ret;
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

int SoftwareTimerStep::operator()(SoftwareTimerBase& softwareTimer) const
{
	if (softwareTimer.isRunning() == true)
		return EINVAL;

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
 * \brief Signal handler
 *
 * If \a handlerStepsRange is empty, error is marked (test handler is unexpected). Otherwise first element if removed
 * from the range and executed. Whole sequence is repeated if TestStep::shouldExecuteMore() of the test step that was
 * just executed returns true.
 *
 * \param [in] signalInformation is a reference to received SignalInformation object
 */

void handler(const SignalInformation& signalInformation)
{
	bool more {true};
	while (more == true && handlerStepsRange.size() != 0)
	{
		// remove the first element from the range
		auto& handlerStep = *handlerStepsRange.begin();
		handlerStepsRange = {handlerStepsRange.begin() + 1, handlerStepsRange.end()};

		const auto ret = handlerStep(sharedSequenceAsserter, softwareTimerPointer, &signalInformation);
		if (ret != 0)
			sharedSigAtomic = ret;

		more = handlerStep.shouldExecuteMore();
	}

	if (more == true)
		sharedSigAtomic = EINVAL;	// execution of signal handler was not expected
}

/**
 * \brief Runner of test steps.
 *
 * If \a testStepsRange is empty, error is marked (function call is unexpected). Otherwise first element if removed from
 * the range and executed. Whole sequence is repeated if TestStep::shouldExecuteMore() of the test step that was just
 * executed returns true.
 *
 * \param [in] testStepsRange is a reference to range of test steps
 */

void testStepsRunner(TestStepsRange& testStepsRange)
{
	bool more {true};
	while (more == true && testStepsRange.size() != 0)
	{
		// remove the first element from the range
		auto& testStep = *testStepsRange.begin();
		testStepsRange = {testStepsRange.begin() + 1, testStepsRange.end()};

		const auto ret = testStep(sharedSequenceAsserter, softwareTimerPointer, nullptr);
		if (ret != 0)
			sharedSigAtomic = ret;

		more = testStep.shouldExecuteMore();
	}

	if (more == true)
		sharedSigAtomic = EINVAL;	// function call was not expected
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests catching of signals generated/queued by current thread for itself.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	static const TestStep threadSteps[]
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

	static const TestStep handlerSteps[]
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

	handlerStepsRange = decltype(handlerStepsRange){handlerSteps};
	auto threadStepsRange = TestStepsRange{threadSteps};

	testStepsRunner(threadStepsRange);

	const size_t handlerStepsSize = std::end(handlerSteps) - std::begin(handlerSteps);
	const size_t threadStepsSize = std::end(threadSteps) - std::begin(threadSteps);
	if (sharedSequenceAsserter.assertSequence(2 * (handlerStepsSize + threadStepsSize)) == false)
		return false;

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests catching of signals generated/queued by interrupt (via software timer) for current thread.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	static const TestStep threadSteps[]
	{
			// part 1 - normal generated signals
			{0, 5, true, SoftwareTimerStep{}},
			{6, 11, true, SoftwareTimerStep{}},
			{12, 17, true, SoftwareTimerStep{}},
			{18, 23, true, SoftwareTimerStep{}},
			{24, 29, true, SoftwareTimerStep{}},
			{30, 35, true, SoftwareTimerStep{}},
			{36, 41, true, SoftwareTimerStep{}},
			{42, 47, true, SoftwareTimerStep{}},
			{48, 53, true, SoftwareTimerStep{}},
			{54, 59, true, SoftwareTimerStep{}},
			// part 2 - normal queued signals
			{60, 65, true, SoftwareTimerStep{}},
			{66, 71, true, SoftwareTimerStep{}},
			{72, 77, true, SoftwareTimerStep{}},
			{78, 83, true, SoftwareTimerStep{}},
			{84, 89, true, SoftwareTimerStep{}},
			{90, 95, true, SoftwareTimerStep{}},
			{96, 101, true, SoftwareTimerStep{}},
			{102, 107, true, SoftwareTimerStep{}},
			{108, 113, true, SoftwareTimerStep{}},
			{114, 119, true, SoftwareTimerStep{}},
			// part 3 - multiple generated signals
			{120, 161, true, SoftwareTimerStep{}},
			// part 4 - multiple queued signals
			{162, 203, true, SoftwareTimerStep{}},
			// part 5 - multiple signals
			{204, 285, true, SoftwareTimerStep{}},
			// part 6 - nested signal handlers - generate signal from interrupt while signal handler is still running
			{286, 287, true, SignalMaskStep{SignalSet{UINT32_MAX << 10}}},
			{288, 347, true, SoftwareTimerStep{}},
			// part 7 - nested signal handlers - queue signal from interrupt while signal handler is still running
			{348, 407, true, SoftwareTimerStep{}},
			{408, 409, false, SignalMaskStep{SignalSet{SignalSet::empty}}},
	};

	static const TestStep interruptSteps[]
	{
			// part 1 - normal generated signals
			{1, 2, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
			{7, 8, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
			{13, 14, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
			{19, 20, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
			{25, 26, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
			{31, 32, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
			{37, 38, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
			{43, 44, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
			{49, 50, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
			{55, 56, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
			// part 2 - normal queued signals
			{61, 62, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x7bbee7c7}},
			{67, 68, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x144006e1}},
			{73, 74, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x3c2f74c3}},
			{79, 80, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x6842c269}},
			{85, 86, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x282ea423}},
			{91, 92, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x4ac997a6}},
			{97, 98, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x60375e15}},
			{103, 104, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x5025b208}},
			{109, 110, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x0ff3e7c1}},
			{115, 116, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x6fc765af}},
			// part 3 - multiple generated signals
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
			{289, 290, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
			{294, 295, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
			{299, 300, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
			{304, 305, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
			{309, 310, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
			{314, 315, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
			{319, 320, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
			{324, 325, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
			{329, 330, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
			{334, 335, false, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
			// part 7 - nested signal handlers - queue signal from interrupt while signal handler is still running
			{349, 350, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x388c64c3}},
			{354, 355, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x49020a59}},
			{359, 360, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x7e8842b9}},
			{364, 365, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x19ccf80b}},
			{369, 370, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x207e6915}},
			{374, 375, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x68e46a0f}},
			{379, 380, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x3bf7a491}},
			{384, 385, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x5ff85ef6}},
			{389, 390, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x7a66bbc6}},
			{394, 395, false, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x4f8adea0}},
	};

	static const TestStep handlerSteps[]
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
			{293, 346, false, SoftwareTimerStep{}},
			{296, 297, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0000100100},
					SignalInformation::Code::Generated, 2}},
			{298, 345, false, SoftwareTimerStep{}},
			{301, 302, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0000100110},
					SignalInformation::Code::Generated, 1}},
			{303, 344, false, SoftwareTimerStep{}},
			{306, 307, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0010100110},
					SignalInformation::Code::Generated, 7}},
			{308, 343, false, SoftwareTimerStep{}},
			{311, 312, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0011100110},
					SignalInformation::Code::Generated, 6}},
			{313, 342, false, SoftwareTimerStep{}},
			{316, 317, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011100110},
					SignalInformation::Code::Generated, 9}},
			{318, 341, false, SoftwareTimerStep{}},
			{321, 322, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011101110},
					SignalInformation::Code::Generated, 3}},
			{323, 340, false, SoftwareTimerStep{}},
			{326, 327, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011101111},
					SignalInformation::Code::Generated, 0}},
			{328, 339, false, SoftwareTimerStep{}},
			{331, 332, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011111111},
					SignalInformation::Code::Generated, 4}},
			{333, 338, false, SoftwareTimerStep{}},
			{336, 337, false, BasicHandlerStep{SignalSet{SignalSet::empty},
					SignalSet{(UINT32_MAX << 10) | 0b1111111111}, SignalInformation::Code::Generated, 8}},
			// part 7 - nested signal handlers - queue signal from interrupt while signal handler is still running
			{351, 352, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0001000000},
					SignalInformation::Code::Queued, 6, 0x388c64c3}},
			{353, 406, false, SoftwareTimerStep{}},
			{356, 357, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0001001000},
					SignalInformation::Code::Queued, 3, 0x49020a59}},
			{358, 405, false, SoftwareTimerStep{}},
			{361, 362, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0001001001},
					SignalInformation::Code::Queued, 0, 0x7e8842b9}},
			{363, 404, false, SoftwareTimerStep{}},
			{366, 367, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b0011001001},
					SignalInformation::Code::Queued, 7, 0x19ccf80b}},
			{368, 403, false, SoftwareTimerStep{}},
			{371, 372, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011001001},
					SignalInformation::Code::Queued, 9, 0x207e6915}},
			{373, 402, false, SoftwareTimerStep{}},
			{376, 377, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011011001},
					SignalInformation::Code::Queued, 4, 0x68e46a0f}},
			{378, 401, false, SoftwareTimerStep{}},
			{381, 382, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011011101},
					SignalInformation::Code::Queued, 2, 0x3bf7a491}},
			{383, 400, false, SoftwareTimerStep{}},
			{386, 387, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1011111101},
					SignalInformation::Code::Queued, 5, 0x5ff85ef6}},
			{388, 399, false, SoftwareTimerStep{}},
			{391, 392, true, BasicHandlerStep{SignalSet{SignalSet::empty}, SignalSet{(UINT32_MAX << 10) | 0b1111111101},
					SignalInformation::Code::Queued, 8, 0x7a66bbc6}},
			{393, 398, false, SoftwareTimerStep{}},
			{396, 397, false, BasicHandlerStep{SignalSet{SignalSet::empty},
					SignalSet{(UINT32_MAX << 10) | 0b1111111111}, SignalInformation::Code::Queued, 1, 0x4f8adea0}},
	};

	auto interruptStepsRange = TestStepsRange{interruptSteps};
	handlerStepsRange = decltype(handlerStepsRange){handlerSteps};
	auto softwareTimer = makeSoftwareTimer(testStepsRunner, std::ref(interruptStepsRange));
	softwareTimerPointer = &softwareTimer;
	auto threadStepsRange = TestStepsRange{threadSteps};

	testStepsRunner(threadStepsRange);

	softwareTimerPointer = {};

	const size_t threadStepsSize = std::end(threadSteps) - std::begin(threadSteps);
	const size_t interruptStepsSize = std::end(interruptSteps) - std::begin(interruptSteps);
	const size_t handlerStepsSize = std::end(handlerSteps) - std::begin(handlerSteps);
	if (sharedSequenceAsserter.assertSequence(2 * (threadStepsSize + interruptStepsSize + handlerStepsSize)) == false)
		return false;

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsCatchingTestCase::run_() const
{
	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (uint8_t signalNumber {}; signalNumber < totalSignals; ++signalNumber)
	{
		const auto setSignalActionResult = ThisThread::Signals::setSignalAction(signalNumber,
				{handler, getSignalMask(totalSignals + signalNumber)});
		if (setSignalActionResult.first != 0)
			return false;
	}

	for (const auto& function : {phase1, phase2})
	{
		// initially no signals may be pending
		if (ThisThread::Signals::getPendingSignalSet().getBitset().any() == true)
			return false;

		sharedSequenceAsserter = {};	// reset object
		sharedSigAtomic = {};			// reset object

		if (function() == false)
			return false;

		if (sharedSigAtomic != 0)
			return false;
	}

	// after the test no signals may be pending
	if (ThisThread::Signals::getPendingSignalSet().getBitset().any() == true)
		return false;

	if (statistics::getContextSwitchCount() - contextSwitchCount != 0)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
