/**
 * \file
 * \brief abortSignalHandler() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SIGNALS_ABORTSIGNALHANDLER_HPP_
#define TEST_SIGNALS_ABORTSIGNALHANDLER_HPP_

namespace distortos
{

class SignalInformation;

namespace test
{

/**
 * \brief Signal handler that is never supposed to be executed - it calls abort().
 *
 * \param [in] signalInformation is a reference to received SignalInformation object
 */

void abortSignalHandler(const SignalInformation& signalInformation);

}	// namespace test

}	// namespace distortos

#endif	// TEST_SIGNALS_ABORTSIGNALHANDLER_HPP_
