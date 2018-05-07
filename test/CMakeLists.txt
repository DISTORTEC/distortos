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

	add_executable(distortosTest.elf
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
	set_target_properties(distortosTest.elf PROPERTIES
			CXX_STANDARD 11
			CXX_STANDARD_REQUIRED ON)
	target_include_directories(distortosTest.elf PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR})
	target_link_libraries(distortosTest.elf
			distortos::distortos
			distortos::allCxxFlags
			-Xlinker -Map="${CMAKE_CURRENT_BINARY_DIR}/distortosTest.map")

	include(architecture/distortosTest.elf-sources.cmake)
	include(CallOnce/distortosTest.elf-sources.cmake)
	include(ConditionVariable/distortosTest.elf-sources.cmake)
	include(Mutex/distortosTest.elf-sources.cmake)
	include(Queue/distortosTest.elf-sources.cmake)
	include(Semaphore/distortosTest.elf-sources.cmake)
	include(Signals/distortosTest.elf-sources.cmake)
	include(SoftwareTimer/distortosTest.elf-sources.cmake)
	include(Thread/distortosTest.elf-sources.cmake)

	bin(distortosTest.elf distortosTest.bin)
	dmp(distortosTest.elf distortosTest.dmp)
	hex(distortosTest.elf distortosTest.hex)
	lss(distortosTest.elf distortosTest.lss)
	size(distortosTest.elf)

endif()
