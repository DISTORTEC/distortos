/**
 * \file
 * \brief openFile() declaration
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_OPENFILE_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_OPENFILE_HPP_

#include <utility>

#include <cstdio>

namespace distortos
{

class FileSystem;

/**
 * \brief Opens file from given file system.
 *
 * Similar to [fopen()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/fopen.html)
 *
 * \pre \a fileSystem is mounted.
 * \pre \a path and \a mode are valid.
 *
 * \param [in] fileSystem is a reference to file system used to open the file
 * \param [in] path is the path of file that will be opened, must be valid
 * \param [in] mode is the mode in which the file will be opened, must be valid, for list of available modes and valid
 * combinations see [fopen()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/fopen.html)
 *
 * \return pair with return code (0 on success, error code otherwise) and `FILE*` with opened file; error codes:
 * - error codes returned by FileSystem::openFile();
 * - error codes returned by fopencookie();
 */

std::pair<int, FILE*> openFile(FileSystem& fileSystem, const char* path, const char* mode);

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_OPENFILE_HPP_
