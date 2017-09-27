/**
 * \file
 * \brief Mocks of enableInterruptMasking()
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_ARCHITECTURE_ENABLEINTERRUPTMASKING_HPP_DISTORTOS_ARCHITECTURE_ENABLEINTERRUPTMASKING_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_ARCHITECTURE_ENABLEINTERRUPTMASKING_HPP_DISTORTOS_ARCHITECTURE_ENABLEINTERRUPTMASKING_HPP_

#include "unit-test-common.hpp"

#include "distortos/architecture/InterruptMask.hpp"

namespace distortos
{

namespace architecture
{

class EnableInterruptMaskingMock
{
public:

	EnableInterruptMaskingMock()
	{
		REQUIRE(getInstanceInternal() == nullptr);
		getInstanceInternal() = this;
	}

	~EnableInterruptMaskingMock()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		getInstanceInternal() = {};
	}

	MAKE_CONST_MOCK0(enableInterruptMasking, InterruptMask());

	static const EnableInterruptMaskingMock& getInstance()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		return *getInstanceInternal();
	}

private:

	static const EnableInterruptMaskingMock*& getInstanceInternal()
	{
		static const EnableInterruptMaskingMock* instance;
		return instance;
	}
};

InterruptMask enableInterruptMasking()
{
	return EnableInterruptMaskingMock::getInstance().enableInterruptMasking();
}

}	// namespace architecture

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_ARCHITECTURE_ENABLEINTERRUPTMASKING_HPP_DISTORTOS_ARCHITECTURE_ENABLEINTERRUPTMASKING_HPP_
