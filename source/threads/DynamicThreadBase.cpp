/**
 * \file
 * \brief DynamicThreadBase class implementation
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/DynamicThreadBase.hpp"

#if CONFIG_THREAD_DETACH_ENABLE == 1

#include "distortos/internal/memory/getDeferredThreadDeleter.hpp"
#include "distortos/internal/memory/DeferredThreadDeleter.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/InterruptMaskingLock.hpp"

#include <cerrno>

#endif	// CONFIG_THREAD_DETACH_ENABLE == 1

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

#if CONFIG_THREAD_DETACH_ENABLE == 1

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

#endif	// CONFIG_THREAD_DETACH_ENABLE == 1

/*---------------------------------------------------------------------------------------------------------------------+
| protected static functions
+---------------------------------------------------------------------------------------------------------------------*/

#if CONFIG_THREAD_DETACH_ENABLE == 1

void DynamicThreadBase::preTerminationHook(Thread& thread)
{
	auto& that = static_cast<DynamicThreadBase&>(thread);

	if (that.owner_ == nullptr)	// thread is detached?
		getDeferredThreadDeleter().lock();	/// \todo error handling?
}

void DynamicThreadBase::terminationHook(Thread& thread)
{
	ThreadCommon::terminationHook(thread);

	auto& that = static_cast<DynamicThreadBase&>(thread);

	if (that.owner_ == nullptr)	// thread is detached?
		getDeferredThreadDeleter()(that.getThreadControlBlock());	/// \todo error handling?
}

#endif	// CONFIG_THREAD_DETACH_ENABLE == 1

/*---------------------------------------------------------------------------------------------------------------------+
| private static functions
+---------------------------------------------------------------------------------------------------------------------*/

void DynamicThreadBase::run(Thread& thread)
{
	static_cast<DynamicThreadBase&>(thread).boundFunction_();
}

}	// namespace internal

}	// namespace distortos
