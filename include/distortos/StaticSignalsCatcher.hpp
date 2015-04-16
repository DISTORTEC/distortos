/**
 * \file
 * \brief StaticSignalsCatcher class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-16
 */

#ifndef INCLUDE_DISTORTOS_STATICSIGNALSCATCHER_HPP_
#define INCLUDE_DISTORTOS_STATICSIGNALSCATCHER_HPP_

#include "distortos/SignalsCatcher.hpp"

namespace distortos
{

/**
 * \brief StaticSignalsCatcher class is a variant of SignalsCatcher that has automatic storage for SignalAction
 * associations.
 *
 * \param MaxAssociations is the maximum number of SignalAction associations
 */

template<size_t MaxAssociations>
class StaticSignalsCatcher : public SignalsCatcher
{
public:

	/**
	 * \brief StaticSignalsCatcher's constructor
	 */

	constexpr StaticSignalsCatcher() :
			SignalsCatcher{storage_}
	{

	}

private:

	/// storage for SignalAction associations
	std::array<Storage, MaxAssociations> storage_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICSIGNALSCATCHER_HPP_
