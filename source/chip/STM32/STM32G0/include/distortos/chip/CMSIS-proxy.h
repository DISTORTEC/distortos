/**
 * \file
 * \brief CMSIS proxy header for STM32G0
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_
#define SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_

#include "distortos/distortosConfiguration.h"

#if defined(DISTORTOS_CHIP_STM32G030)
	#define STM32G030xx
#elif defined(DISTORTOS_CHIP_STM32G031)
	#define STM32G031xx
#elif defined(DISTORTOS_CHIP_STM32G041)
	#define STM32G041xx
#elif defined(DISTORTOS_CHIP_STM32G050)
	#define STM32G050xx
#elif defined(DISTORTOS_CHIP_STM32G051)
	#define STM32G051xx
#elif defined(DISTORTOS_CHIP_STM32G061)
	#define STM32G061xx
#elif defined(DISTORTOS_CHIP_STM32G070)
	#define STM32G070xx
#elif defined(DISTORTOS_CHIP_STM32G071)
	#define STM32G071xx
#elif defined(DISTORTOS_CHIP_STM32G081)
	#define STM32G081xx
#elif defined(DISTORTOS_CHIP_STM32G0B0)
	#define STM32G0B0xx
#elif defined(DISTORTOS_CHIP_STM32G0B1)
	#define STM32G0B1xx
#elif defined(DISTORTOS_CHIP_STM32G0C1)
	#define STM32G0C1xx
#else
	#error "Unsupported STM32G0 chip!"
#endif

#include "stm32g0xx.h"

#endif	/* SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_ */
