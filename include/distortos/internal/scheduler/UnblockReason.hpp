/**
 * \file
 * \brief UnblockReason enum class header
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_UNBLOCKREASON_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_UNBLOCKREASON_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace internal
{

/// reason of thread unblocking
enum class UnblockReason : uint8_t
{
	/// explicit request to unblock the thread - normal unblock
	unblockRequest,
	/// timeout - unblock via software timer
	timeout,

#if DISTORTOS_SIGNALS_ENABLE == 1

	/// signal handler - unblock to deliver unmasked signal
	signal,

#endif	// DISTORTOS_SIGNALS_ENABLE == 1
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_UNBLOCKREASON_HPP_
