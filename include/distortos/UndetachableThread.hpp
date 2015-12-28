/**
 * \file
 * \brief UndetachableThread class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-28
 */

#ifndef INCLUDE_DISTORTOS_UNDETACHABLETHREAD_HPP_
#define INCLUDE_DISTORTOS_UNDETACHABLETHREAD_HPP_

#include "distortos/ThreadCommon.hpp"

namespace distortos
{

/**
 * \brief UndetachableThread class is a ThreadCommon that cannot be detached
 *
 * \ingroup threads
 */

#ifdef CONFIG_THREAD_DETACH_ENABLE

class UndetachableThread : public ThreadCommon
{
public:

	using ThreadCommon::ThreadCommon;
};

#else	// !def CONFIG_THREAD_DETACH_ENABLE

using UndetachableThread = ThreadCommon;

#endif	// !def CONFIG_THREAD_DETACH_ENABLE

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_UNDETACHABLETHREAD_HPP_
