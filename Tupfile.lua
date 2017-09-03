--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

local makeDistortosConfigurationSh = DISTORTOS_TOP .. "scripts/makeDistortosConfiguration.sh"

tup.rule(DISTORTOS_CONFIGURATION_MK, string.format('^ SH %s^ %s "%%f" > "%%o"', makeDistortosConfigurationSh,
		makeDistortosConfigurationSh), {OUTPUT .. "include/distortos/distortosConfiguration.h", TOP .. "<headers>"})

local ldscriptCppFlags = string.format('-nostdinc -undef -C -E -P -x assembler-with-cpp %s',
		tostring(STANDARD_INCLUDES))

if RAW_LDSCRIPT:find(OUTPUT) == 1 then
	tup.rule({filenameToGroup(RAW_LDSCRIPT), extra_inputs = {TOP .. "<headers>"}},
			string.format('^ CPP %s^ %s %s %%<%s> -o %%o', RAW_LDSCRIPT, CPP, ldscriptCppFlags, tup.file(RAW_LDSCRIPT)),
			{LDSCRIPT, filenameToGroup(LDSCRIPT)})
else
	tup.rule({RAW_LDSCRIPT, extra_inputs = {TOP .. "<headers>"}}, string.format('^ CPP %%f^ %s %s %%f -o %%o', CPP,
			ldscriptCppFlags), {LDSCRIPT, filenameToGroup(LDSCRIPT)})
end
