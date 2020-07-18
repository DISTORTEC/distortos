/**
 * \file
 * \brief Definitions of fileDescriptions, fileDescriptionsCount and fileDescriptionsMutex objects
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/FileSystem/fileDescriptions.hpp"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/FileSystem/File.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

std::array<std::unique_ptr<File>, 20> fileDescriptions {};

uint8_t fileDescriptionsCount {};

Mutex fileDescriptionsMutex {Mutex::Protocol::priorityInheritance};

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
