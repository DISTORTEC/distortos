#
# file: Rules.mk
#
# author: Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

#-----------------------------------------------------------------------------------------------------------------------
# subdirectories
#-----------------------------------------------------------------------------------------------------------------------

SUBDIRECTORIES += $(patsubst $(d)%/Rules.mk,%,$(wildcard $(d)*/Rules.mk))

#-----------------------------------------------------------------------------------------------------------------------
# generated headers
#-----------------------------------------------------------------------------------------------------------------------

DISTORTOS_CONFIGURATION_H := $(OUTPUT)include/distortos/distortosConfiguration.h
MAKE_DISTORTOS_CONFIGURATION_SH := $(DISTORTOS_PATH)scripts/makeDistortosConfiguration.sh

$(DISTORTOS_CONFIGURATION_H): $(DISTORTOS_CONFIGURATION_MK)
	$(call PRETTY_PRINT,"SH     " $(MAKE_DISTORTOS_CONFIGURATION_SH))
	$(Q)./$(MAKE_DISTORTOS_CONFIGURATION_SH) "$(dir $<)$(notdir $<)" > "$@"

#-----------------------------------------------------------------------------------------------------------------------
# generated headers depend on this Rules.mk, the script that generates them and the selectedConfiguration.mk file
#-----------------------------------------------------------------------------------------------------------------------

$(DISTORTOS_CONFIGURATION_H): $(d)Rules.mk $(MAKE_DISTORTOS_CONFIGURATION_SH) selectedConfiguration.mk

#-----------------------------------------------------------------------------------------------------------------------
# add generated headers to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(DISTORTOS_CONFIGURATION_H)

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include $(DISTORTOS_PATH)footer.mk
