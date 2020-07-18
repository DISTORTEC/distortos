/**
 * \file
 * \brief DmaChannelPeripheral class header for DMAv2 in STM32
 *
 * \author Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_DMAV2_INCLUDE_DISTORTOS_CHIP_STM32_DMAV2_DMACHANNELPERIPHERAL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_DMAV2_INCLUDE_DISTORTOS_CHIP_STM32_DMAV2_DMACHANNELPERIPHERAL_HPP_

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/// DmaChannelPeripheral class is a raw DMA channel peripheral for DMAv2 in STM32
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
			dmaChannelBase_{dmaBase + (DMA1_Stream0_BASE - DMA1_BASE) +
					channelId * (DMA1_Stream1_BASE - DMA1_Stream0_BASE)},
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
	 * \return current value of CR register
	 */

	uint32_t readCr() const
	{
		return getDmaChannel().CR;
	}

	/**
	 * \return current value of FCR register
	 */

	uint32_t readFcr() const
	{
		return getDmaChannel().FCR;
	}

	/**
	 * \return current value of M0AR register
	 */

	uint32_t readM0ar() const
	{
		return getDmaChannel().M0AR;
	}

	/**
	 * \return current value of M1AR register
	 */

	uint32_t readM1ar() const
	{
		return getDmaChannel().M1AR;
	}

	/**
	 * \return current value of NDTR register
	 */

	uint32_t readNdtr() const
	{
		return getDmaChannel().NDTR;
	}

	/**
	 * \return current value of PAR register
	 */

	uint32_t readPar() const
	{
		return getDmaChannel().PAR;
	}

	/**
	 * \brief Writes value to CR register.
	 *
	 * \param [in] cr is the value that will be written to CR register
	 */

	void writeCr(const uint32_t cr) const
	{
		getDmaChannel().CR = cr;
	}

	/**
	 * \brief Writes value to FCR register.
	 *
	 * \param [in] fcr is the value that will be written to FCR register
	 */

	void writeFcr(const uint32_t fcr) const
	{
		getDmaChannel().FCR = fcr;
	}

	/**
	 * \brief Writes value to M0AR register.
	 *
	 * \param [in] m0ar is the value that will be written to M0AR register
	 */

	void writeM0ar(const uint32_t m0ar) const
	{
		getDmaChannel().M0AR = m0ar;
	}

	/**
	 * \brief Writes value to M1AR register.
	 *
	 * \param [in] m1ar is the value that will be written to M1AR register
	 */

	void writeM1ar(const uint32_t m1ar) const
	{
		getDmaChannel().M1AR = m1ar;
	}

	/**
	 * \brief Writes value to NDTR register.
	 *
	 * \param [in] ndtr is the value that will be written to NDTR register
	 */

	void writeNdtr(const uint32_t ndtr) const
	{
		getDmaChannel().NDTR = ndtr;
	}

	/**
	 * \brief Writes value to PAR register.
	 *
	 * \param [in] par is the value that will be written to PAR register
	 */

	void writePar(const uint32_t par) const
	{
		getDmaChannel().PAR = par;
	}

private:

	/**
	 * \return reference to DMA_Stream_TypeDef object
	 */

	DMA_Stream_TypeDef& getDmaChannel() const
	{
		return *reinterpret_cast<DMA_Stream_TypeDef*>(dmaChannelBase_);
	}

	/// base address of DMA channel peripheral
	uintptr_t dmaChannelBase_;

	/// DMA channel identifier
	uint8_t channelId_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_DMAV2_INCLUDE_DISTORTOS_CHIP_STM32_DMAV2_DMACHANNELPERIPHERAL_HPP_
