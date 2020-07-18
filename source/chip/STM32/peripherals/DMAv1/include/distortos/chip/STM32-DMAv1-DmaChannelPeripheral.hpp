/**
 * \file
 * \brief DmaChannelPeripheral class header for DMAv1 in STM32
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_DMAV1_INCLUDE_DISTORTOS_CHIP_STM32_DMAV1_DMACHANNELPERIPHERAL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_DMAV1_INCLUDE_DISTORTOS_CHIP_STM32_DMAV1_DMACHANNELPERIPHERAL_HPP_

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/// DmaChannelPeripheral class is a raw DMA channel peripheral for DMAv1 in STM32
class DmaChannelPeripheral
{
public:

	/**
	 * \brief DmaChannelPeripheral's constructor
	 *
	 * \param [in] dmaBase is a base address of DMA peripheral
	 * \param [in] channelId is a DMA channel identifier
	 */

	constexpr DmaChannelPeripheral(const uintptr_t dmaBase, const uint8_t channelId) :
			dmaChannelBase_{dmaBase + (DMA1_Channel1_BASE - DMA1_BASE) +
					(channelId - 1) * (DMA1_Channel2_BASE - DMA1_Channel1_BASE)},
			channelId_{channelId}
	{

	}

	/**
	 * \return DMA channel identifier
	 */

	uint8_t getChannelId() const
	{
		return channelId_;
	}

	/**
	 * \return current value of CCR register
	 */

	uint32_t readCcr() const
	{
		return getDmaChannel().CCR;
	}

	/**
	 * \return current value of CMAR register
	 */

	uint32_t readCmar() const
	{
		return getDmaChannel().CMAR;
	}

	/**
	 * \return current value of CNDTR register
	 */

	uint32_t readCndtr() const
	{
		return getDmaChannel().CNDTR;
	}

	/**
	 * \return current value of CPAR register
	 */

	uint32_t readCpar() const
	{
		return getDmaChannel().CPAR;
	}

	/**
	 * \brief Writes value to CCR register.
	 *
	 * \param [in] ccr is the value that will be written to CCR register
	 */

	void writeCcr(const uint32_t ccr) const
	{
		getDmaChannel().CCR = ccr;
	}

	/**
	 * \brief Writes value to CMAR register.
	 *
	 * \param [in] cmar is the value that will be written to CMAR register
	 */

	void writeCmar(const uint32_t cmar) const
	{
		getDmaChannel().CMAR = cmar;
	}

	/**
	 * \brief Writes value to CNDTR register.
	 *
	 * \param [in] cndtr is the value that will be written to CNDTR register
	 */

	void writeCndtr(const uint32_t cndtr) const
	{
		getDmaChannel().CNDTR = cndtr;
	}

	/**
	 * \brief Writes value to CPAR register.
	 *
	 * \param [in] cpar is the value that will be written to CPAR register
	 */

	void writeCpar(const uint32_t cpar) const
	{
		getDmaChannel().CPAR = cpar;
	}

private:

	/**
	 * \return reference to DMA_Channel_TypeDef object
	 */

	DMA_Channel_TypeDef& getDmaChannel() const
	{
		return *reinterpret_cast<DMA_Channel_TypeDef*>(dmaChannelBase_);
	}

	/// base address of DMA channel peripheral
	uintptr_t dmaChannelBase_;

	/// DMA channel identifier
	uint8_t channelId_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_DMAV1_INCLUDE_DISTORTOS_CHIP_STM32_DMAV1_DMACHANNELPERIPHERAL_HPP_
