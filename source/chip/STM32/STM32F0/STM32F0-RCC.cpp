/**
 * \file
 * \brief Implementation of RCC-related functions for STM32F0
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F0-RCC.hpp"

#include "distortos/chip/CMSIS-proxy.h"

#include <utility>

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

int configureApbClockDivider(const uint8_t ppre)
{
	static const std::pair<decltype(ppre), decltype(RCC_CFGR_PPRE_DIV1)> associations[]
	{
			{ppreDiv1, RCC_CFGR_PPRE_DIV1},
			{ppreDiv2, RCC_CFGR_PPRE_DIV2},
			{ppreDiv4, RCC_CFGR_PPRE_DIV4},
			{ppreDiv8, RCC_CFGR_PPRE_DIV8},
			{ppreDiv16, RCC_CFGR_PPRE_DIV16},
	};

	for (auto& association : associations)
		if (association.first == ppre)
		{
			RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE) | association.second;
			return 0;
		}

	return EINVAL;
}

int configurePrediv(const uint8_t prediv)
{
	if (prediv < minPrediv || prediv > maxPrediv)
		return EINVAL;

	RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV) | (prediv - 1) << RCC_CFGR2_PREDIV_Pos;
	return 0;
}

void disableHse()
{
	RCC->CR &= ~RCC_CR_HSEON;
}

#ifdef STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

void disableHsi48()
{
	RCC->CR2 &= ~RCC_CR2_HSI48ON;
}

#endif	// def STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

void disablePll()
{
	RCC->CR &= ~RCC_CR_PLLON;
}

void enableHse(const bool bypass)
{
	RCC->CR = (RCC->CR & ~RCC_CR_HSEBYP) | bypass << RCC_CR_HSEBYP_Pos;
	RCC->CR |= RCC_CR_HSEON;	/// \todo check whether this can be merged with previous write of RCC->CR
	while ((RCC->CR & RCC_CR_HSERDY) == 0);	// wait until HSE oscillator is stable
}

#ifdef STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

void enableHsi48()
{
	RCC->CR2 |= RCC_CR2_HSI48ON;
	while ((RCC->CR2 & RCC_CR2_HSI48RDY) == 0);	// wait until HSI48 clock is stable
}

#endif	// def STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

int enablePll(const PllClockSource pllClockSource, const uint8_t pllmul)
{
	if (pllmul < minPllmul || pllmul > maxPllmul)
		return EINVAL;

	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC)) | (pllmul - 2) << RCC_CFGR_PLLMUL_Pos |
			static_cast<uint8_t>(pllClockSource) << RCC_CFGR_PLLSRC_Pos;
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);	// wait until PLL is stable
	return 0;
}

void switchSystemClock(const SystemClockSource source)
{
	const auto sourceValue = static_cast<uint32_t>(source);
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | sourceValue << RCC_CFGR_SW_Pos;
	while ((RCC->CFGR & RCC_CFGR_SWS) != sourceValue << RCC_CFGR_SWS_Pos);
}

}	// namespace chip

}	// namespace distortos
