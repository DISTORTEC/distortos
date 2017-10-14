/**
 * \file
 * \brief RunnableThread class header
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_RUNNABLETHREAD_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_RUNNABLETHREAD_HPP_

#include "distortos/Thread.hpp"

namespace distortos
{

namespace internal
{

/// RunnableThread class defines additional interface functions required for the thread to be actually runnable
class RunnableThread : public Thread
{

};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_RUNNABLETHREAD_HPP_
