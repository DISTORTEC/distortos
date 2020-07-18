/**
 * \file
 * \brief Mock of SynchronousSdMmcCardLowLevel class
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_DEVICES_MEMORY_SYNCHRONOUSSDMMCCARDLOWLEVEL_HPP_DISTORTOS_DEVICES_MEMORY_SYNCHRONOUSSDMMCCARDLOWLEVEL_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_DEVICES_MEMORY_SYNCHRONOUSSDMMCCARDLOWLEVEL_HPP_DISTORTOS_DEVICES_MEMORY_SYNCHRONOUSSDMMCCARDLOWLEVEL_HPP_

#include "unit-test-common.hpp"

#include "distortos/devices/memory/SdMmcCardBase.hpp"
#include "distortos/devices/memory/SdMmcCardLowLevel.hpp"

namespace distortos
{

namespace devices
{

namespace mock
{

class SynchronousSdMmcCardLowLevel
{
public:

	using Result = SdMmcCardBase::Result;

	SynchronousSdMmcCardLowLevel()
	{
		auto& instance = getInstanceInternal();
		REQUIRE(instance == nullptr);
		instance = this;
	}

	virtual ~SynchronousSdMmcCardLowLevel()
	{
		auto& instance = getInstanceInternal();
		if (instance == this)
			instance = {};
	}

	MAKE_CONST_MOCK2(configure, void(SdMmcCardLowLevel::BusMode, uint32_t));
	MAKE_MOCK4(executeTransaction, Result(uint8_t, uint32_t, SdMmcCardLowLevel::Response, SdMmcCardLowLevel::Transfer));
	MAKE_CONST_MOCK0(start, int());
	MAKE_CONST_MOCK0(stop, void());

	static SynchronousSdMmcCardLowLevel& getInstance()
	{
		const auto instance = getInstanceInternal();
		REQUIRE(instance != nullptr);
		return *instance;
	}

private:

	static SynchronousSdMmcCardLowLevel*& getInstanceInternal()
	{
		static SynchronousSdMmcCardLowLevel* instance;
		return instance;
	}
};

}	// namespace mock

class SynchronousSdMmcCardLowLevel
{
public:

	using Result = mock::SynchronousSdMmcCardLowLevel::Result;

	constexpr explicit SynchronousSdMmcCardLowLevel(SdMmcCardLowLevel&)
	{

	}

	void configure(const SdMmcCardLowLevel::BusMode busMode, const uint32_t clockFrequency) const
	{
		mock::SynchronousSdMmcCardLowLevel::getInstance().configure(busMode, clockFrequency);
	}

	Result executeTransaction(const uint8_t command, const uint32_t argument,
			const SdMmcCardLowLevel::Response response, const SdMmcCardLowLevel::Transfer transfer)
	{
		return mock::SynchronousSdMmcCardLowLevel::getInstance().executeTransaction(command, argument, response,
				transfer);
	}

	int start() const
	{
		return mock::SynchronousSdMmcCardLowLevel::getInstance().start();
	}

	void stop() const
	{
		mock::SynchronousSdMmcCardLowLevel::getInstance().stop();
	}
};

}	// namespace devices

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_DEVICES_MEMORY_SYNCHRONOUSSDMMCCARDLOWLEVEL_HPP_DISTORTOS_DEVICES_MEMORY_SYNCHRONOUSSDMMCCARDLOWLEVEL_HPP_
