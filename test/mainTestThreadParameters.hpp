/**
 * \file
 * \brief mainTestThread object parameters
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#ifndef TEST_MAINTESTTHREADPARAMETERS_HPP_
#define TEST_MAINTESTTHREADPARAMETERS_HPP_

#include <cstddef>
#include <cstdint>

namespace distortos
{

namespace test
{

/// main test thread can receive signals
constexpr bool mainTestThreadCanReceiveSignals {true};

/// main test thread can have 10 signals queued
constexpr size_t mainTestThreadQueuedSignals {10};

/// main test thread can have 10 different SignalAction objects
constexpr size_t mainTestThreadSignalActions {10};

/// priority of main test thread
constexpr uint8_t mainTestThreadPriority {UINT8_MAX / 2};

}	// namespace test

}	// namespace distortos

#endif	// TEST_MAINTESTTHREADPARAMETERS_HPP_
