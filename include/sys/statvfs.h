/**
 * \file
 * \brief Standard [sys/statvfs.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_statvfs.h.html), which
 * is not provided by newlib.
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_SYS_STATVFS_H_
#define INCLUDE_SYS_STATVFS_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/** for f_flag field - read-only file system */
#define ST_RDONLY	(1 << 0)

/** for f_flag field - file system does not support the semantics of the ST_ISUID and ST_ISGID file mode bits */
#define ST_NOSUID	(1 << 1)

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

// toolchains with GCC 5 don't have fsblkcnt_t and fsfilcnt_t types, these were introduced in newlib in
// https://sourceware.org/git/gitweb.cgi?p=newlib-cygwin.git;a=commit;h=f3e587d30a9f65d0c6551ad14095300f6e81672e
#ifndef _FSBLKCNT_T_DECLARED

#ifndef __machine_fsblkcnt_t_defined
typedef __uint64_t __fsblkcnt_t;
#endif
#ifndef __machine_fsfilcnt_t_defined
typedef __uint32_t __fsfilcnt_t;
#endif

typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;

#define _FSBLKCNT_T_DECLARED

#endif	/* !def _FSBLKCNT_T_DECLARED */

/** file System information structure */
struct statvfs
{
	/** file system block size */
	unsigned long f_bsize;

	/** fundamental file system block size */
	unsigned long f_frsize;

	/** total number of blocks on file system in units of f_frsize */
	fsblkcnt_t f_blocks;

	/** total number of free blocks */
	fsblkcnt_t f_bfree;

	/** number of free blocks available to non-privileged process */
	fsblkcnt_t f_bavail;

	/** total number of file serial numbers */
	fsfilcnt_t f_files;

	/** total number of free file serial numbers */
	fsfilcnt_t f_ffree;

	/** number of file serial numbers available to non-privileged process */
	fsfilcnt_t f_favail;

	/** file system ID */
	unsigned long f_fsid;

	/** bit mask of f_flag values */
	unsigned long f_flag;

	/** maximum filename length */
	unsigned long f_namemax;
};

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif /* INCLUDE_SYS_STATVFS_H_ */
