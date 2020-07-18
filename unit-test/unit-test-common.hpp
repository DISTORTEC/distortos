/**
 * \file
 * \brief Common header for unit test
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_UNIT_TEST_COMMON_HPP_
#define UNIT_TEST_UNIT_TEST_COMMON_HPP_

#include "catch.hpp"
#include "trompeloeil.hpp"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

#endif	// UNIT_TEST_UNIT_TEST_COMMON_HPP_
