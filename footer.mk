#
# file: footer.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-09-26
#

#-----------------------------------------------------------------------------------------------------------------------
# source files (only if not provided by parent Rules.mk)
#-----------------------------------------------------------------------------------------------------------------------

ASSOURCES_$(d) ?= $(wildcard $(d)/*.S)
CSOURCES_$(d) ?= $(wildcard $(d)/*.c)
CXXSOURCES_$(d) ?= $(wildcard $(d)/*.cpp)

#-----------------------------------------------------------------------------------------------------------------------
# paths and names of object and dependency files
#-----------------------------------------------------------------------------------------------------------------------

AOBJECTS_$(d) := $(ASSOURCES_$(d):%.S=$(OUTPUT)%.o)
COBJECTS_$(d) := $(CSOURCES_$(d):%.c=$(OUTPUT)%.o)
CXXOBJECTS_$(d) := $(CXXSOURCES_$(d):%.cpp=$(OUTPUT)%.o)
OBJECTS_$(d) := $(AOBJECTS_$(d)) $(COBJECTS_$(d)) $(CXXOBJECTS_$(d))
DEPENDENCIES_$(d) := $(OBJECTS_$(d):%.o=%.d)

#-----------------------------------------------------------------------------------------------------------------------
# add local files to global rules
#-----------------------------------------------------------------------------------------------------------------------

OBJECTS := $(OBJECTS) $(OBJECTS_$(d))
GENERATED := $(GENERATED) $(OBJECTS_$(d)) $(DEPENDENCIES_$(d))

#-----------------------------------------------------------------------------------------------------------------------
# generated files depend on local Rules.mk and global footer.mk
#-----------------------------------------------------------------------------------------------------------------------

$(OBJECTS_$(d)): $(d)/Rules.mk footer.mk

#-----------------------------------------------------------------------------------------------------------------------
# set compilation flags
#-----------------------------------------------------------------------------------------------------------------------

$(AOBJECTS_$(d)): ASFLAGS := $(ASFLAGS) $(ASFLAGS_$(d))
$(COBJECTS_$(d)): CFLAGS := $(CFLAGS) $(CFLAGS_$(d))
$(CXXOBJECTS_$(d)): CXXFLAGS := $(CXXFLAGS) $(CXXFLAGS_$(d))
