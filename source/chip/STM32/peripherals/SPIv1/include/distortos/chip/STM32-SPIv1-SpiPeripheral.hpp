/**
 * \file
 * \brief SpiPeripheral class header for SPIv1 in STM32
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SPIV1_INCLUDE_DISTORTOS_CHIP_STM32_SPIV1_SPIPERIPHERAL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_SPIV1_INCLUDE_DISTORTOS_CHIP_STM32_SPIV1_SPIPERIPHERAL_HPP_

#include "distortos/chip/getBusFrequency.hpp"

namespace distortos
{

namespace chip
{

/// SpiPeripheral class is a raw SPI peripheral for SPIv1 in STM32
class SpiPeripheral
{
public:

	/**
	 * \brief SpiPeripheral's constructor
	 *
	 * \param [in] spiBase is a base address of SPI peripheral
	 */

	constexpr explicit SpiPeripheral(const uintptr_t spiBase) :
			spiBase_{spiBase},
			peripheralFrequency_{getBusFrequency(spiBase)}
	{

	}

	/**
	 * \return peripheral clock frequency, Hz
	 */

	uint32_t getPeripheralFrequency() const
	{
		return peripheralFrequency_;
	}

	/**
	 * \return reference to SPI_TypeDef object
	 */

	SPI_TypeDef& getSpi() const
	{
		return *reinterpret_cast<SPI_TypeDef*>(spiBase_);
	}

private:

	/// base address of SPI peripheral
	uintptr_t spiBase_;

	/// peripheral clock frequency, Hz
	uint32_t peripheralFrequency_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_SPIV1_INCLUDE_DISTORTOS_CHIP_STM32_SPIV1_SPIPERIPHERAL_HPP_
