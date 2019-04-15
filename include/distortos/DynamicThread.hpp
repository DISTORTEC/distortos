/**
 * \file
 * \brief DynamicThread class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_
#define INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_

#include "distortos/internal/scheduler/DynamicThreadBase.hpp"

#include "distortos/assert.h"

namespace distortos
{

/// \addtogroup threads
/// \{

/**
 * \brief DynamicThread class is a type-erased interface for thread that has dynamic storage for bound function, stack
 * and internal DynamicSignalsReceiver object.
 */

#ifdef CONFIG_THREAD_DETACH_ENABLE

class DynamicThread : public Thread
{
public:

	/**
	 * \brief DynamicThread's constructor
	 *
	 * \tparam Function is the function that will be executed in separate thread
	 * \tparam Args are the arguments for \a Function
	 *
	 * \param [in] stackSize is the size of stack, bytes
	 * \param [in] canReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this
	 * thread
	 * \param [in] queuedSignals is the max number of queued signals for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable queuing of signals for this thread
	 * \param [in] signalActions is the max number of different SignalAction objects for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable catching of signals for this thread
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for \a function
	 */

	template<typename Function, typename... Args>
	DynamicThread(size_t stackSize, bool canReceiveSignals, size_t queuedSignals, size_t signalActions,
			uint8_t priority, SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args);

	/**
	 * \brief DynamicThread's constructor
	 *
	 * \tparam Function is the function that will be executed in separate thread
	 * \tparam Args are the arguments for \a Function
	 *
	 * \param [in] parameters is a DynamicThreadParameters struct with thread parameters
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for \a function
	 */

	template<typename Function, typename... Args>
	DynamicThread(const DynamicThreadParameters parameters, Function&& function, Args&&... args) :
			DynamicThread{parameters.stackSize, parameters.canReceiveSignals, parameters.queuedSignals,
					parameters.signalActions, parameters.priority, parameters.schedulingPolicy,
					std::forward<Function>(function), std::forward<Args>(args)...}
	{

	}

	/**
	 * \brief DynamicThread's destructor
	 */

	~DynamicThread() override;

	/**
	 * \brief Detaches the thread.
	 *
	 * Similar to std::thread::detach() - http://en.cppreference.com/w/cpp/thread/thread/detach
	 * Similar to POSIX pthread_detach() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_detach.html
	 *
	 * Detaches the executing thread from the Thread object, allowing execution to continue independently. All resources
	 * allocated for the thread will be deallocated when the thread terminates.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - this thread is already detached;
	 * - error codes returned by internal::DynamicThreadBase::detach() (except EINVAL);
	 */

	int detach() override;

#if DISTORTOS_SIGNALS_ENABLE == 1

	/**
	 * \brief Generates signal for thread.
	 *
	 * Similar to pthread_kill() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_kill.html
	 *
	 * Adds the signalNumber to set of pending signals. If this thread is currently waiting for this signal, it will be
	 * unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be generated, [0; 31]
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - internal thread object was detached;
	 * - error codes returned by internal::DynamicThreadBase::generateSignal();
	 *
	 * \ingroup signals
	 */

	int generateSignal(uint8_t signalNumber) override;

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

	/**
	 * \return effective priority of thread
	 */

	uint8_t getEffectivePriority() const override;

	/**
	 * \return identifier of thread, default-constructed ThreadIdentifier if this object doesn't represent a valid
	 * thread of execution (e.g. after the thread is detached)
	 */

	ThreadIdentifier getIdentifier() const override;

#if DISTORTOS_SIGNALS_ENABLE == 1

	/**
	 * \brief Gets set of currently pending signals.
	 *
	 * Similar to sigpending() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigpending.html
	 *
	 * This function shall return the set of signals that are blocked from delivery and are pending on the thread.
	 *
	 * \return set of currently pending signals
	 *
	 * \ingroup signals
	 */

	SignalSet getPendingSignalSet() const override;

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

	/**
	 * \return priority of thread
	 */

	uint8_t getPriority() const override;

	/**
	 * \return scheduling policy of the thread
	 */

	SchedulingPolicy getSchedulingPolicy() const override;

	/**
	 * \return "high water mark" (max usage) of thread's stack, bytes
	 */

	size_t getStackHighWaterMark() const override;

	/**
	 * \return size of thread's stack, bytes
	 */

	size_t getStackSize() const override;

	/**
	 * \return current state of thread
	 */

	ThreadState getState() const override;

	/**
	 * \brief Waits for thread termination.
	 *
	 * Similar to std::thread::join() - http://en.cppreference.com/w/cpp/thread/thread/join
	 * Similar to POSIX pthread_join() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
	 *
	 * Blocks current thread until this thread finishes its execution. The results of multiple simultaneous calls to
	 * join() on the same target thread are undefined.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - internal thread object was detached;
	 * - error codes returned by internal::DynamicThreadBase::join();
	 *
	 * \ingroup synchronization
	 */

	int join() override;

#if DISTORTOS_SIGNALS_ENABLE == 1

	/**
	 * \brief Queues signal for thread.
	 *
	 * Similar to sigqueue() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigqueue.html
	 *
	 * Adds the signalNumber and signal value (sigval union) to queue of SignalInformation objects. If this thread is
	 * currently waiting for this signal, it will be unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be queued, [0; 31]
	 * \param [in] value is the signal value
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - internal thread object was detached;
	 * - error codes returned by internal::DynamicThreadBase::queueSignal();
	 *
	 * \ingroup signals
	 */

	int queueSignal(uint8_t signalNumber, sigval value) override;

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

	/**
	 * \brief Changes priority of thread.
	 *
	 * If the priority really changes, the position in the thread list is adjusted and context switch may be requested.
	 *
	 * \param [in] priority is the new priority of thread
	 * \param [in] alwaysBehind selects the method of ordering when lowering the priority
	 * - false - the thread is moved to the head of the group of threads with the new priority (default),
	 * - true - the thread is moved to the tail of the group of threads with the new priority.
	 */

	void setPriority(uint8_t priority, bool alwaysBehind = {}) override;

	/**
	 * param [in] schedulingPolicy is the new scheduling policy of the thread
	 */

	void setSchedulingPolicy(SchedulingPolicy schedulingPolicy) override;

	/**
	 * \brief Starts the thread.
	 *
	 * This operation can be performed on threads in "created" state only.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - internal thread object was detached;
	 * - error codes returned by internal::DynamicThreadBase::start();
	 */

	int start();

	DynamicThread(const DynamicThread&) = delete;
	DynamicThread(DynamicThread&&) = default;
	const DynamicThread& operator=(const DynamicThread&) = delete;
	DynamicThread& operator=(DynamicThread&&) = delete;

private:

	/// internal thread object
	std::unique_ptr<internal::DynamicThreadBase> detachableThread_;
};

#else	// !def CONFIG_THREAD_DETACH_ENABLE

class DynamicThread : public internal::DynamicThreadBase
{
public:

	using internal::DynamicThreadBase::DynamicThreadBase;
};

#endif	// !def CONFIG_THREAD_DETACH_ENABLE

/**
 * \brief Helper factory function to make DynamicThread object
 *
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] stackSize is the size of stack, bytes
 * \param [in] canReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this
 * thread
 * \param [in] queuedSignals is the max number of queued signals for this thread, relevant only if
 * \a canReceiveSignals == true, 0 to disable queuing of signals for this thread
 * \param [in] signalActions is the max number of different SignalAction objects for this thread, relevant only if
 * \a canReceiveSignals == true, 0 to disable catching of signals for this thread
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] schedulingPolicy is the scheduling policy of the thread
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for \a function
 *
 * \return DynamicThread object
 */

template<typename Function, typename... Args>
DynamicThread makeDynamicThread(const size_t stackSize, const bool canReceiveSignals, const size_t queuedSignals,
		const size_t signalActions, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		Function&& function, Args&&... args)
{
	return {stackSize, canReceiveSignals, queuedSignals, signalActions, priority, schedulingPolicy,
			std::forward<Function>(function), std::forward<Args>(args)...};
}

/**
 * \brief Helper factory function to make DynamicThread object
 *
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] parameters is a DynamicThreadParameters struct with thread parameters
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for \a function
 *
 * \return DynamicThread object
 */

template<typename Function, typename... Args>
DynamicThread makeDynamicThread(const DynamicThreadParameters parameters, Function&& function, Args&&... args)
{
	return {parameters, std::forward<Function>(function), std::forward<Args>(args)...};
}

/**
 * \brief Helper factory function to make and start DynamicThread object
 *
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] stackSize is the size of stack, bytes
 * \param [in] canReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this
 * thread
 * \param [in] queuedSignals is the max number of queued signals for this thread, relevant only if
 * \a canReceiveSignals == true, 0 to disable queuing of signals for this thread
 * \param [in] signalActions is the max number of different SignalAction objects for this thread, relevant only if
 * \a canReceiveSignals == true, 0 to disable catching of signals for this thread
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] schedulingPolicy is the scheduling policy of the thread
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for \a function
 *
 * \return DynamicThread object
 */

template<typename Function, typename... Args>
DynamicThread makeAndStartDynamicThread(const size_t stackSize, const bool canReceiveSignals,
		const size_t queuedSignals, const size_t signalActions, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args)
{
	auto thread = makeDynamicThread(stackSize, canReceiveSignals, queuedSignals, signalActions, priority,
			schedulingPolicy, std::forward<Function>(function), std::forward<Args>(args)...);
	{
		const auto ret = thread.start();
		assert(ret == 0 && "Could not start thread!");
	}
	return thread;
}

/**
 * \brief Helper factory function to make and start DynamicThread object
 *
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] parameters is a DynamicThreadParameters struct with thread parameters
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for \a function
 *
 * \return DynamicThread object
 */

template<typename Function, typename... Args>
DynamicThread makeAndStartDynamicThread(const DynamicThreadParameters parameters, Function&& function, Args&&... args)
{
	auto thread = makeDynamicThread(parameters, std::forward<Function>(function), std::forward<Args>(args)...);
	{
		const auto ret = thread.start();
		assert(ret == 0 && "Could not start thread!");
	}
	return thread;
}

/// \}

#ifdef CONFIG_THREAD_DETACH_ENABLE

template<typename Function, typename... Args>
DynamicThread::DynamicThread(const size_t stackSize, const bool canReceiveSignals, const size_t queuedSignals,
		const size_t signalActions, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		Function&& function, Args&&... args) :
		detachableThread_{new internal::DynamicThreadBase{stackSize, canReceiveSignals, queuedSignals, signalActions,
				priority, schedulingPolicy, *this, std::forward<Function>(function), std::forward<Args>(args)...}}
{

}

#endif	// def CONFIG_THREAD_DETACH_ENABLE

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_
