/**
 * \file
 * \brief Header with definitions for bit-banding for STM32
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_STM32_BIT_BANDING_H_
#define SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_STM32_BIT_BANDING_H_

#include "distortos/architecture/ARMv7-M-bit-banding.h"

#if defined(DISTORTOS_ARCHITECTURE_ARM_CORTEX_M3) || defined(DISTORTOS_ARCHITECTURE_ARM_CORTEX_M4)

#include <stddef.h>

/*---------------------------------------------------------------------------------------------------------------------+
| global defines
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief implementation of STM32_BITBAND_ADDRESS() and STM32_BITBAND()
 *
 * \param [in] base is the base address of peripheral, [bitbandPeripheralBegin; bitbandPeripheralEnd]
 * \param [in] type is the type of struct with peripheral registers
 * \param [in] member is the member of \a type struct with peripheral registers
 * \param [in] mask is the mask of bit in \a member
 *
 * \return address of bit-band alias of bit described by provided arguments
 */

#define STM32_BITBAND_IMPLEMENTATION(base, type, member, mask) \
		BITBAND_PERIPHERAL_ADDRESS(base + offsetof(type, member), __builtin_ctzl(mask))

/**
 * \brief address of bit-band alias for peripheral region, alternate form for struct member and bitmask
 *
 * \warning This macro must not be used for `FLASH` registers! Use STM32_BITBAND_FLASH_ADDRESS() instead
 *
 * \param [in] registerr is the name of object with peripheral registers, e.g. `RCC`, `GPIOA`, `USART1`, ...
 * \param [in] member is the name of member of \a registerr, e.g. `BDCR` (for `RCC`), `ODR` (for `GPIOA`), `CR1` (for
 * `USART1`), ...
 * \param [in] bit is the name of bit in \a member, e.g. `RTCEN` (in `RCC->BDCR`), `ODR0` (in `GPIOA->ODR`), `TXEIE` (in
 * `USART1->CR1`), ...
 *
 * \return address of bit-band alias of \a bit in <em>registerr->member</em>
 */

#define STM32_BITBAND_ADDRESS(registerr, member, bit) \
		STM32_BITBAND_IMPLEMENTATION(registerr ## _BASE, registerr ## _TypeDef, member, \
		registerr ## _ ## member ## _ ## bit)

/**
 * \brief bit-band alias in peripheral region, alternate form for struct member and bitmask
 *
 * \warning This macro must not be used for `FLASH` registers! Use STM32_BITBAND_FLASH() instead
 *
 * \param [in] registerr is the name of object with peripheral registers, e.g. `RCC`, `GPIOA`, `USART1`, ...
 * \param [in] member is the name of member of \a registerr, e.g. `BDCR` (for `RCC`), `ODR` (for `GPIOA`), `CR1` (for
 * `USART1`), ...
 * \param [in] bit is the name of bit in \a member, e.g. `RTCEN` (in `RCC->BDCR`), `ODR0` (in `GPIOA->ODR`), `TXEIE` (in
 * `USART1->CR1`), ...
 *
 * \return reference to bit-band alias of \a bit in <em>registerr->member</em>
 */

#define STM32_BITBAND(registerr, member, bit) \
		(*(volatile unsigned long*)STM32_BITBAND_IMPLEMENTATION(registerr ## _BASE, registerr ## _TypeDef, member, \
		registerr ## _ ## member ## _ ## bit))

/**
 * \brief address of bit-band alias for FLASH registers, alternate form for struct member and bitmask
 *
 * \param [in] member is the name of member of `FLASH`, e.g. `SR`, `CR`, ...
 * \param [in] bit is the name of bit in \a member, e.g. `BSY` (in `FLASH->SR`), `LOCK` (in `FLASH->CR`), ...
 *
 * \return address of bit-band alias of \a bit in <em>FLASH->member</em>
 */

#define STM32_BITBAND_FLASH_ADDRESS(member, bit) \
		STM32_BITBAND_IMPLEMENTATION(FLASH_R_BASE, FLASH_TypeDef, member, FLASH_ ## member ## _ ## bit)

/**
 * \brief bit-band alias for FLASH registers, alternate form for struct member and bitmask
 *
 * \param [in] member is the name of member of `FLASH`, e.g. `SR`, `CR`, ...
 * \param [in] bit is the name of bit in \a member, e.g. `BSY` (in `FLASH->SR`), `LOCK` (in `FLASH->CR`), ...
 *
 * \return reference to bit-band alias of \a bit in <em>FLASH->member</em>
 */

#define STM32_BITBAND_FLASH(member, bit) \
		(*(volatile unsigned long*)STM32_BITBAND_IMPLEMENTATION(FLASH_R_BASE, FLASH_TypeDef, member, \
		FLASH_ ## member ## _ ## bit))

#endif	/* defined(DISTORTOS_ARCHITECTURE_ARM_CORTEX_M3) || defined(DISTORTOS_ARCHITECTURE_ARM_CORTEX_M4) */

#endif	/* SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_STM32_BIT_BANDING_H_ */
