--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

if CONFIG_CHIP_STM32F4 == "y" then

	local sram2Unified = 0
	local sram3Unified = 0
	local unifiedRamSize = 0

	if CONFIG_CHIP_STM32F4_UNIFY_SRAM1_SRAM2 == "y" then
		sram2Unified = 1
		unifiedRamSize = CONFIG_CHIP_STM32F4_SRAM1_SIZE + CONFIG_CHIP_STM32F4_SRAM2_SIZE
	elseif CONFIG_CHIP_STM32F4_UNIFY_SRAM1_SRAM2_SRAM3 == "y" then
		sram2Unified = 1
		sram3Unified = 1
		unifiedRamSize = CONFIG_CHIP_STM32F4_SRAM1_SIZE + CONFIG_CHIP_STM32F4_SRAM2_SIZE +
				CONFIG_CHIP_STM32F4_SRAM3_SIZE
	else
		unifiedRamSize = CONFIG_CHIP_STM32F4_SRAM1_SIZE
	end

	local ldScriptGenerator = DISTORTOS_TOP .. "source/architecture/ARM/ARMv6-M-ARMv7-M/ARMv6-M-ARMv7-M.ld.sh"
	local ldScriptGeneratorArguments = " \"" .. CONFIG_CHIP .. "\" \"" ..
			CONFIG_CHIP_STM32F4_FLASH_ADDRESS .. "," .. CONFIG_CHIP_STM32F4_FLASH_SIZE .. "\" \"" ..
			CONFIG_CHIP_STM32F4_SRAM1_ADDRESS .. "," .. unifiedRamSize .. "\" \"" ..
			CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE .. "\" \"" .. CONFIG_MAIN_THREAD_STACK_SIZE .. "\""

	if CONFIG_CHIP_STM32F4_BKPSRAM_ADDRESS ~= nil then
		ldScriptGeneratorArguments = ldScriptGeneratorArguments ..
				" \"bkpsram," .. CONFIG_CHIP_STM32F4_BKPSRAM_ADDRESS .. "," .. CONFIG_CHIP_STM32F4_BKPSRAM_SIZE .. "\""
	end

	if CONFIG_CHIP_STM32F4_CCM_ADDRESS ~= nil then
		ldScriptGeneratorArguments = ldScriptGeneratorArguments ..
				" \"ccm," .. CONFIG_CHIP_STM32F4_CCM_ADDRESS .. "," .. CONFIG_CHIP_STM32F4_CCM_SIZE .. "\""
	end

	if CONFIG_CHIP_STM32F4_UNIFY_SRAM2_SRAM3 == "y" then
		sram2Unified = 1
		sram3Unified = 1
		local sram23Size = CONFIG_CHIP_STM32F4_SRAM2_SIZE + CONFIG_CHIP_STM32F4_SRAM3_SIZE
		ldScriptGeneratorArguments = ldScriptGeneratorArguments ..
				" \"sram23," .. CONFIG_CHIP_STM32F4_SRAM2_ADDRESS .. "," .. sram23Size .. "\""
	end

	if CONFIG_CHIP_STM32F4_SRAM2_ADDRESS ~= nil and sram2Unified == 0 then
		ldScriptGeneratorArguments = ldScriptGeneratorArguments ..
				" \"sram2," .. CONFIG_CHIP_STM32F4_SRAM2_ADDRESS .. "," .. CONFIG_CHIP_STM32F4_SRAM2_SIZE .. "\""
	end

	if CONFIG_CHIP_STM32F4_SRAM3_ADDRESS ~= nil and sram3Unified == 0 then
		ldScriptGeneratorArguments = ldScriptGeneratorArguments ..
				" \"sram3," .. CONFIG_CHIP_STM32F4_SRAM3_ADDRESS .. "," .. CONFIG_CHIP_STM32F4_SRAM3_SIZE .. "\""
	end

	local ldscriptOutputs = {LDSCRIPT, filenameToGroup(LDSCRIPT)}
	tup.rule("^ SH " .. ldScriptGenerator .. "^ ./" .. ldScriptGenerator .. ldScriptGeneratorArguments .. " > \"%o\"",
			ldscriptOutputs)

	CFLAGS += STANDARD_INCLUDES

	CXXFLAGS += STANDARD_INCLUDES
	CXXFLAGS += ARCHITECTURE_INCLUDES
	CXXFLAGS += CHIP_INCLUDES

	tup.include(DISTORTOS_TOP .. "compile.lua")

end	-- if CONFIG_CHIP_STM32F4 == "y" then
