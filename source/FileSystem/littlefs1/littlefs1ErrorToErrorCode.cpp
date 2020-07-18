/**
 * \file
 * \brief littlefs1ErrorToErrorCode() implementation
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "littlefs1ErrorToErrorCode.hpp"

#include "lfs1.h"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int littlefs1ErrorToErrorCode(const int littlefs1Error)
{
	if (littlefs1Error == LFS1_ERR_OK)
		return 0;
	if (littlefs1Error == LFS1_ERR_IO)
		return EIO;
	if (littlefs1Error == LFS1_ERR_CORRUPT)
		return EILSEQ;
	if (littlefs1Error == LFS1_ERR_NOENT)
		return ENOENT;
	if (littlefs1Error == LFS1_ERR_EXIST)
		return EEXIST;
	if (littlefs1Error == LFS1_ERR_NOTDIR)
		return ENOTDIR;
	if (littlefs1Error == LFS1_ERR_ISDIR)
		return EISDIR;
	if (littlefs1Error == LFS1_ERR_NOTEMPTY)
		return ENOTEMPTY;
	if (littlefs1Error == LFS1_ERR_BADF)
		return EBADF;
	if (littlefs1Error == LFS1_ERR_INVAL)
		return EINVAL;
	if (littlefs1Error == LFS1_ERR_NOSPC)
		return ENOSPC;
	if (littlefs1Error == LFS1_ERR_NOMEM)
		return ENOMEM;

	return -littlefs1Error;
}

}	// namespace distortos
