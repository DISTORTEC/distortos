/**
 * \file
 * \brief Stub system calls for newlib's libc
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sys/stat.h>
#include <sys/times.h>
#include <cerrno>
#include <cstdint>

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Stub _close_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
int _close_r(_reent*, int)
{
	errno = EBADF;
	return -1;
}

/**
 * \brief Stub _execve_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
int _execve_r(_reent*, const char*, char* const [], char* const [])
{
	errno = ENOMEM;
	return -1;
}

/**
 * \brief Stub _exit() implementation
 */

__attribute__ ((weak))
void _exit(int)
{
	while (1);
}

/**
 * \brief Stub _fork_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
pid_t _fork_r(_reent*)
{

	errno = EAGAIN;
	return -1;
}

/**
 * \brief Stub _fstat_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
int _fstat_r(_reent*, int, struct stat*)
{
	errno = EBADF;
	return -1;
}

/**
 * \brief Stub _getpid_r() implementation
 *
 * \return 1.
 */

__attribute__ ((weak))
pid_t _getpid_r(_reent*)
{
	return 1;
}

/**
 * \brief Stub _isatty_r() implementation
 *
 * \return 0
 */

__attribute__ ((weak))
int _isatty_r(_reent*, int)
{
	errno = EBADF;
	return 0;
}

/**
 * \brief Stub _kill_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
int _kill_r(_reent*, pid_t, int)
{
	errno = EINVAL;
	return -1;
}

/**
 * \brief Stub _link_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
int _link_r(_reent*, const char*, const char*)
{
	errno = EMLINK;
	return -1;
}

/**
 * \brief Stub _lseek_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
off_t _lseek_r(_reent*, int, off_t, int)
{
	errno = EBADF;
	return -1;
}

/**
 * \brief Stub _open_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
int _open_r(_reent*, const char*, int, int)
{
	errno = EMFILE;
	return -1;
}

/**
 * \brief Stub _read_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
ssize_t _read_r(_reent*, int, void*, size_t)
{
	errno = EBADF;
	return -1;
}

/**
 * \brief Stub _sbrk_r() implementation
 *
 * \return -1
*/

__attribute__ ((weak))
void* _sbrk_r(_reent*, intptr_t)
{
	errno = ENOMEM;
	return reinterpret_cast<caddr_t>(-1);
}

/**
 * \brief Stub _stat_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
int _stat_r(_reent*, const char*, struct stat*)
{
	errno = ENOENT;
	return -1;
}

/**
 * \brief Stub _times_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
clock_t _times_r(_reent*, tms*)
{
	return -1;
}

/**
 * \brief Stub _unlink_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
int _unlink_r(_reent*, const char*)
{
	errno = ENOENT;
	return -1;
}

/**
 * \brief Stub _wait_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
pid_t _wait_r(_reent*, int*)
{
	errno = ECHILD;
	return -1;
}

/**
 * \brief Stub _write_r() implementation
 *
 * \return -1
 */

__attribute__ ((weak))
ssize_t _write_r(_reent*, int, const void*, size_t)
{
	errno = EBADF;
	return -1;
}

}	// extern "C"
