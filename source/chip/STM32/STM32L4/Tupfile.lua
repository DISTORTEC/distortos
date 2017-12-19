--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

if CONFIG_CHIP_STM32L4 == "y" then

	if CONFIG_LDSCRIPT == nil then

		local ldScriptGenerator = DISTORTOS_TOP .. "source/architecture/ARM/ARMv6-M-ARMv7-M/ARMv6-M-ARMv7-M.ld.sh"
		local ldScriptGeneratorArguments = string.format('"%s" "0x%x,%u"', CONFIG_CHIP,
				CONFIG_CHIP_ROM_ADDRESS + CONFIG_LDSCRIPT_ROM_BEGIN,
				CONFIG_LDSCRIPT_ROM_END - CONFIG_LDSCRIPT_ROM_BEGIN)

		if CONFIG_CHIP_STM32L4_UNIFY_SRAM1_SRAM2 == "y" then
			ldScriptGeneratorArguments = string.format('%s "0x%x,%u"', ldScriptGeneratorArguments,
					CONFIG_CHIP_STM32L4_SRAM1_ADDRESS, CONFIG_CHIP_STM32L4_SRAM1_SIZE + CONFIG_CHIP_STM32L4_SRAM2_SIZE)
		else
			ldScriptGeneratorArguments = string.format('%s "0x%x,%u" "sram2,0x%x,%u"', ldScriptGeneratorArguments,
					CONFIG_CHIP_STM32L4_SRAM1_ADDRESS, CONFIG_CHIP_STM32L4_SRAM1_SIZE,
					CONFIG_CHIP_STM32L4_SRAM2_ADDRESS, CONFIG_CHIP_STM32L4_SRAM2_SIZE)
		end

		tup.rule(string.format('^ SH %s^ ./%s %s > "%%o"', ldScriptGenerator, ldScriptGenerator,
				ldScriptGeneratorArguments), {RAW_LDSCRIPT, filenameToGroup(RAW_LDSCRIPT)})

	end	-- if CONFIG_LDSCRIPT == nil then

	CXXFLAGS += STANDARD_INCLUDES
	CXXFLAGS += ARCHITECTURE_INCLUDES
	CXXFLAGS += CHIP_INCLUDES

	tup.include(DISTORTOS_TOP .. "compile.lua")

end	-- if CONFIG_CHIP_STM32L4 == "y" then
