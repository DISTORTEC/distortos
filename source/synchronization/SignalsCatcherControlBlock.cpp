/**
 * \file
 * \brief SignalsCatcherControlBlock class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-18
 */

#include "distortos/synchronization/SignalsCatcherControlBlock.hpp"

#include <algorithm>

namespace distortos
{

namespace synchronization
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tries to find SignalsCatcherControlBlock::Association for given signal number in given range.
 *
 * \param [in] begin is a pointer to first element of range of SignalsCatcherControlBlock::Association objects
 * \param [in] end is a pointer to "one past the last" element of range of SignalsCatcherControlBlock::Association
 * objects
 * \param [in] signalNumber is the signal for which the association will be searched
 *
 * \return pointer to found SignalsCatcherControlBlock::Association object, \e end if no match was found
 */

SignalsCatcherControlBlock::Association* findAssociation(SignalsCatcherControlBlock::Association* const begin,
		SignalsCatcherControlBlock::Association* const end, const uint8_t signalNumber)
{
	return std::find_if(begin, end,
			[signalNumber](const SignalsCatcherControlBlock::Association& association)
			{
				return association.first == signalNumber;
			});
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

SignalAction SignalsCatcherControlBlock::clearAssociation(const uint8_t signalNumber)
{
	const auto association = findAssociation(associationsBegin_, associationsEnd_, signalNumber);
	if (association == associationsEnd_)	// there is no association for this signal number?
		return {};

	const auto previousSignalAction = association->second;
	const auto& lastAssociation = *(associationsEnd_ - 1);
	*association = lastAssociation;	// replace removed association with the last association in the range
	lastAssociation.~Association();
	--associationsEnd_;
	return previousSignalAction;
}

}	// namespace synchronization

}	// namespace distortos
