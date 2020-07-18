/**
 * \file
 * \brief Implementation of RCC-related functions for STM32L0
 *
 * \author Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32L0-RCC.hpp"

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
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLSRC) | hse << RCC_CFGR_PLLSRC_Pos;
}

void disableMsi()
{
	RCC->CR &= ~RCC_CR_MSION;
}

void disableHse()
{
	RCC->CR &= ~RCC_CR_HSEON;
}

void disableHsi16()
{
	RCC->CR &= ~RCC_CR_HSION;
}

#ifdef STM32L0X2_STM32L0X3_RCC_FEATURES

void disableHsi48()
{
	RCC->CRRCR &= ~RCC_CRRCR_HSI48ON;
}

#endif	// def STM32L0X2_STM32L0X3_RCC_FEATURES

void disablePll()
{
	RCC->CR &= ~RCC_CR_PLLON;
}

int enableMsi(const uint8_t range)
{
	if (range > maxMsi)
		return EINVAL;

	RCC->ICSCR = (RCC->ICSCR & ~RCC_ICSCR_MSIRANGE) | range << RCC_ICSCR_MSIRANGE_Pos;

	RCC->CR |= RCC_CR_MSION;
	while ((RCC->CR & RCC_CR_MSIRDY) == 0);	// wait until MSI is stable
	return 0;
}

void enableHse(const bool bypass)
{
	RCC->CR = (RCC->CR & ~RCC_CR_HSEBYP) | bypass << RCC_CR_HSEBYP_Pos;
	RCC->CR |= RCC_CR_HSEON;	/// \todo check whether this can be merged with previous write of RCC->CR
	while ((RCC->CR & RCC_CR_HSERDY) == 0);	// wait until HSE oscillator is stable
}

void enableHsi16()
{
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0);	// wait until HSI oscillator is stable
}

#ifdef STM32L0X2_STM32L0X3_RCC_FEATURES

void enableHsi48()
{
	RCC->CRRCR |= RCC_CRRCR_HSI48ON;
	while ((RCC->CRRCR & RCC_CRRCR_HSI48RDY) == 0);	// wait until HSI48 clock is stable
}

#endif	// def STM32L0X2_STM32L0X3_RCC_FEATURES

int enablePll(const uint8_t pllmul, const uint8_t plldiv)
{
	if (plldiv < minPlldiv || plldiv > maxPlldiv)
		return EINVAL;

	static const std::pair<decltype(pllmul), decltype(RCC_CFGR_PLLMUL3)> associations[]
	{
			{pllMul3, RCC_CFGR_PLLMUL3},
			{pllMul4, RCC_CFGR_PLLMUL4},
			{pllMul6, RCC_CFGR_PLLMUL6},
			{pllMul8, RCC_CFGR_PLLMUL8},
			{pllMul12, RCC_CFGR_PLLMUL12},
			{pllMul16, RCC_CFGR_PLLMUL16},
			{pllMul24, RCC_CFGR_PLLMUL24},
			{pllMul32, RCC_CFGR_PLLMUL32},
			{pllMul48, RCC_CFGR_PLLMUL48},
	};

	for (auto& association : associations)
		if (association.first == pllmul)
		{
			RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLMUL | RCC_CFGR_PLLDIV)) |
					association.second | (plldiv - 1) << RCC_CFGR_PLLDIV_Pos;

			RCC->CR |= RCC_CR_PLLON;
			while ((RCC->CR & RCC_CR_PLLRDY) == 0);	// wait until PLL is stable
			return 0;
		}

	return EINVAL;
}

void switchSystemClock(const SystemClockSource source)
{
	const auto sourceValue = static_cast<uint32_t>(source);
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | sourceValue << RCC_CFGR_SW_Pos;
	while ((RCC->CFGR & RCC_CFGR_SWS) != sourceValue << RCC_CFGR_SWS_Pos);
}

}	// namespace chip

}	// namespace distortos
