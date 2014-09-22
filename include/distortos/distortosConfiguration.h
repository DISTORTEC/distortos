/**
 * \file
 * \brief distortos configuration
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-05
 */

#ifndef INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_
#define INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_

#define CONFIG_CHIP_STM32F407

#define CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI 8

#define CONFIG_TICK_CLOCK 16000000

#define CONFIG_TICK_RATE_HZ 1000

#define CONFIG_ROUND_ROBIN_RATE_HZ	10

#endif	// INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_
