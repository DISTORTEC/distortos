/**
 * \file
 * \brief Implementation of RCC-related functions for STM32G0
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32G0-RCC.hpp"

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
	static const std::pair<decltype(hpre), decltype(RCC->CFGR)> associations[]
	{
			{hpreDiv1, 0b0000 << RCC_CFGR_HPRE_Pos},
			{hpreDiv2, 0b1000 << RCC_CFGR_HPRE_Pos},
			{hpreDiv4, 0b1001 << RCC_CFGR_HPRE_Pos},
			{hpreDiv8, 0b1010 << RCC_CFGR_HPRE_Pos},
			{hpreDiv16, 0b1011 << RCC_CFGR_HPRE_Pos},
			{hpreDiv64, 0b1100 << RCC_CFGR_HPRE_Pos},
			{hpreDiv128, 0b1101 << RCC_CFGR_HPRE_Pos},
			{hpreDiv256, 0b1110 << RCC_CFGR_HPRE_Pos},
			{hpreDiv512, 0b1111 << RCC_CFGR_HPRE_Pos},
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
	static const std::pair<decltype(ppre), decltype(RCC->CFGR)> associations[]
	{
			{ppreDiv1, 0b000 << RCC_CFGR_PPRE_Pos},
			{ppreDiv2, 0b100 << RCC_CFGR_PPRE_Pos},
			{ppreDiv4, 0b101 << RCC_CFGR_PPRE_Pos},
			{ppreDiv8, 0b110 << RCC_CFGR_PPRE_Pos},
			{ppreDiv16, 0b111 << RCC_CFGR_PPRE_Pos},
	};

	for (auto& association : associations)
		if (association.first == ppre)
		{
			RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE) | association.second;
			return 0;
		}

	return EINVAL;
}

int configureHsi16ClockDivider(const uint8_t hsidiv)
{
	static const std::pair<decltype(hsidiv), decltype(RCC->CR)> associations[]
	{
			{hsi16Div1, 0b000 << RCC_CR_HSIDIV_Pos},
			{hsi16Div2, 0b001 << RCC_CR_HSIDIV_Pos},
			{hsi16Div4, 0b010 << RCC_CR_HSIDIV_Pos},
			{hsi16Div8, 0b011 << RCC_CR_HSIDIV_Pos},
			{hsi16Div16, 0b100 << RCC_CR_HSIDIV_Pos},
			{hsi16Div32, 0b101 << RCC_CR_HSIDIV_Pos},
			{hsi16Div64, 0b110 << RCC_CR_HSIDIV_Pos},
			{hsi16Div128, 0b111 << RCC_CR_HSIDIV_Pos},
	};

	for (auto& association : associations)
		if (association.first == hsidiv)
		{
			RCC->CR = (RCC->CR & ~RCC_CR_HSIDIV) | association.second;
			return 0;
		}

	return EINVAL;
}

void disableHse()
{
	RCC->CR &= ~RCC_CR_HSEON;
}

void disableHsi16()
{
	RCC->CR &= ~RCC_CR_HSION;
}

void disableLse()
{
	RCC->BDCR &= ~RCC_BDCR_LSEON;
}

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

void enableHsi16()
{
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0);	// wait until HSI16 oscillator is stable
}

void enableLse(const bool bypass, const LseDriveCapability lseDriveCapability)
{
	RCC->BDCR = (RCC->BDCR & ~(RCC_BDCR_LSEDRV | RCC_BDCR_LSEBYP)) |
			static_cast<uint32_t>(lseDriveCapability) << RCC_BDCR_LSEDRV_Pos | bypass << RCC_BDCR_LSEBYP_Pos;
	RCC->BDCR |= RCC_BDCR_LSEON;	/// \todo check whether this can be merged with previous write of RCC->BDCR
	while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0);	// wait until LSE oscillator is stable
}

int enablePll(const PllClockSource pllClockSource, const uint8_t pllm, const uint16_t plln, const uint8_t pllp,
		const uint8_t pllq, const uint8_t pllr)
{
	if (pllm < minPllm || pllm > maxPllm)
		return EINVAL;
	if (plln < minPlln || plln > maxPlln)
		return EINVAL;
	if (pllp < minPllp || pllp > maxPllp)
		return EINVAL;
	if (pllq < minPllq || pllq > maxPllq)
		return EINVAL;
	if (pllr < minPllr || pllr > maxPllr)
		return EINVAL;

	RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP |
			RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLR)) | static_cast<uint8_t>(pllClockSource) << RCC_PLLCFGR_PLLSRC_Pos |
			(pllm - 1) << RCC_PLLCFGR_PLLM_Pos | plln << RCC_PLLCFGR_PLLN_Pos | (pllp - 1) << RCC_PLLCFGR_PLLP_Pos |
			(pllq - 1) << RCC_PLLCFGR_PLLQ_Pos | (pllr - 1) << RCC_PLLCFGR_PLLR_Pos;

	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);	// wait until PLL is stable
	return 0;
}

void enablePllOutput(const PllOutput pllOutput, const bool enable)
{
	const auto mask = pllOutput == PllOutput::p ? RCC_PLLCFGR_PLLPEN :
			pllOutput == PllOutput::q ? RCC_PLLCFGR_PLLQEN : /* pllOutput == PllOutput::r ? */ RCC_PLLCFGR_PLLREN;
	if (enable == false)
		RCC->PLLCFGR &= ~mask;
	else
		RCC->PLLCFGR |= mask;
}

void switchSystemClock(const SystemClockSource source)
{
	const auto sourceValue = static_cast<uint32_t>(source);
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | sourceValue << RCC_CFGR_SW_Pos;
	while ((RCC->CFGR & RCC_CFGR_SWS) != sourceValue << RCC_CFGR_SWS_Pos);
}

}	// namespace chip

}	// namespace distortos
