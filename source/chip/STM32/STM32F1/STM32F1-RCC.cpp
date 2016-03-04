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
	RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV1) | (prediv << RCC_CFGR2_PREDIV1_bit);
#elif defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)
	RCC->CFGR2 = (RCC->CFGR2 & ~(prediv2 == true ? RCC_CFGR2_PREDIV2 : RCC_CFGR2_PREDIV1)) |
			(prediv << (prediv2 == true ? RCC_CFGR2_PREDIV2_bit : RCC_CFGR2_PREDIV1_bit));
#else	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)
	static_cast<void>(prediv2);	// suppress warning
	RCC_CFGR_PLLXTPRE_bb = prediv == 2;
#endif	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)
	return 0;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

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

}	// namespace chip

}	// namespace distortos
