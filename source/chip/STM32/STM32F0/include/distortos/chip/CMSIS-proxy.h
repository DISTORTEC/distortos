/**
 * \file
 * \brief CMSIS proxy header for STM32F0
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_
#define SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_

#include "distortos/distortosConfiguration.h"

#if defined(DISTORTOS_CHIP_STM32F030F4) || defined(DISTORTOS_CHIP_STM32F030C6) || defined(DISTORTOS_CHIP_STM32F030K6)
	#define STM32F030x6
#elif defined(DISTORTOS_CHIP_STM32F030C8) || defined(DISTORTOS_CHIP_STM32F030R8)
	#define STM32F030x8
#elif defined(DISTORTOS_CHIP_STM32F030CC) || defined(DISTORTOS_CHIP_STM32F030RC)
	#define STM32F030xC
#elif defined(DISTORTOS_CHIP_STM32F031)
	#define STM32F031x6
#elif defined(DISTORTOS_CHIP_STM32F038)
	#define STM32F038xx
#elif defined(DISTORTOS_CHIP_STM32F042)
	#define STM32F042x6
#elif defined(DISTORTOS_CHIP_STM32F048)
	#define STM32F048x6
#elif defined(DISTORTOS_CHIP_STM32F051)
	#define STM32F051x8
#elif defined(DISTORTOS_CHIP_STM32F058)
	#define STM32F058xx
#elif defined(DISTORTOS_CHIP_STM32F070C6) || defined(DISTORTOS_CHIP_STM32F070F6)
	#define STM32F070x6
#elif defined(DISTORTOS_CHIP_STM32F070CB) || defined(DISTORTOS_CHIP_STM32F070RB)
	#define STM32F070xB
#elif defined(DISTORTOS_CHIP_STM32F071)
	#define STM32F071xB
#elif defined(DISTORTOS_CHIP_STM32F072)
	#define STM32F072xB
#elif defined(DISTORTOS_CHIP_STM32F078)
	#define STM32F078xx
#elif defined(DISTORTOS_CHIP_STM32F091)
	#define STM32F091xC
#elif defined(DISTORTOS_CHIP_STM32F098)
	#define STM32F098xx
#else
	#error "Unsupported STM32F0 chip!"
#endif

/** STM32F0 headers define "SVC_IRQn", distortos expects "SVCall_IRQn" */
#define SVC_IRQn							SVCall_IRQn

#include "stm32f0xx.h"

#endif	/* SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_ */
