/**
 * \file
 * \brief SoftwareTimerOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SoftwareTimerOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/StaticSoftwareTimer.hpp"
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
	volatile uint32_t value {};
	auto softwareTimer = makeStaticSoftwareTimer(
			[&value]()
			{
				++value;
			});

	{
		constexpr auto duration = TickClock::duration{11};

		if (softwareTimer.isRunning() != false || value != 0)	// initially must be stopped and must not execute
			return false;

		waitForNextTick();
		if (softwareTimer.start(duration) != 0)
			return false;
		if (softwareTimer.isRunning() != true || value != 0)	// must be started, but may not execute yet
			return false;

		if (softwareTimer.stop() != 0)
			return false;
		if (softwareTimer.isRunning() != false || value != 0)	// must be stopped, must not execute
			return false;

		ThisThread::sleepFor(duration * 2);
		if (softwareTimer.isRunning() != false || value != 0)	// make sure it did not execute
			return false;
	}
	{
		constexpr auto duration = TickClock::duration{9};

		waitForNextTick();
		if (softwareTimer.start(duration) != 0)
			return false;
		if (softwareTimer.isRunning() != true || value != 0)	// must be started, but may not execute yet
			return false;

		const auto start = TickClock::now();
		while (softwareTimer.isRunning() == true);

		// must be stopped, function must be executed, real duration must equal what is expected
		if (softwareTimer.isRunning() != false || value != 1 ||
				TickClock::now() - start != duration + decltype(duration){1})
			return false;
	}
	{
		waitForNextTick();
		const auto wakeUpTimePoint = TickClock::now() + TickClock::duration{13};
		if (softwareTimer.start(wakeUpTimePoint) != 0)
			return false;
		if (softwareTimer.isRunning() != true || value != 1)	// must be started, but may not execute yet
			return false;

		while (softwareTimer.isRunning() == true);

		// must be stopped, function must be executed, wake up time point must equal what is expected
		if (softwareTimer.isRunning() != false || value != 2 || TickClock::now() != wakeUpTimePoint)
			return false;
	}
	{
		constexpr auto period = TickClock::duration{6};

		waitForNextTick();
		const auto wakeUpTimePoint = TickClock::now() + TickClock::duration{17};
		if (softwareTimer.start(wakeUpTimePoint, period) != 0)
			return false;
		if (softwareTimer.isRunning() != true || value != 2)	// must be started, but may not execute yet
			return false;

		for (size_t iteration {}; iteration < 4; ++iteration)
		{
			while (softwareTimer.isRunning() == true && value == 2 + iteration);

			// must be still running, function must be executed, wake up time point must equal what is expected
			if (softwareTimer.isRunning() != true || value != 3 + iteration ||
					TickClock::now() != wakeUpTimePoint + period * iteration)
				return false;
		}

		if (softwareTimer.stop() != 0)
			return false;
		if (softwareTimer.isRunning() != false || value != 6)	// must be stopped
			return false;

		ThisThread::sleepFor(period * 2);
		if (softwareTimer.isRunning() != false || value != 6)	// make sure it did not execute again
			return false;
	}
	{
		// restart one-shot into one-shot
		for (size_t iteration {}; iteration < 4; ++iteration)
		{
			constexpr auto duration = TickClock::duration{15};
			const auto wakeUpTimePoint = TickClock::now() + duration;
			if (softwareTimer.start(wakeUpTimePoint) != 0)
				return false;
			if (softwareTimer.isRunning() != true || value != 6)	// must be started, but may not execute yet
				return false;

			ThisThread::sleepUntil(wakeUpTimePoint - duration / 2);

			if (softwareTimer.isRunning() != true || value != 6)	// must be started, but may not execute yet
				return false;
		}

		// restart one-shot into periodic, then periodic into periodic
		for (size_t iteration {}; iteration < 4; ++iteration)
		{
			constexpr auto delay = TickClock::duration{12};

			const auto wakeUpTimePoint = TickClock::now() + delay;
			if (softwareTimer.start(wakeUpTimePoint, TickClock::duration{10}) != 0)
				return false;
			if (softwareTimer.isRunning() != true || value != 6)	// must be started, but may not execute yet
				return false;

			ThisThread::sleepUntil(wakeUpTimePoint - delay / 2);

			if (softwareTimer.isRunning() != true || value != 6)	// must be started, but may not execute yet
				return false;
		}

		// restart periodic into periodic
		{
			constexpr auto period = TickClock::duration{19};

			waitForNextTick();
			const auto wakeUpTimePoint = TickClock::now() + TickClock::duration{14};
			if (softwareTimer.start(wakeUpTimePoint, period) != 0)
				return false;
			if (softwareTimer.isRunning() != true || value != 6)	// must be started, but may not execute yet
				return false;

			for (size_t iteration {}; iteration < 4; ++iteration)
			{
				while (softwareTimer.isRunning() == true && value == 6 + iteration);

				// must be still running, function must be executed, wake up time point must equal what is expected
				if (softwareTimer.isRunning() != true || value != 7 + iteration ||
						TickClock::now() != wakeUpTimePoint + period * iteration)
					return false;
			}
		}

		// restart periodic into periodic with different settings
		{
			constexpr auto period = TickClock::duration{7};

			waitForNextTick();
			const auto wakeUpTimePoint = TickClock::now() + TickClock::duration{5};
			if (softwareTimer.start(wakeUpTimePoint, period) != 0)
				return false;
			if (softwareTimer.isRunning() != true || value != 10)	// must be started, but may not execute yet
				return false;

			for (size_t iteration {}; iteration < 4; ++iteration)
			{
				while (softwareTimer.isRunning() == true && value == 10 + iteration);

				// must be still running, function must be executed, wake up time point must equal what is expected
				if (softwareTimer.isRunning() != true || value != 11 + iteration ||
						TickClock::now() != wakeUpTimePoint + period * iteration)
					return false;
			}
		}

		// restart periodic into one-shot
		waitForNextTick();
		const auto wakeUpTimePoint = TickClock::now() + TickClock::duration{16};
		if (softwareTimer.start(wakeUpTimePoint) != 0)
			return false;
		if (softwareTimer.isRunning() != true || value != 14)	// must be started, but may not execute yet
			return false;

		while (softwareTimer.isRunning() == true);

		// must be stopped, function must be executed, wake up time point must equal what is expected
		if (softwareTimer.isRunning() != false || value != 15 || TickClock::now() != wakeUpTimePoint)
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
