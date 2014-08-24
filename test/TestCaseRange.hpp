/**
 * \file
 * \brief TestCaseRange class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-24
 */

#ifndef TEST_TESTCASERANGE_HPP_
#define TEST_TESTCASERANGE_HPP_

#include "distortos/estd/ContiguousRange.hpp"
#include "distortos/estd/ReferenceHolder.hpp"

namespace distortos
{

namespace test
{

class TestCase;

/// range of references to TestCase objects
using TestCaseRange = estd::ContiguousRange<const estd::ReferenceHolder<const TestCase>>;

}	// namespace test

}	// namespace distortos

#endif	// TEST_TESTCASERANGE_HPP_
