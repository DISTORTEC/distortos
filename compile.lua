--
-- file: compile.lua
--
-- author: Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- date: 2014-07-21
--

for index, filename in ipairs(tup.glob('*.S')) do
	as(filename)
end

for index, filename in ipairs(tup.glob('*.c')) do
	cc(filename)
end

for index, filename in ipairs(tup.glob('*.cpp')) do
	cxx(filename)
end
