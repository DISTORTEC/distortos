/**
 * \file
 * \brief Implementation of RCC-related functions for STM32F4
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F4-RCC.hpp"

#include "distortos/chip/STM32F4-RCC-bits.h"

#include "distortos/chip/CMSIS-proxy.h"

#include <array>

#include <cerrno>

namespace distortos
{

namespace chip
{

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

void configurePllClockSource(const bool hse)
{
	RCC_PLLCFGR_PLLSRC_bb = hse;
}

int configurePllInputClockDivider(const uint8_t pllm)
{
	if (pllm < minPllm || pllm > maxPllm)
		return EINVAL;

	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | (pllm << RCC_PLLCFGR_PLLM_bit);
	return 0;
}

void disableHse()
{
	RCC_CR_HSEON_bb = 0;
}

void disablePll()
{
	RCC_CR_PLLON_bb = 0;
}

void enableHse(const bool bypass)
{
	RCC_CR_HSEBYP_bb = bypass;
	RCC_CR_HSEON_bb = 1;
	while (RCC_CR_HSERDY_bb == 0);	// wait until HSE oscillator is stable
}

#if defined(CONFIG_CHIP_STM32F412) || defined(CONFIG_CHIP_STM32F413) || defined(CONFIG_CHIP_STM32F423) || \
		defined(CONFIG_CHIP_STM32F446) || defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)
int enablePll(const uint16_t plln, const uint8_t pllp, const uint8_t pllq, const uint8_t pllr)
#else	// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423) &&
		// !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)
int enablePll(const uint16_t plln, const uint8_t pllp, const uint8_t pllq)
#endif	// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423) &&
		// !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)
{
	if (plln < minPlln || plln > maxPlln ||
			(pllp != pllpDiv2 && pllp != pllpDiv4 && pllp != pllpDiv6 && pllp != pllpDiv8) ||
			pllq < minPllq || pllq > maxPllq)
		return EINVAL;

#if defined(CONFIG_CHIP_STM32F412) || defined(CONFIG_CHIP_STM32F413) || defined(CONFIG_CHIP_STM32F423) || \
		defined(CONFIG_CHIP_STM32F446) || defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)

	if (pllr < minPllr || pllr > maxPllr)
		return EINVAL;

	RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLR)) |
			(plln << RCC_PLLCFGR_PLLN_bit) | ((pllp / 2 - 1) << RCC_PLLCFGR_PLLP_bit) | (pllq << RCC_PLLCFGR_PLLQ_bit) |
			(pllr << RCC_PLLCFGR_PLLR_bit);

#else	// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423) &&
		// !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)

	RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ)) |
			(plln << RCC_PLLCFGR_PLLN_bit) | ((pllp / 2 - 1) << RCC_PLLCFGR_PLLP_bit) | (pllq << RCC_PLLCFGR_PLLQ_bit);

#endif	// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423) &&
		// !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)

	RCC_CR_PLLON_bb = 1;
	while (RCC_CR_PLLRDY_bb == 0);	// wait until PLL is stable
	return 0;
}

void switchSystemClock(const SystemClockSource source)
{
	const auto sourceValue = static_cast<uint32_t>(source);
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | (sourceValue << RCC_CFGR_SW_bit);
	while ((RCC->CFGR & RCC_CFGR_SWS) != sourceValue << RCC_CFGR_SWS_bit);
}

}	// namespace chip

}	// namespace distortos
