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
 * \date 2015-05-16
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
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// test step executed in signal handler
class HandlerStep
{
public:

	/**
	 * \brief HandlerStep's constructor
	 *
	 * \param [in] sequencePoint is the sequence point of test step
	 * \param [in] pendingSignalSet is the expected set of currently pending signals for current thread
	 * \param [in] signalMask is the expected signal mask for current thread
	 * \param [in] code is the expected signal code in received SignalInformation object
	 * \param [in] signalNumber is the expected signal number in received SignalInformation object
	 * \param [in] value is the expected signal value in received SignalInformation object, default - default
	 * constructed int value
	 */

	constexpr HandlerStep(const unsigned int sequencePoint, const SignalSet pendingSignalSet,
			const SignalSet signalMask, const SignalInformation::Code code, const uint8_t signalNumber,
			const int value = {}) :
			sequencePoint_{sequencePoint},
			pendingSignalSet_{pendingSignalSet},
			signalMask_{signalMask},
			value_{value},
			code_{code},
			signalNumber_{signalNumber}
	{

	}

	/**
	 * \brief HandlerStep's function call operator
	 *
	 * Compares set of pending signals and signal mask of current thread with expected values. Verifies received
	 * SignalInformation object.
	 *
	 * \param [in] signalInformation is a reference to received SignalInformation object
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(const SignalInformation& signalInformation, SequenceAsserter& sequenceAsserter) const;

private:

	/// sequence point of test step
	unsigned int sequencePoint_;

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

/// test step executed in thread
class ThreadStep
{
public:

	/**
	 * \brief ThreadStep's constructor
	 *
	 * \param [in] sequencePoint is the sequence point of test step
	 * \param [in] code selects whether the signal will be generated (SignalInformation::Code::Generated) or queued
	 * (SignalInformation::Code::Queued)
	 * \param [in] signalNumber is the signal number that will be generated or queued
	 * \param [in] value is the value that will be queued with the signal, relevant only if \a code is
	 * SignalInformation::Code::Generated, default - default constructed int value
	 */

	constexpr ThreadStep(const unsigned int sequencePoint, const SignalInformation::Code code,
			const uint8_t signalNumber, const int value = {}) :
			sequencePoint_{sequencePoint},
			value_{value},
			code_{code},
			signalNumber_{signalNumber}
	{

	}

	/**
	 * \brief ThreadStep's function call operator
	 *
	 * Generates or queues selected signal.
	 *
	 * \param [in] sequenceAsserter is a reference to shared SequenceAsserter object
	 *
	 * \return 0 on success, error code otherwise
	 */

	int operator()(SequenceAsserter& sequenceAsserter) const;

private:

	/// sequence point of test step
	unsigned int sequencePoint_;

	/// value that will be queued with the signal, relevant only if \a code_ is SignalInformation::Code::Generated
	int value_;

	/// selects whether the signal will be generated (SignalInformation::Code::Generated) or queued
	/// (SignalInformation::Code::Queued)
	SignalInformation::Code code_;

	/// signal number that will be generated or queued
	uint8_t signalNumber_;
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
| HandlerStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int HandlerStep::operator()(const SignalInformation& signalInformation, SequenceAsserter& sequenceAsserter) const
{
	sequenceAsserter.sequencePoint(sequencePoint_);

	if (ThisThread::Signals::getPendingSignalSet().getBitset() != pendingSignalSet_.getBitset())
		return EINVAL;
	if (ThisThread::Signals::getSignalMask().getBitset() != signalMask_.getBitset())
		return EINVAL;
	if (signalInformation.getCode() != code_ || signalInformation.getSignalNumber() != signalNumber_ ||
			signalInformation.getValue().sival_int != value_)
		return EINVAL;

	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| ThreadStep's public functions
+---------------------------------------------------------------------------------------------------------------------*/

int ThreadStep::operator()(SequenceAsserter& sequenceAsserter) const
{
	sequenceAsserter.sequencePoint(sequencePoint_);

	return code_ == SignalInformation::Code::Generated ?  ThisThread::Signals::generateSignal(signalNumber_) :
			ThisThread::Signals::queueSignal(signalNumber_, sigval{value_});
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
			{0, SignalInformation::Code::Generated, 0},
			{2, SignalInformation::Code::Generated, 1},
			{4, SignalInformation::Code::Generated, 2},
			{6, SignalInformation::Code::Generated, 3},
			{8, SignalInformation::Code::Generated, 4},
			{10, SignalInformation::Code::Generated, 5},
			{12, SignalInformation::Code::Generated, 6},
			{14, SignalInformation::Code::Generated, 7},
			{16, SignalInformation::Code::Generated, 8},
			{18, SignalInformation::Code::Generated, 9},
			{20, SignalInformation::Code::Queued, 0, 0x6c3d9ebc},
			{22, SignalInformation::Code::Queued, 1, 0x52e04282},
			{24, SignalInformation::Code::Queued, 2, 0x29f9fc86},
			{26, SignalInformation::Code::Queued, 3, 0x19677883},
			{28, SignalInformation::Code::Queued, 4, 0x7f2d693b},
			{30, SignalInformation::Code::Queued, 5, 0x1a98ab78},
			{32, SignalInformation::Code::Queued, 6, 0x6b96c96b},
			{34, SignalInformation::Code::Queued, 7, 0x463445cc},
			{36, SignalInformation::Code::Queued, 8, 0x38dccfd2},
			{38, SignalInformation::Code::Queued, 9, 0x1e8ac134},
	};

	static const HandlerStep handlerSteps[]
	{
			{1, SignalSet{SignalSet::empty}, getSignalMask(0), SignalInformation::Code::Generated, 0},
			{3, SignalSet{SignalSet::empty}, getSignalMask(1), SignalInformation::Code::Generated, 1},
			{5, SignalSet{SignalSet::empty}, getSignalMask(2), SignalInformation::Code::Generated, 2},
			{7, SignalSet{SignalSet::empty}, getSignalMask(3), SignalInformation::Code::Generated, 3},
			{9, SignalSet{SignalSet::empty}, getSignalMask(4), SignalInformation::Code::Generated, 4},
			{11, SignalSet{SignalSet::empty}, getSignalMask(5), SignalInformation::Code::Generated, 5},
			{13, SignalSet{SignalSet::empty}, getSignalMask(6), SignalInformation::Code::Generated, 6},
			{15, SignalSet{SignalSet::empty}, getSignalMask(7), SignalInformation::Code::Generated, 7},
			{17, SignalSet{SignalSet::empty}, getSignalMask(8), SignalInformation::Code::Generated, 8},
			{19, SignalSet{SignalSet::empty}, getSignalMask(9), SignalInformation::Code::Generated, 9},
			{21, SignalSet{SignalSet::empty}, getSignalMask(0), SignalInformation::Code::Queued, 0, 0x6c3d9ebc},
			{23, SignalSet{SignalSet::empty}, getSignalMask(1), SignalInformation::Code::Queued, 1, 0x52e04282},
			{25, SignalSet{SignalSet::empty}, getSignalMask(2), SignalInformation::Code::Queued, 2, 0x29f9fc86},
			{27, SignalSet{SignalSet::empty}, getSignalMask(3), SignalInformation::Code::Queued, 3, 0x19677883},
			{29, SignalSet{SignalSet::empty}, getSignalMask(4), SignalInformation::Code::Queued, 4, 0x7f2d693b},
			{31, SignalSet{SignalSet::empty}, getSignalMask(5), SignalInformation::Code::Queued, 5, 0x1a98ab78},
			{33, SignalSet{SignalSet::empty}, getSignalMask(6), SignalInformation::Code::Queued, 6, 0x6b96c96b},
			{35, SignalSet{SignalSet::empty}, getSignalMask(7), SignalInformation::Code::Queued, 7, 0x463445cc},
			{37, SignalSet{SignalSet::empty}, getSignalMask(8), SignalInformation::Code::Queued, 8, 0x38dccfd2},
			{39, SignalSet{SignalSet::empty}, getSignalMask(9), SignalInformation::Code::Queued, 9, 0x1e8ac134},
	};

	handlerStepsRange = decltype(handlerStepsRange){handlerSteps};
	bool testResult {true};

	for (auto& step : threadSteps)
		if (step(sharedSequenceAsserter) != 0)
			testResult = false;

	const size_t handlerStepsSize = std::end(handlerSteps) - std::begin(handlerSteps);
	const size_t threadStepsSize = std::end(threadSteps) - std::begin(threadSteps);
	if (sharedSequenceAsserter.assertSequence(handlerStepsSize + threadStepsSize) == false)
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
