--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

local stm32f4_ld_sh = "STM32F4.ld.sh"
local ldscriptOutputs = {LDSCRIPT, filenameToGroup(LDSCRIPT)}

tup.rule(DISTORTOS_CONFIGURATION_MK, "^ SH " .. stm32f4_ld_sh .. "^ ./" .. stm32f4_ld_sh .. " \"%f\" > \"%o\"",
		ldscriptOutputs)

CFLAGS += STANDARD_INCLUDES

CXXFLAGS += STANDARD_INCLUDES
CXXFLAGS += ARCHITECTURE_INCLUDES
CXXFLAGS += CHIP_INCLUDES

tup.include(TOP .. "/compile.lua")
