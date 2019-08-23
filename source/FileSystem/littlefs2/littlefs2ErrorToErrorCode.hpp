/**
 * \file
 * \brief littlefs2ErrorToErrorCode() declaration
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_FILESYSTEM_LITTLEFS2_LITTLEFS2ERRORTOERRORCODE_HPP_
#define SOURCE_FILESYSTEM_LITTLEFS2_LITTLEFS2ERRORTOERRORCODE_HPP_

namespace distortos
{

/**
 * \brief Converts littlefs-v2 error to error code.
 *
 * \param [in] littlefs2Error is an error returned by littlefs-v2 functions
 *
 * \return \a littlefs2Error converted to error code
 */

int littlefs2ErrorToErrorCode(int littlefs2Error);

}	// namespace distortos

#endif	// SOURCE_FILESYSTEM_LITTLEFS2_LITTLEFS2ERRORTOERRORCODE_HPP_
