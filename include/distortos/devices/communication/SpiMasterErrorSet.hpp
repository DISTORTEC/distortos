/**
 * \file
 * \brief SpiMasterErrorSet class header
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERERRORSET_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERERRORSET_HPP_

#include <bitset>

namespace distortos
{

namespace devices
{

/**
 * SpiMasterErrorSet class is a bitset with error bits related to SPI master.
 *
 * \ingroup devices
 */

class SpiMasterErrorSet : public std::bitset<3>
{
public:

	/// indexes of error bits in SpiMasterErrorSet
	enum ErrorBits
	{
		/// CRC error
		crcError,
		/// master mode fault
		masterModeFault,
		/// overrun error
		overrunError,

		/// number of supported error bits - must be last!
		errorBitsMax
	};
};

static_assert(SpiMasterErrorSet{}.size() == SpiMasterErrorSet::errorBitsMax,
		"Invalid size of SpiMasterErrorSet's underlying bitset!");

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERERRORSET_HPP_
