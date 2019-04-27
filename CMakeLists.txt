#
# file: CMakeLists.txt
#
# This is the main CMakeLists.txt for distortos
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
	message(FATAL_ERROR "In-source builds are not supported. Please use separate directory for build.")
endif()

cmake_minimum_required(VERSION 3.7)
project(distortos)

distortosSetConfiguration(BOOLEAN
		distortos_Build_00_Static_destructors
		OFF
		HELP "Enable static destructors.

		Enable destructors for objects with static storage duration. As embedded applications almost never \"exit\",
		these destructors are usually never executed, wasting ROM."
		OUTPUT_NAME DISTORTOS_STATIC_DESTRUCTORS_ENABLE)

distortosSetConfiguration(INTEGER
		distortos_Scheduler_00_Tick_frequency
		1000
		MIN 1
		HELP "System's tick frequency, Hz."
		OUTPUT_NAME DISTORTOS_TICK_FREQUENCY)

distortosSetConfiguration(INTEGER
		distortos_Scheduler_01_Round_robin_frequency
		10
		MIN 1
		MAX ${distortos_Scheduler_00_Tick_frequency}
		HELP "Round-robin frequency, Hz."
		OUTPUT_NAME DISTORTOS_ROUND_ROBIN_FREQUENCY)

distortosSetConfiguration(BOOLEAN
		distortos_Scheduler_02_Support_for_signals
		OFF
		HELP "Enable support for signals.

		Enable namespaces, functions and classes related to signals:
		- ThisThread::Signals namespace;
		- Thread::generateSignal();
		- Thread::getPendingSignalSet();
		- Thread::queueSignal();
		- DynamicSignalsReceiver class;
		- SignalInformationQueueWrapper class;
		- SignalsCatcher class;
		- SignalsReceiver class;
		- StaticSignalsReceiver class;

		When this options is not selected, these namespaces, functions and classes are not available at all."
		OUTPUT_NAME DISTORTOS_SIGNALS_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Scheduler_03_Support_for_thread_detachment
		OFF
		HELP "Enable support for thread detachment.

		Enable functions that \"detach\" dynamic threads:
		- ThisThread::detach();
		- Thread::detach();

		When this options is not selected, these functions are not available at all.

		When dynamic and detached thread terminates, it will be added to the global list of threads pending for deferred
		deletion. The thread will actually be deleted in idle thread, but only when two mutexes are successfully locked:
		- mutex that protects dynamic memory allocator;
		- mutex that synchronizes access to the list of threads pending for deferred deletion;"
		OUTPUT_NAME DISTORTOS_THREAD_DETACH_ENABLE)

distortosSetConfiguration(INTEGER
		distortos_Scheduler_04_Main_thread_stack_size
		2048
		MIN 1
		HELP "Size (in bytes) of stack used by thread with main() function."
		OUTPUT_NAME DISTORTOS_MAIN_THREAD_STACK_SIZE)

distortosSetConfiguration(INTEGER
		distortos_Scheduler_05_Main_thread_priority
		127
		MIN 1
		MAX 255
		HELP "Initial priority of main thread."
		OUTPUT_NAME DISTORTOS_MAIN_THREAD_PRIORITY)

if(distortos_Scheduler_02_Support_for_signals)

	distortosSetConfiguration(BOOLEAN
			distortos_Scheduler_06_Reception_of_signals_by_main_thread
			OFF
			HELP "Enable reception of signals for main thread."
			OUTPUT_NAME DISTORTOS_MAIN_THREAD_CAN_RECEIVE_SIGNALS)

	if(distortos_Scheduler_06_Reception_of_signals_by_main_thread)

		distortosSetConfiguration(INTEGER
				distortos_Scheduler_07_Queued_signals_for_main_thread
				0
				MIN 0
				HELP "Maximal number of queued signals for main thread. 0 disables queuing of signals for main thread."
				OUTPUT_NAME DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS)

		distortosSetConfiguration(INTEGER
				distortos_Scheduler_08_SignalAction_objects_for_main_thread
				0
				MIN 0
				MAX 32
				HELP "Maximal number of different SignalAction objects for main thread. 0 disables catching of signals
				for main thread."
				OUTPUT_NAME DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS)

	endif(distortos_Scheduler_06_Reception_of_signals_by_main_thread)

endif(distortos_Scheduler_02_Support_for_signals)

distortosSetConfiguration(BOOLEAN
		distortos_Checks_00_Context_of_functions
		OFF
		HELP "Check context of functions.

		Some functions may only be used from thread context, as using them from interrupt context results in undefined
		behaviour. There are several groups of functions to which this restriction applies (some functions fall into
		several categories at once):
		- all blocking functions, like callOnce(), FifoQueue::push(), Semaphore::wait(), ..., as an attempt to block
		current thread of execution (not to be confused with current thread) is not possible in interrupt context;
		- all mutex functions, as the concept of ownership by a thread - core feature of mutex - cannot be fulfilled in
		interrupt context;
		- all functions from ThisThread namespace (including ThisThread::Signals namespace), as in interrupt context
		they would access a random thread that happened to be executing at that particular moment;

		Using such functions from interrupt context is a common bug in applications which can be easily introduced and
		very hard to find, as the symptoms may appear only under certain circumstances.

		Selecting this option enables context checks in all functions with such requirements. If any of them is used
		from interrupt context, FATAL_ERROR() will be called."
		OUTPUT_NAME DISTORTOS_CHECK_FUNCTION_CONTEXT_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Checks_01_Stack_pointer_range_during_context_switch
		OFF
		HELP "Check stack pointer range during context switch.

		Simple range checking of preempted thread's stack pointer can be performed during context switches. It is
		relatively fast, but cannot detect all stack overflows. The check is done before the software stack frame is
		pushed on thread's stack, but the size of this pending stack frame is accounted for - the intent is to detect a
		stack overflow which is about to happen, before it can cause (further) data corrution. FATAL_ERROR() will be
		called if the stack pointer is outside valid range."
		OUTPUT_NAME DISTORTOS_CHECK_STACK_POINTER_RANGE_CONTEXT_SWITCH_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Checks_02_Stack_pointer_range_during_system_tick
		OFF
		HELP "Check stack pointer range during system tick.

		Similar to \"distortos_Checks_01_Stack_pointer_range_during_context_switch\", but executed during every system
		tick."
		OUTPUT_NAME DISTORTOS_CHECK_STACK_POINTER_RANGE_SYSTEM_TICK_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Checks_03_Stack_guard_contents_during_context_switch
		OFF
		HELP "Check stack guard contents during context switch.

		Selecting this option extends stacks for all threads (including main() thread) with a \"stack guard\" at the
		overflow end. This \"stack guard\" - just as the whole stack - is filled with a sentinel value 0xed419f25 during
		thread initialization. The contents of \"stack guard\" of preempted thread are checked during each context
		switch and if any byte has changed, FATAL_ERROR() will be called.

		This method is slower than simple stack pointer range checking, but is able to detect stack overflows much more
		reliably. It is still sufficiently fast, assuming that the size of \"stack guard\" is reasonable.

		Be advised that uninitialized variables on stack which are larger than size of \"stack guard\" can create
		\"holes\" in the stack, thus circumventing this detection mechanism. This especially applies to arrays used as
		buffers."
		OUTPUT_NAME DISTORTOS_CHECK_STACK_GUARD_CONTEXT_SWITCH_ENABLE)

distortosSetConfiguration(BOOLEAN
		distortos_Checks_04_Stack_guard_contents_during_system_tick
		OFF
		HELP "Check stack guard contents during system tick.

		Similar to \"distortos_Checks_03_Stack_guard_contents_during_context_switch\", but executed during every system
		tick."
		OUTPUT_NAME DISTORTOS_CHECK_STACK_GUARD_SYSTEM_TICK_ENABLE)

if(distortos_Checks_03_Stack_guard_contents_during_context_switch OR
		distortos_Checks_04_Stack_guard_contents_during_system_tick)

	distortosSetConfiguration(INTEGER
			distortos_Checks_05_Stack_guard_size
			32
			MIN 1
			HELP "Size (in bytes) of \"stack guard\".

			Any value which is not a multiple of stack alignment required by architecture, will be rounded up."
			OUTPUT_NAME DISTORTOS_STACK_GUARD_SIZE)

endif(distortos_Checks_03_Stack_guard_contents_during_context_switch OR
		distortos_Checks_04_Stack_guard_contents_during_system_tick)

distortosSetConfiguration(BOOLEAN
		distortos_Checks_06_Asserts
		ON
		HELP "Enable asserts.

		Some errors, which are clearly program bugs, are never reported using error codes. When this option is enabled,
		these preconditions, postconditions, invariants and assertions are checked with assert() macro. On the other
		hand - with this option disabled, they are completely ignored.

		It is highly recommended to keep this option enabled until the application is thoroughly tested."
		NO_OUTPUT)

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to RelWithDebInfo")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE RelWithDebInfo)
endif()

#-----------------------------------------------------------------------------------------------------------------------
# local functions
#-----------------------------------------------------------------------------------------------------------------------

#
# Adds given folders to doxygen `INPUT`, `INCLUDE_PATH` and/or `EXCLUDE`.
#
# `doxygen([INPUT inputFolder ...] [INCLUDE_PATH includePathFolder ...] [EXCLUDE excludeFolder ...])`
#

function(doxygen)
	cmake_parse_arguments(PARSE_ARGV 0 DOXYGEN "" "" "EXCLUDE;INCLUDE_PATH;INPUT")
	if(DOXYGEN_EXCLUDE)
		set(envDoxygenExclude "$ENV{doxygenExclude}")
		list(APPEND envDoxygenExclude ${DOXYGEN_EXCLUDE})
		set(ENV{doxygenExclude} "${envDoxygenExclude}")
	endif()
	if(DOXYGEN_INCLUDE_PATH)
		set(envDoxygenIncludePath "$ENV{doxygenIncludePath}")
		list(APPEND envDoxygenIncludePath ${DOXYGEN_INCLUDE_PATH})
		set(ENV{doxygenIncludePath} "${envDoxygenIncludePath}")
	endif()
	if(DOXYGEN_INPUT)
		set(envDoxygenInput "$ENV{doxygenInput}")
		list(APPEND envDoxygenInput ${DOXYGEN_INPUT})
		set(ENV{doxygenInput} "${envDoxygenInput}")
	endif()
endfunction()

#
# Recursively gets `property` from the `target` and all of its (interface) link libraries, saving the result to
# `outputVariable`.
#

function(getTargetPropertyRecursive outputVariable target property)
	get_target_property(type ${target} TYPE)
	if(type STREQUAL INTERFACE_LIBRARY)
		set(interfacePrefix INTERFACE_)
	else()
		unset(interfacePrefix)
	endif()

	get_target_property(linkLibraries ${target} "${interfacePrefix}LINK_LIBRARIES")
	foreach(linkLibrary ${linkLibraries})
		if(linkLibrary)
			getTargetPropertyRecursive(${outputVariable} ${linkLibrary} ${property})
		endif()
	endforeach()

	get_target_property(propertyValue ${target} "${interfacePrefix}${property}")
	if(propertyValue)
		list(APPEND ${outputVariable} ${propertyValue})
		list(REMOVE_DUPLICATES ${outputVariable})
	endif()

	set(${outputVariable} ${${outputVariable}} PARENT_SCOPE)
endfunction()

#-----------------------------------------------------------------------------------------------------------------------
# distortos static library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos STATIC "")
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.8)
	target_compile_features(distortos PUBLIC
			cxx_std_11
			c_std_99)
else()
	# equivalents of C++11 and C99 for earlier versions of CMake
	target_compile_features(distortos PUBLIC
			cxx_constexpr
			c_restrict)
endif()
target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_BINARY_DIR}/include
		include)
target_link_libraries(distortos PUBLIC
		littlefs1)

if(NOT distortos_Checks_06_Asserts)
	target_compile_definitions(distortos PUBLIC
			NDEBUG)
endif(NOT distortos_Checks_06_Asserts)

include(${DISTORTOS_BOARD_PATH}/distortos-board-sources.cmake)

set(DISTORTOS_BOARD_VERSION_MIN 11)
if(NOT DEFINED DISTORTOS_BOARD_VERSION OR NOT DISTORTOS_BOARD_VERSION EQUAL DISTORTOS_BOARD_VERSION_MIN)
	if(NOT DEFINED DISTORTOS_BOARD_VERSION)
		message(FATAL_ERROR "Generated board has no version information. "
				"Please regenerate your board with scripts/generateBoard.py")
	else()
		message(FATAL_ERROR "Generated board has version ${DISTORTOS_BOARD_VERSION} while"
				" ${DISTORTOS_BOARD_VERSION_MIN} is required. "
				"Please regenerate your board with scripts/generateBoard.py")
	endif()
endif()

include(source/distortos-sources.cmake)

#-----------------------------------------------------------------------------------------------------------------------
# distortos-distortos interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos-distortos INTERFACE)
target_link_libraries(distortos-distortos INTERFACE
		-Wl,--whole-archive
		distortos
		-Wl,--no-whole-archive)

#-----------------------------------------------------------------------------------------------------------------------
# distortos::distortos alias library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos::distortos ALIAS distortos-distortos)

#-----------------------------------------------------------------------------------------------------------------------
# distortos-doxygen
#-----------------------------------------------------------------------------------------------------------------------

doxygen(INPUT ${CMAKE_CURRENT_SOURCE_DIR}/documentation ${CMAKE_CURRENT_BINARY_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/include
		INCLUDE_PATH ${CMAKE_CURRENT_BINARY_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include)

getTargetPropertyRecursive(doxygenCompileOptions distortos COMPILE_OPTIONS)
# for now C++11 standard with GNU extensions is hardcoded here...
execute_process(COMMAND ${CMAKE_COMMAND} -E echo ""
		COMMAND ${CMAKE_CXX_COMPILER} ${doxygenCompileOptions} -std=gnu++11 -E -P -dD -x c++ -
		OUTPUT_VARIABLE preprocessedOutput)
string(REGEX REPLACE "[\r\n]+" ";" preprocessedOutput "${preprocessedOutput}")
foreach(predefinedEntry ${preprocessedOutput})
	if("${predefinedEntry}" MATCHES "^#define ([^ ]+) (.*)$")
		set(key ${CMAKE_MATCH_1})
		set(value ${CMAKE_MATCH_2})
		string(REPLACE "\"" "\\\"" value "${value}")
		list(APPEND doxygenPredefined "\"${key}=${value}\"")
	endif()
endforeach()

execute_process(COMMAND git describe --dirty
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE gitDescribeResult
		OUTPUT_VARIABLE doxygenProjectNumber
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT gitDescribeResult EQUAL 0)
	string(TIMESTAMP doxygenProjectNumber "%Y%m%d%H%M%S")
endif()

foreach(doxygenExcludeEntry $ENV{doxygenExclude})
	string(APPEND doxygenExclude " \"${doxygenExcludeEntry}\"")
endforeach()
foreach(doxygenIncludePathEntry $ENV{doxygenIncludePath})
	string(APPEND doxygenIncludePath " \"${doxygenIncludePathEntry}\"")
endforeach()
foreach(doxygenInputEntry $ENV{doxygenInput})
	string(APPEND doxygenInput " \"${doxygenInputEntry}\"")
endforeach()
set(doxygenStripFromIncludePathList "$ENV{doxygenIncludePath};$ENV{doxygenInput}")
list(REMOVE_DUPLICATES doxygenStripFromIncludePathList)
foreach(doxygenStripFromIncludePathEntry ${doxygenStripFromIncludePathList})
	string(APPEND doxygenStripFromIncludePath " \"${doxygenStripFromIncludePathEntry}\"")
endforeach()
string(REPLACE ";" " " doxygenPredefined "${doxygenPredefined}")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.extension
		"EXCLUDE =${doxygenExclude}\n"
		"INCLUDE_PATH =${doxygenIncludePath}\n"
		"INPUT =${doxygenInput}\n"
		"PREDEFINED = DOXYGEN \"__attribute__(x)=\" \"__GNUC_PREREQ(x, y)=1\" ${doxygenPredefined}\n"
		"PROJECT_NUMBER = ${doxygenProjectNumber}\n"
		"STRIP_FROM_INC_PATH =${doxygenStripFromIncludePath}\n"
		"STRIP_FROM_PATH = \"${CMAKE_CURRENT_SOURCE_DIR}\"\n")

set(doxyfiles "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile;${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.extension")
add_custom_target(distortos-doxygen
		${CMAKE_COMMAND} -D "DOXYFILES=${doxyfiles}" -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake
		VERBATIM)

#-----------------------------------------------------------------------------------------------------------------------
# update distortos configuration
#-----------------------------------------------------------------------------------------------------------------------

set(newConfigurationVersion 3)

if(DISTORTOS_CONFIGURATION_VERSION LESS newConfigurationVersion)
	message(STATUS "Updated configuration from version ${DISTORTOS_CONFIGURATION_VERSION} to "
			"${newConfigurationVersion}")
endif(DISTORTOS_CONFIGURATION_VERSION LESS newConfigurationVersion)

unset(DISTORTOS_CONFIGURATION_VERSION)
distortosSetConfiguration(INTEGER
		DISTORTOS_CONFIGURATION_VERSION
		${newConfigurationVersion}
		INTERNAL
		NO_OUTPUT)

#-----------------------------------------------------------------------------------------------------------------------
# cleanup distortos configuration
#-----------------------------------------------------------------------------------------------------------------------

if(DISTORTOS_CONFIGURATION_NAMES)
	set(namesToUnset ${DISTORTOS_CONFIGURATION_NAMES})
	list(REMOVE_ITEM namesToUnset ${DISTORTOS_ACTIVE_CONFIGURATION_NAMES})
endif()

list(SORT DISTORTOS_ACTIVE_CONFIGURATION_NAMES)
set(DISTORTOS_CONFIGURATION_NAMES ${DISTORTOS_ACTIVE_CONFIGURATION_NAMES} CACHE INTERNAL "")

get_cmake_property(variables VARIABLES)
foreach(variable ${variables})
	get_property(type CACHE ${variable} PROPERTY TYPE)
	if(type AND NOT type STREQUAL STATIC AND variable MATCHES "^distortos_.+" AND
			NOT variable IN_LIST DISTORTOS_ACTIVE_CONFIGURATION_NAMES AND
			NOT variable IN_LIST namesToUnset)
		# unset unknown cache variables which are prefixed with "distortos_", excluding CMake's "STATIC" variables
		list(APPEND namesToUnset ${variable})
	endif()
endforeach()

if(namesToUnset)
	message(STATUS "Unsetting removed configuration variables:")
	foreach(nameToUnset ${namesToUnset})
		message(STATUS "${nameToUnset} = ${${nameToUnset}}")
		unset(${nameToUnset} CACHE)
	endforeach()
endif()

#-----------------------------------------------------------------------------------------------------------------------
# distortosConfiguration.h
#-----------------------------------------------------------------------------------------------------------------------

message(STATUS "Generating ${CMAKE_CURRENT_BINARY_DIR}/include/distortos/distortosConfiguration.h")

unset(distortosConfiguration)
foreach(name ${DISTORTOS_ACTIVE_CONFIGURATION_NAMES} ${DISTORTOS_FIXED_CONFIGURATION_NAMES})

	set(type ${${name}_TYPE})
	set(outputName ${${name}_OUTPUT_NAME})
	set(outputTypes ${${name}_OUTPUT_TYPES})
	set(value ${${name}})

	foreach(outputType ${outputTypes})

		if(outputType STREQUAL type)
			set(configurationName ${outputName})
			set(configurationValue ${value})
		elseif(outputType STREQUAL BOOLEAN)
			string(TOUPPER ${outputName}_${value} configurationName)
			set(configurationValue ON)
		elseif(outputType STREQUAL INTEGER)
			distortosCheckInteger("${name}" "${value}" -2147483648 2147483647)
			set(configurationName ${outputName})
			set(configurationValue ${value})
		else()
			message(FATAL_ERROR "Conversion from \"${type}\" to \"${outputType}\" is not implemented/possible")
		endif()

		if(outputType STREQUAL BOOLEAN)
			if(configurationValue)
				list(APPEND distortosConfiguration "#define ${configurationName} 1")
			endif()
		elseif(outputType STREQUAL INTEGER)
			list(APPEND distortosConfiguration "#define ${configurationName} ${configurationValue}")
		elseif(outputType STREQUAL STRING)
			list(APPEND distortosConfiguration "#define ${configurationName} \"${configurationValue}\"")
		endif()

	endforeach()
endforeach()

string(TIMESTAMP timestamp "%Y-%m-%d %H:%M:%S")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/include/distortos/distortosConfiguration.h
		"/**\n"
		" * \\file\n"
		" * \\brief distortos configuration\n"
		" *\n"
		" * \\warning\n"
		" * Automatically generated file - do not edit!\n"
		" *\n"
		" * \\date ${timestamp}\n"
		" */\n"
		"\n"
		"#ifndef INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_\n"
		"#define INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_\n"
		"\n")
list(SORT distortosConfiguration)
foreach(line ${distortosConfiguration})
	file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/include/distortos/distortosConfiguration.h "${line}\n")
endforeach()
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/include/distortos/distortosConfiguration.h
		"\n"
		"#ifndef DISTORTOS_STACK_GUARD_SIZE\n"
		"	#define DISTORTOS_STACK_GUARD_SIZE 0\n"
		"#endif	/* !def DISTORTOS_STACK_GUARD_SIZE */\n"
		"\n"
		"#endif	/* INCLUDE_DISTORTOS_DISTORTOSCONFIGURATION_H_ */")

#-----------------------------------------------------------------------------------------------------------------------
# distortosConfiguration.cmake
#-----------------------------------------------------------------------------------------------------------------------

message(STATUS "Generating ${CMAKE_CURRENT_BINARY_DIR}/distortosConfiguration.cmake")

set(cmakeConfigurationNames
		CMAKE_BUILD_TYPE
		CMAKE_CXX_FLAGS
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_MINSIZEREL
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_RELWITHDEBINFO
		CMAKE_C_FLAGS
		CMAKE_C_FLAGS_DEBUG
		CMAKE_C_FLAGS_MINSIZEREL
		CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_RELWITHDEBINFO
		CMAKE_EXE_LINKER_FLAGS
		CMAKE_EXE_LINKER_FLAGS_DEBUG
		CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
		CMAKE_EXE_LINKER_FLAGS_RELEASE
		CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
		CMAKE_EXPORT_COMPILE_COMMANDS
		CMAKE_MODULE_LINKER_FLAGS
		CMAKE_MODULE_LINKER_FLAGS_DEBUG
		CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL
		CMAKE_MODULE_LINKER_FLAGS_RELEASE
		CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO
		CMAKE_SHARED_LINKER_FLAGS
		CMAKE_SHARED_LINKER_FLAGS_DEBUG
		CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL
		CMAKE_SHARED_LINKER_FLAGS_RELEASE
		CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
		CMAKE_STATIC_LINKER_FLAGS
		CMAKE_STATIC_LINKER_FLAGS_DEBUG
		CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL
		CMAKE_STATIC_LINKER_FLAGS_RELEASE
		CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO
		CMAKE_TOOLCHAIN_FILE
		CMAKE_VERBOSE_MAKEFILE)

file(RELATIVE_PATH distortosPath "${CMAKE_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/distortosConfiguration.cmake
		"#\n"
		"# \\file\n"
		"# \\brief distortos configuration\n"
		"#\n"
		"# \\warning\n"
		"# Automatically generated file - do not edit!\n"
		"#\n"
		"\n"
		"if(DEFINED ENV{DISTORTOS_PATH})\n"
		"	set(DISTORTOS_PATH \"\$ENV{DISTORTOS_PATH}\")\n"
		"else()\n"
		"	set(DISTORTOS_PATH \"${distortosPath}\")\n"
		"endif()\n"
		"\n")
foreach(cacheVariable ${cmakeConfigurationNames} ${DISTORTOS_ACTIVE_CONFIGURATION_NAMES} DISTORTOS_CONFIGURATION_NAMES)
	get_property(helpstring CACHE "${cacheVariable}" PROPERTY HELPSTRING)
	string(REPLACE "\n" "\\n" helpstring "${helpstring}")
	string(REPLACE "\"" "\\\"" helpstring "${helpstring}")
	get_property(type CACHE "${cacheVariable}" PROPERTY TYPE)
	set(value "${${cacheVariable}}")

	if(type STREQUAL FILEPATH)
		file(RELATIVE_PATH value "${CMAKE_CURRENT_SOURCE_DIR}" "${value}")
		set(value "\${DISTORTOS_PATH}/${value}")
	endif()

	file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/distortosConfiguration.cmake
			"set(\"${cacheVariable}\"\n"
			"		\"${value}\"\n"
			"		CACHE\n"
			"		\"${type}\"\n"
			"		\"${helpstring}\")\n")
endforeach()

#-----------------------------------------------------------------------------------------------------------------------
# preprocessed linker script
#-----------------------------------------------------------------------------------------------------------------------

if(NOT DISTORTOS_RAW_LINKER_SCRIPT)
	message(FATAL_ERROR "DISTORTOS_RAW_LINKER_SCRIPT not set")
endif()
set(DISTORTOS_RAW_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/${DISTORTOS_RAW_LINKER_SCRIPT}")
if(NOT EXISTS ${DISTORTOS_RAW_LINKER_SCRIPT})
	message(FATAL_ERROR "Linker script file '${DISTORTOS_RAW_LINKER_SCRIPT}' does not exist")
endif()
get_filename_component(filenameComponent ${DISTORTOS_RAW_LINKER_SCRIPT} NAME_WE)
set(ENV{DISTORTOS_LINKER_SCRIPT} "${CMAKE_CURRENT_BINARY_DIR}/${filenameComponent}.preprocessed.ld")

message(STATUS "Generating $ENV{DISTORTOS_LINKER_SCRIPT}")

execute_process(COMMAND ${CMAKE_CXX_COMPILER} -nostdinc -undef -C -E -P -x assembler-with-cpp
		-I${CMAKE_CURRENT_BINARY_DIR}/include ${DISTORTOS_RAW_LINKER_SCRIPT} -o $ENV{DISTORTOS_LINKER_SCRIPT}
		RESULT_VARIABLE result)

if(result)
	message(FATAL_ERROR "Preprocessing linker script failed, error code ${result}")
endif()

get_filename_component(filenameComponent ${DISTORTOS_RAW_LINKER_SCRIPT} NAME)
# force CMake to rerun if raw linker script changes
configure_file(${DISTORTOS_RAW_LINKER_SCRIPT} ${filenameComponent} COPYONLY)

#-----------------------------------------------------------------------------------------------------------------------
# .gitignore for build directory
#-----------------------------------------------------------------------------------------------------------------------

message(STATUS "Generating ${CMAKE_BINARY_DIR}/.gitignore")

file(WRITE ${CMAKE_BINARY_DIR}/.gitignore
		"#\n"
		"# \\file\n"
		"# \\brief .gitignore for build directory\n"
		"#\n"
		"# \\warning\n"
		"# Automatically generated file - do not edit!\n"
		"#\n"
		"\n"
		"# ignore everything\n"
		"*\n")

#-----------------------------------------------------------------------------------------------------------------------
# distortosTest application
#-----------------------------------------------------------------------------------------------------------------------

add_subdirectory(test)
