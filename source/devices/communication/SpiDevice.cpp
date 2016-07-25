/**
 * \file
 * \brief SpiDevice class implementation
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiDevice.hpp"

#include "distortos/devices/communication/SpiMaster.hpp"
#include "distortos/devices/communication/SpiMasterOperation.hpp"

#include "distortos/assert.h"
#include "distortos/ThisThread.hpp"

#include "estd/ScopeGuard.hpp"

#include <cerrno>

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiDevice::~SpiDevice()
{
	if (openCount_ == 0)
		return;

	mutex_.lock();
	const auto lockRet = lockInternal();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this, lockRet]()
			{
				if (lockRet == 0)
					unlockInternal();
				mutex_.unlock();
			});

	spiMaster_.close();
}

int SpiDevice::close()
{
	mutex_.lock();
	const auto lockRet = lockInternal();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this, lockRet]()
			{
				if (lockRet == 0)
					unlockInternal();
				mutex_.unlock();
			});

	if (openCount_ == 0)	// device is not open anymore?
		return EBADF;

	if (openCount_ == 1)	// last close?
	{
		const auto ret = spiMaster_.close();
		if (ret != 0)
			return ret;
	}

	--openCount_;
	return 0;
}

std::pair<int, size_t> SpiDevice::executeTransaction(const SpiMasterOperationRange operationRange)
{
	if (operationRange.size() == 0)
		return {EINVAL, {}};

	mutex_.lock();
	const auto lockRet = lockInternal();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this, lockRet]()
			{
				if (lockRet == 0)
					unlockInternal();
				mutex_.unlock();
			});

	if (openCount_ == 0)
		return {EBADF, {}};

	return spiMaster_.executeTransaction(*this, operationRange);
}

int SpiDevice::lock()
{
	mutex_.lock();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				mutex_.unlock();
			});

	return lockInternal();
}

int SpiDevice::open()
{
	mutex_.lock();
	const auto lockRet = lockInternal();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this, lockRet]()
			{
				if (lockRet == 0)
					unlockInternal();
				mutex_.unlock();
			});

	if (openCount_ == std::numeric_limits<decltype(openCount_)>::max())	// device is already opened too many times?
		return EMFILE;

	if (openCount_ == 0)	// first open?
	{
		const auto ret = spiMaster_.open();
		if (ret != 0)
			return ret;
	}

	++openCount_;
	return 0;
}

int SpiDevice::unlock()
{
	mutex_.lock();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this]()
			{
				mutex_.unlock();
			});

	return unlockInternal();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int SpiDevice::lockInternal()
{
	const auto& thisThread = ThisThread::get();
	if (owner_ == &thisThread)
		return EDEADLK;

	const auto ret = conditionVariable_.wait(mutex_,
			[this]()
			{
				return owner_ == nullptr;
			});
	assert(ret == 0 && "Unexpected value returned by ConditionVariable::wait()!");

	owner_ = &thisThread;
	return 0;
}

int SpiDevice::unlockInternal()
{
	const auto& thisThread = ThisThread::get();
	if (owner_ != &thisThread)
		return EPERM;

	owner_ = nullptr;
	conditionVariable_.notifyOne();
	return 0;
}

}	// namespace devices

}	// namespace distortos
