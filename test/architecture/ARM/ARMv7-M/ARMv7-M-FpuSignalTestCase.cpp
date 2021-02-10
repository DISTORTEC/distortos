/**
 * \file
 * \brief FpuSignalTestCase class implementation for ARMv7-M
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ARMv7-M-FpuSignalTestCase.hpp"

#include "distortos/chip/CMSIS-proxy.h"

/// configuration required by FpuSignalTestCase
#define ARMV7_M_FPU_SIGNAL_TEST_CASE_ENABLED __FPU_PRESENT == 1 && __FPU_USED == 1 && DISTORTOS_SIGNALS_ENABLE == 1 && \
		DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS > 0 && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0

#if ARMV7_M_FPU_SIGNAL_TEST_CASE_ENABLED == 1

#include "ARMv7-M-checkFpuRegisters.hpp"
#include "ARMv7-M-setFpuRegisters.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/ThisThread-Signals.hpp"

#include "estd/ScopeGuard.hpp"

#include <cerrno>

#endif	// ARMV7_M_FPU_SIGNAL_TEST_CASE_ENABLED == 1

namespace distortos
{

namespace test
{

#if ARMV7_M_FPU_SIGNAL_TEST_CASE_ENABLED == 1

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// values used in single test stage
struct Stage
{
	/// value written to all FPU registers in main thread, don't use FPU in main thread if 0
	uint32_t threadValue;

	/// value written to "lower" FPU registers in "sender" before queuing of signal, skip this step if 0
	uint32_t senderValueBefore;

	/// value written to "lower" FPU registers in "sender" after queuing of signal, skip this step if 0
	uint32_t senderValueAfter;

	/// signal value, written to "lower" FPU registers in handler, don't use FPU in handler if 0
	int signalValue;
};

/// description of single test phase
struct Phase
{
	/// type of "sender" function
	using Sender = int(Thread&, const Stage&);

	/// reference to "sender" function
	Sender& sender;

	/// expected number of context switches for single stage executed in this phase
	decltype(statistics::getContextSwitchCount()) contextSwitchCount;
};

/// context shared with interrupt
struct Context
{
	/// shared return value
	int sharedRet;

	/// pointer to test stage
	const Stage* stage;

	/// pointer to thread to which the signal will be queued
	Thread* thread;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// tested signal number
constexpr uint8_t testSignalNumber {16};

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/// pointer to context shared with interrupt
Context* volatile context;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Disables FPU context for current thread by clearing FPCA bit in CONTROL register.
 */

void disableFpuContext()
{
	const auto control = __get_CONTROL();
	__set_CONTROL(control & ~CONTROL_FPCA_Msk);
}

/**
 * \brief Signal handler
 *
 * Sets "lower" FPU registers (s0-s15 and fpscr) using the value queued with signal, unless this value is 0.
 *
 * \param [in] signalInformation is a reference to received SignalInformation object
 */

void handler(const SignalInformation& signalInformation)
{
	const auto value = signalInformation.getValue().sival_int;
	if (value == 0)
		return;

	setFpuRegisters(value, false);
}

/**
 * \brief Tests whether FPU context is active for current thread.
 *
 * \return true if FPU context is active for current thread (FPCA bit in CONTROL register is set), false otherwise
 */

bool isFpuContextActive()
{
	const auto control = __get_CONTROL();
	return (control & CONTROL_FPCA_Msk) != 0;
}

/**
 * \brief Test wrapper for Thread::queueSignal() that also modifies FPU registers.
 *
 * \param [in] thread is a reference to thread to which the signal will be queued
 * \param [in] stage is a reference to test stage
 * \param [in] full is the \a full argument passed to setFpuRegisters()
 * \param [in] sharedRet is a reference to int variable which will be written with 0 on success, error code otherwise
 */

void queueSignalWrapper(Thread& thread, const Stage& stage, const bool full, int& sharedRet)
{
	if (stage.senderValueBefore != 0)	// should FPU be used at the beginning of "sender"?
		setFpuRegisters(stage.senderValueBefore, full);

	sharedRet = thread.queueSignal(testSignalNumber, sigval{stage.signalValue});

	if (stage.senderValueAfter != 0)	// should FPU be used at th"sender""sender"?
		setFpuRegisters(stage.senderValueAfter, full);
}

/**
 * \brief Queues signal from interrupt (via manually triggered UsageFault) to current thread.
 *
 * \param [in] thread is a reference to thread to which the signal will be queued
 * \param [in] stage is a reference to test stage
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by Thread::queueSignal();
 */

int queueSignalFromInterrupt(Thread& thread, const Stage& stage)
{
	Context localContext {};
	context = &localContext;
	const auto contextScopeGuard = estd::makeScopeGuard(
			[]()
			{
				context = {};
			});

	context->thread = &thread;
	context->stage = &stage;

	SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;			// enable trapping of "divide by 0" by UsageFault
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;	// enable UsageFault

	volatile int a {};
	volatile int b {};
	a /= b;	// trigger UsageFault via "divide by 0"

	return context->sharedRet;
}

/**
 * \brief Queues signal from thread to non-current thread.
 *
 * \param [in] thread is a reference to thread to which the signal will be queued
 * \param [in] stage is a reference to test stage
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by Thread::queueSignal();
 */

int queueSignalFromThread(Thread& thread, const Stage& stage)
{
	constexpr decltype(FpuSignalTestCase::getTestCasePriority()) highPriority
	{
			FpuSignalTestCase::getTestCasePriority() + 1
	};

	int sharedRet {EINVAL};
	auto testThread = makeAndStartDynamicThread({testThreadStackSize, highPriority}, queueSignalWrapper,
			std::ref(thread), std::ref(stage), true, std::ref(sharedRet));
	testThread.join();

	return sharedRet;
}

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// test phases
const Phase phases[]
{
		{queueSignalFromInterrupt, 0},
		{queueSignalFromThread, 2},
};

/// test stages
const Stage stages[]
{
		{0, 0, 0, 0},								// don't use FPU in all contexts
		{0, 0, 0, 0x0b39fa96},						// in handler
		{0, 0, 0x8606151d, 0},						// in "sender" (at the end)
		{0, 0, 0x8bdd3b5e, 0x7d21d1c6},				// in "sender" (at the end) and in handler
		{0, 0x2f884196, 0, 0},						// in "sender" (at the beginning)
		{0, 0x0b0bbc86, 0, 0x0e43811b},				// in "sender" (at the beginning) and in handler
		{0, 0xb72b2917, 0x8c27baa7, 0},				// in "sender"
		{0, 0xa83b80c2, 0xd2b7dd4d, 0x626ca399},	// in "sender" and in handler
		{0xb4e40525, 0, 0, 0},						// in main thread
		{0x772bdf91, 0, 0, 0x0bb325b7},				// in main thread and in handler
		{0x8a19625e, 0, 0x32378f7b, 0},				// in main thread and in "sender" (at the end)
		{0xd17a21db, 0, 0xaa807a91, 0x03caf264},	// in main thread, in "sender" (at the end) and in handler
		{0xe4b44073, 0xa88c0cf5, 0, 0},				// in main thread and in "sender" (at the beginning)
		{0xb94c722b, 0x5f8ca773, 0, 0x288301cf},	// in main thread, in "sender" (at the beginning) and in handler
		{0x347ecfc5, 0xcb4a3584, 0x5a8bf219, 0},	// in main thread and in "sender"
		{0x788ed92e, 0x4b0ddff9, 0x73776a21, 0x48fb1969},	// in all contexts
};

}	// namespace

#endif	// ARMV7_M_FPU_SIGNAL_TEST_CASE_ENABLED == 1

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FpuSignalTestCase::finalize() const
{
#if ARMV7_M_FPU_SIGNAL_TEST_CASE_ENABLED == 1

	disableFpuContext();

#endif	// ARMV7_M_FPU_SIGNAL_TEST_CASE_ENABLED == 1

	return SignalsTestCaseCommon::finalize();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FpuSignalTestCase::run_() const
{
#if ARMV7_M_FPU_SIGNAL_TEST_CASE_ENABLED == 1

	const auto contextSwitchCount = statistics::getContextSwitchCount();
	auto expectedContextSwitchCount = decltype(contextSwitchCount){};

	{
		int ret;
		std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(testSignalNumber,
				SignalAction{handler, SignalSet{SignalSet::empty}});
		if (ret != 0)
			return false;
	}

	auto& currentThread = ThisThread::get();

	for (auto& phase : phases)
		for (auto& stage : stages)
		{
			disableFpuContext();

			decltype(setFpuRegisters({}, {})) fpscr {};
			if (stage.threadValue != 0)	// should FPU be used in main thread?
				fpscr = setFpuRegisters(stage.threadValue, true);

			if (phase.sender(currentThread, stage) != 0)
				return false;

			// FPU context may be active only if FPU was used in main thread
			if (isFpuContextActive() != (stage.threadValue != 0))
				return false;

			if (stage.threadValue != 0)	// was FPU used in main thread?
				if (checkFpuRegisters(stage.threadValue, fpscr) == false)
					return false;

			expectedContextSwitchCount += phase.contextSwitchCount;
			if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
				return false;
		}

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

#endif	// ARMV7_M_FPU_SIGNAL_TEST_CASE_ENABLED == 1

	return true;
}

}	// namespace test

}	// namespace distortos

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief UsageFault_Handler() for ARMv7-M
 */

extern "C" void UsageFault_Handler()
{
	SCB->CFSR = SCB_CFSR_DIVBYZERO_Msk;			// clear "divide by 0" flag
	SCB->CCR &= ~SCB_CCR_DIV_0_TRP_Msk;			// disable trapping of "divide by 0" by UsageFault
	SCB->SHCSR &= ~SCB_SHCSR_USGFAULTENA_Msk;	// disable UsageFault

	using namespace distortos::test;
	queueSignalWrapper(*context->thread, *context->stage, false, context->sharedRet);
}
