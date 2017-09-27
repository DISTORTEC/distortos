/**
 * \file
 * \brief C-API for distortos::internal::ThreadList
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_C_API_INTERNAL_THREADLIST_H_
#define INCLUDE_DISTORTOS_C_API_INTERNAL_THREADLIST_H_

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief C-API equivalent of distortos::internal::ThreadList
 *
 * \sa distortos::internal::ThreadList
 */

struct distortos_internal_ThreadList
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
 * \brief Initializer for distortos_internal_ThreadList
 *
 * \sa distortos::internal::ThreadList::ThreadList()
 *
 * \param [in] self is an equivalent of `this` hidden argument
 */

#define DISTORTOS_INTERNAL_THREADLIST_INITIALIZER(self)	{&(self), &(self)}

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* INCLUDE_DISTORTOS_C_API_INTERNAL_THREADLIST_H_ */
