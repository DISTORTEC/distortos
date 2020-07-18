#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/ConditionVariable.cpp
		${CMAKE_CURRENT_LIST_DIR}/DynamicRawFifoQueue.cpp
		${CMAKE_CURRENT_LIST_DIR}/DynamicRawMessageQueue.cpp
		${CMAKE_CURRENT_LIST_DIR}/DynamicSignalsReceiver.cpp
		${CMAKE_CURRENT_LIST_DIR}/FifoQueueBase.cpp
		${CMAKE_CURRENT_LIST_DIR}/MemcpyPopQueueFunctor.cpp
		${CMAKE_CURRENT_LIST_DIR}/MemcpyPushQueueFunctor.cpp
		${CMAKE_CURRENT_LIST_DIR}/MessageQueueBase.cpp
		${CMAKE_CURRENT_LIST_DIR}/MutexControlBlock.cpp
		${CMAKE_CURRENT_LIST_DIR}/Mutex.cpp
		${CMAKE_CURRENT_LIST_DIR}/RawFifoQueue.cpp
		${CMAKE_CURRENT_LIST_DIR}/RawMessageQueue.cpp
		${CMAKE_CURRENT_LIST_DIR}/Semaphore.cpp
		${CMAKE_CURRENT_LIST_DIR}/SemaphoreTryWaitForFunctor.cpp
		${CMAKE_CURRENT_LIST_DIR}/SemaphoreTryWaitFunctor.cpp
		${CMAKE_CURRENT_LIST_DIR}/SemaphoreTryWaitUntilFunctor.cpp
		${CMAKE_CURRENT_LIST_DIR}/SemaphoreWaitFunctor.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalInformationQueue.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalsCatcherControlBlock.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalSet.cpp
		${CMAKE_CURRENT_LIST_DIR}/SignalsReceiverControlBlock.cpp
		${CMAKE_CURRENT_LIST_DIR}/ThisThread-Signals.cpp)
