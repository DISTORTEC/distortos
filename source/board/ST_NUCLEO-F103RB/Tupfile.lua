--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- Automatically generated file - do not edit!
--

if CONFIG_BOARD_SOURCE_BOARD_ST_NUCLEO_F103RB_ST_NUCLEO_F103RB == "y" then

	CXXFLAGS += STANDARD_INCLUDES
	CXXFLAGS += CHIP_INCLUDES
	CXXFLAGS += BOARD_INCLUDES

	tup.include(DISTORTOS_TOP .. "compile.lua")

end	-- if CONFIG_BOARD_SOURCE_BOARD_ST_NUCLEO_F103RB_ST_NUCLEO_F103RB == "y" then
