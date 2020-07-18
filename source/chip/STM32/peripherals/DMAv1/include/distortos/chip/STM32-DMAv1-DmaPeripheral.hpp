/**
 * \file
 * \brief DmaPeripheral class header for DMAv1 in STM32
 *
 * \author Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_DMAV1_INCLUDE_DISTORTOS_CHIP_STM32_DMAV1_DMAPERIPHERAL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_DMAV1_INCLUDE_DISTORTOS_CHIP_STM32_DMAV1_DMAPERIPHERAL_HPP_

#include "distortos/chip/CMSIS-proxy.h"

#ifdef DMA_CSELR_C2S_Pos
#define DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS	DMA_CSELR_C2S_Pos
#else
#define DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS	0
#endif

namespace distortos
{

namespace chip
{

/// DmaPeripheral class is a raw DMA peripheral for DMAv1 in STM32
class DmaPeripheral
{
public:

	/**
	 * \brief DmaPeripheral's constructor
	 *
	 * \param [in] dmaBase is a base address of DMA peripheral
	 */

	constexpr explicit DmaPeripheral(const uintptr_t dmaBase) :
			dmaBase_{dmaBase}
	{

	}

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

	/**
	 * \return current value of CSELR register
	 */

	uint32_t readCselr() const
	{
		return getCselr();
	}

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

	/**
	 * \return current value of ISR register
	 */

	uint32_t readIsr() const
	{
		return getDma().ISR;
	}

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

	/**
	 * \brief Writes value to CSELR register.
	 *
	 * \param [in] cselr is the value that will be written to CSELR register
	 */

	void writeCselr(const uint32_t cselr) const
	{
		getCselr() = cselr;
	}

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

	/**
	 * \brief Writes value to IFCR register.
	 *
	 * \param [in] ifcr is the value that will be written to IFCR register
	 */

	void writeIfcr(const uint32_t ifcr) const
	{
		getDma().IFCR = ifcr;
	}

private:

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

	/**
	 * \return reference to CSELR register in DMA peripheral
	 */

	volatile uint32_t& getCselr() const
	{
#ifndef DMA1_CSELR_BASE

		return getDma().CSELR;

#else	// !def DMA1_CSELR_BASE

		// in some chip families (like L0 or L4) CSELR is not part of DMA_TypeDef struct (as in - for example - F0), but
		// it is the only member of separate DMA_Request_TypeDef struct

		constexpr uintptr_t cselrOffset {DMA1_CSELR_BASE - DMA1_BASE};
		return *(volatile uint32_t*)(dmaBase_ + cselrOffset);

#endif	// !def DMA1_CSELR_BASE
	}

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

	/**
	 * \return reference to DMA_TypeDef object
	 */

	DMA_TypeDef& getDma() const
	{
		return *reinterpret_cast<DMA_TypeDef*>(dmaBase_);
	}

	/// base address of DMA peripheral
	uintptr_t dmaBase_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_DMAV1_INCLUDE_DISTORTOS_CHIP_STM32_DMAV1_DMAPERIPHERAL_HPP_
