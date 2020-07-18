#
# file: doxygen.cmake
#
# author: Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

foreach(doxyfile ${DOXYFILES})
	file(READ ${doxyfile} doxyfileContents)
	string(APPEND doxyfilesContents ${doxyfileContents})
endforeach()
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile ${doxyfilesContents} "HTML_FOOTER = $ENV{HTML_FOOTER}\n")

execute_process(COMMAND doxygen ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
