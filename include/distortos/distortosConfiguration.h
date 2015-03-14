/**
 * \file
 * \brief distortos configuration
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-14
 */

#ifndef INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_
#define INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_

/**
 * \brief chip used by system
 */

#define CONFIG_CHIP_STM32F407

/**
 * \brief max priority (inclusive) of interrupt handlers that can use system's functions
 *
 * \note "lower" priority has "higher" numeric value
 */

#define CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI 8

/**
 * \brief frequency of timer used for system ticks, Hz
 */

#define CONFIG_TICK_CLOCK 16000000

/**
 * \brief system's tick rate, Hz
 */

#define CONFIG_TICK_RATE_HZ 1000

/**
 * \brief round-robin rate, Hz
 */

#define CONFIG_ROUND_ROBIN_RATE_HZ	10

#endif	/* INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_ */
