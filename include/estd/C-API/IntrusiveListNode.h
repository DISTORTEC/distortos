/**
 * \file
 * \brief C-API for estd::IntrusiveListNode
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_ESTD_C_API_INTRUSIVELISTNODE_H_
#define INCLUDE_ESTD_C_API_INTRUSIVELISTNODE_H_

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief C-API equivalent of estd::IntrusiveListNode
 *
 * \sa estd::IntrusiveListNode
 */

struct estd_IntrusiveListNode
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
 * \brief Initializer for estd_IntrusiveListNode
 *
 * \sa estd::IntrusiveListNode::IntrusiveListNode()
 *
 * \param [in] self is an equivalent of `this` hidden argument
 */

#define ESTD_INTRUSIVELISTNODE_INITIALIZER(self)	{&(self), &(self)}

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* INCLUDE_ESTD_C_API_INTRUSIVELISTNODE_H_ */
