/**
 * \file
 * \brief littlefsErrorToErrorCode() declaration
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_FILESYSTEM_LITTLEFS1_LITTLEFSERRORTOERRORCODE_HPP_
#define SOURCE_FILESYSTEM_LITTLEFS1_LITTLEFSERRORTOERRORCODE_HPP_

namespace distortos
{

/**
 * \brief Converts littlefs error to error code.
 *
 * \param [in] littlefsError is an error returned by littlefs functions
 *
 * \return \a littlefsError converted to error code
 */

int littlefsErrorToErrorCode(int littlefsError);

}	// namespace distortos

#endif	// SOURCE_FILESYSTEM_LITTLEFS1_LITTLEFSERRORTOERRORCODE_HPP_
