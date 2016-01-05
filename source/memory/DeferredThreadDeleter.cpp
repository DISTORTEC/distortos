/**
 * \file
 * \brief DeferredThreadDeleter class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/memory/DeferredThreadDeleter.hpp"

#ifdef CONFIG_THREAD_DETACH_ENABLE

#include "distortos/internal/memory/getMallocMutex.hpp"

#include "distortos/internal/scheduler/ThreadControlBlock.hpp"

#include "distortos/Thread.hpp"

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
		const auto& thread = localList.front().getOwner();
		localList.pop_front();
		delete &thread;
	}

	return ret;
}

}	// namespace internal

}	// namespace distortos

#endif	// def CONFIG_THREAD_DETACH_ENABLE
