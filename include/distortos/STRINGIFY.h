/**
 * \file
 * \brief STRINGIFY() macro
 *
 * \author Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STRINGIFY_H_
#define INCLUDE_DISTORTOS_STRINGIFY_H_

/**
 * \brief Implementation of STRINGIFY()
 *
 * \param [in] a is the token
 */

#define STRINGIFY_IMPLEMENTATION(a)	#a

/**
 * \brief Stringifies the token.
 *
 * \param [in] a is the token
 */

#define STRINGIFY(a)	STRINGIFY_IMPLEMENTATION(a)

#endif	/* INCLUDE_DISTORTOS_STRINGIFY_H_ */
