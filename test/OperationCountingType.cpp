/**
 * \file
 * \brief OperationCountingType class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "OperationCountingType.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| public static functions
+---------------------------------------------------------------------------------------------------------------------*/

bool OperationCountingType::checkCounters(const size_t constructed, const size_t copyConstructed,
		const size_t moveConstructed, const size_t destructed, const size_t copyAssigned, const size_t moveAssigned,
		const size_t swapped)
{
	return constructed == constructed_ && copyConstructed == copyConstructed_ &&
			moveConstructed == moveConstructed_ && destructed == destructed_ && copyAssigned == copyAssigned_ &&
			moveAssigned == moveAssigned_ && swapped == swapped_;
}

void OperationCountingType::resetCounters()
{
	constructed_ = {};
	copyConstructed_ = {};
	moveConstructed_ = {};
	destructed_ = {};
	copyAssigned_ = {};
	moveAssigned_ = {};
	swapped_ = {};
}

/*---------------------------------------------------------------------------------------------------------------------+
| private static member variables
+---------------------------------------------------------------------------------------------------------------------*/

size_t OperationCountingType::constructed_;
size_t OperationCountingType::copyConstructed_;
size_t OperationCountingType::moveConstructed_;
size_t OperationCountingType::destructed_;
size_t OperationCountingType::copyAssigned_;
size_t OperationCountingType::moveAssigned_;
size_t OperationCountingType::swapped_;

}	// namespace test

}	// namespace distortos
