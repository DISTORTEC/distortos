#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-10-09
#

#-----------------------------------------------------------------------------------------------------------------------
# subdirectories
#-----------------------------------------------------------------------------------------------------------------------

SUBDIRECTORIES += source
SUBDIRECTORIES += test

#-----------------------------------------------------------------------------------------------------------------------
# generated headers
#-----------------------------------------------------------------------------------------------------------------------

DISTORTOS_CONFIGURATION_H := $(OUTPUT)include/distortos/distortosConfiguration.h

$(DISTORTOS_CONFIGURATION_H): $(DISTORTOS_CONFIGURATION_MK)
	./scripts/makeDistortosConfiguration.awk "$(dir $<)$(notdir $<)" > "$@"

#-----------------------------------------------------------------------------------------------------------------------
# generated headers depend on this Rules.mk, the script that generates them and the selectedConfiguration.mk file
#-----------------------------------------------------------------------------------------------------------------------

$(DISTORTOS_CONFIGURATION_H): Rules.mk scripts/makeDistortosConfiguration.awk selectedConfiguration.mk

#-----------------------------------------------------------------------------------------------------------------------
# add generated headers to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(DISTORTOS_CONFIGURATION_H)

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk
