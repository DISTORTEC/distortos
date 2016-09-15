/**
 * \file
 * \brief Header with definition of bits in STM32 GPIOv2 control registers
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_GPIOV2_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV2_BITS_H_
#define SOURCE_CHIP_STM32_PERIPHERALS_GPIOV2_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV2_BITS_H_

/*---------------------------------------------------------------------------------------------------------------------+
| GPIOx_MODER - GPIO port mode register
+---------------------------------------------------------------------------------------------------------------------*/

#define GPIO_MODER_mask						3
#define GPIO_MODER_IN_value					0
#define GPIO_MODER_OUT_value				1
#define GPIO_MODER_ALT_value				2
#define GPIO_MODER_ANALOG_value				3

/*---------------------------------------------------------------------------------------------------------------------+
| GPIOx_OTYPER - GPIO port output type register
+---------------------------------------------------------------------------------------------------------------------*/

#define GPIO_OTYPER_mask					1
#define GPIO_OTYPER_PP_value				0
#define GPIO_OTYPER_OD_value				1

/*---------------------------------------------------------------------------------------------------------------------+
| GPIOx_OSPEEDR - GPIO port output speed register
+---------------------------------------------------------------------------------------------------------------------*/

#define GPIO_OSPEEDR_mask					3
#define GPIO_OSPEEDR_LOW_value				0
#define GPIO_OSPEEDR_MEDIUM_value			1
#define GPIO_OSPEEDR_HIGH_value				2
#define GPIO_OSPEEDR_VERY_HIGH_value		3

/*---------------------------------------------------------------------------------------------------------------------+
| GPIOx_PUPDR - GPIO port pull-up/pull-down register
+---------------------------------------------------------------------------------------------------------------------*/

#define GPIO_PUPDR_mask						3
#define GPIO_PUPDR_FLOATING_value			0
#define GPIO_PUPDR_PULL_UP_value			1
#define GPIO_PUPDR_PULL_DOWN_value			2

/*---------------------------------------------------------------------------------------------------------------------+
| GPIOx_AFRx - GPIO alternate function registers
+---------------------------------------------------------------------------------------------------------------------*/

#define GPIO_AFRx_mask						15

#endif	/* SOURCE_CHIP_STM32_PERIPHERALS_GPIOV2_INCLUDE_DISTORTOS_CHIP_STM32_GPIOV2_BITS_H_ */
