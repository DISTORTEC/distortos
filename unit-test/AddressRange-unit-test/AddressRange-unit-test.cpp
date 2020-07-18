/**
 * \file
 * \brief AddressRange test cases
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unit-test-common.hpp"

#include "AddressRange.hpp"

using trompeloeil::_;
using AddressRange = distortos::devices::AddressRange;

namespace Catch
{

    template<>
    struct is_range<AddressRange>
	{
        static const bool value = false;
    };

}	// namespace Catch

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing AddressRange equality operator", "[equality]")
{
	// empty ranges are equal, even with different addresses
	REQUIRE(AddressRange{0xfb3ca915, 0} == AddressRange{0xa8548382, 0});

	// identical ranges are equal
	REQUIRE(AddressRange{0x356be1c3, 0x923a8831} == AddressRange{0x356be1c3, 0x923a8831});
}

TEST_CASE("Testing AddressRange inequality operator", "[inequality]")
{
	// ranges with different address are not equal
	REQUIRE(AddressRange{0x05c88071, 0xf32b898e} != AddressRange{0xf644e0c6, 0xf32b898e});

	// ranges with different size are not equal
	REQUIRE(AddressRange{0x50bf8ca3, 0x0f3da509} != AddressRange{0x50bf8ca3, 0xecf977a0});
	REQUIRE(AddressRange{0xbcc22722, 0} != AddressRange{0x888ce13f, 0x9ed5d2fb});
	REQUIRE(AddressRange{0x014de31f, 0x0aa9103c} != AddressRange{0x714224ea, 0});

	// ranges with different address and size are not equal
	REQUIRE(AddressRange{0x8b88ca59, 0x3ee36e77} != AddressRange{0x7e719b0b, 0x2c8aa4a2});
}

TEST_CASE("Testing AddressRange bitwise AND operator", "[AND]")
{
	// intersecting ranges

	// r1:      ----------11111----
	// r2:      ----------22222----
	// r1 & r2: ----------&&&&&----
	REQUIRE((AddressRange{10, 5} & AddressRange{10, 5}) == AddressRange{10, 5});
	// r1:      ----------11111----
	// r2:      ---------22--------
	// r1 & r2: ----------&--------
	REQUIRE((AddressRange{10, 5} & AddressRange{9, 2}) == AddressRange{10, 1});
	// r1:      ----------11111----
	// r2:      --------------22---
	// r1 & r2: --------------&----
	REQUIRE((AddressRange{10, 5} & AddressRange{14, 2}) == AddressRange{14, 1});
	// r1:      ----------11111----
	// r2:      ----------2222-----
	// r1 & r2: ----------&&&&-----
	REQUIRE((AddressRange{10, 5} & AddressRange{10, 4}) == AddressRange{10, 4});
	// r1:      ----------11111----
	// r2:      -----------2222----
	// r1 & r2: -----------&&&&----
	REQUIRE((AddressRange{10, 5} & AddressRange{11, 4}) == AddressRange{11, 4});
	// r1:      ----------11111----
	// r2:      -----------222-----
	// r1 & r2: -----------&&&-----
	REQUIRE((AddressRange{10, 5} & AddressRange{11, 3}) == AddressRange{11, 3});
	// r1:      ----------11111----
	// r2:      ---------222222----
	// r1 & r2: ----------&&&&&----
	REQUIRE((AddressRange{10, 5} & AddressRange{9, 6}) == AddressRange{10, 5});
	// r1:      ----------11111----
	// r2:      ----------222222---
	// r1 & r2: ----------&&&&&----
	REQUIRE((AddressRange{10, 5} & AddressRange{10, 6}) == AddressRange{10, 5});
	// r1:      ----------11111----
	// r2:      ---------2222222---
	// r1 & r2: ----------&&&&&----
	REQUIRE((AddressRange{10, 5} & AddressRange{9, 7}) == AddressRange{10, 5});

	// non-intersecting ranges

	// r1: ----------11111----
	// r2: --------22---------
	REQUIRE((AddressRange{10, 5} & AddressRange{8, 2}) == AddressRange{});
	// r1: ----------11111----
	// r2: ---------------22--
	REQUIRE((AddressRange{10, 5} & AddressRange{15, 2}) == AddressRange{});
	// r1: ----------11111----
	// r2: -------22----------
	REQUIRE((AddressRange{10, 5} & AddressRange{7, 2}) == AddressRange{});
	// r1: ----------11111----
	// r2: ----------------22-
	REQUIRE((AddressRange{10, 5} & AddressRange{16, 2}) == AddressRange{});
	// r1: ----------11111----
	// r2: -------------------
	REQUIRE((AddressRange{10, 5} & AddressRange{12, 0}) == AddressRange{});
	// r1: -------------------
	// r2: ----------22222----
	REQUIRE((AddressRange{12, 0} & AddressRange{10, 5}) == AddressRange{});
	// r1: -------------------
	// r2: -------------------
	REQUIRE((AddressRange{10, 0} & AddressRange{10, 0}) == AddressRange{});
}

TEST_CASE("Testing AddressRange bitwise OR operator", "[OR]")
{
	// intersecting ranges

	// r1:      ----------11111----
	// r2:      ----------22222----
	// r1 | r2: ----------|||||----
	REQUIRE((AddressRange{10, 5} | AddressRange{10, 5}) == AddressRange{10, 5});
	// r1:      ----------11111----
	// r2:      ---------22--------
	// r1 | r2: ---------||||||----
	REQUIRE((AddressRange{10, 5} | AddressRange{9, 2}) == AddressRange{9, 6});
	// r1:      ----------11111----
	// r2:      --------------22---
	// r1 | r2: ----------||||||---
	REQUIRE((AddressRange{10, 5} | AddressRange{14, 2}) == AddressRange{10, 6});
	// r1:      ----------11111----
	// r2:      ----------2222-----
	// r1 | r2: ----------|||||----
	REQUIRE((AddressRange{10, 5} | AddressRange{10, 4}) == AddressRange{10, 5});
	// r1:      ----------11111----
	// r2:      -----------2222----
	// r1 | r2: ----------|||||----
	REQUIRE((AddressRange{10, 5} | AddressRange{11, 4}) == AddressRange{10, 5});
	// r1:      ----------11111----
	// r2:      -----------222-----
	// r1 | r2: ----------|||||----
	REQUIRE((AddressRange{10, 5} | AddressRange{11, 3}) == AddressRange{10, 5});
	// r1:      ----------11111----
	// r2:      ---------222222----
	// r1 | r2: ---------||||||----
	REQUIRE((AddressRange{10, 5} | AddressRange{9, 6}) == AddressRange{9, 6});
	// r1:      ----------11111----
	// r2:      ----------222222---
	// r1 | r2: ----------||||||---
	REQUIRE((AddressRange{10, 5} | AddressRange{10, 6}) == AddressRange{10, 6});
	// r1:      ----------11111----
	// r2:      ---------2222222---
	// r1 | r2: ---------|||||||---
	REQUIRE((AddressRange{10, 5} | AddressRange{9, 7}) == AddressRange{9, 7});

	// adjacent ranges

	// r1:      -----11111---------
	// r2:      ----------22222----
	// r1 | r2: -----||||||||||----
	REQUIRE((AddressRange{5, 5} | AddressRange{10, 5}) == AddressRange{5, 10});
	// r1:      ----------11111----
	// r2:      -----22222---------
	// r1 | r2: -----||||||||||----
	REQUIRE((AddressRange{10, 5} | AddressRange{5, 5}) == AddressRange{5, 10});

	// empty ranges

	// r1:      ----------11111----
	// r2:      -------------------
	// r1 | r2: ----------|||||----
	REQUIRE((AddressRange{10, 5} | AddressRange{12, 0}) == AddressRange{10, 5});
	// r1:      -------------------
	// r2:      ----------22222----
	// r1 | r2: ----------|||||----
	REQUIRE((AddressRange{12, 0} | AddressRange{10, 5}) == AddressRange{10, 5});
	// r1:      -------------------
	// r2:      -------------------
	// r1 | r2: -------------------
	REQUIRE((AddressRange{10, 0} | AddressRange{10, 0}) == AddressRange{});

	// non-adjacent ranges

	// r1:      -----11111---------
	// r2:      -----------22222---
	REQUIRE((AddressRange{5, 5} | AddressRange{11, 5}) == AddressRange{});
	// r1:      -----------11111---
	// r2:      -----22222---------
	REQUIRE((AddressRange{11, 5} | AddressRange{5, 5}) == AddressRange{});
}
