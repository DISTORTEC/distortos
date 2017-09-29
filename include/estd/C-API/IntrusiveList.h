/**
 * \file
 * \brief C-API for estd::IntrusiveList
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_ESTD_C_API_INTRUSIVELIST_H_
#define INCLUDE_ESTD_C_API_INTRUSIVELIST_H_

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief C-API equivalent of estd::IntrusiveList
 *
 * \sa estd::IntrusiveList
 */

struct estd_IntrusiveList
{
	/** reference to next node on the list */
	const void* nextNode;

	/** reference to previous node on the list */
	const void* previousNode;
};

/*---------------------------------------------------------------------------------------------------------------------+
| global defines
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Initializer for estd_IntrusiveList
 *
 * \sa estd::IntrusiveList::IntrusiveList()
 *
 * \param [in] self is an equivalent of `this` hidden argument
 */

#define ESTD_INTRUSIVELIST_INITIALIZER(self)	{&(self), &(self)}

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* INCLUDE_ESTD_C_API_INTRUSIVELIST_H_ */
