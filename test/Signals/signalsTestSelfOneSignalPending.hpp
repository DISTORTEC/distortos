/**
 * \file
 * \brief signalsTestSelfOneSignalPending() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SIGNALS_SIGNALSTESTSELFONESIGNALPENDING_HPP_
#define TEST_SIGNALS_SIGNALSTESTSELFONESIGNALPENDING_HPP_

#include <cstdint>

namespace distortos
{

namespace test
{

/**
 * \brief Tests whether exactly one signal is pending for current thread.
 *
 * \param [in] signalNumber is the signal number that may be pending for current thread
 *
 * \return true if test succeeded, false otherwise
 */

bool signalsTestSelfOneSignalPending(uint8_t signalNumber);

}	// namespace test

}	// namespace distortos

#endif	// TEST_SIGNALS_SIGNALSTESTSELFONESIGNALPENDING_HPP_
