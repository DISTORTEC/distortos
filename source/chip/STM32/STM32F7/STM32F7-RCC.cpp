/**
 * \file
 * \brief Implementation of RCC-related functions for STM32F7
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F7-RCC.hpp"

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

void configurePll48ClockSource(const bool pllsaip)
{
	RCC->DCKCFGR2 = (RCC->DCKCFGR2 & ~RCC_DCKCFGR2_CK48MSEL) | pllsaip << RCC_DCKCFGR2_CK48MSEL_Pos;
}

void configurePllClockSource(const bool hse)
{
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLSRC) | hse << RCC_PLLCFGR_PLLSRC_Pos;
}

int configurePllInputClockDivider(const uint8_t pllm)
{
	if (pllm < minPllm || pllm > maxPllm)
		return EINVAL;

	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | pllm << RCC_PLLCFGR_PLLM_Pos;
	return 0;
}

void disableHse()
{
	RCC->CR &= ~RCC_CR_HSEON;
}

void disablePll()
{
	RCC->CR &= ~RCC_CR_PLLON;
}

void disablePlli2s()
{
	RCC->CR &= ~RCC_CR_PLLI2SON;
}

void disablePllsai()
{
	RCC->CR &= ~RCC_CR_PLLSAION;
}

void enableHse(const bool bypass)
{
	RCC->CR = (RCC->CR & ~RCC_CR_HSEBYP) | bypass << RCC_CR_HSEBYP_Pos;
	RCC->CR |= RCC_CR_HSEON;
	while ((RCC->CR & RCC_CR_HSERDY) == 0);	// wait until HSE oscillator is stable
}

#if defined(DISTORTOS_CHIP_STM32F76) || defined(DISTORTOS_CHIP_STM32F77)
int enablePll(const uint16_t plln, const uint8_t pllp, const uint8_t pllq, const uint8_t pllr)
#else	// !defined(DISTORTOS_CHIP_STM32F76) && !defined(DISTORTOS_CHIP_STM32F77)
int enablePll(const uint16_t plln, const uint8_t pllp, const uint8_t pllq)
#endif	// !defined(DISTORTOS_CHIP_STM32F76) && !defined(DISTORTOS_CHIP_STM32F77)
{
	if (plln < minPlln || plln > maxPlln ||
			(pllp != pllpDiv2 && pllp != pllpDiv4 && pllp != pllpDiv6 && pllp != pllpDiv8) ||
			pllq < minPllq || pllq > maxPllq)
		return EINVAL;

#if defined(DISTORTOS_CHIP_STM32F76) || defined(DISTORTOS_CHIP_STM32F77)

	if (pllr < minPllr || pllr > maxPllr)
		return EINVAL;

	RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLR)) |
			plln << RCC_PLLCFGR_PLLN_Pos | (pllp / 2 - 1) << RCC_PLLCFGR_PLLP_Pos | pllq << RCC_PLLCFGR_PLLQ_Pos |
			pllr << RCC_PLLCFGR_PLLR_Pos;

#else	// !defined(DISTORTOS_CHIP_STM32F76) && !defined(DISTORTOS_CHIP_STM32F77)

	RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ)) |
			plln << RCC_PLLCFGR_PLLN_Pos | (pllp / 2 - 1) << RCC_PLLCFGR_PLLP_Pos | pllq << RCC_PLLCFGR_PLLQ_Pos;

#endif	// !defined(DISTORTOS_CHIP_STM32F76) && !defined(DISTORTOS_CHIP_STM32F77)

	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);	// wait until PLL is stable
	return 0;
}

#if defined(DISTORTOS_CHIP_STM32F72) || defined(DISTORTOS_CHIP_STM32F73)
int enablePlli2s(const uint16_t plli2sn, const uint8_t plli2sq, const uint8_t plli2sr)
#else	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)
int enablePlli2s(const uint16_t plli2sn, const uint8_t plli2sp, const uint8_t plli2sq, const uint8_t plli2sr)
#endif	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)
{
	if (plli2sn < minPlln || plli2sn > maxPlln || plli2sq < minPllq || plli2sq > maxPllq ||
			plli2sr < minPllr || plli2sr > maxPllr)
		return EINVAL;

#if defined(DISTORTOS_CHIP_STM32F72) || defined(DISTORTOS_CHIP_STM32F73)

	RCC->PLLI2SCFGR = (RCC->PLLI2SCFGR & ~(RCC_PLLI2SCFGR_PLLI2SN | RCC_PLLI2SCFGR_PLLI2SQ | RCC_PLLI2SCFGR_PLLI2SR)) |
			plli2sn << RCC_PLLI2SCFGR_PLLI2SN_Pos |
			plli2sq << RCC_PLLI2SCFGR_PLLI2SQ_Pos |
			plli2sr << RCC_PLLI2SCFGR_PLLI2SR_Pos;

#else	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)

	if (plli2sp != pllpDiv2 && plli2sp != pllpDiv4 && plli2sp != pllpDiv6 && plli2sp != pllpDiv8)
		return EINVAL;

	RCC->PLLI2SCFGR = (RCC->PLLI2SCFGR & ~(RCC_PLLI2SCFGR_PLLI2SN | RCC_PLLI2SCFGR_PLLI2SP | RCC_PLLI2SCFGR_PLLI2SQ |
			RCC_PLLI2SCFGR_PLLI2SR)) |
			plli2sn << RCC_PLLI2SCFGR_PLLI2SN_Pos |
			(plli2sp / 2 - 1) << RCC_PLLI2SCFGR_PLLI2SP_Pos |
			plli2sq << RCC_PLLI2SCFGR_PLLI2SQ_Pos |
			plli2sr << RCC_PLLI2SCFGR_PLLI2SR_Pos;

#endif	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)

	RCC->CR |= RCC_CR_PLLI2SON;
	while ((RCC->CR & RCC_CR_PLLI2SRDY) == 0);	// wait until PLLI2S is stable
	return 0;
}

#if defined(DISTORTOS_CHIP_STM32F72) || defined(DISTORTOS_CHIP_STM32F73)
int enablePllsai(const uint16_t pllsain, const uint8_t pllsaip, const uint8_t pllsaiq)
#else	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)
int enablePllsai(const uint16_t pllsain, const uint8_t pllsaip, const uint8_t pllsaiq, const uint8_t pllsair)
#endif	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)
{
	if (pllsain < minPlln || pllsain > maxPlln ||
			(pllsaip != pllpDiv2 && pllsaip != pllpDiv4 && pllsaip != pllpDiv6 && pllsaip != pllpDiv8) ||
			pllsaiq < minPllq || pllsaiq > maxPllq)
		return EINVAL;

#if defined(DISTORTOS_CHIP_STM32F72) || defined(DISTORTOS_CHIP_STM32F73)

	RCC->PLLSAICFGR = (RCC->PLLSAICFGR & ~(RCC_PLLSAICFGR_PLLSAIN | RCC_PLLSAICFGR_PLLSAIP | RCC_PLLSAICFGR_PLLSAIQ)) |
			pllsain << RCC_PLLSAICFGR_PLLSAIN_Pos |
			(pllsaip / 2 - 1) << RCC_PLLSAICFGR_PLLSAIP_Pos |
			pllsaiq << RCC_PLLSAICFGR_PLLSAIQ_Pos;

#else	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)

	if (pllsair < minPllr || pllsair > maxPllr)
		return EINVAL;

	RCC->PLLSAICFGR = (RCC->PLLSAICFGR & ~(RCC_PLLSAICFGR_PLLSAIN | RCC_PLLSAICFGR_PLLSAIP | RCC_PLLSAICFGR_PLLSAIQ |
			RCC_PLLSAICFGR_PLLSAIR)) |
			pllsain << RCC_PLLSAICFGR_PLLSAIN_Pos |
			(pllsaip / 2 - 1) << RCC_PLLSAICFGR_PLLSAIP_Pos |
			pllsaiq << RCC_PLLSAICFGR_PLLSAIQ_Pos |
			pllsair << RCC_PLLSAICFGR_PLLSAIR_Pos;

#endif	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)

	RCC->CR |= RCC_CR_PLLSAION;
	while ((RCC->CR & RCC_CR_PLLSAIRDY) == 0);	// wait until PLLSAI is stable
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
