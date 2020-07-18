/**
 * \file
 * \brief mutexTestTryLockWhenLocked() header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_MUTEX_MUTEXTESTTRYLOCKWHENLOCKED_HPP_
#define TEST_MUTEX_MUTEXTESTTRYLOCKWHENLOCKED_HPP_

#include <cstdint>

namespace distortos
{

class Mutex;

namespace test
{

/**
 * \brief Tests Mutex::tryLock() when mutex is locked - it must succeed immediately and return EBUSY
 *
 * Mutex::tryLock() is called from another thread.
 *
 * \param [in] mutex is a reference to mutex that will be tested
 * \param [in] priority is the priority of the created thread, default - max
 *
 * \return true if test succeeded, false otherwise
 */

bool mutexTestTryLockWhenLocked(Mutex& mutex, uint8_t priority = UINT8_MAX);

}	// namespace test

}	// namespace distortos

#endif	// TEST_MUTEX_MUTEXTESTTRYLOCKWHENLOCKED_HPP_
