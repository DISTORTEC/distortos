/**
 * \file
 * \brief CMSIS proxy header for STM32L0
 *
 * \author Copyright (C) 2017-2019 Cezary Gapinski cezary.gapinski@gmail.com
 * \author Copyright (C) 2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_
#define SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_

#include "distortos/distortosConfiguration.h"

#if defined(DISTORTOS_CHIP_STM32L011)
	#define STM32L011xx
#elif defined(DISTORTOS_CHIP_STM32L021)
	#define STM32L021xx
#elif defined(DISTORTOS_CHIP_STM32L031)
	#define STM32L031xx
#elif defined(DISTORTOS_CHIP_STM32L041)
	#define STM32L041xx
#elif defined(DISTORTOS_CHIP_STM32L051)
	#define STM32L051xx
#elif defined(DISTORTOS_CHIP_STM32L052)
	#define STM32L052xx
#elif defined(DISTORTOS_CHIP_STM32L053)
	#define STM32L053xx
#elif defined(DISTORTOS_CHIP_STM32L061)
	#define STM32L061xx
#elif defined(DISTORTOS_CHIP_STM32L062)
	#define STM32L062xx
#elif defined(DISTORTOS_CHIP_STM32L063)
	#define STM32L063xx
#elif defined(DISTORTOS_CHIP_STM32L071)
	#define STM32L071xx
#elif defined(DISTORTOS_CHIP_STM32L072)
	#define STM32L072xx
#elif defined(DISTORTOS_CHIP_STM32L073)
	#define STM32L073xx
#elif defined(DISTORTOS_CHIP_STM32L081)
	#define STM32L081xx
#elif defined(DISTORTOS_CHIP_STM32L082)
	#define STM32L082xx
#elif defined(DISTORTOS_CHIP_STM32L083)
	#define STM32L083xx
#else
	#error "Unsupported STM32L0 chip!"
#endif

/** STM32L0 headers define "SVC_IRQn", distortos expects "SVCall_IRQn" */
#define SVC_IRQn							SVCall_IRQn

#include "stm32l0xx.h"

/* undefine all IRQHandler aliases, as these conflict with functions defined in vector table */

#ifdef ADC1_COMP_IRQHandler
	#undef ADC1_COMP_IRQHandler
#endif
#ifdef ADC1_IRQHandler
	#undef ADC1_IRQHandler
#endif
#ifdef AES_LPUART1_IRQHandler
	#undef AES_LPUART1_IRQHandler
#endif
#ifdef AES_RNG_LPUART1_IRQHandler
	#undef AES_RNG_LPUART1_IRQHandler
#endif
#ifdef DMA1_Channel4_5_6_7_IRQHandler
	#undef DMA1_Channel4_5_6_7_IRQHandler
#endif
#ifdef DMA1_Channel4_5_IRQHandler
	#undef DMA1_Channel4_5_IRQHandler
#endif
#ifdef LPUART1_IRQHandler
	#undef LPUART1_IRQHandler
#endif
#ifdef RCC_CRS_IRQHandler
	#undef RCC_CRS_IRQHandler
#endif
#ifdef RCC_IRQHandler
	#undef RCC_IRQHandler
#endif
#ifdef RNG_LPUART1_IRQHandler
	#undef RNG_LPUART1_IRQHandler
#endif
#ifdef TIM6_DAC_IRQHandler
	#undef TIM6_DAC_IRQHandler
#endif
#ifdef TIM6_IRQHandler
	#undef TIM6_IRQHandler
#endif

#endif	/* SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_ */
