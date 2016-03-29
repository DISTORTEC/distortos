/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32F1
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/STM32F1-FLASH.hpp"
#include "distortos/chip/STM32F1-RCC.hpp"

#include "distortos/architecture/ARMv6-M-ARMv7-M-configureSysTick.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
#ifndef CONFIG_CHIP_STM32F100

#ifdef CONFIG_CHIP_STM32F1_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(true);
#else	// !def CONFIG_CHIP_STM32F1_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(false);
#endif	// !def CONFIG_CHIP_STM32F1_FLASH_PREFETCH_ENABLE

#endif	// !def CONFIG_CHIP_STM32F100

#ifdef CONFIG_CHIP_STM32F1_FLASH_HALF_CYCLE_ACCESS_ENABLE
	configureFlashHalfCycleAccess(true);
#else	// !def CONFIG_CHIP_STM32F1_FLASH_HALF_CYCLE_ACCESS_ENABLE
	configureFlashHalfCycleAccess(false);
#endif	// !def CONFIG_CHIP_STM32F1_FLASH_HALF_CYCLE_ACCESS_ENABLE

#ifdef CONFIG_CHIP_STM32F1_STANDARD_CLOCK_CONFIGURATION_ENABLE

#ifdef CONFIG_CHIP_STM32F1_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F1_RCC_HSE_CLOCK_BYPASS
	constexpr bool hseClockBypass {true};
#else	// !def CONFIG_CHIP_STM32F1_RCC_HSE_CLOCK_BYPASS
	constexpr bool hseClockBypass {};
#endif	// !def CONFIG_CHIP_STM32F1_RCC_HSE_CLOCK_BYPASS
	enableHse(hseClockBypass);

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

#ifdef CONFIG_CHIP_STM32F1_RCC_PREDIV2_PLL2_PLL3_ENABLE

	constexpr uint32_t pll23InFrequency {CONFIG_CHIP_STM32F1_RCC_HSE_FREQUENCY / CONFIG_CHIP_STM32F1_RCC_PREDIV2};
	static_assert(minPll23InFrequency <= pll23InFrequency && pll23InFrequency <= maxPll23InFrequency,
			"Invalid PLL2 and PLL3 input frequency!");
	configurePrediv2(CONFIG_CHIP_STM32F1_RCC_PREDIV2);

#ifdef CONFIG_CHIP_STM32F1_RCC_PLL2_ENABLE

#if defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL8)
	constexpr uint8_t pll2mul {8};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL9)
	constexpr uint8_t pll2mul {9};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL10)
	constexpr uint8_t pll2mul {10};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL11)
	constexpr uint8_t pll2mul {11};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL12)
	constexpr uint8_t pll2mul {12};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL13)
	constexpr uint8_t pll2mul {13};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL14)
	constexpr uint8_t pll2mul {14};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL16)
	constexpr uint8_t pll2mul {16};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL20)
	constexpr uint8_t pll2mul {20};
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PLL2MUL20)

	constexpr uint32_t pll2OutFrequency {pll23InFrequency * pll2mul};
	static_assert(minPll23OutFrequency <= pll2OutFrequency && pll2OutFrequency <= maxPll23OutFrequency,
			"Invalid PLL2 output frequency!");
	enablePll2(pll2mul);

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL2_ENABLE

#ifdef CONFIG_CHIP_STM32F1_RCC_PLL3_ENABLE

#if defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL8)
	constexpr uint8_t pll3mul {8};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL9)
	constexpr uint8_t pll3mul {9};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL10)
	constexpr uint8_t pll3mul {10};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL11)
	constexpr uint8_t pll3mul {11};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL12)
	constexpr uint8_t pll3mul {12};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL13)
	constexpr uint8_t pll3mul {13};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL14)
	constexpr uint8_t pll3mul {14};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL16)
	constexpr uint8_t pll3mul {16};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL20)
	constexpr uint8_t pll3mul {20};
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PLL3MUL20)

	constexpr uint32_t pll3OutFrequency {pll23InFrequency * pll3mul};
	static_assert(minPll23OutFrequency <= pll3OutFrequency && pll3OutFrequency <= maxPll23OutFrequency,
			"Invalid PLL3 output frequency!");
	enablePll3(pll3mul);

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL3_ENABLE

#endif	// def CONFIG_CHIP_STM32F1_RCC_PREDIV2_PLL2_PLL3_ENABLE

#endif	// defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

#endif	// def CONFIG_CHIP_STM32F1_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F1_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_HSIDIV2)

	constexpr bool pllClockSourcePrediv1 {};
	constexpr uint32_t pllInFrequency {hsiFrequency / 2};

#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_PREDIV1)

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

#if defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_HSE)
	constexpr bool prediv1ClockSourcePll2 {};
	constexpr uint32_t prediv1InFrequency {CONFIG_CHIP_STM32F1_RCC_HSE_FREQUENCY};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_PLL2)
	constexpr bool prediv1ClockSourcePll2 {true};
	constexpr uint32_t prediv1InFrequency {pll2OutFrequency};
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_PLL2)

	configurePrediv1ClockSource(prediv1ClockSourcePll2);

#else	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

	constexpr uint32_t prediv1InFrequency {CONFIG_CHIP_STM32F1_RCC_HSE_FREQUENCY};

#endif	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

	constexpr uint32_t prediv1OutFrequency {prediv1InFrequency / CONFIG_CHIP_STM32F1_RCC_PREDIV1};
	configurePrediv1(CONFIG_CHIP_STM32F1_RCC_PREDIV1);

	constexpr bool pllClockSourcePrediv1 {true};
	constexpr uint32_t pllInFrequency {prediv1OutFrequency};

#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_PREDIV1)

#if defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL2)
	constexpr uint8_t pllmul {2};
	constexpr uint8_t pllmulNumerator {2};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL3)
	constexpr uint8_t pllmul {3};
	constexpr uint8_t pllmulNumerator {3};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL4)
	constexpr uint8_t pllmul {4};
	constexpr uint8_t pllmulNumerator {4};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL5)
	constexpr uint8_t pllmul {5};
	constexpr uint8_t pllmulNumerator {5};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL6)
	constexpr uint8_t pllmul {6};
	constexpr uint8_t pllmulNumerator {6};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL6_5)
	constexpr uint8_t pllmul {pllmul6_5};
	constexpr uint8_t pllmulNumerator {13};
	constexpr uint8_t pllmulDenominator {2};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL7)
	constexpr uint8_t pllmul {7};
	constexpr uint8_t pllmulNumerator {7};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL8)
	constexpr uint8_t pllmul {8};
	constexpr uint8_t pllmulNumerator {8};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL9)
	constexpr uint8_t pllmul {9};
	constexpr uint8_t pllmulNumerator {9};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL10)
	constexpr uint8_t pllmul {10};
	constexpr uint8_t pllmulNumerator {10};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL11)
	constexpr uint8_t pllmul {11};
	constexpr uint8_t pllmulNumerator {11};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL12)
	constexpr uint8_t pllmul {12};
	constexpr uint8_t pllmulNumerator {12};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL13)
	constexpr uint8_t pllmul {13};
	constexpr uint8_t pllmulNumerator {13};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL14)
	constexpr uint8_t pllmul {14};
	constexpr uint8_t pllmulNumerator {14};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL15)
	constexpr uint8_t pllmul {15};
	constexpr uint8_t pllmulNumerator {15};
	constexpr uint8_t pllmulDenominator {1};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL16)
	constexpr uint8_t pllmul {16};
	constexpr uint8_t pllmulNumerator {16};
	constexpr uint8_t pllmulDenominator {1};
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PLLMUL16)

	static_assert(minPllInFrequency <= pllInFrequency && pllInFrequency <= maxPllInFrequency,
			"Invalid PLL input frequency!");
	constexpr uint32_t pllOutFrequency {pllInFrequency * pllmulNumerator / pllmulDenominator};
	static_assert(minPllOutFrequency <= pllOutFrequency && pllOutFrequency <= maxPllOutFrequency,
				"Invalid PLL output frequency!");
	enablePll(pllClockSourcePrediv1, pllmul);

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_HSI)
	constexpr uint32_t sysclkFrequency {hsiFrequency};
	constexpr SystemClockSource systemClockSource {SystemClockSource::hsi};
#elif defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_HSE)
	constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F1_RCC_HSE_FREQUENCY};
	constexpr SystemClockSource systemClockSource {SystemClockSource::hse};
#elif defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_PLL)
	constexpr uint32_t sysclkFrequency {pllOutFrequency};
	constexpr SystemClockSource systemClockSource {SystemClockSource::pll};
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_PLL)

#if defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV1)
	constexpr auto hpre = hpreDiv1;
#elif defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV2)
	constexpr auto hpre = hpreDiv2;
#elif defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV4)
	constexpr auto hpre = hpreDiv4;
#elif defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV8)
	constexpr auto hpre = hpreDiv8;
#elif defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV16)
	constexpr auto hpre = hpreDiv16;
#elif defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV64)
	constexpr auto hpre = hpreDiv64;
#elif defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV128)
	constexpr auto hpre = hpreDiv128;
#elif defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV256)
	constexpr auto hpre = hpreDiv256;
#elif defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV512)
	constexpr auto hpre = hpreDiv512;
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_AHB_DIV512)

	constexpr uint32_t ahbFrequency {sysclkFrequency / hpre};
	configureAhbClockDivider(hpre);

#if defined(CONFIG_CHIP_STM32F1_RCC_APB1_DIV1)
	constexpr auto ppre1 = ppreDiv1;
#elif defined(CONFIG_CHIP_STM32F1_RCC_APB1_DIV2)
	constexpr auto ppre1 = ppreDiv2;
#elif defined(CONFIG_CHIP_STM32F1_RCC_APB1_DIV4)
	constexpr auto ppre1 = ppreDiv4;
#elif defined(CONFIG_CHIP_STM32F1_RCC_APB1_DIV8)
	constexpr auto ppre1 = ppreDiv8;
#elif defined(CONFIG_CHIP_STM32F1_RCC_APB1_DIV16)
	constexpr auto ppre1 = ppreDiv16;
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_APB1_DIV16)

	constexpr uint32_t apb1Frequency {ahbFrequency / ppre1};
	static_assert(apb1Frequency <= maxApb1Frequency, "Invalid APB1 (low speed) frequency!");
	configureApbClockDivider(false, ppre1);

#if defined(CONFIG_CHIP_STM32F1_RCC_APB2_DIV1)
	constexpr auto ppre2 = ppreDiv1;
#elif defined(CONFIG_CHIP_STM32F1_RCC_APB2_DIV2)
	constexpr auto ppre2 = ppreDiv2;
#elif defined(CONFIG_CHIP_STM32F1_RCC_APB2_DIV4)
	constexpr auto ppre2 = ppreDiv4;
#elif defined(CONFIG_CHIP_STM32F1_RCC_APB2_DIV8)
	constexpr auto ppre2 = ppreDiv8;
#elif defined(CONFIG_CHIP_STM32F1_RCC_APB2_DIV16)
	constexpr auto ppre2 = ppreDiv16;
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_APB2_DIV16)

	constexpr uint32_t apb2Frequency {ahbFrequency / ppre2};
	static_assert(apb2Frequency <= maxApb2Frequency, "Invalid APB2 (high speed) frequency!");
	configureApbClockDivider(true, ppre2);

#ifndef CONFIG_CHIP_STM32F100

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / 24000000};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#endif	// !def CONFIG_CHIP_STM32F100

	switchSystemClock(systemClockSource);

#else	// !def CONFIG_CHIP_STM32F1_STANDARD_CLOCK_CONFIGURATION_ENABLE

	constexpr uint32_t ahbFrequency {CONFIG_CHIP_STM32F1_RCC_AHB_FREQUENCY};

#endif	// !def CONFIG_CHIP_STM32F1_STANDARD_CLOCK_CONFIGURATION_ENABLE

	constexpr uint32_t period {ahbFrequency / CONFIG_TICK_FREQUENCY};
	constexpr uint32_t periodDividedBy8 {period / 8};
	constexpr bool divideBy8 {period > architecture::maxSysTickPeriod};
	// at least one of the periods must be valid
	static_assert(period <= architecture::maxSysTickPeriod || periodDividedBy8 <= architecture::maxSysTickPeriod,
			"Invalid SysTick configuration!");
	architecture::configureSysTick(divideBy8 == false ? period : periodDividedBy8, divideBy8);
}

}	// namespace chip

}	// namespace distortos
