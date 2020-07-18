/**
 * \file
 * \brief CONCATENATE() macros
 *
 * \author Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_CONCATENATE_H_
#define INCLUDE_DISTORTOS_CONCATENATE_H_

/**
 * \brief Implementation of CONCATENATE2()
 *
 * \param [in] a is the first token
 * \param [in] b is the second token
 */

#define CONCATENATE2_IMPLEMENTATION(a, b)	a ## b

/**
 * \brief Concatenates 2 tokens.
 *
 * \param [in] a is the first token
 * \param [in] b is the second token
 */

#define CONCATENATE2(a, b)	CONCATENATE2_IMPLEMENTATION(a, b)

/**
 * \brief Implementation of CONCATENATE3()
 *
 * \param [in] a is the first token
 * \param [in] b is the second token
 * \param [in] c is the third token
 */

#define CONCATENATE3_IMPLEMENTATION(a, b, c)	a ## b ## c

/**
 * \brief Concatenates 3 tokens.
 *
 * \param [in] a is the first token
 * \param [in] b is the second token
 * \param [in] c is the third token
 */

#define CONCATENATE3(a, b, c)	CONCATENATE3_IMPLEMENTATION(a, b, c)

/**
 * \brief Implementation of CONCATENATE4()
 *
 * \param [in] a is the first token
 * \param [in] b is the second token
 * \param [in] c is the third token
 * \param [in] d is the fourth token
 */

#define CONCATENATE4_IMPLEMENTATION(a, b, c, d)	a ## b ## c ## d

/**
 * \brief Concatenates 4 tokens.
 *
 * \param [in] a is the first token
 * \param [in] b is the second token
 * \param [in] c is the third token
 * \param [in] d is the fourth token
 */

#define CONCATENATE4(a, b, c, d)	CONCATENATE4_IMPLEMENTATION(a, b, c, d)

#endif	/* INCLUDE_DISTORTOS_CONCATENATE_H_ */
