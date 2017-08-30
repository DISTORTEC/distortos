/**
 * \file
 * \brief Main code block.
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "TestCaseGroup.hpp"
#include "testCases.hpp"

#include "distortos/distortosConfiguration.h"

#ifdef CONFIG_BOARD_LEDS_ENABLE

#include "distortos/board/leds.hpp"

#include "distortos/chip/ChipOutputPin.hpp"

#endif	// def CONFIG_BOARD_LEDS_ENABLE

#include "distortos/ThisThread.hpp"

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Main code block of test application
 *
 * Runs all test cases. The result is signaled with blinking of all board's LEDs:
 * - success - slow blinking, 1 Hz frequency,
 * - failure - fast blinking, 10 Hz frequency.
 * If the board doesn't provide LEDs, the result can be examined with the debugger by checking the value of "result"
 * variable.
 */

int main()
{
#if	defined(CONFIG_BOARD_LEDS_ENABLE)

	for (size_t ledIndex {}; ledIndex < distortos::board::totalLeds; ++ledIndex)
	{
		auto& led = distortos::board::leds[ledIndex];
		led.set(true);
	}

#endif	// def CONFIG_BOARD_LEDS_ENABLE
	// "volatile" to allow examination of the value with debugger - the variable will not be optimized out
	const volatile auto result = distortos::test::testCases.run();

	// next line is a good place for a breakpoint that will be hit right after test cases
	const auto duration = result == true ? std::chrono::milliseconds{500} : std::chrono::milliseconds{50};
	while (1)
	{
#ifdef CONFIG_BOARD_LEDS_ENABLE

		for (size_t ledIndex {}; ledIndex < distortos::board::totalLeds; ++ledIndex)
		{
			auto& led = distortos::board::leds[ledIndex];
			led.set(!led.get());
		}

#endif	// def CONFIG_BOARD_LEDS_ENABLE

		distortos::ThisThread::sleepFor(duration);
	}
}
