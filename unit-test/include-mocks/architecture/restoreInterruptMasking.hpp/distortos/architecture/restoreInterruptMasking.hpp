/**
 * \file
 * \brief Mocks of restoreInterruptMasking()
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_ARCHITECTURE_RESTOREINTERRUPTMASKING_HPP_DISTORTOS_ARCHITECTURE_RESTOREINTERRUPTMASKING_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_ARCHITECTURE_RESTOREINTERRUPTMASKING_HPP_DISTORTOS_ARCHITECTURE_RESTOREINTERRUPTMASKING_HPP_

#include "unit-test-common.hpp"

#include "distortos/architecture/InterruptMask.hpp"

namespace distortos
{

namespace architecture
{

class RestoreInterruptMaskingMock
{
public:

	RestoreInterruptMaskingMock()
	{
		REQUIRE(getInstanceInternal() == nullptr);
		getInstanceInternal() = this;
	}

	~RestoreInterruptMaskingMock()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		getInstanceInternal() = {};
	}

	MAKE_CONST_MOCK1(restoreInterruptMasking, void(InterruptMask));

	static const RestoreInterruptMaskingMock& getInstance()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		return *getInstanceInternal();
	}

private:

	static const RestoreInterruptMaskingMock*& getInstanceInternal()
	{
		static const RestoreInterruptMaskingMock* instance;
		return instance;
	}
};

void restoreInterruptMasking(const InterruptMask interruptMask)
{
	RestoreInterruptMaskingMock::getInstance().restoreInterruptMasking(interruptMask);
}

}	// namespace architecture

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_ARCHITECTURE_RESTOREINTERRUPTMASKING_HPP_DISTORTOS_ARCHITECTURE_RESTOREINTERRUPTMASKING_HPP_
