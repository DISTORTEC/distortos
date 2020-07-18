/**
 * \file
 * \brief Definitions of low-level SD/MMC card drivers for SDMMCv1 in ST,32F429IDISCOVERY (ST,STM32F429ZI chip)
 *
 * \author Copyright (C) 2019-2020 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/chip/sdmmcs.hpp"

#include "distortos/chip/dmas.hpp"
#include "distortos/chip/PinInitializer.hpp"
#include "distortos/chip/SdMmcCardLowLevel.hpp"
#include "distortos/chip/STM32-SDMMCv1-SdmmcPeripheral.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

/**
 * \brief Generates name of DMA channel object in the form `dma<dmaId>Channel<channelId>`.
 *
 * \param [in] dmaId is a DMA identifier
 * \param [in] channelId is a DMA channel identifier
 */

#define DMA_CHANNEL(dmaId, channelId)	CONCATENATE4(dma, dmaId, Channel, channelId)

namespace distortos
{

namespace chip
{

#ifdef DISTORTOS_CHIP_SDIO_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SDIO
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

/**
 * \brief Low-level chip initializer for SDIO
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void sdioLowLevelInitializer()
{
#if defined(RCC_APB2ENR_SDIOEN)
	RCC->APB2ENR |= RCC_APB2ENR_SDIOEN;
#else
	#error "Unsupported bus for SDIO!"
#endif
}

BIND_LOW_LEVEL_INITIALIZER(50, sdioLowLevelInitializer);

/// raw SDIO peripheral
const SdmmcPeripheral sdioPeripheral {SDIO_BASE, sdioclkFrequency};

}	// namespace

SdMmcCardLowLevel sdio
{
		sdioPeripheral,
		DMA_CHANNEL(DISTORTOS_CHIP_SDIO_DMA, DISTORTOS_CHIP_SDIO_DMA_CHANNEL),
		DISTORTOS_CHIP_SDIO_DMA_REQUEST
};

/**
 * \brief SDIO interrupt handler
 */

extern "C" void SDIO_IRQHandler()
{
	sdio.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_SDIO_ENABLE

}	// namespace chip

}	// namespace distortos
