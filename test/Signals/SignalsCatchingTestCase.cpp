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
 * \date 2015-05-23
 */

#include "SignalsCatchingTestCase.hpp"

#include "SequenceAsserter.hpp"

#include "distortos/SignalAction.hpp"
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

/// test step executed in signal handler
class HandlerStep
{
public:

	/// type of test step
	enum class Type : uint8_t
	{
		/// BasicHandlerStep
		Basic,
		/// GenerateQueueSignalStep
		GenerateQueueSignal,
		/// SignalMaskStep
		SignalMask,
	};

	/**
	 * \brief HandlerStep's constructor for Basic type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available HandlerStep should be executed in the same signal handler
	 * (true) or not (false)
	 * \param [in] basicHandlerStep is the BasicHandlerStep that will be executed in test step
	 */

	constexpr HandlerStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint, const bool more,
			const BasicHandlerStep basicHandlerStep) :
			basicHandlerStep_{basicHandlerStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::Basic}
	{

	}

	/**
	 * \brief HandlerStep's constructor for GenerateQueueSignal type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available HandlerStep should be executed in the same signal handler
	 * (true) or not (false)
	 * \param [in] generateQueueSignalStep is the GenerateQueueSignalStep that will be executed in test step
	 */

	constexpr HandlerStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint, const bool more,
			const GenerateQueueSignalStep generateQueueSignalStep) :
			generateQueueSignalStep_{generateQueueSignalStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::GenerateQueueSignal}
	{

	}

	/**
	 * \brief HandlerStep's constructor for SignalMask type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available HandlerStep should be executed in the same signal handler
	 * (true) or not (false)
	 * \param [in] signalMaskStep is the SignalMaskStep that will be executed in test step
	 */

	constexpr HandlerStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint, const bool more,
			const SignalMaskStep signalMaskStep) :
			signalMaskStep_{signalMaskStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::SignalMask}
	{

	}

	/**
	 * \brief HandlerStep's function call operator
	 *
	 * Marks first sequence point, executes internal test step and marks last sequence point.
	 *
	 * \param [in] signalInformation is a reference to received SignalInformation object
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(const SignalInformation& signalInformation, SequenceAsserter& sequenceAsserter) const;

	/**
	 * \return true if another available HandlerStep should be executed in the same signal handler, false otherwise
	 */

	bool shouldExecuteMore() const
	{
		return more_;
	}

private:

	/// internal test step that will be executed
	union
	{
		/// BasicHandlerStep test step - valid only if type_ == Type::Basic
		BasicHandlerStep basicHandlerStep_;

		/// GenerateQueueSignalStep test step - valid only if type_ == Type::GenerateQueueSignal
		GenerateQueueSignalStep generateQueueSignalStep_;

		/// SignalMaskStep test step - valid only if type_ == Type::SignalMask
		SignalMaskStep signalMaskStep_;
	};

	/// sequence points of test step
	SequencePoints sequencePoints_;

	/// selects whether another available HandlerStep should be executed in the same signal handler (true) or not
	/// (false)
	bool more_;

	/// type of test step
	Type type_;
};

/// test step executed in interrupt (via software timer)
class InterruptStep
{
public:

	/// type of test step
	enum class Type : uint8_t
	{
		/// GenerateQueueSignalStep
		GenerateQueueSignal,
		/// SignalMaskStep
		SignalMask,
	};

	/**
	 * \brief InterruptStep's constructor for GenerateQueueSignal type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available InterruptStep should be executed in the same interrupt (true)
	 * or not (false)
	 * \param [in] generateQueueSignalStep is the GenerateQueueSignalStep that will be executed in test step
	 */

	constexpr InterruptStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint,
			const bool more, const GenerateQueueSignalStep generateQueueSignalStep) :
			generateQueueSignalStep_{generateQueueSignalStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::GenerateQueueSignal}
	{

	}

	/**
	 * \brief InterruptStep's constructor for SignalMask type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] more selects whether another available InterruptStep should be executed in the same interrupt (true)
	 * or not (false)
	 * \param [in] signalMaskStep is the SignalMaskStep that will be executed in test step
	 */

	constexpr InterruptStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint,
			const bool more, const SignalMaskStep signalMaskStep) :
			signalMaskStep_{signalMaskStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			more_{more},
			type_{Type::SignalMask}
	{

	}

	/**
	 * \brief InterruptStep's function call operator
	 *
	 * Marks first sequence point, executes internal test step and marks last sequence point.
	 *
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(SequenceAsserter& sequenceAsserter) const;

	/**
	 * \return true if another available InterruptStep should be executed in the same interrupt, false otherwise
	 */

	bool shouldExecuteMore() const
	{
		return more_;
	}

private:

	/// internal test step that will be executed
	union
	{
		/// GenerateQueueSignalStep test step - valid only if type_ == Type::GenerateQueueSignal
		GenerateQueueSignalStep generateQueueSignalStep_;

		/// SignalMaskStep test step - valid only if type_ == Type::SignalMask
		SignalMaskStep signalMaskStep_;
	};

	/// sequence points of test step
	SequencePoints sequencePoints_;

	/// selects whether another available InterruptStep should be executed in the same interrupt (true) or not (false)
	bool more_;

	/// type of test step
	Type type_;
};

/// test step executed in thread
class ThreadStep
{
public:

	/// type of test step
	enum class Type : uint8_t
	{
		/// GenerateQueueSignalStep
		GenerateQueueSignal,
		/// SignalMaskStep
		SignalMask,
	};

	/**
	 * \brief ThreadStep's constructor for GenerateQueueSignal type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] generateQueueSignalStep is the GenerateQueueSignalStep that will be executed in test step
	 */

	constexpr ThreadStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint,
			const GenerateQueueSignalStep generateQueueSignalStep) :
			generateQueueSignalStep_{generateQueueSignalStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			type_{Type::GenerateQueueSignal}
	{

	}

	/**
	 * \brief ThreadStep's constructor for SignalMask type.
	 *
	 * \param [in] firstSequencePoint is the first sequence point of test step
	 * \param [in] lastSequencePoint is the last sequence point of test step
	 * \param [in] signalMaskStep is the SignalMaskStep that will be executed in test step
	 */

	constexpr ThreadStep(const unsigned int firstSequencePoint, const unsigned int lastSequencePoint,
			const SignalMaskStep signalMaskStep) :
			signalMaskStep_{signalMaskStep},
			sequencePoints_{firstSequencePoint, lastSequencePoint},
			type_{Type::SignalMask}
	{

	}

	/**
	 * \brief ThreadStep's function call operator
	 *
	 * Marks first sequence point, executes internal test step and marks last sequence point.
	 *
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(SequenceAsserter& sequenceAsserter) const;

private:

	/// internal test step that will be executed
	union
	{
		/// GenerateQueueSignalStep test step - valid only if type_ == Type::GenerateQueueSignal
		GenerateQueueSignalStep generateQueueSignalStep_;

		/// SignalMaskStep test step - valid only if type_ == Type::SignalMask
		SignalMaskStep signalMaskStep_;
	};

	/// sequence points of test step
	SequencePoints sequencePoints_;

	/// type of test step
	Type type_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// total number of signals that are tested
constexpr size_t totalSignals {10};

/// range of test steps for signal handler
estd::ContiguousRange<const HandlerStep> handlerStepsRange;

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

int GenerateQueueSignalStep::operator()() const
{
	return code_ == SignalInformation::Code::Generated ?  ThisThread::Signals::generateSignal(signalNumber_) :
			ThisThread::Signals::queueSignal(signalNumber_, sigval{value_});
}

/*---------------------------------------------------------------------------------------------------------------------+
| HandlerStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int HandlerStep::operator()(const SignalInformation& signalInformation, SequenceAsserter& sequenceAsserter) const
{
	sequenceAsserter.sequencePoint(sequencePoints_.first);

	const auto ret = type_ == Type::Basic ? basicHandlerStep_(signalInformation) :
			type_ == Type::GenerateQueueSignal ? generateQueueSignalStep_() : signalMaskStep_();

	sequenceAsserter.sequencePoint(sequencePoints_.second);

	return ret;
}

/*---------------------------------------------------------------------------------------------------------------------+
| InterruptStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int InterruptStep::operator()(SequenceAsserter& sequenceAsserter) const
{
	sequenceAsserter.sequencePoint(sequencePoints_.first);

	const auto ret = type_ == Type::GenerateQueueSignal ? generateQueueSignalStep_() : signalMaskStep_();

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
| ThreadStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int ThreadStep::operator()(SequenceAsserter& sequenceAsserter) const
{
	sequenceAsserter.sequencePoint(sequencePoints_.first);

	const auto ret = type_ == Type::GenerateQueueSignal ? generateQueueSignalStep_() : signalMaskStep_();

	sequenceAsserter.sequencePoint(sequencePoints_.second);

	return ret;
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
 * from the range and executed. Whole sequence is repeated if HandlerStep::shouldExecuteMore() of the test step that was
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

		const auto ret = handlerStep(signalInformation, sharedSequenceAsserter);
		if (ret != 0)
			sharedSigAtomic = ret;

		more = handlerStep.shouldExecuteMore();
	}

	if (more == true)
		sharedSigAtomic = EINVAL;	// execution of signal handler was not expected
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
	static const ThreadStep threadSteps[]
	{
			// part 1 - normal generated signals
			{0, 3, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
			{4, 7, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
			{8, 11, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
			{12, 15, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
			{16, 19, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
			{20, 23, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
			{24, 27, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
			{28, 31, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
			{32, 35, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
			{36, 39, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
			// part 2 - normal queued signals
			{40, 43, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x6c3d9ebc}},
			{44, 47, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x52e04282}},
			{48, 51, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x29f9fc86}},
			{52, 55, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x19677883}},
			{56, 59, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x7f2d693b}},
			{60, 63, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x1a98ab78}},
			{64, 67, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x6b96c96b}},
			{68, 71, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x463445cc}},
			{72, 75, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x38dccfd2}},
			{76, 79, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x1e8ac134}},
			// part 3 - unmasking of pending generated signals
			{80, 81, SignalMaskStep{SignalSet{SignalSet::full}}},
			{82, 83, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
			{84, 85, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
			{86, 87, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
			{88, 89, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
			{90, 91, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
			{92, 93, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
			{94, 95, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
			{96, 97, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
			{98, 99, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
			{100, 101, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
			{102, 123, SignalMaskStep{SignalSet{SignalSet::empty}}},
			// part 4 - unmasking of pending queued signals
			{124, 125, SignalMaskStep{SignalSet{SignalSet::full}}},
			{126, 127, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x08055dbe}},
			{128, 129, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x2c9530e7}},
			{130, 131, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x79b6c040}},
			{132, 133, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x620f2acf}},
			{134, 135, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x7537d600}},
			{136, 137, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x605724fd}},
			{138, 139, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x4f8d74b5}},
			{140, 141, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x7f2e4b25}},
			{142, 143, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x2b56f970}},
			{144, 145, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x3898dc9e}},
			{146, 167, SignalMaskStep{SignalSet{SignalSet::empty}}},
			// part 5 - unmasking of pending signals
			{168, 169, SignalMaskStep{SignalSet{SignalSet::full}}},
			{170, 171, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
			{172, 173, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
			{174, 175, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
			{176, 177, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
			{178, 179, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
			{180, 181, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
			{182, 183, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
			{184, 185, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
			{186, 187, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
			{188, 189, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
			{190, 191, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x3e37ad5}},
			{192, 193, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x6c9f1f2}},
			{194, 195, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x5b12eca3}},
			{196, 197, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x48b947e9}},
			{198, 199, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x381cacad}},
			{200, 201, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x299d82f6}},
			{202, 203, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x44bcee3c}},
			{204, 205, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x6b98cc3d}},
			{206, 207, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x1cfc75b5}},
			{208, 209, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x15c508ba}},
			{210, 251, SignalMaskStep{SignalSet{SignalSet::empty}}},
			// part 6 - unmasking of pending signals - one at a time
			{252, 253, SignalMaskStep{SignalSet{SignalSet::full}}},
			{254, 255, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
			{256, 257, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
			{258, 259, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
			{260, 261, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
			{262, 263, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
			{264, 265, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
			{266, 267, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
			{268, 269, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
			{270, 271, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
			{272, 273, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
			{274, 275, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x3d982f37}},
			{276, 277, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x75143ba1}},
			{278, 279, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x4cf013f5}},
			{280, 281, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x1c010698}},
			{282, 283, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0xd4dc81a}},
			{284, 285, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x8894ac1}},
			{286, 287, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x399dbe15}},
			{288, 289, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x7abf0fb2}},
			{290, 291, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x587c305e}},
			{292, 293, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x3f6fa768}},
			{294, 299, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111111011}}},
			{300, 305, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111011011}}},
			{306, 311, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111010011}}},
			{312, 317, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1011010011}}},
			{318, 323, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1001010011}}},
			{324, 329, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1000010011}}},
			{330, 335, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b0000010011}}},
			{336, 341, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b0000010001}}},
			{342, 347, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b0000000001}}},
			{348, 353, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b0000000000}}},
			// part 7 - nested signal handlers - generate signal from within signal handler
			{354, 433, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
			// part 8 - nested signal handlers - queue signal from within signal handler
			{434, 513, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x39a52149}},
			// part 9 - nested signal handlers - unmasking of pending signals (one at a time) from within signal handler
			{514, 515, SignalMaskStep{SignalSet{SignalSet::full}}},
			{516, 517, GenerateQueueSignalStep{SignalInformation::Code::Generated, 0}},
			{518, 519, GenerateQueueSignalStep{SignalInformation::Code::Generated, 4}},
			{520, 521, GenerateQueueSignalStep{SignalInformation::Code::Generated, 6}},
			{522, 523, GenerateQueueSignalStep{SignalInformation::Code::Generated, 9}},
			{524, 525, GenerateQueueSignalStep{SignalInformation::Code::Generated, 8}},
			{526, 527, GenerateQueueSignalStep{SignalInformation::Code::Generated, 7}},
			{528, 529, GenerateQueueSignalStep{SignalInformation::Code::Generated, 5}},
			{530, 531, GenerateQueueSignalStep{SignalInformation::Code::Generated, 3}},
			{532, 533, GenerateQueueSignalStep{SignalInformation::Code::Generated, 2}},
			{534, 535, GenerateQueueSignalStep{SignalInformation::Code::Generated, 1}},
			{536, 537, GenerateQueueSignalStep{SignalInformation::Code::Queued, 2, 0x2e02ef41}},
			{538, 539, GenerateQueueSignalStep{SignalInformation::Code::Queued, 6, 0x406f37db}},
			{540, 541, GenerateQueueSignalStep{SignalInformation::Code::Queued, 5, 0x1e14b4d4}},
			{542, 543, GenerateQueueSignalStep{SignalInformation::Code::Queued, 9, 0x579cdee3}},
			{544, 545, GenerateQueueSignalStep{SignalInformation::Code::Queued, 1, 0x4cd69c9e}},
			{546, 547, GenerateQueueSignalStep{SignalInformation::Code::Queued, 8, 0x49ad2898}},
			{548, 549, GenerateQueueSignalStep{SignalInformation::Code::Queued, 4, 0x248f49de}},
			{550, 551, GenerateQueueSignalStep{SignalInformation::Code::Queued, 3, 0x116dd593}},
			{552, 553, GenerateQueueSignalStep{SignalInformation::Code::Queued, 0, 0x5d8d88c9}},
			{554, 555, GenerateQueueSignalStep{SignalInformation::Code::Queued, 7, 0x0a8b8d2c}},
			{556, 615, SignalMaskStep{SignalSet{(UINT32_MAX << 10) | 0b1111101111}}},
			{616, 617, SignalMaskStep{SignalSet{SignalSet::empty}}},
	};

	static const HandlerStep handlerSteps[]
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
	bool testResult {true};

	for (auto& step : threadSteps)
		if (step(sharedSequenceAsserter) != 0)
			testResult = false;

	const size_t handlerStepsSize = std::end(handlerSteps) - std::begin(handlerSteps);
	const size_t threadStepsSize = std::end(threadSteps) - std::begin(threadSteps);
	if (sharedSequenceAsserter.assertSequence(2 * (handlerStepsSize + threadStepsSize)) == false)
		return false;

	return testResult;
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

	for (const auto& function : {phase1})
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
