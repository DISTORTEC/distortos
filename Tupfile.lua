--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

local makeDistortosConfigurationSh = DISTORTOS_TOP .. "scripts/makeDistortosConfiguration.sh"

tup.rule(DISTORTOS_CONFIGURATION_MK, string.format('^ SH %s^ %s "%%f" > "%%o"', makeDistortosConfigurationSh,
		makeDistortosConfigurationSh), {OUTPUT .. "include/distortos/distortosConfiguration.h", TOP .. "<headers>"})
