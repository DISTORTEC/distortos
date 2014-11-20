/**
 * \file
 * \brief SoftwareTimerOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-20
 */

#include "SoftwareTimerOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/SoftwareTimer.hpp"
#include "distortos/ThisThread.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SoftwareTimerOperationsTestCase::run_() const
{
	constexpr auto singleDuration = TickClock::duration{10};

	volatile uint32_t value {};
	auto softwareTimer = makeSoftwareTimer(
			[&value]()
			{
				++value;
			});

	if (softwareTimer.isRunning() != false || value != 0)	// initially must be stopped and must not execute
		return false;

	waitForNextTick();
	softwareTimer.start(singleDuration);
	if (softwareTimer.isRunning() != true || value != 0)	// must be started, but may not execute yet
		return false;

	softwareTimer.stop();
	if (softwareTimer.isRunning() != false || value != 0)	// must be stopped, must not execute
		return false;

	ThisThread::sleepFor(singleDuration * 2);
	if (softwareTimer.isRunning() != false || value != 0)	// make sure it did not execute
		return false;

	waitForNextTick();
	softwareTimer.start(singleDuration);
	if (softwareTimer.isRunning() != true || value != 0)	// must be started, but may not execute yet
		return false;

	const auto start = TickClock::now();
	while (softwareTimer.isRunning() == true)
	{

	}

	// must be stopped, function must be executed, real duration must equal what is expected
	if (softwareTimer.isRunning() != false || value != 1 ||
			TickClock::now() - start != singleDuration + decltype(singleDuration){1})
		return false;

	waitForNextTick();
	const auto wakeUpTimePoint = TickClock::now() + singleDuration;
	softwareTimer.start(wakeUpTimePoint);
	if (softwareTimer.isRunning() != true || value != 1)	// must be started, but may not execute yet
		return false;

	while (softwareTimer.isRunning() == true)
	{

	}

	// must be stopped, function must be executed, wake up time point must equal what is expected
	if (softwareTimer.isRunning() != false || value != 2 || TickClock::now() != wakeUpTimePoint)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
