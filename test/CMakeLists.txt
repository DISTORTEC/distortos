#
# file: CMakeLists.txt
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(CONFIG_TEST_APPLICATION_ENABLE)

	include(distortos-utilities)

#-----------------------------------------------------------------------------------------------------------------------
# distortosTest application
#-----------------------------------------------------------------------------------------------------------------------

	add_executable(distortosTest
			main.cpp
			OperationCountingType.cpp
			PrioritizedTestCase.cpp
			priorityTestPhases.cpp
			SequenceAsserter.cpp
			TestCaseCommon.cpp
			TestCase.cpp
			TestCaseGroup.cpp
			testCases.cpp
			waitForNextTick.cpp
			wasteTime.cpp)
	set_target_properties(distortosTest PROPERTIES
			CXX_STANDARD 11
			CXX_STANDARD_REQUIRED ON)
	target_include_directories(distortosTest PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR})
	target_link_libraries(distortosTest PRIVATE
			distortos::distortos
			distortos::allCxxFlags)

	include(architecture/distortosTest-sources.cmake)
	include(CallOnce/distortosTest-sources.cmake)
	include(ConditionVariable/distortosTest-sources.cmake)
	include(Mutex/distortosTest-sources.cmake)
	include(Queue/distortosTest-sources.cmake)
	include(Semaphore/distortosTest-sources.cmake)
	include(Signals/distortosTest-sources.cmake)
	include(SoftwareTimer/distortosTest-sources.cmake)
	include(Thread/distortosTest-sources.cmake)

	distortosBin(distortosTest distortosTest.bin)
	distortosDmp(distortosTest distortosTest.dmp)
	distortosHex(distortosTest distortosTest.hex)
	distortosLss(distortosTest distortosTest.lss)
	distortosMap(distortosTest distortosTest.map)
	distortosSize(distortosTest)

endif()
