--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

if CONFIG_CHIP_STM32L0 == "y" then

	if CONFIG_LDSCRIPT == nil then

		local ldScriptGenerator = DISTORTOS_TOP .. "source/architecture/ARM/ARMv6-M-ARMv7-M/ARMv6-M-ARMv7-M.ld.sh"

		tup.rule(string.format('^ SH %s^ ./%s "%s" "0x%x,%u" "0x%x,%u" > "%%o"', ldScriptGenerator, ldScriptGenerator,
				CONFIG_CHIP, CONFIG_CHIP_ROM_ADDRESS + CONFIG_LDSCRIPT_ROM_BEGIN,
				CONFIG_LDSCRIPT_ROM_END - CONFIG_LDSCRIPT_ROM_BEGIN, CONFIG_CHIP_STM32L0_SRAM_ADDRESS,
				CONFIG_CHIP_STM32L0_SRAM_SIZE), {LDSCRIPT, filenameToGroup(LDSCRIPT)})

	end	-- if CONFIG_LDSCRIPT == nil then

	CXXFLAGS += STANDARD_INCLUDES
	CXXFLAGS += ARCHITECTURE_INCLUDES
	CXXFLAGS += CHIP_INCLUDES

	tup.include(DISTORTOS_TOP .. "compile.lua")

end	-- if CONFIG_CHIP_STM32L0 == "y" then
