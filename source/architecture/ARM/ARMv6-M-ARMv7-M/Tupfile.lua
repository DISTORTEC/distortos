--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

if CONFIG_ARCHITECTURE_ARMV6_M == "y" or CONFIG_ARCHITECTURE_ARMV7_M == "y" then

	local symbolsLdGenerator = DISTORTOS_TOP .. "scripts/symbols.ld.sh"

	tup.rule(string.format('^ SH %s^ ./%s "__main_stack_size = ALIGN(%u, %u)" ' ..
			'"__process_stack_size = ALIGN(%u, %u) + ALIGN(%u, %u)" > "%%o"', symbolsLdGenerator, symbolsLdGenerator,
			CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE, CONFIG_ARCHITECTURE_STACK_ALIGNMENT,
			CONFIG_MAIN_THREAD_STACK_SIZE, CONFIG_ARCHITECTURE_STACK_ALIGNMENT, CONFIG_STACK_GUARD_SIZE,
			CONFIG_ARCHITECTURE_STACK_ALIGNMENT), {SYMBOLS_LD, filenameToGroup(SYMBOLS_LD)})

	CXXFLAGS += STANDARD_INCLUDES
	CXXFLAGS += ARCHITECTURE_INCLUDES
	CXXFLAGS += CHIP_INCLUDES

	tup.include(DISTORTOS_TOP .. "compile.lua")

end	-- if CONFIG_ARCHITECTURE_ARMV6_M == "y" or CONFIG_ARCHITECTURE_ARMV7_M == "y" then
