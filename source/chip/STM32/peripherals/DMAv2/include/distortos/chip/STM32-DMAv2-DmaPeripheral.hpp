/**
 * \file
 * \brief DmaPeripheral class header for DMAv2 in STM32
 *
 * \author Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_DMAV2_INCLUDE_DISTORTOS_CHIP_STM32_DMAV2_DMAPERIPHERAL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_DMAV2_INCLUDE_DISTORTOS_CHIP_STM32_DMAV2_DMAPERIPHERAL_HPP_

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/// DmaPeripheral class is a raw DMA peripheral for DMAv2 in STM32
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

	/**
	 * \return current value of HISR register
	 */

	uint32_t readHisr() const
	{
		return getDma().HISR;
	}

	/**
	 * \return current value of LISR register
	 */

	uint32_t readLisr() const
	{
		return getDma().LISR;
	}

	/**
	 * \brief Writes value to HIFCR register.
	 *
	 * \param [in] hifcr is the value that will be written to HIFCR register
	 */

	void writeHifcr(const uint32_t hifcr) const
	{
		getDma().HIFCR = hifcr;
	}

	/**
	 * \brief Writes value to LIFCR register.
	 *
	 * \param [in] lifcr is the value that will be written to LIFCR register
	 */

	void writeLifcr(const uint32_t lifcr) const
	{
		getDma().LIFCR = lifcr;
	}

private:

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

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_DMAV2_INCLUDE_DISTORTOS_CHIP_STM32_DMAV2_DMAPERIPHERAL_HPP_
