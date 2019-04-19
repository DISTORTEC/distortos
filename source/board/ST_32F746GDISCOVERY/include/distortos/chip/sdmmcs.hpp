/**
 * \file
 * \brief Declarations of low-level SD/MMC card drivers for SDMMCv1 in ST,32F746GDISCOVERY (ST,STM32F746NG chip)
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#ifndef SOURCE_BOARD_ST_32F746GDISCOVERY_INCLUDE_DISTORTOS_CHIP_SDMMCS_HPP_
#define SOURCE_BOARD_ST_32F746GDISCOVERY_INCLUDE_DISTORTOS_CHIP_SDMMCS_HPP_

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

class SdMmcCardLowLevel;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_CHIP_SDMMC1_ENABLE

/// SD/MMC card low-level driver for SDMMC1
extern SdMmcCardLowLevel sdmmc1;

#endif	// def DISTORTOS_CHIP_SDMMC1_ENABLE

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_BOARD_ST_32F746GDISCOVERY_INCLUDE_DISTORTOS_CHIP_SDMMCS_HPP_
