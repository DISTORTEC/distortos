/**
 * \file
 * \brief DynamicThreadBase class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/DynamicThreadBase.hpp"

#if DISTORTOS_THREAD_DETACH_ENABLE == 1

#include "distortos/internal/memory/getDeferredThreadDeleter.hpp"
#include "distortos/internal/memory/DeferredThreadDeleter.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/InterruptMaskingLock.hpp"

#include <cerrno>

#endif	// DISTORTOS_THREAD_DETACH_ENABLE == 1

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

#if DISTORTOS_THREAD_DETACH_ENABLE == 1

int DynamicThreadBase::detach()
{
	const InterruptMaskingLock interruptMaskingLock;

	if (owner_ == nullptr)	// already detached?
		return EINVAL;

	const auto owner = owner_;
	owner_ = {};

	const auto ret = owner->detach();
	return ret == EINVAL ? 0 : ret;
}

#endif	// DISTORTOS_THREAD_DETACH_ENABLE == 1

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

#if DISTORTOS_THREAD_DETACH_ENABLE == 1

void DynamicThreadBase::exit0Hook()
{
	ThreadCommon::exit0Hook();

	if (owner_ == nullptr)	// thread is detached?
		getDeferredThreadDeleter().lock();	/// \todo error handling?
}

void DynamicThreadBase::exit1Hook()
{
	ThreadCommon::exit1Hook();

	if (owner_ == nullptr)	// thread is detached?
		getDeferredThreadDeleter()(getThreadControlBlock());	/// \todo error handling?
}

#endif	// DISTORTOS_THREAD_DETACH_ENABLE == 1

void DynamicThreadBase::run()
{
	boundFunction_();
	boundFunction_ = {};
}

}	// namespace internal

}	// namespace distortos
