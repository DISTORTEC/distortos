/**
 * \file
 * \brief SequenceAsserter class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SequenceAsserter.hpp"

#include "distortos/InterruptMaskingLock.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SequenceAsserter::assertSequence(const unsigned int sequencePoints)
{
	return expectedSequencePoint_ == sequencePoints && sequencePointsCount_ == sequencePoints;
}

void SequenceAsserter::sequencePoint(const unsigned int sequencePointt)
{
	const InterruptMaskingLock interruptMaskingLock;

	++sequencePointsCount_;

	if (expectedSequencePoint_ == sequencePointt)
		++expectedSequencePoint_;
}

}	// namespace test

}	// namespace distortos
