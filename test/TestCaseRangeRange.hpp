/**
 * \file
 * \brief TestCaseRangeRange class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-24
 */

#ifndef TEST_TESTCASERANGERANGE_HPP_
#define TEST_TESTCASERANGERANGE_HPP_

#include "TestCaseRange.hpp"

namespace distortos
{

namespace test
{

/// range of references to TestCaseRange objects
using TestCaseRangeRange = estd::ContiguousRange<const estd::ReferenceHolder<const TestCaseRange>>;

}	// namespace test

}	// namespace distortos

#endif	// TEST_TESTCASERANGERANGE_HPP_
