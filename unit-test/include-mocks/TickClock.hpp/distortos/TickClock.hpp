/**
 * \file
 * \brief Mock of TickClock class
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_TICKCLOCK_HPP_DISTORTOS_TICKCLOCK_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_TICKCLOCK_HPP_DISTORTOS_TICKCLOCK_HPP_

#include "unit-test-common.hpp"

#include "distortos/distortosConfiguration.h"

#include <chrono>

namespace distortos
{

class TickClock
{
public:

	using rep = int64_t;
	using period = std::ratio<1, DISTORTOS_TICK_FREQUENCY>;
	using duration = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<TickClock>;

	TickClock()
	{
		REQUIRE(getInstance() == nullptr);
		getInstance() = this;
	}

	~TickClock()
	{
		REQUIRE(getInstance() != nullptr);
		getInstance() = {};
	}

	MAKE_CONST_MOCK0(nowMock, time_point());

	static time_point now()
	{
		REQUIRE(getInstance() != nullptr);
		return getInstance()->nowMock();
	}

	constexpr static bool is_steady {true};

private:

	static const TickClock*& getInstance()
	{
		static const TickClock* instance;
		return instance;
	}
};

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_TICKCLOCK_HPP_DISTORTOS_TICKCLOCK_HPP_
