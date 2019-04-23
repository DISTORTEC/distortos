/**
 * \file
 * \brief littlefsErrorToErrorCode() implementation
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "littlefs1ErrorToErrorCode.hpp"

#include "lfs.h"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int littlefsErrorToErrorCode(const int littlefsError)
{
	if (littlefsError == LFS_ERR_OK)
		return 0;
	if (littlefsError == LFS_ERR_IO)
		return EIO;
	if (littlefsError == LFS_ERR_CORRUPT)
		return EILSEQ;
	if (littlefsError == LFS_ERR_NOENT)
		return ENOENT;
	if (littlefsError == LFS_ERR_EXIST)
		return EEXIST;
	if (littlefsError == LFS_ERR_NOTDIR)
		return ENOTDIR;
	if (littlefsError == LFS_ERR_ISDIR)
		return EISDIR;
	if (littlefsError == LFS_ERR_NOTEMPTY)
		return ENOTEMPTY;
	if (littlefsError == LFS_ERR_BADF)
		return EBADF;
	if (littlefsError == LFS_ERR_INVAL)
		return EINVAL;
	if (littlefsError == LFS_ERR_NOSPC)
		return ENOSPC;
	if (littlefsError == LFS_ERR_NOMEM)
		return ENOMEM;

	return -littlefsError;
}

}	// namespace distortos
