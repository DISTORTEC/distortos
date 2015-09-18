--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- date: 2015-09-18
--

local ldscriptOutputs = {LDSCRIPT, "$(TOP)/<ldscripts>"}

tup.rule(DISTORTOS_CONFIGURATION_MK, "./STM32F4.ld.sh \"%f\" > \"%o\"", ldscriptOutputs)

CFLAGS += STANDARD_INCLUDES

CXXFLAGS += STANDARD_INCLUDES
CXXFLAGS += "-I" .. TOP .. "/source/architecture/ARM/ARMv7-M/include"
CXXFLAGS += "-I" .. TOP .. "/source/chip/STMicroelectronics/STM32F4/include"
CXXFLAGS += "-I" .. TOP .. "/external/CMSIS-STM32F4"
CXXFLAGS += "-I" .. TOP .. "/external/CMSIS"

tup.include(TOP .. "/compile.lua")
