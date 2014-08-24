/**
 * \file
 * \brief SequenceAsserter class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-27
 */

#include "SequenceAsserter.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SequenceAsserter::assertSequence(const unsigned int sequencePoints)
{
	return expectedSequencePoint_ == sequencePoints;
}

void SequenceAsserter::sequencePoint(const unsigned int sequencePointt)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (expectedSequencePoint_ == sequencePointt)
		++expectedSequencePoint_;
}

}	// namespace test

}	// namespace distortos
