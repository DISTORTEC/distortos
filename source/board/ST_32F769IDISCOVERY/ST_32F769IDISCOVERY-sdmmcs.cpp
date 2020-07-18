/**
 * \file
 * \brief Definitions of low-level SD/MMC card drivers for SDMMCv1 in ST,32F769IDISCOVERY (ST,STM32F769NI chip)
 *
 * \author Copyright (C) 2019-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
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

#ifdef DISTORTOS_CHIP_SDMMC1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SDMMC1
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

/**
 * \brief Low-level chip initializer for SDMMC1
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void sdmmc1LowLevelInitializer()
{
#if defined(RCC_APB2ENR_SDMMC1EN)
	RCC->APB2ENR |= RCC_APB2ENR_SDMMC1EN;
#else
	#error "Unsupported bus for SDMMC1!"
#endif
}

BIND_LOW_LEVEL_INITIALIZER(50, sdmmc1LowLevelInitializer);

/// raw SDMMC1 peripheral
const SdmmcPeripheral sdmmc1Peripheral {SDMMC1_BASE, sdmmc1clkFrequency};

}	// namespace

SdMmcCardLowLevel sdmmc1
{
		sdmmc1Peripheral,
		DMA_CHANNEL(DISTORTOS_CHIP_SDMMC1_DMA, DISTORTOS_CHIP_SDMMC1_DMA_CHANNEL),
		DISTORTOS_CHIP_SDMMC1_DMA_REQUEST
};

/**
 * \brief SDMMC1 interrupt handler
 */

extern "C" void SDMMC1_IRQHandler()
{
	sdmmc1.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_SDMMC1_ENABLE

#ifdef DISTORTOS_CHIP_SDMMC2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| SDMMC2
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

/// pin initializers for SDMMC2
const PinInitializer sdmmc2PinInitializers[]
{
		// SDMMC2 CK
		makeAlternateFunctionPinInitializer(Pin::pd6,
				PinAlternateFunction::af11,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::none),
		// SDMMC2 CMD
		makeAlternateFunctionPinInitializer(Pin::pd7,
				PinAlternateFunction::af11,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::none),
		// SDMMC2 D0
		makeAlternateFunctionPinInitializer(Pin::pg9,
				PinAlternateFunction::af11,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::none),
		// SDMMC2 D1
		makeAlternateFunctionPinInitializer(Pin::pg10,
				PinAlternateFunction::af11,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::none),
		// SDMMC2 D2
		makeAlternateFunctionPinInitializer(Pin::pb3,
				PinAlternateFunction::af10,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::none),
		// SDMMC2 D3
		makeAlternateFunctionPinInitializer(Pin::pb4,
				PinAlternateFunction::af10,
				false,
				PinOutputSpeed::veryHigh,
				PinPull::none),
};

/**
 * \brief Low-level chip initializer for SDMMC2
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void sdmmc2LowLevelInitializer()
{
#if defined(RCC_APB2ENR_SDMMC2EN)
	RCC->APB2ENR |= RCC_APB2ENR_SDMMC2EN;
#else
	#error "Unsupported bus for SDMMC2!"
#endif

	for (auto& pinInitializer : sdmmc2PinInitializers)
		pinInitializer();
}

BIND_LOW_LEVEL_INITIALIZER(50, sdmmc2LowLevelInitializer);

/// raw SDMMC2 peripheral
const SdmmcPeripheral sdmmc2Peripheral {SDMMC2_BASE, sdmmc2clkFrequency};

}	// namespace

SdMmcCardLowLevel sdmmc2
{
		sdmmc2Peripheral,
		DMA_CHANNEL(DISTORTOS_CHIP_SDMMC2_DMA, DISTORTOS_CHIP_SDMMC2_DMA_CHANNEL),
		DISTORTOS_CHIP_SDMMC2_DMA_REQUEST
};

/**
 * \brief SDMMC2 interrupt handler
 */

extern "C" void SDMMC2_IRQHandler()
{
	sdmmc2.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_SDMMC2_ENABLE

}	// namespace chip

}	// namespace distortos
