/**
 * \file
 * \brief Mock of getScheduler()
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_GETSCHEDULER_HPP_DISTORTOS_INTERNAL_SCHEDULER_GETSCHEDULER_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_GETSCHEDULER_HPP_DISTORTOS_INTERNAL_SCHEDULER_GETSCHEDULER_HPP_

#include "unit-test-common.hpp"

namespace distortos
{

namespace internal
{

class Scheduler;

class GetSchedulerMock
{
public:

	GetSchedulerMock()
	{
		REQUIRE(getInstanceInternal() == nullptr);
		getInstanceInternal() = this;
	}

	~GetSchedulerMock()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		getInstanceInternal() = {};
	}

	MAKE_CONST_MOCK0(getScheduler, Scheduler&());

	static const GetSchedulerMock& getInstance()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		return *getInstanceInternal();
	}

private:

	static const GetSchedulerMock*& getInstanceInternal()
	{
		static const GetSchedulerMock* instance;
		return instance;
	}
};

inline static Scheduler& getScheduler()
{
	return GetSchedulerMock::getInstance().getScheduler();
}

}	// namespace internal

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_INTERNAL_SCHEDULER_GETSCHEDULER_HPP_DISTORTOS_INTERNAL_SCHEDULER_GETSCHEDULER_HPP_
