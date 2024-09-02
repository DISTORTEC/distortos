/**
 * \file
 * \brief CMSIS proxy header for STM32U5
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32U5_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_
#define SOURCE_CHIP_STM32_STM32U5_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_

#include "distortos/distortosConfiguration.h"


#if defined(DISTORTOS_CHIP_STM32U535)
	#define STM32U535xx
#elif defined(DISTORTOS_CHIP_STM32U545)
	#define STM32U545xx
#elif defined(DISTORTOS_CHIP_STM32U575)
	#define STM32U575xx
#elif defined(DISTORTOS_CHIP_STM32U585)
	#define STM32U585xx
#elif defined(DISTORTOS_CHIP_STM32U595)
	#define STM32U595xx
#elif defined(DISTORTOS_CHIP_STM32U599)
	#define STM32U599xx
#elif defined(DISTORTOS_CHIP_STM32U5A5)
	#define STM32U5A5xx
#elif defined(DISTORTOS_CHIP_STM32U5A9)
	#define STM32U5A9xx
#elif defined(DISTORTOS_CHIP_STM32U5F7)
	#define STM32U5F7xx
#elif defined(DISTORTOS_CHIP_STM32U5F9)
	#define STM32U5F9xx
#elif defined(DISTORTOS_CHIP_STM32U5G7)
	#define STM32U5G7xx
#elif defined(DISTORTOS_CHIP_STM32U5G9)
	#define STM32U5G9xx
#else
	#error "Unsupported STM32U5 chip!"
#endif

#include "stm32u5xx.h"

#endif	/* SOURCE_CHIP_STM32_STM32U5_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_ */
