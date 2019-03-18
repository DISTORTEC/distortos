/**
 * \file
 * \brief Mocks of ThisThread
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_THISTHREAD_HPP_DISTORTOS_THISTHREAD_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_THISTHREAD_HPP_DISTORTOS_THISTHREAD_HPP_

#include "distortos/TickClock.hpp"

namespace distortos
{

class ThisThreadMock
{
public:

	ThisThreadMock()
	{
		auto& instance = getInstanceInternal();
		REQUIRE(instance == nullptr);
		instance = this;
	}

	~ThisThreadMock()
	{
		auto& instance = getInstanceInternal();
		REQUIRE(instance != nullptr);
		instance = {};
	}

	MAKE_CONST_MOCK1(sleepFor, int(TickClock::duration));

	static ThisThreadMock& getInstance()
	{
		const auto instance = getInstanceInternal();
		REQUIRE(instance != nullptr);
		return *instance;
	}

private:

	static ThisThreadMock*& getInstanceInternal()
	{
		static ThisThreadMock* instance;
		return instance;
	}
};

namespace ThisThread
{

inline static int sleepFor(const TickClock::duration duration)
{
	return ThisThreadMock::getInstance().sleepFor(duration);
}

template<typename Rep, typename Period>
inline static int sleepFor(const std::chrono::duration<Rep, Period> duration)
{
	return sleepFor(std::chrono::duration_cast<TickClock::duration>(duration));
}

}	// namespace ThisThread

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_THISTHREAD_HPP_DISTORTOS_THISTHREAD_HPP_
