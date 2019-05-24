/**
 * \file
 * \brief distortos version
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DISTORTOSVERSION_H_
#define INCLUDE_DISTORTOS_DISTORTOSVERSION_H_

#include "distortos/STRINGIFY.h"

/// major version of distortos
#define DISTORTOS_VERSION_MAJOR		0

/// minor version of distortos
#define DISTORTOS_VERSION_MINOR		7

/// patch version of distortos
#define DISTORTOS_VERSION_PATCH		0

/// distortos version as a 32-bit variable
#define DISTORTOS_VERSION	(DISTORTOS_VERSION_MAJOR << 16 | DISTORTOS_VERSION_MINOR << 8 | DISTORTOS_VERSION_PATCH)

/// distortos version as string
#define DISTORTOS_VERSION_STRING	\
		STRINGIFY(DISTORTOS_VERSION_MAJOR) "." STRINGIFY(DISTORTOS_VERSION_MINOR) "." STRINGIFY(DISTORTOS_VERSION_PATCH)

#endif	/* INCLUDE_DISTORTOS_DISTORTOSVERSION_H_ */
