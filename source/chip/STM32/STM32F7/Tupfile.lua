--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

if CONFIG_CHIP_STM32F7 == "y" then

	if CONFIG_LDSCRIPT == nil then

		local sram1Unified = 0
		local sram2Unified = 0
		local unifiedRamSize = 0

		if CONFIG_CHIP_STM32F7_UNIFY_DTCM_SRAM1 == "y" then
			sram1Unified = 1
			unifiedRamSize = CONFIG_CHIP_STM32F7_DTCM_SIZE + CONFIG_CHIP_STM32F7_SRAM1_SIZE
		elseif CONFIG_CHIP_STM32F7_UNIFY_DTCM_SRAM1_SRAM2 == "y" then
			sram1Unified = 1
			sram2Unified = 1
			unifiedRamSize = CONFIG_CHIP_STM32F7_DTCM_SIZE + CONFIG_CHIP_STM32F7_SRAM1_SIZE +
					CONFIG_CHIP_STM32F7_SRAM2_SIZE
		else
			unifiedRamSize = CONFIG_CHIP_STM32F7_DTCM_SIZE
		end

		local ldScriptGenerator = DISTORTOS_TOP .. "source/architecture/ARM/ARMv6-M-ARMv7-M/ARMv6-M-ARMv7-M.ld.sh"
		local ldScriptGeneratorArguments = string.format('"%s" "0x%x,%u" "0x%x,%u"', CONFIG_CHIP,
				CONFIG_CHIP_ROM_ADDRESS + CONFIG_LDSCRIPT_ROM_BEGIN,
				CONFIG_LDSCRIPT_ROM_END - CONFIG_LDSCRIPT_ROM_BEGIN, CONFIG_CHIP_STM32F7_DTCM_ADDRESS, unifiedRamSize)

		if CONFIG_CHIP_STM32F7_BKPSRAM_ADDRESS ~= nil then
			ldScriptGeneratorArguments = string.format('%s "bkpsram,0x%x,%u"', ldScriptGeneratorArguments,
					CONFIG_CHIP_STM32F7_BKPSRAM_ADDRESS, CONFIG_CHIP_STM32F7_BKPSRAM_SIZE)
		end

		if CONFIG_CHIP_STM32F7_UNIFY_SRAM1_SRAM2 == "y" then
			sram1Unified = 1
			sram2Unified = 1
			ldScriptGeneratorArguments = string.format('%s "sram12,0x%x,%u"', ldScriptGeneratorArguments,
					CONFIG_CHIP_STM32F7_SRAM1_ADDRESS, CONFIG_CHIP_STM32F7_SRAM1_SIZE + CONFIG_CHIP_STM32F7_SRAM2_SIZE)
		end

		if CONFIG_CHIP_STM32F7_SRAM1_ADDRESS ~= nil and sram1Unified == 0 then
			ldScriptGeneratorArguments = string.format('%s "sram1,0x%x,%u"', ldScriptGeneratorArguments,
					CONFIG_CHIP_STM32F7_SRAM1_ADDRESS, CONFIG_CHIP_STM32F7_SRAM1_SIZE)
		end

		if CONFIG_CHIP_STM32F7_SRAM2_ADDRESS ~= nil and sram2Unified == 0 then
			ldScriptGeneratorArguments = string.format('%s "sram2,0x%x,%u"', ldScriptGeneratorArguments,
					CONFIG_CHIP_STM32F7_SRAM2_ADDRESS, CONFIG_CHIP_STM32F7_SRAM2_SIZE)
		end

		tup.rule(string.format('^ SH %s^ ./%s %s > "%%o"', ldScriptGenerator, ldScriptGenerator,
				ldScriptGeneratorArguments), {RAW_LDSCRIPT, filenameToGroup(RAW_LDSCRIPT)})

	end	-- if CONFIG_LDSCRIPT == nil then

	CXXFLAGS += STANDARD_INCLUDES
	CXXFLAGS += ARCHITECTURE_INCLUDES
	CXXFLAGS += CHIP_INCLUDES

	tup.include(DISTORTOS_TOP .. "compile.lua")

end	-- if CONFIG_CHIP_STM32F7 == "y" then
