--
-- file: Tupfile.lua
--
-- author: Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- date: 2014-07-24
--

CXXFLAGS += "-I" .. TOP .. "/include"

tup.include(TOP .. "/compile.lua")
