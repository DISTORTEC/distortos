/**
 * \file
 * \brief DynamicThread class implementation
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/DynamicThread.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#ifdef CONFIG_THREAD_DETACH_ENABLE

#include "distortos/InterruptMaskingLock.hpp"

#include <cerrno>

#endif	// def CONFIG_THREAD_DETACH_ENABLE

namespace distortos
{

#ifdef CONFIG_THREAD_DETACH_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

DynamicThread::~DynamicThread()
{

}

int DynamicThread::detach()
{
	CHECK_FUNCTION_CONTEXT();

	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)	// already detached?
		return EINVAL;

	const auto state = detachableThread_->getState();
	if (state == ThreadState::created || state == ThreadState::terminated)	// thread not yet started or already terminated?
	{
		detachableThread_.reset();	// just delete it...
		return 0;
	}

	const auto detachableThread = detachableThread_.release();

	const auto ret = detachableThread->detach();
	return ret == EINVAL ? 0 : ret;
}

int DynamicThread::generateSignal(const uint8_t signalNumber)
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return EINVAL;

	return detachableThread_->generateSignal(signalNumber);
}

uint8_t DynamicThread::getEffectivePriority() const
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return {};

	return detachableThread_->getEffectivePriority();
}

SignalSet DynamicThread::getPendingSignalSet() const
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return SignalSet{SignalSet::empty};

	return detachableThread_->getPendingSignalSet();
}

uint8_t DynamicThread::getPriority() const
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return {};

	return detachableThread_->getPriority();
}

SchedulingPolicy DynamicThread::getSchedulingPolicy() const
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return {};

	return detachableThread_->getSchedulingPolicy();
}

size_t DynamicThread::getStackHighWaterMark() const
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return {};

	return detachableThread_->getStackHighWaterMark();
}

size_t DynamicThread::getStackSize() const
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return {};

	return detachableThread_->getStackSize();
}

ThreadState DynamicThread::getState() const
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return ThreadState::detached;

	return detachableThread_->getState();
}

int DynamicThread::join()
{
	CHECK_FUNCTION_CONTEXT();

	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return EINVAL;

	return detachableThread_->join();
}

int DynamicThread::queueSignal(const uint8_t signalNumber, const sigval value)
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return EINVAL;

	return detachableThread_->queueSignal(signalNumber, value);
}

void DynamicThread::setPriority(const uint8_t priority, const bool alwaysBehind)
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return;

	detachableThread_->setPriority(priority, alwaysBehind);
}

void DynamicThread::setSchedulingPolicy(const SchedulingPolicy schedulingPolicy)
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return;

	detachableThread_->setSchedulingPolicy(schedulingPolicy);
}

int DynamicThread::start()
{
	const InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return EINVAL;

	return detachableThread_->start();
}

#endif	// def CONFIG_THREAD_DETACH_ENABLE

}	// namespace distortos
