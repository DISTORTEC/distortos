--
-- file: Tuprules.lua
--
-- author: Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--

------------------------------------------------------------------------------------------------------------------------
-- support for building distortos as a submodule
------------------------------------------------------------------------------------------------------------------------

-- parse this file only once if it was included by parent project
if topLevelTuprulesLuaFileParsed ~= nil then
	return
end

-- relative path to top-level directory of distortos submodule
DISTORTOS_TOP = tup.getcwd() .. "/"

-- relative path to top-level directory of parent project, may be set by parent project
if TOP == nil then
	TOP = DISTORTOS_TOP
else
	-- strip trailing slashes (if any) and add a single one
	TOP = TOP:gsub("([^/]+)/*", "%1/")
end

-- node variable of top-level directory of parent project, may be set by parent project
if TOP_NODE == nil then
	TOP_NODE = tup.nodevariable(".")
end

------------------------------------------------------------------------------------------------------------------------
-- project configuration
------------------------------------------------------------------------------------------------------------------------

-- global assembler flags
ASFLAGS = ""

-- global C flags
CFLAGS = "-Wall -Wstrict-prototypes -Wextra -Wshadow -std=gnu99"

-- global C++ flags
CXXFLAGS = "-Wall -Wextra -Wshadow -std=gnu++11"

-- linker flags
LDFLAGS = ""

------------------------------------------------------------------------------------------------------------------------
-- output folder
------------------------------------------------------------------------------------------------------------------------

OUTPUT = TOP .. "output/"

------------------------------------------------------------------------------------------------------------------------
-- load configuration variables from distortosConfiguration.mk file selected by user
------------------------------------------------------------------------------------------------------------------------

-- parses configuration file named filename with kconfig-style entries into global CONFIG_... constants
function parseConfigurationFile(filename)
	local file = assert(io.open(filename, "r"))
	for line in file:lines() do
		local configPattern = "CONFIG_[A-Za-z0-9_]+"
		local standardConfigPattern = "^(" .. configPattern .. ")=(.*)$"
		local name, value = line:match(standardConfigPattern)
		if name ~= nil and value ~= nil then
			local unquotedValue = value:match('^"(.*)"$')
			if unquotedValue ~= nil then
				value = unquotedValue
			end
		else
			local notSetConfigPattern = "^# (" .. configPattern .. ") is not set$"
			name = line:match(notSetConfigPattern)
			if name ~= nil then
				value = "n"
			end
		end

		if name ~= nil and value ~= nil then
			_G[name] = value
		end
	end
	file:close()
end

-- parse CONFIG_SELECTED_CONFIGURATION variable from selectedConfiguration.mk file
parseConfigurationFile(tostring(TOP_NODE) .. "/selectedConfiguration.mk")

-- path to distortosConfiguration.mk file selected by $(CONFIG_SELECTED_CONFIGURATION) variable
DISTORTOS_CONFIGURATION_MK = TOP .. CONFIG_SELECTED_CONFIGURATION

-- parse configuration constants from selected configuration file
parseConfigurationFile(tostring(TOP_NODE) .. "/" .. CONFIG_SELECTED_CONFIGURATION)

------------------------------------------------------------------------------------------------------------------------
-- toolchain configuration
------------------------------------------------------------------------------------------------------------------------

AS = CONFIG_TOOLCHAIN_PREFIX .. "gcc"
CC = CONFIG_TOOLCHAIN_PREFIX .. "gcc"
CXX = CONFIG_TOOLCHAIN_PREFIX .. "g++"
AR = CONFIG_TOOLCHAIN_PREFIX .. "gcc-ar"
LD = CONFIG_TOOLCHAIN_PREFIX .. "g++"
OBJCOPY = CONFIG_TOOLCHAIN_PREFIX .. "objcopy"
OBJDUMP = CONFIG_TOOLCHAIN_PREFIX .. "objdump"
SIZE = CONFIG_TOOLCHAIN_PREFIX .. "size"

------------------------------------------------------------------------------------------------------------------------
-- add obligatory compilation flags
------------------------------------------------------------------------------------------------------------------------

ASFLAGS += CONFIG_DEBUGGING_INFORMATION_COMPILATION
ASFLAGS += CONFIG_ARCHITECTURE_FLAGS

CFLAGS += CONFIG_DEBUGGING_INFORMATION_COMPILATION
CFLAGS += CONFIG_ARCHITECTURE_FLAGS
CFLAGS += CONFIG_BUILD_OPTIMIZATION
CFLAGS += CONFIG_LINK_TIME_OPTIMIZATION_COMPILATION
CFLAGS += "-ffunction-sections -fdata-sections"
CFLAGS += CONFIG_ASSERT

CXXFLAGS += CONFIG_DEBUGGING_INFORMATION_COMPILATION
CXXFLAGS += CONFIG_ARCHITECTURE_FLAGS
CXXFLAGS += CONFIG_BUILD_OPTIMIZATION
CXXFLAGS += CONFIG_LINK_TIME_OPTIMIZATION_COMPILATION
CXXFLAGS += "-ffunction-sections -fdata-sections -fno-rtti -fno-exceptions"
CXXFLAGS += CONFIG_ASSERT

-- path to linker script fragment with symbols (generated automatically)
SYMBOLS_LD = OUTPUT .. "symbols.ld"

-- path to linker script (generated automatically)
LDSCRIPT = OUTPUT .. CONFIG_CHIP .. ".ld"

LDSCRIPTS = {SYMBOLS_LD, LDSCRIPT}

LDFLAGS += CONFIG_DEBUGGING_INFORMATION_LINKING
LDFLAGS += CONFIG_ARCHITECTURE_FLAGS
LDFLAGS += CONFIG_BUILD_OPTIMIZATION
LDFLAGS += CONFIG_LINK_TIME_OPTIMIZATION_LINKING
LDFLAGS += "-Wl,--cref,--gc-sections"

------------------------------------------------------------------------------------------------------------------------
-- "constants" with include paths
------------------------------------------------------------------------------------------------------------------------

-- "standard" includes
STANDARD_INCLUDES += "-I" .. OUTPUT .. "include -I" .. DISTORTOS_TOP .. "include"

-- architecture includes
ARCHITECTURE_INCLUDES += CONFIG_ARCHITECTURE_INCLUDES:gsub("(%g+)", "-I" .. DISTORTOS_TOP .. "%1")

-- chip includes
CHIP_INCLUDES += CONFIG_CHIP_INCLUDES:gsub("(%g+)", "-I" .. DISTORTOS_TOP .. "%1")

-- board includes
if CONFIG_BOARD_INCLUDES ~= nil then
	BOARD_INCLUDES += CONFIG_BOARD_INCLUDES:gsub("(%g+)", "-I" .. DISTORTOS_TOP .. "%1")
else
	BOARD_INCLUDES += ""
end
------------------------------------------------------------------------------------------------------------------------
-- tup/lua functions
------------------------------------------------------------------------------------------------------------------------

-- group of <objects>, used as output in as(), cc() and cxx()
objectsGroup = nil

-- starts new group of <objects>, used as output in as(), cc() and cxx()
function startObjectsGroup()
	objectsGroup = tup.getcwd() .. "/<objects>"
end

-- start default group of <objects> - all generated objects
startObjectsGroup()

-- converts filename "some/path/to/file.ext" to group "some/path/to/<file.ext>"; if filename is already a group, then no
-- change is performed
function filenameToGroup(filename)
	local lastSlash = filename:match("^.*()[/\\]") or 0
	local path = string.sub(filename, 1, lastSlash)
	local name = string.sub(filename, (lastSlash) + 1)
	name = name:match("^<?([^>]*)>?$")
	return path .. "<" .. name .. ">"
end

-- get "file specific flags" from table (ASFLAGS, CFLAGS or CXXFLAGS) for file named filename
function getSpecificFlags(table, filename)
	if table[filename] ~= nil then
		return table[filename]
	end
	return ""
end

-- assemble file named input
function as(input)
	tup.rule({input, extra_inputs = {TOP .. "<headers>"}},
			string.format("^c AS %%f^ %s %s %s -c %%f -o %%o", AS, tostring(ASFLAGS), getSpecificFlags(ASFLAGS, input)),
			{OUTPUT .. tup.getrelativedir(TOP) .. "/%B.o", objectsGroup})
end

-- compile (C) file named input
function cc(input)
	tup.rule({input, extra_inputs = {TOP .. "<headers>"}},
			string.format("^c CC %%f^ %s %s %s -c %%f -o %%o", CC, tostring(CFLAGS), getSpecificFlags(CFLAGS, input)),
			{OUTPUT .. tup.getrelativedir(TOP) .. "/%B.o", objectsGroup})
end

-- compile (C++) file named input
function cxx(input)
	tup.rule({input, extra_inputs = {TOP .. "<headers>"}}, string.format("^c CXX %%f^ %s %s %s -c %%f -o %%o", CXX,
			tostring(CXXFLAGS), getSpecificFlags(CXXFLAGS, input)),
			{OUTPUT .. tup.getrelativedir(TOP) .. "/%B.o", objectsGroup})
end

-- archive all objects from groups given in the vararg into file named output; all elements of vararg are parsed by
-- filenameToGroup() before use;
function ar(output, ...)
	local inputs = {}
	local objects = ""
	for i, element in ipairs({...}) do
		element = filenameToGroup(element)
		local path, group = element:match("^([^<]*)(<[^>]+>)$")
		if path ~= nil and group ~= nil then
			table.insert(inputs, path .. group)
			objects = objects .. " %" .. group
		end
	end

	tup.rule(inputs , "^ AR %o^ " .. AR .. " rcs %o " .. objects, {output, filenameToGroup(output)})
end

-- link all objects given in the vararg into file named output; all elements of vararg which begin with OUTPUT are
-- parsed by filenameToGroup() and used as groups, the same is happens if the element is a group; vararg may also
-- contain:
-- - paths to linker scripts (.ld extension);
-- - paths to archives (.a extension);
function link(output, ...)
	local inputs = {}
	local objects = ""
	local ldScripts = ""
	local archives = ""
	local outputLength = string.len(OUTPUT)
	for i, element in ipairs({...}) do
		local extension = tup.ext(element)
		local value
		if element:sub(1, outputLength):find(OUTPUT) ~= nil or element:match("<[^>]+>$") ~= nil then
			element = filenameToGroup(element)
			value = "%" .. element:match("^[^<]*(<[^>]+>)$")
		else
			value = element
		end
		table.insert(inputs, element)
		if extension == "ld" then
			ldScripts = ldScripts .. " -T" .. value
		elseif extension == "a" then
			archives = archives .. " -l:" .. value
		else
			objects = objects .. " " .. value
		end
	end

	local map = output:match("^(.*)" .. tup.ext(output) .. "$") .. "map"
	tup.rule(inputs,
			string.format("^ LD %%o^ %s %s -Wl,-Map=%s %s%s -Wl,--whole-archive %s -Wl,--no-whole-archive -o %%o", LD,
			tostring(LDFLAGS), map, ldScripts, objects, archives), {output, extra_outputs = {map}})
end

-- convert file named input (elf) to intel hex file named output
function hex(input, output)
	tup.rule(input, "^ HEX %o^ " .. OBJCOPY .. " -O ihex %f %o", output)
end

-- convert file named input (elf) to binary file named output
function bin(input, output)
	tup.rule(input, "^ BIN %o^ " .. OBJCOPY .. " -O binary %f %o", output)
end

-- dump symbols from file named input (elf) to file named output
function dmp(input, output)
	tup.rule(input, "^ DMP %o^ " .. OBJDUMP .. " -x --syms --demangle %f > %o", output)
end

-- generate disassembly of file named input (elf) to file named output
function lss(input, output)
	tup.rule(input, "^ LSS %o^ " .. OBJDUMP .. " --demangle -S %f > %o", output)
end

-- print size of file named input (elf)
function size(input)
	tup.rule({input}, "^ SIZE %f^ " .. SIZE .. " -B %f")
end

------------------------------------------------------------------------------------------------------------------------
-- parse this file only once if it was included by parent project
------------------------------------------------------------------------------------------------------------------------

topLevelTuprulesLuaFileParsed = 1
