/**
 * \file
 * \brief littlefs2ErrorToErrorCode() implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "littlefs2ErrorToErrorCode.hpp"

#include "lfs2.h"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int littlefs2ErrorToErrorCode(const int littlefs2Error)
{
	if (littlefs2Error == LFS2_ERR_OK)
		return 0;
	if (littlefs2Error == LFS2_ERR_IO)
		return EIO;
	if (littlefs2Error == LFS2_ERR_CORRUPT)
		return EILSEQ;
	if (littlefs2Error == LFS2_ERR_NOENT)
		return ENOENT;
	if (littlefs2Error == LFS2_ERR_EXIST)
		return EEXIST;
	if (littlefs2Error == LFS2_ERR_NOTDIR)
		return ENOTDIR;
	if (littlefs2Error == LFS2_ERR_ISDIR)
		return EISDIR;
	if (littlefs2Error == LFS2_ERR_NOTEMPTY)
		return ENOTEMPTY;
	if (littlefs2Error == LFS2_ERR_BADF)
		return EBADF;
	if (littlefs2Error == LFS2_ERR_FBIG)
		return EFBIG;
	if (littlefs2Error == LFS2_ERR_INVAL)
		return EINVAL;
	if (littlefs2Error == LFS2_ERR_NOSPC)
		return ENOSPC;
	if (littlefs2Error == LFS2_ERR_NOMEM)
		return ENOMEM;
	if (littlefs2Error == LFS2_ERR_NOATTR)
		return ENOENT;
	if (littlefs2Error == LFS2_ERR_NAMETOOLONG)
		return ENAMETOOLONG;

	return -littlefs2Error;
}

}	// namespace distortos
