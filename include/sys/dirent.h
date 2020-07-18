/**
 * \file
 * \brief [dirent.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/dirent.h.html), suitable for newlib, which
 * does not provide one.
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_SYS_DIRENT_H_
#define INCLUDE_SYS_DIRENT_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

/** maximum number of bytes in a filename (not including the terminating null of a filename string) */
#define NAME_MAX	255

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/** directory stream struct */
struct DirStruct;

/** directory stream */
typedef struct DirStruct DIR;

/** directory entry information structure */
struct dirent
{
	/** file serial number */
	ino_t d_ino;

	/** filename string of entry */
	char d_name[NAME_MAX + 1];
};

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif /* INCLUDE_SYS_DIRENT_H_ */
