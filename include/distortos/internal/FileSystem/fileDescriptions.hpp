/**
 * \file
 * \brief Declarations of fileDescriptions, fileDescriptionsCount and fileDescriptionsMutex objects
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_FILEDESCRIPTIONS_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_FILEDESCRIPTIONS_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include <memory>

namespace distortos
{

class File;
class Mutex;

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array of unique pointers to files
extern std::array<std::unique_ptr<File>, 20> fileDescriptions;

/// number of reserved elements in \a fileDescriptions
extern uint8_t fileDescriptionsCount;

/// mutex used to serialize access to \a fileDescriptions and \a fileDescriptionsCount
extern Mutex fileDescriptionsMutex;

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_FILEDESCRIPTIONS_HPP_
