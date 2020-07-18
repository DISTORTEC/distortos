/**
 * \file
 * \brief SignalSet class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/SignalSet.hpp"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public member variables
+---------------------------------------------------------------------------------------------------------------------*/

constexpr SignalSet::Empty SignalSet::empty;

constexpr SignalSet::Full SignalSet::full;

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, bool> SignalSet::test(const uint8_t signalNumber) const
{
	if (signalNumber >= bitset_.size())
		return {EINVAL, {}};

	return {{}, bitset_[signalNumber]};
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int SignalSet::set(const uint8_t signalNumber, const bool value)
{
	if (signalNumber >= bitset_.size())
		return EINVAL;

	bitset_[signalNumber] = value;
	return 0;
}

}	// namespace distortos
