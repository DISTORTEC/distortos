/**
 * \file
 * \brief CMSIS proxy header for architecture.
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_

#include "distortos/distortosConfiguration.h"

#if defined(CONFIG_CHIP_STM32F405)
	#define STM32F405xx
#elif defined (CONFIG_CHIP_STM32F415)
	#define STM32F415xx
#elif defined (CONFIG_CHIP_STM32F407)
	#define STM32F407xx
#elif defined (CONFIG_CHIP_STM32F417)
	#define STM32F417xx
#elif defined (CONFIG_CHIP_STM32F427)
	#define STM32F427xx
#elif defined (CONFIG_CHIP_STM32F437)
	#define STM32F437xx
#elif defined (CONFIG_CHIP_STM32F429)
	#define STM32F429xx
#elif defined (CONFIG_CHIP_STM32F439)
	#define STM32F439xx
#elif defined(CONFIG_CHIP_STM32F401CC) || defined(CONFIG_CHIP_STM32F401RC) || defined(CONFIG_CHIP_STM32F401VC)
	#define STM32F401xC
#elif defined(CONFIG_CHIP_STM32F401CE) || defined(CONFIG_CHIP_STM32F401RE) || defined(CONFIG_CHIP_STM32F401VE)
	#define STM32F401xE
#elif defined (CONFIG_CHIP_STM32F410T)
	#define STM32F410Tx
#elif defined (CONFIG_CHIP_STM32F410C)
	#define STM32F410Cx
#elif defined (CONFIG_CHIP_STM32F410R)
	#define STM32F410Rx
#elif defined (CONFIG_CHIP_STM32F411)
	#define STM32F411xE
#elif defined (CONFIG_CHIP_STM32F446)
	#define STM32F446xx
#elif defined (CONFIG_CHIP_STM32F469)
	#define STM32F469xx
#elif defined (CONFIG_CHIP_STM32F479)
	#define STM32F479xx
#else
	#error "Unsupported STM32F4 chip!"
#endif

#include "stm32f4xx.h"

#endif	/* SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_ */
