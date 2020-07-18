/**
 * \file
 * \brief littlefs1ErrorToErrorCode() declaration
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_FILESYSTEM_LITTLEFS1_LITTLEFS1ERRORTOERRORCODE_HPP_
#define SOURCE_FILESYSTEM_LITTLEFS1_LITTLEFS1ERRORTOERRORCODE_HPP_

namespace distortos
{

/**
 * \brief Converts littlefs-v1 error to error code.
 *
 * \param [in] littlefs1Error is an error returned by littlefs-v1 functions
 *
 * \return \a littlefs1Error converted to error code
 */

int littlefs1ErrorToErrorCode(int littlefs1Error);

}	// namespace distortos

#endif	// SOURCE_FILESYSTEM_LITTLEFS1_LITTLEFS1ERRORTOERRORCODE_HPP_
