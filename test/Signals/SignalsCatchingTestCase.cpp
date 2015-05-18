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
 * \date 2015-05-18
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
	};

	/// sequence points of test step
	SequencePoints sequencePoints_;

	/// selects whether another available HandlerStep should be executed in the same signal handler (true) or not
	/// (false)
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

	const auto ret = basicHandlerStep_(signalInformation);

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
 * from the range and executed.
 *
 * \param [in] signalInformation is a reference to received SignalInformation object
 */

void handler(const SignalInformation& signalInformation)
{
	if (handlerStepsRange.size() != 0)
	{
		// remove the first element from the range
		auto& handlerStep = *handlerStepsRange.begin();
		handlerStepsRange = {handlerStepsRange.begin() + 1, handlerStepsRange.end()};

		const auto ret = handlerStep(signalInformation, sharedSequenceAsserter);
		if (ret != 0)
			sharedSigAtomic = ret;
	}
	else
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
	};

	static const HandlerStep handlerSteps[]
	{
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
