--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- date: 2015-08-24
--

CXXFLAGS += "-I" .. TOP .. "/test"
CXXFLAGS += "-I" .. OUTPUT .. "include"
CXXFLAGS += "-I" .. TOP .. "/include"
CXXFLAGS += "-I" .. TOP .. "/source/architecture/ARM/ARMv7-M/include"

tup.include(TOP .. "/compile.lua")
