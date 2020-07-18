/**
 * \file
 * \brief SoftwareTimerOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SoftwareTimerOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/DynamicSoftwareTimer.hpp"
#include "distortos/ThisThread.hpp"

#include <malloc.h>

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// wrapper for software timer
struct SoftwareTimerWrapper
{
public:

	/**
	 * \brief SoftwareTimerWrapper's constructor
	 *
	 * \param [in,out] value is a reference to volatile uint32_t variable which will be incremented when timer's
	 * function is executed
	 */

	explicit SoftwareTimerWrapper(volatile uint32_t& value) :
			softwareTimer_{&SoftwareTimerWrapper::function, std::ref(*this)},
			timePoint_{},
			period_{},
			value_{value},
			stateInFunction_{},
			stopRequested_{}
	{

	}

	/**
	 * \return reference to internal software timer
	 */

	SoftwareTimer& get()
	{
		return softwareTimer_;
	}

	/**
	 * \return result of SoftwareTimer::isRunning() executed at the end of timer's function
	 */

	bool getStateInFunction() const
	{
		return stateInFunction_;
	}

	/**
	 * \brief Requests timer's function to restart the timer.
	 *
	 * \param [in] timePoint is the time point used to restart timer
	 * \param [in] period is the period used to restart timer, 0 for one-shot software timers, default - 0
	 */

	void requestRestart(const TickClock::time_point timePoint, const TickClock::duration period = {})
	{
		timePoint_ = timePoint;
		period_ = period;
	}

	/**
	 * \brief Requests timer's function to stop the timer.
	 */

	void requestStop()
	{
		stopRequested_ = true;
	}

private:

	/**
	 * \brief Function used by software timer
	 */

	void function()
	{
		++value_;

		if (timePoint_ != decltype(timePoint_){} || period_ != decltype(period_){})
		{
			softwareTimer_.start(timePoint_, period_);
			timePoint_ = {};
			period_ = {};
		}

		if (stopRequested_ == true)
		{
			softwareTimer_.stop();
			stopRequested_ = {};
		}

		stateInFunction_ = softwareTimer_.isRunning();
	}

	/// internal software timer
	DynamicSoftwareTimer softwareTimer_;

	/// time point used to restart timer
	TickClock::time_point timePoint_;

	/// period used to restart timer
	TickClock::duration period_;

	/// reference to volatile uint32_t variable which will be incremented when timer's function is executed
	volatile uint32_t& value_;

	/// result of SoftwareTimer::isRunning() executed at the end of timer's function
	bool stateInFunction_;

	/// true if timer's function was requested to stop the timer, false otherwise
	bool stopRequested_;
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SoftwareTimerOperationsTestCase::run_() const
{
	const auto allocatedMemory = mallinfo().uordblks;

	{
		volatile uint32_t value {};
		SoftwareTimerWrapper softwareTimerWrapper {value};
		auto& softwareTimer = softwareTimerWrapper.get();

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
			if (softwareTimer.isRunning() != false || softwareTimerWrapper.getStateInFunction() != false ||
					value != 1 || TickClock::now() - start != duration + decltype(duration){1})
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
			if (softwareTimer.isRunning() != false || softwareTimerWrapper.getStateInFunction() != false ||
					value != 2 || TickClock::now() != wakeUpTimePoint)
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
				if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true ||
						value != 3 + iteration || TickClock::now() != wakeUpTimePoint + period * iteration)
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
					if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true ||
							value != 7 + iteration || TickClock::now() != wakeUpTimePoint + period * iteration)
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
					if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true ||
							value != 11 + iteration || TickClock::now() != wakeUpTimePoint + period * iteration)
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
			if (softwareTimer.isRunning() != false || softwareTimerWrapper.getStateInFunction() != false ||
					value != 15 || TickClock::now() != wakeUpTimePoint)
				return false;
		}
		{
			// request to stop one-shot timer from timer's function should be a no-op
			softwareTimerWrapper.requestStop();

			waitForNextTick();
			const auto wakeUpTimePoint = TickClock::now() + TickClock::duration{20};
			if (softwareTimer.start(wakeUpTimePoint) != 0)
				return false;
			if (softwareTimer.isRunning() != true || value != 15)	// must be started, but may not execute yet
				return false;

			while (softwareTimer.isRunning() == true);

			// must be stopped, function must be executed, wake up time point must equal what is expected
			if (softwareTimer.isRunning() != false || softwareTimerWrapper.getStateInFunction() != false ||
					value != 16 || TickClock::now() != wakeUpTimePoint)
				return false;
		}
		{
			constexpr auto period = TickClock::duration{8};

			waitForNextTick();
			const auto wakeUpTimePoint = TickClock::now() + TickClock::duration{18};
			if (softwareTimer.start(wakeUpTimePoint, period) != 0)
				return false;
			if (softwareTimer.isRunning() != true || value != 16)	// must be started, but may not execute yet
				return false;

			for (size_t iteration {}; iteration < 4; ++iteration)
			{
				while (softwareTimer.isRunning() == true && value == 16 + iteration);

				// must be still running, function must be executed, wake up time point must equal what is expected
				if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true ||
						value != 17 + iteration || TickClock::now() != wakeUpTimePoint + period * iteration)
					return false;
			}

			// request to stop periodic timer from timer's function should be executed in next cycle of timer
			softwareTimerWrapper.requestStop();

			while (softwareTimer.isRunning() == true && value == 20);

			// must be stopped, function must be executed, wake up time point must equal what is expected
			if (softwareTimer.isRunning() != false || softwareTimerWrapper.getStateInFunction() != false ||
					value != 21 || TickClock::now() != wakeUpTimePoint + period * 4)
				return false;

			ThisThread::sleepFor(period * 2);
			if (softwareTimer.isRunning() != false || value != 21)	// make sure it did not execute again
				return false;
		}
		{
			waitForNextTick();
			const auto wakeUpTimePoint1 = TickClock::now() + TickClock::duration{11};
			const auto wakeUpTimePoint2 = wakeUpTimePoint1 + TickClock::duration{9};
			// request to restart one-shot into one-shot from timer's function
			softwareTimerWrapper.requestRestart(wakeUpTimePoint2);
			if (softwareTimer.start(wakeUpTimePoint1) != 0)
				return false;
			if (softwareTimer.isRunning() != true || value != 21)	// must be started, but may not execute yet
				return false;

			while (softwareTimer.isRunning() == true && value == 21);

			// must be still running, function must be executed, wake up time point must equal what is expected
			if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true || value != 22 ||
					TickClock::now() != wakeUpTimePoint1)
				return false;

			const auto wakeUpTimePoint3 = wakeUpTimePoint2 + TickClock::duration{13};
			constexpr auto period3 = TickClock::duration{6};
			// request to restart one-shot into periodic from timer's function
			softwareTimerWrapper.requestRestart(wakeUpTimePoint3, period3);

			while (softwareTimer.isRunning() == true && value == 22);

			// must be still running, function must be executed, wake up time point must equal what is expected
			if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true || value != 23 ||
					TickClock::now() != wakeUpTimePoint2)
				return false;

			for (size_t iteration {}; iteration < 4; ++iteration)
			{
				while (softwareTimer.isRunning() == true && value == 23 + iteration);

				// must be still running, function must be executed, wake up time point must equal what is expected
				if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true ||
						value != 24 + iteration || TickClock::now() != wakeUpTimePoint3 + period3 * iteration)
					return false;
			}

			const auto wakeUpTimePoint4 = wakeUpTimePoint3 + period3 * 4 + TickClock::duration{17};
			constexpr auto period4 = TickClock::duration{15};
			// request to restart periodic into periodic with different settings from timer's function
			softwareTimerWrapper.requestRestart(wakeUpTimePoint4, period4);

			while (softwareTimer.isRunning() == true && value == 27);

			// must be still running, function must be executed, wake up time point must equal what is expected
			if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true || value != 28 ||
					TickClock::now() != wakeUpTimePoint3 + period3 * 4)
				return false;

			for (size_t iteration {}; iteration < 4; ++iteration)
			{
				while (softwareTimer.isRunning() == true && value == 28 + iteration);

				// must be still running, function must be executed, wake up time point must equal what is expected
				if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true ||
						value != 29 + iteration || TickClock::now() != wakeUpTimePoint4 + period4 * iteration)
					return false;
			}

			const auto wakeUpTimePoint5 = wakeUpTimePoint4 + period4 * 4 + TickClock::duration{12};
			// request to restart periodic into one-shot from timer's function
			softwareTimerWrapper.requestRestart(wakeUpTimePoint5);

			while (softwareTimer.isRunning() == true && value == 32);

			// must be still running, function must be executed, wake up time point must equal what is expected
			if (softwareTimer.isRunning() != true || softwareTimerWrapper.getStateInFunction() != true || value != 33 ||
					TickClock::now() != wakeUpTimePoint4 + period4 * 4)
				return false;

			while (softwareTimer.isRunning() == true && value == 33);

			// must be stopped, function must be executed, wake up time point must equal what is expected
			if (softwareTimer.isRunning() != false || softwareTimerWrapper.getStateInFunction() != false ||
					value != 34 || TickClock::now() != wakeUpTimePoint5)
				return false;
		}
	}

	if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after test
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
