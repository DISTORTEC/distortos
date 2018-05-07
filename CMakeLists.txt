#
# file: CMakeLists.txt
#
# This is the main CMakeLists.txt for distortos
#
# author: Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 3.1)
project(distortos)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(distortos-utilities)

#-----------------------------------------------------------------------------------------------------------------------
# load variables from selectedConfiguration.mk
#-----------------------------------------------------------------------------------------------------------------------

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/selectedConfiguration.mk)
	message(FATAL_ERROR "${CMAKE_SOURCE_DIR}/selectedConfiguration.mk file does not exist; Please run first "
	"'make configure [CONFIG_PATH=<path-to-distortosConfiguration.mk>]'")
endif()
# force CMake to rerun if selectedConfiguration.mk file changes
configure_file(${CMAKE_SOURCE_DIR}/selectedConfiguration.mk selectedConfiguration.mk)
loadConfiguration(${CMAKE_SOURCE_DIR}/selectedConfiguration.mk)

#-----------------------------------------------------------------------------------------------------------------------
# load variables from selected distortosConfiguration.mk
#-----------------------------------------------------------------------------------------------------------------------

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${CONFIG_SELECTED_CONFIGURATION})
	message(FATAL_ERROR "Selected configuration file '${CMAKE_SOURCE_DIR}/${CONFIG_SELECTED_CONFIGURATION}' does not "
	"exist")
endif()
# force CMake to rerun if selected distortosConfiguration.mk file changes
configure_file(${CMAKE_SOURCE_DIR}/${CONFIG_SELECTED_CONFIGURATION} ${CONFIG_SELECTED_CONFIGURATION})
loadConfiguration(${CMAKE_SOURCE_DIR}/${CONFIG_SELECTED_CONFIGURATION})

#-----------------------------------------------------------------------------------------------------------------------
# distortosConfiguration.h
#-----------------------------------------------------------------------------------------------------------------------

add_custom_command(OUTPUT include/distortos/distortosConfiguration.h
		COMMAND ${CMAKE_COMMAND} -E make_directory include/distortos
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/makeDistortosConfiguration.sh
		${CMAKE_SOURCE_DIR}/${CONFIG_SELECTED_CONFIGURATION} > include/distortos/distortosConfiguration.h
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/makeDistortosConfiguration.sh
		DEPENDS ${CMAKE_SOURCE_DIR}/${CONFIG_SELECTED_CONFIGURATION}
		DEPENDS ${CMAKE_SOURCE_DIR}/selectedConfiguration.mk
		VERBATIM
		USES_TERMINAL)
add_custom_target(distortosConfiguration-h ALL DEPENDS include/distortos/distortosConfiguration.h)

#-----------------------------------------------------------------------------------------------------------------------
# distortos-includes interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos-includes INTERFACE)
target_include_directories(distortos-includes INTERFACE
		${CMAKE_CURRENT_BINARY_DIR}/include
		include
		${CONFIG_ARCHITECTURE_INCLUDES}
		${CONFIG_CHIP_INCLUDES}
		${CONFIG_BOARD_INCLUDES})
add_dependencies(distortos-includes distortosConfiguration-h)

#-----------------------------------------------------------------------------------------------------------------------
# distortos::includes alias library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos::includes ALIAS distortos-includes)

#-----------------------------------------------------------------------------------------------------------------------
# distortos-baseCFlags interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos-baseCFlags INTERFACE)
target_compile_options(distortos-baseCFlags INTERFACE
		${CONFIG_ARCHITECTURE_FLAGS})

#-----------------------------------------------------------------------------------------------------------------------
# distortos::baseCFlags alias library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos::baseCFlags ALIAS distortos-baseCFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos-cFlags interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos-cFlags INTERFACE)
target_compile_options(distortos-cFlags INTERFACE
		${CONFIG_DEBUGGING_INFORMATION_COMPILATION}
		${CONFIG_BUILD_OPTIMIZATION}
		${CONFIG_LINK_TIME_OPTIMIZATION_COMPILATION}
		-ffunction-sections
		-fdata-sections
		${CONFIG_ASSERT})
target_link_libraries(distortos-cFlags INTERFACE
		distortos::baseCFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos::cFlags alias library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos::cFlags ALIAS distortos-cFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos-allCFlags interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos-allCFlags INTERFACE)
target_compile_options(distortos-allCFlags INTERFACE
		-Wall
		-Wextra
		-Wshadow)
target_link_libraries(distortos-allCFlags INTERFACE
		distortos::cFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos::allCFlags alias library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos::allCFlags ALIAS distortos-allCFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos-baseCxxFlags interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos-baseCxxFlags INTERFACE)
target_compile_options(distortos-baseCxxFlags INTERFACE
		${CONFIG_STATIC_DESTRUCTORS_RUN_TIME_REGISTRATION}
		-fno-rtti
		-fno-exceptions)
target_link_libraries(distortos-baseCxxFlags INTERFACE
		distortos::baseCFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos::baseCxxFlags alias library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos::baseCxxFlags ALIAS distortos-baseCxxFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos-cxxFlags interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos-cxxFlags INTERFACE)
target_link_libraries(distortos-cxxFlags INTERFACE
		distortos::baseCxxFlags
		distortos::cFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos::cxxFlags alias library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos::cxxFlags ALIAS distortos-cxxFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos-allCxxFlags interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos-allCxxFlags INTERFACE)
target_link_libraries(distortos-allCxxFlags INTERFACE
		distortos::cxxFlags
		distortos::allCFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos::allCxxFlags alias library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos::allCxxFlags ALIAS distortos-allCxxFlags)

#-----------------------------------------------------------------------------------------------------------------------
# distortos static library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos STATIC "")
set_target_properties(distortos PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED ON)
target_compile_options(distortos PRIVATE
		-Wall
		-Wextra
		-Wshadow
		${CONFIG_ARCHITECTURE_FLAGS}
		${CONFIG_DEBUGGING_INFORMATION_COMPILATION}
		${CONFIG_BUILD_OPTIMIZATION}
		${CONFIG_LINK_TIME_OPTIMIZATION_COMPILATION}
		${CONFIG_STATIC_DESTRUCTORS_RUN_TIME_REGISTRATION}
		-ffunction-sections
		-fdata-sections
		-fno-rtti
		-fno-exceptions
		${CONFIG_ASSERT})
target_include_directories(distortos PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/include
		include
		${CONFIG_ARCHITECTURE_INCLUDES}
		${CONFIG_CHIP_INCLUDES}
		${CONFIG_BOARD_INCLUDES})
add_dependencies(distortos distortosConfiguration-h)

include(source/distortos-sources.cmake)

file(GLOB_RECURSE distortosBoardSourcesFiles FOLLOW_SYMLINKS LIST_DIRECTORIES false
		"${CMAKE_SOURCE_DIR}/*/distortos-board-sources.cmake")
foreach(distortosBoardSources ${distortosBoardSourcesFiles})
	include(${distortosBoardSources})
endforeach()

#-----------------------------------------------------------------------------------------------------------------------
# preprocess linker script
#-----------------------------------------------------------------------------------------------------------------------

if(NOT LDSCRIPT)
	if(NOT CONFIG_LDSCRIPT)
		message(FATAL_ERROR "Non-board configurations are not supported")
	endif()
	set(LDSCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_LDSCRIPT}")
endif()
if(NOT EXISTS ${LDSCRIPT})
	message(FATAL_ERROR "Linker script file '${LDSCRIPT}' does not exist")
endif()
get_filename_component(PREPROCESSED_LDSCRIPT ${LDSCRIPT} NAME_WE)
set(PREPROCESSED_LDSCRIPT "${PREPROCESSED_LDSCRIPT}.preprocessed.ld")
add_custom_command(OUTPUT ${PREPROCESSED_LDSCRIPT}
		COMMAND ${CMAKE_CXX_COMPILER} -nostdinc -undef -C -E -P -x assembler-with-cpp
		-I${CMAKE_CURRENT_BINARY_DIR}/include ${LDSCRIPT} -o ${PREPROCESSED_LDSCRIPT}
		DEPENDS ${LDSCRIPT}
		DEPENDS distortosConfiguration-h
		DEPENDS include/distortos/distortosConfiguration.h
		VERBATIM
		USES_TERMINAL)
add_custom_target(ldscript ALL DEPENDS ${PREPROCESSED_LDSCRIPT})

#-----------------------------------------------------------------------------------------------------------------------
# distortos-distortos interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos-distortos INTERFACE)
target_compile_options(distortos-distortos INTERFACE
		-Wall
		-Wextra
		-Wshadow
		${CONFIG_ARCHITECTURE_FLAGS}
		${CONFIG_DEBUGGING_INFORMATION_COMPILATION}
		${CONFIG_BUILD_OPTIMIZATION}
		${CONFIG_LINK_TIME_OPTIMIZATION_COMPILATION}
		${CONFIG_STATIC_DESTRUCTORS_RUN_TIME_REGISTRATION}
		-ffunction-sections
		-fdata-sections
		-fno-rtti
		-fno-exceptions
		${CONFIG_ASSERT})
target_include_directories(distortos-distortos INTERFACE
		${CMAKE_CURRENT_BINARY_DIR}/include
		include
		${CONFIG_ARCHITECTURE_INCLUDES}
		${CONFIG_CHIP_INCLUDES}
		${CONFIG_BOARD_INCLUDES})
target_link_libraries(distortos-distortos INTERFACE
		${CONFIG_ARCHITECTURE_FLAGS}
		${CONFIG_DEBUGGING_INFORMATION_LINKING}
		${CONFIG_BUILD_OPTIMIZATION}
		${CONFIG_LINK_TIME_OPTIMIZATION_LINKING}
		-Wl,--gc-sections
		-L"${CMAKE_CURRENT_BINARY_DIR}"
		-T${PREPROCESSED_LDSCRIPT}
		-Wl,--whole-archive -ldistortos -Wl,--no-whole-archive)
add_dependencies(distortos-distortos distortos ldscript)

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

get_target_property(doxygenCompileOptions distortos COMPILE_OPTIONS)
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
		DEPENDS distortosConfiguration-h
		DEPENDS include/distortos/distortosConfiguration.h
		VERBATIM)

#-----------------------------------------------------------------------------------------------------------------------
# distortosTest application
#-----------------------------------------------------------------------------------------------------------------------

add_subdirectory(test)
