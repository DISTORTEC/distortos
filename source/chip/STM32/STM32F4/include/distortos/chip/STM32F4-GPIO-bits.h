/**
 * \file
 * \brief Header with definition of bits in GPIO control registers
 *
 * This file covers devices as described in following places:
 * - RM0368 reference manual (STM32F401xB/C and STM32F401xD/E), Revision 4, 2015-05-04
 * - RM0090 reference manual (STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439), Revision 11, 2015-10-20
 * - RM0401 reference manual (STM32F410), Revision 2, 2015-10-26
 * - RM0383 reference manual (STM32F411xC/E), Revision 1, 2014-07-24
 * - RM0390 reference manual (STM32F446xx), Revision 1, 2015-03-17
 * - RM0386 reference manual (STM32F469xx and STM32F479xx), Revision 2, 2015-11-19
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_GPIO_BITS_H_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_GPIO_BITS_H_

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

#endif /* SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_GPIO_BITS_H_ */
