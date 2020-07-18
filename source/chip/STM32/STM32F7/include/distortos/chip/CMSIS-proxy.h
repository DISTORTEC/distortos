/**
 * \file
 * \brief CMSIS proxy header for STM32F7
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_
#define SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_

#include "distortos/distortosConfiguration.h"

#if defined(DISTORTOS_CHIP_STM32F722)
	#define STM32F722xx
#elif defined(DISTORTOS_CHIP_STM32F723)
	#define STM32F723xx
#elif defined(DISTORTOS_CHIP_STM32F732)
	#define STM32F732xx
#elif defined(DISTORTOS_CHIP_STM32F733)
	#define STM32F733xx
#elif defined(DISTORTOS_CHIP_STM32F745)
	#define STM32F745xx
#elif defined(DISTORTOS_CHIP_STM32F746)
	#define STM32F746xx
#elif defined(DISTORTOS_CHIP_STM32F756)
	#define STM32F756xx
#elif defined(DISTORTOS_CHIP_STM32F765)
	#define STM32F765xx
#elif defined(DISTORTOS_CHIP_STM32F767)
	#define STM32F767xx
#elif defined(DISTORTOS_CHIP_STM32F769)
	#define STM32F769xx
#elif defined(DISTORTOS_CHIP_STM32F777)
	#define STM32F777xx
#elif defined(DISTORTOS_CHIP_STM32F779)
	#define STM32F779xx
#else
	#error "Unsupported STM32F7 chip!"
#endif

#include "stm32f7xx.h"

#endif	/* SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_ */
