/**
 * \file
 * \brief FpuThreadTestCase class implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-28
 */

#include "ARMv7-M-FpuThreadTestCase.hpp"

#include "distortos/chip/CMSIS-proxy.h"

#if __FPU_PRESENT == 1 && __FPU_USED == 1

#include "distortos/StaticThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

namespace distortos
{

namespace test
{

#if __FPU_PRESENT == 1 && __FPU_USED == 1

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// number of iterations of sequence in test thread
constexpr uint32_t sequenceIterations {10};

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/// number of test threads
constexpr size_t totalThreads {10};

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread function
using TestThreadFunction = void(uint32_t, bool&);

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, std::declval<TestThreadFunction>(),
		std::declval<uint32_t>(), std::ref(std::declval<bool&>())));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Saves test value to all FPU registers (including FPSCR), causes context switch to next test thread and - after
 * return - checks whether all modified registers still have the same value. This sequence is repeated
 * \a sequenceIterations times with increasing test value (or until first failure).
 *
 * In case of failure \a sharedResult is set to "false", otherwise it is not modified.
 *
 * \param [in] value is the first test value
 * \param [out] sharedResult is a reference to variable with shared result of the test
 */

void thread(uint32_t value, bool& sharedResult)
{
	for (uint32_t iteration {}; iteration < sequenceIterations && sharedResult == true; ++iteration)
	{
		uint32_t fpscr;
		asm volatile
		(
				"	vmsr	FPSCR, %[value]	\n"	// move test value in FPSCR
				"	vmrs	%[fpscr], FPSCR	\n"	// read FPSCR to variable (not all fields of FPSCR are writable)
				"	vmov	s0, %[value]	\n"	// move test value to FPU register
				"	vmov	s1, %[value]	\n"
				"	vmov	s2, %[value]	\n"
				"	vmov	s3, %[value]	\n"
				"	vmov	s4, %[value]	\n"
				"	vmov	s5, %[value]	\n"
				"	vmov	s6, %[value]	\n"
				"	vmov	s7, %[value]	\n"
				"	vmov	s8, %[value]	\n"
				"	vmov	s9, %[value]	\n"
				"	vmov	s10, %[value]	\n"
				"	vmov	s11, %[value]	\n"
				"	vmov	s12, %[value]	\n"
				"	vmov	s13, %[value]	\n"
				"	vmov	s14, %[value]	\n"
				"	vmov	s15, %[value]	\n"
				"	vmov	s16, %[value]	\n"
				"	vmov	s17, %[value]	\n"
				"	vmov	s18, %[value]	\n"
				"	vmov	s19, %[value]	\n"
				"	vmov	s20, %[value]	\n"
				"	vmov	s21, %[value]	\n"
				"	vmov	s22, %[value]	\n"
				"	vmov	s23, %[value]	\n"
				"	vmov	s24, %[value]	\n"
				"	vmov	s25, %[value]	\n"
				"	vmov	s26, %[value]	\n"
				"	vmov	s27, %[value]	\n"
				"	vmov	s28, %[value]	\n"
				"	vmov	s29, %[value]	\n"
				"	vmov	s30, %[value]	\n"
				"	vmov	s31, %[value]	\n"

				:	[fpscr] "=r" (fpscr)
				:	[value] "r" (value)
		);

		ThisThread::yield();	// cause context switch

		asm volatile
		(
				"	vmov	r0, s0					\n"	// move test value from FPU register
				"	cmp		r0, %[value]			\n"	// compare with expected value
				"	it		ne						\n"	// no match?
				"	bne		1f						\n"	// branch to error handling
				"	vmov	r0, s1					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s2					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s3					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s4					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s5					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s6					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s7					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s8					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s9					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s10					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s11					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s12					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s13					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s14					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s15					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s16					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s17					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s18					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s19					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s20					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s21					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s22					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s23					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s24					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s25					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s26					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s27					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s28					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s29					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s30					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmov	r0, s31					\n"
				"	cmp		r0, %[value]			\n"
				"	it		ne						\n"
				"	bne		1f						\n"
				"	vmrs	r0, FPSCR				\n"	// move test value from FPSCR
				"	cmp		r0, %[fpscr]			\n"	// compare with saved FPSCR value

				"	it		eq						\n"
				"	beq		2f						\n"	// branch "out" if all compared values were equal

				"1:									\n"	// error handling
				"	mov		r0, #0					\n"
				"	strb	r0, [%[sharedResult]]	\n"	// sharedResult = false;

				"2:									\n"

				::	[value] "r" (value),
				 	[sharedResult] "r" (&sharedResult),
					[fpscr] "r" (fpscr)
				: 	"r0"
		);

		value++;
	}
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] value is the first test value
 * \param [out] sharedResult is a reference to variable with shared result of the test
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const uint32_t value, bool& sharedResult)
{
	return makeStaticThread<testThreadStackSize>(1, thread, static_cast<uint32_t>(value), std::ref(sharedResult));
}

}	// namespace

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FpuThreadTestCase::run_() const
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	bool sharedResult {true};
	std::array<TestThread, totalThreads> threads
	{{
			makeTestThread(0x3c6f0686, sharedResult),
			makeTestThread(0x42f94e89, sharedResult),
			makeTestThread(0x77f24bc2, sharedResult),
			makeTestThread(0x2924b4c4, sharedResult),
			makeTestThread(0xcbd6c082, sharedResult),
			makeTestThread(0x59c00e8a, sharedResult),
			makeTestThread(0x20fb443b, sharedResult),
			makeTestThread(0x7421f59b, sharedResult),
			makeTestThread(0x509d19ca, sharedResult),
			makeTestThread(0xc25198ec, sharedResult),
	}};

	for (auto& thread : threads)
		thread.start();

	for (auto& thread : threads)
		thread.join();

	constexpr auto totalContextSwitches = totalThreads * sequenceIterations + 2 * totalThreads;
	if (statistics::getContextSwitchCount() - contextSwitchCount != totalContextSwitches)
		return false;

	return sharedResult;

#else

	return true;

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
}

}	// namespace test

}	// namespace distortos
