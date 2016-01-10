--
-- file: Tuprules.lua
--
-- author: Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
DISTORTOS_TOP = tup.getcwd()

-- relative path to top-level directory of parent project, may be set by parent project
if TOP == nil then
	TOP = DISTORTOS_TOP
end

-- node variable of top-level directory of parent project, may be set by parent project
if TOP_NODE == nil then
	TOP_NODE = tup.nodevariable(".")
end

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
			local unquotedValue = value:match("^\"(.*)\"$")
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
DISTORTOS_CONFIGURATION_MK = TOP .. "/" .. CONFIG_SELECTED_CONFIGURATION

-- parse configuration constants from selected configuration file
parseConfigurationFile(tostring(TOP_NODE) .. "/" .. CONFIG_SELECTED_CONFIGURATION)

------------------------------------------------------------------------------------------------------------------------
-- toolchain configuration
------------------------------------------------------------------------------------------------------------------------

AS = CONFIG_TOOLCHAIN_PREFIX .. "gcc"
CC = CONFIG_TOOLCHAIN_PREFIX .. "gcc"
CXX = CONFIG_TOOLCHAIN_PREFIX .. "g++"
AR = CONFIG_TOOLCHAIN_PREFIX .. "ar"
LD = CONFIG_TOOLCHAIN_PREFIX .. "g++"
OBJCOPY = CONFIG_TOOLCHAIN_PREFIX .. "objcopy"
OBJDUMP = CONFIG_TOOLCHAIN_PREFIX .. "objdump"
SIZE = CONFIG_TOOLCHAIN_PREFIX .. "size"

------------------------------------------------------------------------------------------------------------------------
-- project configuration
------------------------------------------------------------------------------------------------------------------------

-- output folder
OUTPUT = TOP .. "/output/"

-- project name
PROJECT = "distortos"

-- global C++ flags
CXXFLAGS = ""

-- global C flags
CFLAGS = ""

-- global assembler flags
ASFLAGS = ""

-- define warning options here
CXXWARNINGS = "-Wall -Wextra -Wshadow"
CWARNINGS = "-Wall -Wstrict-prototypes -Wextra -Wshadow"

-- C++ language standard ("c++98", "gnu++98" - default, "c++11", "gnu++11")
CXXSTD = "-std=gnu++11"

-- C language standard ("c89" / "iso9899:1990", "iso9899:199409", "c99" / "iso9899:1999", "gnu89" - default, "gnu99")
CSTD = "-std=gnu99"

-- debug flags
DBGFLAGS = "-g -ggdb3"

-- linker flags
LDFLAGS = ""

------------------------------------------------------------------------------------------------------------------------
-- compilation flags
------------------------------------------------------------------------------------------------------------------------

ASFLAGS += CONFIG_ARCHITECTURE_FLAGS
ASFLAGS += DBGFLAGS

CFLAGS += CONFIG_ARCHITECTURE_FLAGS
CFLAGS += CONFIG_BUILD_OPTIMIZATION
CFLAGS += CWARNINGS
CFLAGS += CSTD
CFLAGS += DBGFLAGS
CFLAGS += "-ffunction-sections -fdata-sections"

CXXFLAGS += CONFIG_ARCHITECTURE_FLAGS
CXXFLAGS += CONFIG_BUILD_OPTIMIZATION
CXXFLAGS += CXXWARNINGS
CXXFLAGS += CXXSTD
CXXFLAGS += DBGFLAGS
CXXFLAGS += "-ffunction-sections -fdata-sections -fno-rtti -fno-exceptions"

-- path to linker script (generated automatically)
LDSCRIPT = OUTPUT .. CONFIG_CHIP .. ".ld"

LDFLAGS += CONFIG_ARCHITECTURE_FLAGS
LDFLAGS += "-g -Wl,--cref,--gc-sections"

------------------------------------------------------------------------------------------------------------------------
-- "constants" with include paths
------------------------------------------------------------------------------------------------------------------------

-- "standard" includes
STANDARD_INCLUDES += "-I" .. OUTPUT .. "include -I" .. DISTORTOS_TOP .. "/include"

-- architecture includes
ARCHITECTURE_INCLUDES += CONFIG_ARCHITECTURE_INCLUDES:gsub("(%g+)", "-I" .. DISTORTOS_TOP .. "/%1")

-- chip includes
CHIP_INCLUDES += CONFIG_CHIP_INCLUDES:gsub("(%g+)", "-I" .. DISTORTOS_TOP .. "/%1")

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
	local specificFlags = getSpecificFlags(ASFLAGS, input)
	local inputs = {input, extra_inputs = {TOP .. "/<headers>"}}
	local outputs = {OUTPUT .. tup.getrelativedir(TOP) .. "/%B.o", objectsGroup}
	tup.rule(inputs, "^c AS %f^ " .. AS .. " " .. tostring(ASFLAGS) .. " " .. specificFlags .. " -c %f -o %o", outputs)
end

-- compile (C) file named input
function cc(input)
	local specificFlags = getSpecificFlags(CFLAGS, input)
	local inputs = {input, extra_inputs = {TOP .. "/<headers>"}}
	local outputs = {OUTPUT .. tup.getrelativedir(TOP) .. "/%B.o", objectsGroup}
	tup.rule(inputs, "^c CC %f^ " .. CC .. " " .. tostring(CFLAGS) .. " " .. specificFlags .. " -c %f -o %o", outputs)
end

-- compile (C++) file named input
function cxx(input)
	local specificFlags = getSpecificFlags(CXXFLAGS, input)
	local inputs = {input, extra_inputs = {TOP .. "/<headers>"}}
	local outputs = {OUTPUT .. tup.getrelativedir(TOP) .. "/%B.o", objectsGroup}
	tup.rule(inputs, "^c CXX %f^ " .. CXX .. " " .. tostring(CXXFLAGS) .. " " .. specificFlags .. " -c %f -o %o",
			outputs)
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

	local outputs = {output, filenameToGroup(output)}
	tup.rule(inputs , "^ AR %o^ " .. AR .. " rcs %o " .. objects, outputs)
end

-- link all objects from groups given in the vararg into file named output; all elements of vararg are parsed by
-- filenameToGroup() before use; vararg may also contain:
-- - paths to linker scripts (.ld extension);
-- - paths to archives (.a extension);
function link(output, ...)
	local inputs = {}
	local objects = ""
	local ldScripts = ""
	local archives = ""
	for i, element in ipairs({...}) do
		element = filenameToGroup(element)
		local path, group = element:match("^([^<]*)(<[^>]+>)$")
		if path ~= nil and group ~= nil then
			table.insert(inputs, path .. group)
			local extension = group:match("%.([a-zA-Z0-9]+)>$")
			if extension == "ld" then
				ldScripts = ldScripts .. " -T%" .. group
			elseif extension == "a" then
				archives = archives .. " -l:%" .. group
			else
				objects = objects .. " %" .. group
			end
		end
	end

	local inputsString = ldScripts .. objects .. " -Wl,--whole-archive " .. archives .. " -Wl,--no-whole-archive"
	local map = output:match("^(.*)" .. tup.ext(output) .. "$") .. "map"
	local mapString = "-Wl,-Map=" .. map
	local outputs = {output, extra_outputs = {map}}
	tup.rule(inputs, "^ LD %o^ " .. LD .. " " .. tostring(LDFLAGS) .. " " .. mapString .. " " .. inputsString ..
			" -o %o", outputs)
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
