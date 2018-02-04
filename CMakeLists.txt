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
# generate distortosConfiguration.h
#-----------------------------------------------------------------------------------------------------------------------

add_custom_command(OUTPUT include/distortos/distortosConfiguration.h
		COMMAND ${CMAKE_COMMAND} -E make_directory include/distortos
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/makeDistortosConfiguration.sh
		${CMAKE_SOURCE_DIR}/${CONFIG_SELECTED_CONFIGURATION} > include/distortos/distortosConfiguration.h
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/makeDistortosConfiguration.sh
		DEPENDS ${CMAKE_SOURCE_DIR}/${CONFIG_SELECTED_CONFIGURATION}
		DEPENDS ${CMAKE_SOURCE_DIR}/selectedConfiguration.mk
		USES_TERMINAL)
add_custom_target(distortosConfiguration_h ALL DEPENDS include/distortos/distortosConfiguration.h)

#-----------------------------------------------------------------------------------------------------------------------
# preprocess linker script
#-----------------------------------------------------------------------------------------------------------------------

if(NOT CONFIG_LDSCRIPT)
	message(FATAL_ERROR "Non-board configurations are not supported")
endif()
set(RAW_LDSCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_LDSCRIPT}")
if(NOT EXISTS ${RAW_LDSCRIPT})
	message(FATAL_ERROR "Linker script file '${RAW_LDSCRIPT}' does not exist")
endif()
get_filename_component(LDSCRIPT ${RAW_LDSCRIPT} NAME_WE)
set(LDSCRIPT "${LDSCRIPT}.preprocessed.ld")
add_custom_command(OUTPUT ${LDSCRIPT}
		COMMAND ${CMAKE_CXX_COMPILER} -nostdinc -undef -C -E -P -x assembler-with-cpp
		-I${CMAKE_CURRENT_BINARY_DIR}/include ${RAW_LDSCRIPT} -o ${LDSCRIPT}
		DEPENDS ${RAW_LDSCRIPT}
		DEPENDS include/distortos/distortosConfiguration.h
		USES_TERMINAL)
add_custom_target(ldscript ALL DEPENDS ${LDSCRIPT})

#-----------------------------------------------------------------------------------------------------------------------
# generate distortos static library
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
add_dependencies(distortos distortosConfiguration_h)

include(source/distortos-sources.cmake)

file(GLOB_RECURSE distortosBoardSourcesFiles FOLLOW_SYMLINKS LIST_DIRECTORIES false "distortos-board-sources.cmake")
foreach(distortosBoardSources ${distortosBoardSourcesFiles})
	include(${distortosBoardSources})
endforeach()

#-----------------------------------------------------------------------------------------------------------------------
# distortos interface library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortosInterface INTERFACE)
target_compile_options(distortosInterface INTERFACE
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
target_include_directories(distortosInterface INTERFACE
		${CMAKE_CURRENT_BINARY_DIR}/include
		include
		${CONFIG_ARCHITECTURE_INCLUDES}
		${CONFIG_CHIP_INCLUDES}
		${CONFIG_BOARD_INCLUDES})
target_link_libraries(distortosInterface INTERFACE
		${CONFIG_ARCHITECTURE_FLAGS}
		${CONFIG_DEBUGGING_INFORMATION_LINKING}
		${CONFIG_BUILD_OPTIMIZATION}
		${CONFIG_LINK_TIME_OPTIMIZATION_LINKING}
		-Wl,--gc-sections
		-L${CMAKE_CURRENT_BINARY_DIR}
		-T${LDSCRIPT}
		-Wl,--whole-archive -ldistortos -Wl,--no-whole-archive)
add_dependencies(distortosInterface distortos ldscript)

#-----------------------------------------------------------------------------------------------------------------------
# distortos::distortos alias library
#-----------------------------------------------------------------------------------------------------------------------

add_library(distortos::distortos ALIAS distortosInterface)

#-----------------------------------------------------------------------------------------------------------------------
# distortosTest application
#-----------------------------------------------------------------------------------------------------------------------

add_subdirectory(test)
