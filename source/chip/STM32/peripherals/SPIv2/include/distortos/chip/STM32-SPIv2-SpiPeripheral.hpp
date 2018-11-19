/**
 * \file
 * \brief SpiPeripheral class header for SPIv2 in STM32
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_STM32_SPIV2_SPIPERIPHERAL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_STM32_SPIV2_SPIPERIPHERAL_HPP_

#include "distortos/chip/getBusFrequency.hpp"

namespace distortos
{

namespace chip
{

/// SpiPeripheral class is a raw SPI peripheral for SPIv2 in STM32
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
	 * \return current value of CR1 register
	 */

	uint32_t readCr1() const
	{
		return getSpi().CR1;
	}

	/**
	 * \return current value of CR2 register
	 */

	uint32_t readCr2() const
	{
		return getSpi().CR2;
	}

	/**
	 * \brief Reads current value of DR register.
	 *
	 * \param [in] wordLength selects word length, bits, [4; 16] or
	 * [ChipSpiMasterLowLevel::minWordLength; ChipSpiMasterLowLevel::maxWordLength]
	 *
	 * \return current value of DR register
	 */

	uint32_t readDr(const uint8_t wordLength) const
	{
		return wordLength <= 8 ? *reinterpret_cast<volatile uint8_t*>(&getSpi().DR) : getSpi().DR;
	}

	/**
	 * \return current value of SR register
	 */

	uint32_t readSr() const
	{
		return getSpi().SR;
	}

	/**
	 * \brief Writes value to CR1 register.
	 *
	 * \param [in] cr1 is the value that will be written to CR1 register
	 */

	void writeCr1(const uint32_t cr1) const
	{
		getSpi().CR1 = cr1;
	}

	/**
	 * \brief Writes value to CR2 register.
	 *
	 * \param [in] cr2 is the value that will be written to CR2 register
	 */

	void writeCr2(const uint32_t cr2) const
	{
		getSpi().CR2 = cr2;
	}

	/**
	 * \brief Writes value to DR register.
	 *
	 * \param [in] wordLength selects word length, bits, [4; 16] or
	 * [ChipSpiMasterLowLevel::minWordLength; ChipSpiMasterLowLevel::maxWordLength]
	 * \param [in] dr is the value that will be written to DR register
	 */

	void writeDr(const uint8_t wordLength, const uint32_t dr) const
	{
		if (wordLength <= 8)
			*reinterpret_cast<volatile uint8_t*>(&getSpi().DR) = dr;
		else
			getSpi().DR = dr;
	}

	/**
	 * \brief Writes value to SR register.
	 *
	 * \param [in] sr is the value that will be written to SR register
	 */

	void writeSr(const uint32_t sr) const
	{
		getSpi().SR = sr;
	}

private:

	/**
	 * \return reference to SPI_TypeDef object
	 */

	SPI_TypeDef& getSpi() const
	{
		return *reinterpret_cast<SPI_TypeDef*>(spiBase_);
	}

	/// base address of SPI peripheral
	uintptr_t spiBase_;

	/// peripheral clock frequency, Hz
	uint32_t peripheralFrequency_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_STM32_SPIV2_SPIPERIPHERAL_HPP_
