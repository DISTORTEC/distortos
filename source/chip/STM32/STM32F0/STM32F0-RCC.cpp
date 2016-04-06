/**
 * \file
 * \brief Implementation of RCC-related functions for STM32F0
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F0-RCC.hpp"

#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32F0-RCC-bits.h"

#include <cerrno>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int configurePrediv(const uint8_t prediv)
{
	if (prediv < minPrediv || prediv > maxPrediv)
		return EINVAL;

	RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV) | ((prediv - 1) << RCC_CFGR2_PREDIV_bit);
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
	RCC->CR = (RCC->CR & ~RCC_CR_HSEBYP) | (bypass << RCC_CR_HSEBYP_bit);
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

	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC)) | ((pllmul - 2) << RCC_CFGR_PLLMUL_bit) |
			(static_cast<uint8_t>(pllClockSource) << RCC_CFGR_PLLSRC_bit);
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);	// wait until PLL is stable
	return 0;
}

}	// namespace chip

}	// namespace distortos
