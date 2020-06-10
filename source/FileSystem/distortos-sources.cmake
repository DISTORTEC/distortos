#
# file: distortos-sources.cmake
#
# author: Copyright (C) 2018-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

distortosSetConfiguration(BOOLEAN
		distortos_FileSystems_00_Integration_with_standard_library
		ON
		HELP "Enable integration of file systems with standard library.

		Enables functionality for accessing multiple distortos::FileSystem objects via functions from standard library
		headers. When this option is enabled, following features are enabled:
		- global functions distortos::mount() and distortos::unmount() (which supports deferred unmount of busy file
		system);
		- support for (most likely) all functions from <stdio.h> header, like fopen(), fclose(), fread(), fwrite(),
		fprintf(), fscanf() and so on;
		- support for selected I/O-related functions from <fcntl.h>, <unistd.h> and <sys/stat.h> headers: open(),
		close(), read(), write(), isatty(), lseek(), fstat(), mkdir(), stat() and unlink() (which supports both files
		and directories);
		- support for selected functions from <dirent.h> header: opendir(), closedir(), readdir_r(), rewinddir(),
		seekdir() and telldir();
		- support for statvfs() function from <sys/statvfs.h> header;"
		OUTPUT_NAME DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/fileDescriptions.cpp
		${CMAKE_CURRENT_LIST_DIR}/mkdir.cpp
		${CMAKE_CURRENT_LIST_DIR}/MountPoint.cpp
		${CMAKE_CURRENT_LIST_DIR}/opendir.cpp
		${CMAKE_CURRENT_LIST_DIR}/openFile.cpp
		${CMAKE_CURRENT_LIST_DIR}/rootFileSystem.cpp
		${CMAKE_CURRENT_LIST_DIR}/VirtualDirectory2.cpp
		${CMAKE_CURRENT_LIST_DIR}/VirtualFile.cpp
		${CMAKE_CURRENT_LIST_DIR}/VirtualFileSystem.cpp)

include(${CMAKE_CURRENT_LIST_DIR}/FAT/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/littlefs1/distortos-sources.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/littlefs2/distortos-sources.cmake)
