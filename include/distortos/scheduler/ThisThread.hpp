/**
 * \file
 * \brief ThisThread namespace header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-28
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THISTHREAD_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THISTHREAD_HPP_

namespace distortos
{

namespace scheduler
{

/// ThisThread namespace groups functions used to control current thread
namespace ThisThread
{

/**
 * \brief Yields time slot of the scheduler to next thread.
 */

void yield();

}	// namespace ThisThread

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THISTHREAD_HPP_
