/**
 * \file
 * \brief CMSIS proxy header for STM32L4
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_
#define SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_

#include "distortos/distortosConfiguration.h"

#if defined(DISTORTOS_CHIP_STM32L431)
	#define STM32L431xx
#elif defined(DISTORTOS_CHIP_STM32L432)
	#define STM32L432xx
#elif defined(DISTORTOS_CHIP_STM32L433)
	#define STM32L433xx
#elif defined(DISTORTOS_CHIP_STM32L442)
	#define STM32L442xx
#elif defined(DISTORTOS_CHIP_STM32L443)
	#define STM32L443xx
#elif defined(DISTORTOS_CHIP_STM32L451)
	#define STM32L451xx
#elif defined(DISTORTOS_CHIP_STM32L452)
	#define STM32L452xx
#elif defined(DISTORTOS_CHIP_STM32L462)
	#define STM32L462xx
#elif defined(DISTORTOS_CHIP_STM32L471)
	#define STM32L471xx
#elif defined(DISTORTOS_CHIP_STM32L475)
	#define STM32L475xx
#elif defined(DISTORTOS_CHIP_STM32L476)
	#define STM32L476xx
#elif defined(DISTORTOS_CHIP_STM32L485)
	#define STM32L485xx
#elif defined(DISTORTOS_CHIP_STM32L486)
	#define STM32L486xx
#elif defined(DISTORTOS_CHIP_STM32L496)
	#define STM32L496xx
#elif defined(DISTORTOS_CHIP_STM32L4A6)
	#define STM32L4A6xx
#elif defined(DISTORTOS_CHIP_STM32L4R5)
	#define STM32L4R5xx
#elif defined(DISTORTOS_CHIP_STM32L4R7)
	#define STM32L4R7xx
#elif defined(DISTORTOS_CHIP_STM32L4R9)
	#define STM32L4R9xx
#elif defined(DISTORTOS_CHIP_STM32L4S5)
	#define STM32L4S5xx
#elif defined(DISTORTOS_CHIP_STM32L4S7)
	#define STM32L4S7xx
#elif defined(DISTORTOS_CHIP_STM32L4S9)
	#define STM32L4S9xx
#else
	#error "Unsupported STM32L4 chip!"
#endif

#include "stm32l4xx.h"

#endif /* SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_ */
