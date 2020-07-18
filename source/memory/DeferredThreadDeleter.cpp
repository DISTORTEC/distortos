/**
 * \file
 * \brief DeferredThreadDeleter class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/memory/DeferredThreadDeleter.hpp"

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

#include "distortos/internal/newlib/locking.hpp"

#include "distortos/internal/scheduler/RunnableThread.hpp"
#include "distortos/internal/scheduler/ThreadControlBlock.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int DeferredThreadDeleter::operator()(ThreadControlBlock& threadControlBlock)
{
	list_.push_back(threadControlBlock);
	notEmpty_ = true;

	return mutex_.unlock();
}

int DeferredThreadDeleter::lock()
{
	return mutex_.lock();
}

int DeferredThreadDeleter::tryCleanup()
{
	if (notEmpty_ == false)
		return 0;

	auto& mallocMutex = getMallocMutex();

	{
		const auto ret = mallocMutex.tryLock();
		if (ret != 0)
			return ret;
	}

	const auto ret = tryCleanupInternal();

	{
		const auto ret2 = mallocMutex.unlock();
		if (ret2 != 0)
			return ret2;
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int DeferredThreadDeleter::tryCleanupInternal()
{
	{
		const auto ret = mutex_.tryLock();
		if (ret != 0)
			return ret;
	}

	decltype(list_) localList {std::move(list_)};
	notEmpty_ = false;

	const auto ret = mutex_.unlock();

	while (localList.empty() == false)
	{
		const auto& runnableThread = localList.front().getOwner();
		localList.pop_front();
		delete &runnableThread;
	}

	return ret;
}

}	// namespace internal

}	// namespace distortos

#endif	// def DISTORTOS_THREAD_DETACH_ENABLE
