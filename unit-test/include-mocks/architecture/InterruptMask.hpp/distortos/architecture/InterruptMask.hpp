/**
 * \file
 * \brief Mock of architecture::InterruptMask
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_ARCHITECTURE_INTERRUPTMASK_HPP_DISTORTOS_ARCHITECTURE_INTERRUPTMASK_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_ARCHITECTURE_INTERRUPTMASK_HPP_DISTORTOS_ARCHITECTURE_INTERRUPTMASK_HPP_

#include <cstdint>

namespace distortos
{

namespace architecture
{

/// interrupt mask
using InterruptMask = uint32_t;

}	// namespace architecture

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_ARCHITECTURE_INTERRUPTMASK_HPP_DISTORTOS_ARCHITECTURE_INTERRUPTMASK_HPP_
