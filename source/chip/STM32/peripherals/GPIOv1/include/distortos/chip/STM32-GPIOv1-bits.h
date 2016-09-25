/**
 * \file
 * \brief Header with definition of bits in STM32 GPIOv1 control registers
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV1_BITS_H_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV1_BITS_H_

/*---------------------------------------------------------------------------------------------------------------------+
| GPIOx_CRx - Port Configuration Register, Low & High
+---------------------------------------------------------------------------------------------------------------------*/

#define GPIO_CRx_MODE_IN_value				0
#define GPIO_CRx_MODE_OUT_10M_value			1
#define GPIO_CRx_MODE_OUT_2M_value			2
#define GPIO_CRx_MODE_OUT_50M_value			3
#define GPIO_CRx_MODE_mask					3

#define GPIO_CRx_CNF_IN_ANALOG_value		0
#define GPIO_CRx_CNF_IN_FLOATING_value		1
#define GPIO_CRx_CNF_IN_PULL_UP_DOWN_value	2
#define GPIO_CRx_CNF_OUT_PP_value			0
#define GPIO_CRx_CNF_OUT_OD_value			1
#define GPIO_CRx_CNF_ALT_PP_value			2
#define GPIO_CRx_CNF_ALT_OD_value			3
#define GPIO_CRx_CNF_mask					3

#endif	/* SOURCE_CHIP_STM32_PERIPHERALS_GPIOV1_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV1_BITS_H_ */
