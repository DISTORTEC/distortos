/**
 * \file
 * \brief ufatErrorToErrorCode() implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ufatErrorToErrorCode.hpp"

#include "ufat.h"

#include <type_traits>

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int ufatErrorToErrorCode(const int ufatError)
{
	static const int errorCodes[]
	{
			0,				// UFAT_OK, 0
			EIO,			// UFAT_ERR_IO, 1
			EINVAL,			// UFAT_ERR_BLOCK_SIZE, 2
			EILSEQ,			// UFAT_ERR_INVALID_BPB, 3
			EFAULT,			// UFAT_ERR_BLOCK_ALIGNMENT, 4
			EFAULT,			// UFAT_ERR_INVALID_CLUSTER, 5
			ENAMETOOLONG,	// UFAT_ERR_NAME_TOO_LONG, 6
			ENOTDIR,		// UFAT_ERR_NOT_DIRECTORY, 7
			EISDIR,			// UFAT_ERR_NOT_FILE, 8
			EACCES,			// UFAT_ERR_IMMUTABLE, 9
			ENOTEMPTY,		// UFAT_ERR_DIRECTORY_NOT_EMPTY, 10
			EINVAL,			// UFAT_ERR_ILLEGAL_NAME, 11
			EEXIST,			// UFAT_ERR_FILE_EXISTS, 12
			EILSEQ,			// UFAT_ERR_BAD_ENCODING, 13
			ENOSPC,			// UFAT_ERR_DIRECTORY_FULL, 14
			ENOSPC,			// UFAT_ERR_NO_CLUSTERS, 15
	};

	std::make_unsigned<decltype(ufatError)>::type absoluteUfatError = ufatError > 0 ? ufatError : -ufatError;
	if (absoluteUfatError >= sizeof(errorCodes) / sizeof(*errorCodes))
		return EINVAL;

	return errorCodes[absoluteUfatError];
}

}	// namespace distortos
