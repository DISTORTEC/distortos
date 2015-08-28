--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- date: 2015-08-28
--

local distortosConfigurationMk = TOP .. "/" .. CONFIG_SELECTED_CONFIGURATION .. "/distortosConfiguration.mk"
local distortosConfigurationOutputs = {OUTPUT .. "include/distortos/%B.h", "$(TOP)/<headers>"}

tup.rule(distortosConfigurationMk, "$(TOP)/scripts/makeDistortosConfiguration.awk \"%f\" > \"%o\"",
		distortosConfigurationOutputs)

link("$(OUTPUT)$(PROJECT).elf")
size("$(OUTPUT)$(PROJECT).elf")
hex("$(OUTPUT)$(PROJECT).elf", "$(OUTPUT)$(PROJECT).hex")
bin("$(OUTPUT)$(PROJECT).elf", "$(OUTPUT)$(PROJECT).bin")
dmp("$(OUTPUT)$(PROJECT).elf", "$(OUTPUT)$(PROJECT).dmp")
lss("$(OUTPUT)$(PROJECT).elf", "$(OUTPUT)$(PROJECT).lss")
