/**
 * \file
 * \brief deliverSignals() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-01
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_DELIVERSIGNALS_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_DELIVERSIGNALS_HPP_

namespace distortos
{

namespace synchronization
{

/**
 * \brief Delivers all unmasked signals that are pending/queued for current thread.
 */

void deliverSignals();

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_DELIVERSIGNALS_HPP_
