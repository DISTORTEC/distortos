/**
 * \file
 * \brief DynamicThread class implementation
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2016-01-02
 */

#include "distortos/DynamicThread.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

#ifdef CONFIG_THREAD_DETACH_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

DynamicThread::~DynamicThread()
{

}

int DynamicThread::generateSignal(const uint8_t signalNumber)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return EINVAL;

	return detachableThread_->generateSignal(signalNumber);
}

uint8_t DynamicThread::getEffectivePriority() const
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return {};

	return detachableThread_->getEffectivePriority();
}

SignalSet DynamicThread::getPendingSignalSet() const
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return SignalSet{SignalSet::empty};

	return detachableThread_->getPendingSignalSet();
}

uint8_t DynamicThread::getPriority() const
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return {};

	return detachableThread_->getPriority();
}

SchedulingPolicy DynamicThread::getSchedulingPolicy() const
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return {};

	return detachableThread_->getSchedulingPolicy();
}

ThreadState DynamicThread::getState() const
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return ThreadState::Detached;

	return detachableThread_->getState();
}

int DynamicThread::join()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return EINVAL;

	return detachableThread_->join();
}

int DynamicThread::queueSignal(const uint8_t signalNumber, const sigval value)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return EINVAL;

	return detachableThread_->queueSignal(signalNumber, value);
}

void DynamicThread::setPriority(const uint8_t priority, const bool alwaysBehind)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return;

	detachableThread_->setPriority(priority, alwaysBehind);
}

void DynamicThread::setSchedulingPolicy(const SchedulingPolicy schedulingPolicy)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return;

	detachableThread_->setSchedulingPolicy(schedulingPolicy);
}

int DynamicThread::start()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (detachableThread_ == nullptr)
		return EINVAL;

	return detachableThread_->start();
}

#endif	// def CONFIG_THREAD_DETACH_ENABLE

}	// namespace distortos
