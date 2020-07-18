/**
 * \file
 * \brief CMSIS proxy header for STM32F4
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_

#include "distortos/distortosConfiguration.h"

#if defined(DISTORTOS_CHIP_STM32F401CB) || defined(DISTORTOS_CHIP_STM32F401CC) || \
		defined(DISTORTOS_CHIP_STM32F401RB) || defined(DISTORTOS_CHIP_STM32F401RC) || \
		defined(DISTORTOS_CHIP_STM32F401VB) || defined(DISTORTOS_CHIP_STM32F401VC)
	#define STM32F401xC
#elif defined(DISTORTOS_CHIP_STM32F401CD) || defined(DISTORTOS_CHIP_STM32F401CE) || \
		defined(DISTORTOS_CHIP_STM32F401RD) || defined(DISTORTOS_CHIP_STM32F401RE) || \
		defined(DISTORTOS_CHIP_STM32F401VD) || defined(DISTORTOS_CHIP_STM32F401VE)
	#define STM32F401xE
#elif defined(DISTORTOS_CHIP_STM32F405)
	#define STM32F405xx
#elif defined(DISTORTOS_CHIP_STM32F407)
	#define STM32F407xx
#elif defined(DISTORTOS_CHIP_STM32F410C8) || defined(DISTORTOS_CHIP_STM32F410CB)
	#define STM32F410Cx
#elif defined(DISTORTOS_CHIP_STM32F410R8) || defined(DISTORTOS_CHIP_STM32F410RB)
	#define STM32F410Rx
#elif defined(DISTORTOS_CHIP_STM32F410T8) || defined(DISTORTOS_CHIP_STM32F410TB)
	#define STM32F410Tx
#elif defined(DISTORTOS_CHIP_STM32F411)
	#define STM32F411xE
#elif defined(DISTORTOS_CHIP_STM32F412C)
	#define STM32F412Cx
#elif defined(DISTORTOS_CHIP_STM32F412R)
	#define STM32F412Rx
#elif defined(DISTORTOS_CHIP_STM32F412V)
	#define STM32F412Vx
#elif defined(DISTORTOS_CHIP_STM32F412Z)
	#define STM32F412Zx
#elif defined(DISTORTOS_CHIP_STM32F413)
	#define STM32F413xx
#elif defined(DISTORTOS_CHIP_STM32F415)
	#define STM32F415xx
#elif defined(DISTORTOS_CHIP_STM32F417)
	#define STM32F417xx
#elif defined(DISTORTOS_CHIP_STM32F423)
	#define STM32F423xx
#elif defined(DISTORTOS_CHIP_STM32F427)
	#define STM32F427xx
#elif defined(DISTORTOS_CHIP_STM32F429)
	#define STM32F429xx
#elif defined(DISTORTOS_CHIP_STM32F437)
	#define STM32F437xx
#elif defined(DISTORTOS_CHIP_STM32F439)
	#define STM32F439xx
#elif defined(DISTORTOS_CHIP_STM32F446)
	#define STM32F446xx
#elif defined(DISTORTOS_CHIP_STM32F469)
	#define STM32F469xx
#elif defined(DISTORTOS_CHIP_STM32F479)
	#define STM32F479xx
#else
	#error "Unsupported STM32F4 chip!"
#endif

#include "stm32f4xx.h"

#endif	/* SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_ */
