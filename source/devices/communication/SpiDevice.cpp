/**
 * \file
 * \brief SpiDevice class implementation
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/communication/SpiDevice.hpp"

#include "distortos/devices/communication/SpiMaster.hpp"
#include "distortos/devices/communication/SpiMasterOperation.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/assert.h"
#include "distortos/ThisThread.hpp"

#include "estd/ScopeGuard.hpp"

#include <mutex>

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
	CHECK_FUNCTION_CONTEXT();

	if (openCount_ == 0)
		return;

	const std::lock_guard<distortos::Mutex> lock {mutex_};

	const auto previousLockState = lockInternal();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this, previousLockState]()
			{
				unlockInternal(previousLockState);
			});

	spiMaster_.close();
}

int SpiDevice::close()
{
	const std::lock_guard<distortos::Mutex> lock {mutex_};

	const auto previousLockState = lockInternal();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this, previousLockState]()
			{
				unlockInternal(previousLockState);
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
	CHECK_FUNCTION_CONTEXT();

	if (operationRange.size() == 0)
		return {EINVAL, {}};

	const std::lock_guard<distortos::Mutex> lock {mutex_};

	const auto previousLockState = lockInternal();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this, previousLockState]()
			{
				unlockInternal(previousLockState);
			});

	if (openCount_ == 0)
		return {EBADF, {}};

	return spiMaster_.executeTransaction(*this, operationRange);
}

bool SpiDevice::lock()
{
	const std::lock_guard<distortos::Mutex> lock {mutex_};

	return lockInternal();
}

int SpiDevice::open()
{
	const std::lock_guard<distortos::Mutex> lock {mutex_};

	const auto previousLockState = lockInternal();
	const auto mutexScopeGuard = estd::makeScopeGuard(
			[this, previousLockState]()
			{
				unlockInternal(previousLockState);
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

void SpiDevice::unlock(const bool previousLockState)
{
	const std::lock_guard<distortos::Mutex> lock {mutex_};

	unlockInternal(previousLockState);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SpiDevice::lockInternal()
{
	const auto& thisThread = ThisThread::get();
	if (owner_ == &thisThread)
		return true;

	const auto ret = conditionVariable_.wait(mutex_,
			[this]()
			{
				return owner_ == nullptr;
			});
	assert(ret == 0 && "Unexpected value returned by ConditionVariable::wait()!");

	owner_ = &thisThread;
	return false;
}

void SpiDevice::unlockInternal(const bool previousLockState)
{
	if (previousLockState == true || owner_ != &ThisThread::get())
		return;

	owner_ = nullptr;
	conditionVariable_.notifyOne();
}

}	// namespace devices

}	// namespace distortos
