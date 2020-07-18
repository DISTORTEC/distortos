/**
 * \file
 * \brief ufatErrorToErrorCode() declaration
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_FILESYSTEM_FAT_UFATERRORTOERRORCODE_HPP_
#define SOURCE_FILESYSTEM_FAT_UFATERRORTOERRORCODE_HPP_

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Converts uFAT error to error code.
 *
 * \param [in] ufatError is an error returned by uFAT functions
 *
 * \return \a ufatError converted to error code
 */

int ufatErrorToErrorCode(int ufatError);

}	// namespace distortos

#endif	// SOURCE_FILESYSTEM_FAT_UFATERRORTOERRORCODE_HPP_
