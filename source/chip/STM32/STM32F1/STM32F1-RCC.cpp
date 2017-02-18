/**
 * \file
 * \brief Implementation of RCC-related functions for STM32F1
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F1-RCC.hpp"

#include "distortos/chip/STM32F1-RCC-bits.h"

#include "distortos/chip/CMSIS-proxy.h"

#include <array>

#include <cerrno>

namespace distortos
{

namespace chip
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures PREDIV1 or PREDIV2 division factor.
 *
 * \param [in] prediv2 selects whether PREDIV1 (false) or PREDIV2 (true) will be configured
 * \param [in] prediv is the PREDIV1 or PREDIV2 division factor, [minPrediv; maxPrediv]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a prediv value is invalid;
 */

int configurePrediv(const bool prediv2, const uint8_t prediv)
{
	if (prediv < minPrediv || prediv > maxPrediv)
		return EINVAL;

#if defined(CONFIG_CHIP_STM32F100)
	static_cast<void>(prediv2);	// suppress warning
	RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV1) | ((prediv - 1) << RCC_CFGR2_PREDIV1_bit);
#elif defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)
	RCC->CFGR2 = (RCC->CFGR2 & ~(prediv2 == true ? RCC_CFGR2_PREDIV2 : RCC_CFGR2_PREDIV1)) |
			((prediv - 1) << (prediv2 == true ? RCC_CFGR2_PREDIV2_bit : RCC_CFGR2_PREDIV1_bit));
#else	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)
	static_cast<void>(prediv2);	// suppress warning
	RCC_CFGR_PLLXTPRE_bb = prediv == 2;
#endif	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)
	return 0;
}

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

/**
 * \brief Enables PLL2 or PLL3.
 *
 * Enables PLL2 or PLL3 using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of PLL2 make sure that it is not used in any way (as source of main PLL).
 * Before changing configuration of PLL3 make sure that it is not used in any way (as source of peripheral clocks).
 *
 * \param [in] pll3 selects whether PLL2 (false) or PLL3 (true) will be configured
 * \param [in] pll23Mul is the PLL2MUL value for PLL2 or PLL3MUL value for PLL3, [minPll23Mul; maxPll23Mul] and
 * {pll23Mul16, pll23Mul20}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pll23Mul value is invalid;
 */

int enablePll23(const bool pll3, const uint8_t pll23Mul)
{
	if ((pll23Mul < minPll23Mul || pll23Mul > maxPll23Mul) && pll23Mul != pll23Mul16 && pll23Mul != pll23Mul20)
		return EINVAL;

	const auto convertedPll23Mul = pll23Mul - 2 <= 0xf ? pll23Mul - 2 : 0xf;
	RCC->CFGR2 = (RCC->CFGR2 & ~(pll3 == true ? RCC_CFGR2_PLL3MUL : RCC_CFGR2_PLL2MUL)) |
			(convertedPll23Mul << (pll3 == true ? RCC_CFGR2_PLL3MUL_bit : RCC_CFGR2_PLL2MUL_bit));
	(pll3 == true ? RCC_CR_PLL3ON_bb : RCC_CR_PLL2ON_bb) = 1;
	while ((pll3 == true ? RCC_CR_PLL3RDY_bb : RCC_CR_PLL2RDY_bb) == 0);	// wait until PLL is stable
	return 0;
}

#endif	// defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int configureAhbClockDivider(const uint16_t hpre)
{
	static const std::pair<decltype(hpre), decltype(RCC_CFGR_HPRE_DIV1)> associations[]
	{
			{hpreDiv1, RCC_CFGR_HPRE_DIV1},
			{hpreDiv2, RCC_CFGR_HPRE_DIV2},
			{hpreDiv4, RCC_CFGR_HPRE_DIV4},
			{hpreDiv8, RCC_CFGR_HPRE_DIV8},
			{hpreDiv16, RCC_CFGR_HPRE_DIV16},
			{hpreDiv64, RCC_CFGR_HPRE_DIV64},
			{hpreDiv128, RCC_CFGR_HPRE_DIV128},
			{hpreDiv256, RCC_CFGR_HPRE_DIV256},
			{hpreDiv512, RCC_CFGR_HPRE_DIV512},
	};

	for (auto& association : associations)
		if (association.first == hpre)
		{
			RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | association.second;
			return 0;
		}

	return EINVAL;
}

int configureApbClockDivider(const bool ppre2, const uint8_t ppre)
{
	static const std::pair<decltype(ppre), std::array<decltype(RCC_CFGR_PPRE1_DIV1), 2>> associations[]
	{
			{ppreDiv1, {RCC_CFGR_PPRE1_DIV1, RCC_CFGR_PPRE2_DIV1}},
			{ppreDiv2, {RCC_CFGR_PPRE1_DIV2, RCC_CFGR_PPRE2_DIV2}},
			{ppreDiv4, {RCC_CFGR_PPRE1_DIV4, RCC_CFGR_PPRE2_DIV4}},
			{ppreDiv8, {RCC_CFGR_PPRE1_DIV8, RCC_CFGR_PPRE2_DIV8}},
			{ppreDiv16, {RCC_CFGR_PPRE1_DIV16, RCC_CFGR_PPRE2_DIV16}},
	};

	for (auto& association : associations)
		if (association.first == ppre)
		{
			static const decltype(RCC_CFGR_PPRE1) masks[] {RCC_CFGR_PPRE1, RCC_CFGR_PPRE2};
			RCC->CFGR = (RCC->CFGR & ~masks[ppre2]) | association.second[ppre2];
			return 0;
		}

	return EINVAL;
}

int configurePrediv1(const uint8_t prediv1)
{
	return configurePrediv(false, prediv1);
}

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

void configurePrediv1ClockSource(const bool pll2)
{
	RCC_CFGR2_PREDIV1SRC_bb = pll2;
}

int configurePrediv2(const uint8_t prediv2)
{
	return configurePrediv(true, prediv2);
}

#endif	// defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

void disableHse()
{
	RCC_CR_HSEON_bb = 0;
}

void disablePll()
{
	RCC_CR_PLLON_bb = 0;
}

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

void disablePll2()
{
	RCC_CR_PLL2ON_bb = 0;
}

void disablePll3()
{
	RCC_CR_PLL3ON_bb = 0;
}

#endif	// defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

void enableHse(const bool bypass)
{
	RCC_CR_HSEBYP_bb = bypass;
	RCC_CR_HSEON_bb = 1;
	while (RCC_CR_HSERDY_bb == 0);	// wait until HSE oscillator is stable
}

int enablePll(const bool prediv1, const uint8_t pllmul)
{
#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

	if ((pllmul < minPllmul || pllmul > maxPllmul) && pllmul != pllmul6_5)
		return EINVAL;

#else	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

	if (pllmul < minPllmul || pllmul > maxPllmul)
		return EINVAL;

#endif	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLMULL | RCC_CFGR_PLLSRC)) | ((pllmul - 2) << RCC_CFGR_PLLMUL_bit) |
			(prediv1 << RCC_CFGR_PLLSRC_bit);
	RCC_CR_PLLON_bb = 1;
	while (RCC_CR_PLLRDY_bb == 0);	// wait until PLL is stable
	return 0;
}

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

int enablePll2(const uint8_t pll2Mul)
{
	return enablePll23(false, pll2Mul);
}

int enablePll3(const uint8_t pll3Mul)
{
	return enablePll23(true, pll3Mul);
}

#endif	// defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

void switchSystemClock(const SystemClockSource source)
{
	const auto sourceValue = static_cast<uint32_t>(source);
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | (sourceValue << RCC_CFGR_SW_bit);
	while ((RCC->CFGR & RCC_CFGR_SWS) != sourceValue << RCC_CFGR_SWS_bit);
}

}	// namespace chip

}	// namespace distortos
