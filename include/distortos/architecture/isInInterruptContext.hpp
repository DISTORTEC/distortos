/**
 * \file
 * \brief isInInterruptContext() declaration
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_ISININTERRUPTCONTEXT_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_ISININTERRUPTCONTEXT_HPP_

namespace distortos
{

namespace architecture
{

/**
 * \brief Architecture-specific check of context.
 *
 * Checks whether thread or interrupt context is currently active. An example use is when the application needs to
 * decide between blocking or non-blocking behaviour, as blocking is not possible in interrupt context.
 *
 * \return true if the function was called from interrupt context or false if it was called from thread context
 */

bool isInInterruptContext();

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_ISININTERRUPTCONTEXT_HPP_
