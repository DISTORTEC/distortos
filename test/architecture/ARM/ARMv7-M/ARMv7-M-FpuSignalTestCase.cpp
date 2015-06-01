/**
 * \file
 * \brief FpuSignalTestCase class implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-06-01
 */

#include "ARMv7-M-FpuSignalTestCase.hpp"

#include "distortos/chip/CMSIS-proxy.h"

#if __FPU_PRESENT == 1 && __FPU_USED == 1

#include "checkFpuRegisters.hpp"
#include "setFpuRegisters.hpp"

#include "distortos/SoftwareTimer.hpp"
#include "distortos/ThisThread-Signals.hpp"

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

namespace distortos
{

namespace test
{

#if __FPU_PRESENT == 1 && __FPU_USED == 1

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

	/// value written to "lower" FPU registers in interrupt before queuing of signal, skip this step if 0
	uint32_t interruptValueBefore;

	/// value written to "lower" FPU registers in interrupt after queuing of signal, skip this step if 0
	uint32_t interruptValueAfter;

	/// signal value, written to "lower" FPU registers in handler, don't use FPU in handler if 0
	int signalValue;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// tested signal number
constexpr uint8_t testSignalNumber {16};

/// test stages
const Stage stages[]
{
		{0, 0, 0, 0},								// don't use FPU in all contexts
		{0, 0, 0, 0x0b39fa96},						// in handler
		{0, 0, 0x8606151d, 0},						// in interrupt (at the end)
		{0, 0, 0x8bdd3b5e, 0x7d21d1c6},				// in interrupt (at the end) and in handler
		{0, 0x2f884196, 0, 0},						// in interrupt (at the beginning)
		{0, 0x0b0bbc86, 0, 0x0e43811b},				// in interrupt (at the beginning) and in handler
		{0, 0xb72b2917, 0x8c27baa7, 0},				// in interrupt
		{0, 0xa83b80c2, 0xd2b7dd4d, 0x626ca399},	// in interrupt and in handler
		{0xb4e40525, 0, 0, 0},						// in main thread
		{0x772bdf91, 0, 0, 0x0bb325b7},				// in main thread and in handler
		{0x8a19625e, 0, 0x32378f7b, 0},				// in main thread and in interrupt (at the end)
		{0xd17a21db, 0, 0xaa807a91, 0x03caf264},	// in main thread, in interrupt (at the end) and in handler
		{0xe4b44073, 0xa88c0cf5, 0, 0},				// in main thread and in interrupt (at the beginning)
		{0xb94c722b, 0x5f8ca773, 0, 0x288301cf},	// in main thread, in interrupt (at the beginning) and in handler
		{0x347ecfc5, 0xcb4a3584, 0x5a8bf219, 0},	// in main thread and in interrupt
		{0x788ed92e, 0x4b0ddff9, 0x73776a21, 0x48fb1969},	// in all contexts
};

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
 * Sets "lower" FPU registers (s0-s15 and FPSCR) using the value queued with signal, unless this value is 0.
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

}	// namespace

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FpuSignalTestCase::finalize() const
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	disableFpuContext();

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

	return SignalsTestCaseCommon::finalize();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FpuSignalTestCase::run_() const
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	{
		int ret;
		std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(testSignalNumber,
				SignalAction{handler, SignalSet{SignalSet::empty}});
		if (ret != 0)
			return false;
	}

	for (auto& stage : stages)
	{
		disableFpuContext();

		decltype(setFpuRegisters({}, {})) fpscr {};
		if (stage.threadValue != 0)	// should FPU be used in main thread?
			fpscr = setFpuRegisters(stage.threadValue, true);

		int sharedRet {true};
		auto softwareTimer = makeSoftwareTimer(
				[&sharedRet, &stage]()
				{
					if (stage.interruptValueBefore != 0)	// should FPU be used at the beginning of interrupt?
						setFpuRegisters(stage.interruptValueBefore, false);

					sharedRet = ThisThread::Signals::queueSignal(testSignalNumber, sigval{stage.signalValue});

					if (stage.interruptValueAfter != 0)	// should FPU be used at the end of interrupt?
						setFpuRegisters(stage.interruptValueAfter, false);
				});

		softwareTimer.start(TickClock::duration{});
		while (softwareTimer.isRunning() == true);

		if (sharedRet != 0)
			return false;

		// FPU context may be active only if FPU was used in main thread
		if (isFpuContextActive() != (stage.threadValue != 0))
			return false;

		if (stage.threadValue != 0)	// was FPU used in main thread?
			if (checkFpuRegisters(stage.threadValue, fpscr) == false)
				return false;
	}

	return true;

#else

	return true;

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
}

}	// namespace test

}	// namespace distortos
