/**
 * \file
 * \brief Mock of InterruptMaskingLock class
 *
 * \author Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_INTERRUPTMASKINGLOCK_HPP_DISTORTOS_INTERRUPTMASKINGLOCK_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_INTERRUPTMASKINGLOCK_HPP_DISTORTOS_INTERRUPTMASKINGLOCK_HPP_

#include "unit-test-common.hpp"

namespace distortos
{

class InterruptMaskingLock
{
public:

	class Proxy
	{
	public:

		Proxy()
		{
			auto& instance = getInstanceInternal();
			REQUIRE(instance == nullptr);
			instance = this;
		}

		~Proxy()
		{
			auto& instance = getInstanceInternal();
			REQUIRE(instance != nullptr);
			instance = {};
		}

		MAKE_MOCK0(construct, void());
		MAKE_MOCK0(destruct, void());

		static Proxy& getInstance()
		{
			const auto instance = getInstanceInternal();
			REQUIRE(instance != nullptr);
			return *instance;
		}

	private:

		static Proxy*& getInstanceInternal()
		{
			static Proxy* instance;
			return instance;
		}
	};

	InterruptMaskingLock()
	{
		Proxy::getInstance().construct();
	}

	~InterruptMaskingLock()
	{
		Proxy::getInstance().destruct();
	}
};

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_INTERRUPTMASKINGLOCK_HPP_DISTORTOS_INTERRUPTMASKINGLOCK_HPP_
