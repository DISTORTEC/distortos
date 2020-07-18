/**
 * \file
 * \brief Implementation of RCC-related functions for STM32L4
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32L4-RCC.hpp"

#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32-bit-banding.h"

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

void configurePllClockSource(const PllClockSource pllClockSource)
{
	RCC->PLLCFGR =
			(RCC->PLLCFGR & ~RCC_PLLCFGR_PLLSRC) | static_cast<uint8_t>(pllClockSource) << RCC_PLLCFGR_PLLSRC_Pos;
}

int configurePllInputClockDivider(const uint8_t pllm)
{
	if (pllm < minPllm || pllm > maxPllm)
		return EINVAL;

	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | (pllm - 1) << RCC_PLLCFGR_PLLM_Pos;
	return 0;
}

void disableHse()
{
	STM32_BITBAND(RCC, CR, HSEON) = 0;
}

void disableHsi16()
{
	STM32_BITBAND(RCC, CR, HSION) = 0;
}

void disableMsi()
{
	STM32_BITBAND(RCC, CR, MSION) = 0;
}

void disablePll()
{
	STM32_BITBAND(RCC, CR, PLLON) = 0;
}

void enableHse(const bool bypass)
{
	STM32_BITBAND(RCC, CR, HSEBYP) = bypass;
	STM32_BITBAND(RCC, CR, HSEON) = 1;
	while (STM32_BITBAND(RCC, CR, HSERDY) == 0);	// wait until HSE oscillator is stable
}

void enableHsi16()
{
	STM32_BITBAND(RCC, CR, HSION) = 1;
	while (STM32_BITBAND(RCC, CR, HSIRDY) == 0);	// wait until HSI16 oscillator is stable
}

int enableMsi(const uint8_t range)
{
	if (range > maxMsi)
		return EINVAL;

	RCC->CR = (RCC->CR & ~RCC_CR_MSIRANGE) | range << RCC_CR_MSIRANGE_Pos;

	STM32_BITBAND(RCC, CR, MSIRGSEL) = 1;
	STM32_BITBAND(RCC, CR, MSION) = 1;
	while (STM32_BITBAND(RCC, CR, MSIRDY) == 0);	// wait until MSI oscillator is stable
	return 0;
}

int enablePll(const uint16_t plln, const uint8_t pllp, const uint8_t pllq, const uint8_t pllr)
{
	if (plln < minPlln || plln > maxPlln)
		return EINVAL;
	if (pllq != pllqDiv2 && pllq != pllqDiv4 && pllq != pllqDiv6 && pllq != pllqDiv8)
		return EINVAL;
	if (pllr != pllrDiv2 && pllr != pllrDiv4 && pllr != pllrDiv6 && pllr != pllrDiv8)
		return EINVAL;

#if defined(DISTORTOS_CHIP_STM32L43) || defined(DISTORTOS_CHIP_STM32L44) || defined(DISTORTOS_CHIP_STM32L45) || \
		defined(DISTORTOS_CHIP_STM32L46) || defined(DISTORTOS_CHIP_STM32L496) || defined(DISTORTOS_CHIP_STM32L4A6)

	if (pllp < minPllp || pllp > maxPllp)
		return EINVAL;

	RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLPDIV | RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLR)) |
			plln << RCC_PLLCFGR_PLLN_Pos | pllp << RCC_PLLCFGR_PLLPDIV_Pos |
			(pllq / 2 - 1) << RCC_PLLCFGR_PLLQ_Pos | (pllr / 2 - 1) << RCC_PLLCFGR_PLLR_Pos;

#else	// !defined(DISTORTOS_CHIP_STM32L43) && !defined(DISTORTOS_CHIP_STM32L44) &&
		// !defined(DISTORTOS_CHIP_STM32L45) && !defined(DISTORTOS_CHIP_STM32L46) &&
		// !defined(DISTORTOS_CHIP_STM32L496) && !defined(DISTORTOS_CHIP_STM32L4A6)

	if (pllp != pllpDiv7 && pllp != pllpDiv17)
		return EINVAL;

	RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLR)) |
			plln << RCC_PLLCFGR_PLLN_Pos | (pllp == pllpDiv17 ? RCC_PLLCFGR_PLLP : 0) |
			(pllq / 2 - 1) << RCC_PLLCFGR_PLLQ_Pos | (pllr / 2 - 1) << RCC_PLLCFGR_PLLR_Pos;

#endif	// !defined(DISTORTOS_CHIP_STM32L43) && !defined(DISTORTOS_CHIP_STM32L44) &&
		// !defined(DISTORTOS_CHIP_STM32L45) && !defined(DISTORTOS_CHIP_STM32L46) &&
		// !defined(DISTORTOS_CHIP_STM32L496) && !defined(DISTORTOS_CHIP_STM32L4A6)

	STM32_BITBAND(RCC, CR, PLLON) = 1;
	while (STM32_BITBAND(RCC, CR, PLLRDY) == 0);	// wait until PLL is stable
	return 0;
}

void enablePllOutput(const PllOutput pllOutput, const bool enable)
{
	if (pllOutput == PllOutput::p)
		STM32_BITBAND(RCC, PLLCFGR, PLLPEN) = enable;
	else if (pllOutput == PllOutput::q)
		STM32_BITBAND(RCC, PLLCFGR, PLLQEN) = enable;
	else	// if (pllOutput == PllOutput::r)
		STM32_BITBAND(RCC, PLLCFGR, PLLREN) = enable;
}

void switchSystemClock(const SystemClockSource source)
{
	const auto sourceValue = static_cast<uint32_t>(source);
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | sourceValue << RCC_CFGR_SW_Pos;
	while ((RCC->CFGR & RCC_CFGR_SWS) != sourceValue << RCC_CFGR_SWS_Pos);
}

}	// namespace chip

}	// namespace distortos
