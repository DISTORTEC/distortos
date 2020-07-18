/**
 * \file
 * \brief SequenceAsserter class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SEQUENCEASSERTER_HPP_
#define TEST_SEQUENCEASSERTER_HPP_

namespace distortos
{

namespace test
{

/**
 * \brief SequenceAsserter class is used to assert that some sequencePoint of events are performed in specific order
 *
 * This asserter expects a number of calls to sequencePoint(x) with x increasing by 1 with each call, starting with 0.
 * If this sequence occurs, a call to assertSequence(y) will return true if y equals the number of calls to
 * sequencePoint() (y equals "last x" + 1).
 */

class SequenceAsserter
{
public:

	/**
	 * \brief SequenceAsserter's constructor
	 */

	constexpr SequenceAsserter() :
			expectedSequencePoint_{},
			sequencePointsCount_{}
	{

	}

	/**
	 * \brief Checks whether the sequence of calls to sequencePoint() was correct.
	 *
	 * \param [in] sequencePoints is the number of calls to sequencePoint() that is expected
	 *
	 * \return true if sequence was correct, false otherwise
	 */

	bool assertSequence(unsigned int sequencePoints);

	/**
	 * \brief Increments the sequence by 1 if the order of calls matches. Each entry to this function is counted.
	 *
	 * Entry to this function is synchronized with interrupt masking lock.
	 *
	 * \param [in] sequencePointt is the sequence point
	 */

	void sequencePoint(unsigned int sequencePointt);

private:

	/// next expected sequence point
	unsigned int expectedSequencePoint_;

	/// count of calls to sequencePoint()
	unsigned int sequencePointsCount_;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_SEQUENCEASSERTER_HPP_
