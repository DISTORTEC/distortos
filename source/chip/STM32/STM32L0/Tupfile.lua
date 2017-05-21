--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

if CONFIG_CHIP_STM32L0 == "y" then

	local ldScriptGenerator = DISTORTOS_TOP .. "source/architecture/ARM/ARMv6-M-ARMv7-M/ARMv6-M-ARMv7-M.ld.sh"
	local mainThreadStackSize = (math.ceil(CONFIG_MAIN_THREAD_STACK_SIZE / CONFIG_ARCHITECTURE_STACK_ALIGNMENT) +
			math.ceil(CONFIG_STACK_GUARD_SIZE / CONFIG_ARCHITECTURE_STACK_ALIGNMENT)) *
			CONFIG_ARCHITECTURE_STACK_ALIGNMENT

	tup.rule(string.format('^ SH %s^ ./%s "%s" "0x%x,%u" "0x%x,%u" "%u" "%u" > "%%o"', ldScriptGenerator,
			ldScriptGenerator, CONFIG_CHIP, CONFIG_CHIP_ROM_ADDRESS + CONFIG_LDSCRIPT_ROM_BEGIN,
			CONFIG_LDSCRIPT_ROM_END - CONFIG_LDSCRIPT_ROM_BEGIN, CONFIG_CHIP_STM32L0_SRAM_ADDRESS,
			CONFIG_CHIP_STM32L0_SRAM_SIZE, CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE, mainThreadStackSize),
			{LDSCRIPT, filenameToGroup(LDSCRIPT)})

	CXXFLAGS += STANDARD_INCLUDES
	CXXFLAGS += ARCHITECTURE_INCLUDES
	CXXFLAGS += CHIP_INCLUDES

	tup.include(DISTORTOS_TOP .. "compile.lua")

end	-- if CONFIG_CHIP_STM32L0 == "y" then
