/**
 * \file
 * \brief wasteTime() implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-04
 */

#include "wasteTime.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void wasteTime(const TickClock::duration duration)
{
	/// \todo implementat something more precise and reliable
	for (TickClock::duration d {}; d <= duration; ++d)
		wasteTime(TickClock::now() + decltype(duration){1});
}

void wasteTime(const TickClock::time_point timePoint)
{
	while (timePoint > TickClock::now());
}

}	// namespace test

}	// namespace distortos
