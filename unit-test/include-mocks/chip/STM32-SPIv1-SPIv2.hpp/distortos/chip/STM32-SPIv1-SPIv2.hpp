/**
 * \file
 * \brief Mock of STM32 SPIv1 & SPIv2 functions
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_SPIV1_SPIV2_HPP_DISTORTOS_CHIP_STM32_SPIV1_SPIV2_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_SPIV1_SPIV2_HPP_DISTORTOS_CHIP_STM32_SPIV1_SPIV2_HPP_

#include "unit-test-common.hpp"

#include "distortos/devices/communication/SpiMode.hpp"

namespace distortos
{

namespace chip
{

class SpiPeripheral;

class Stm32Spiv1Spiv2Mock
{
public:

	Stm32Spiv1Spiv2Mock()
	{
		auto& instance = getInstanceInternal();
		REQUIRE(instance == nullptr);
		instance = this;
	}

	~Stm32Spiv1Spiv2Mock()
	{
		auto& instance = getInstanceInternal();
		REQUIRE(instance != nullptr);
		instance = {};
	}

	MAKE_MOCK5(configureSpi, uint32_t(const SpiPeripheral&, devices::SpiMode, uint32_t, uint8_t, bool));

	static Stm32Spiv1Spiv2Mock& getInstance()
	{
		const auto instance = getInstanceInternal();
		REQUIRE(instance != nullptr);
		return *instance;
	}

private:

	static Stm32Spiv1Spiv2Mock*& getInstanceInternal()
	{
		static Stm32Spiv1Spiv2Mock* instance;
		return instance;
	}
};

inline static uint32_t configureSpi(const SpiPeripheral& spiPeripheral, const devices::SpiMode mode,
		const uint32_t clockFrequency, const uint8_t wordLength, const bool lsbFirst)
{
	return Stm32Spiv1Spiv2Mock::getInstance().configureSpi(spiPeripheral, mode, clockFrequency, wordLength, lsbFirst);
}

}	// namespace chip

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_SPIV1_SPIV2_HPP_DISTORTOS_CHIP_STM32_SPIV1_SPIV2_HPP_
