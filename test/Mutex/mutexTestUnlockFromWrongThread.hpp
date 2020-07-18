/**
 * \file
 * \brief mutexTestUnlockFromWrongThread() header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_MUTEX_MUTEXTESTUNLOCKFROMWRONGTHREAD_HPP_
#define TEST_MUTEX_MUTEXTESTUNLOCKFROMWRONGTHREAD_HPP_

namespace distortos
{

class Mutex;

namespace test
{

/**
 * \brief Tests unlocking of errorChecking or recursive mutex from a wrong thread.
 *
 * Such operation must fail with EPERM immediately.
 *
 * \param [in] mutex is a reference to mutex that will checked, must be errorChecking or recursive type!
 *
 * \return true if test succeeded, false otherwise
 */

bool mutexTestUnlockFromWrongThread(Mutex& mutex);

}	// namespace test

}	// namespace distortos

#endif	// TEST_MUTEX_MUTEXTESTUNLOCKFROMWRONGTHREAD_HPP_
