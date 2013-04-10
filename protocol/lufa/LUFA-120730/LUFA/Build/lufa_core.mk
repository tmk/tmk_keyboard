#
#             LUFA Library
#     Copyright (C) Dean Camera, 2012.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#

LUFA_BUILD_MODULES         += CORE
LUFA_BUILD_TARGETS         += help list_targets list_modules list_mandatory list_optional list_provided list_macros
LUFA_BUILD_MANDATORY_VARS  += 
LUFA_BUILD_OPTIONAL_VARS   += 
LUFA_BUILD_PROVIDED_VARS   += 
LUFA_BUILD_PROVIDED_MACROS += 

# -----------------------------------------------------------------------------
#               LUFA Core Build System Makefile Module.
# -----------------------------------------------------------------------------
# DESCRIPTION:
#   Provides a set of core build targets for the LUFA build system
# -----------------------------------------------------------------------------
# TARGETS:
#
#    help                      - Build system help
#    list_targets              - List all build targets
#    list_modules              - List all build modules
#    list_mandatory            - List all mandatory make variables required by
#                                the included build modules of the application
#    list_optional             - List all optional make variables required by
#                                the included build modules of the application
#    list_provided             - List all provided make variables from the
#                                included build modules of the application
#    list_macros               - List all provided make macros from the
#                                included build modules of the application
#
# MANDATORY PARAMETERS:
#
#    (None)
#
# OPTIONAL PARAMETERS:
#
#    (None)
#
# PROVIDED VARIABLES:
#
#    (None)
#
# PROVIDED MACROS:
#
#    (None)
#
# -----------------------------------------------------------------------------

SHELL = /bin/sh

# Build sorted and filtered lists of the included build module data
SORTED_LUFA_BUILD_MODULES   = $(sort $(LUFA_BUILD_MODULES))
SORTED_LUFA_BUILD_TARGETS   = $(sort $(LUFA_BUILD_TARGETS))
SORTED_LUFA_MANDATORY_VARS  = $(sort $(LUFA_BUILD_MANDATORY_VARS))
SORTED_LUFA_OPTIONAL_VARS   = $(filter-out $(SORTED_LUFA_MANDATORY_VARS), $(sort $(LUFA_BUILD_OPTIONAL_VARS)))
SORTED_LUFA_PROVIDED_VARS   = $(sort $(LUFA_BUILD_PROVIDED_VARS))
SORTED_LUFA_PROVIDED_MACROS = $(sort $(LUFA_BUILD_PROVIDED_MACROS))

# Create printable versions of the sorted build module data (use "(None)" when no data is available)
PRINTABLE_LUFA_BUILD_MODULES   = $(if $(strip $(SORTED_LUFA_BUILD_MODULES)), $(SORTED_LUFA_BUILD_MODULES), (None))
PRINTABLE_LUFA_BUILD_TARGETS   = $(if $(strip $(SORTED_LUFA_BUILD_TARGETS)), $(SORTED_LUFA_BUILD_TARGETS), (None))
PRINTABLE_LUFA_MANDATORY_VARS  = $(if $(strip $(SORTED_LUFA_MANDATORY_VARS)), $(SORTED_LUFA_MANDATORY_VARS), (None))
PRINTABLE_LUFA_OPTIONAL_VARS   = $(if $(strip $(SORTED_LUFA_OPTIONAL_VARS)), $(SORTED_LUFA_OPTIONAL_VARS), (None))
PRINTABLE_LUFA_PROVIDED_VARS   = $(if $(strip $(SORTED_LUFA_PROVIDED_VARS)), $(SORTED_LUFA_PROVIDED_VARS), (None))
PRINTABLE_LUFA_PROVIDED_MACROS = $(if $(strip $(SORTED_LUFA_PROVIDED_MACROS)), $(SORTED_LUFA_PROVIDED_MACROS), (None))

help:
	@echo "==================================================================="
	@echo "                       LUFA Build System 2.0                       "
	@echo "      (C) Dean Camera, 2012 { dean @ fourwalledcubicle . com }     "
	@echo "==================================================================="
	@echo "DESCRIPTION:                                                       "
	@echo " This build system is a set of makefile modules for (GNU) Make, to "
	@echo " provide a simple system for building LUFA powered applications.   "
	@echo " Each makefile module can be included from within a user makefile, "
	@echo " to expose the build rules documented in the comments at the top of"
	@echo " each build module.                                                "
	@echo "                                                                   "
	@echo "USAGE:                                                             "	
	@echo " To execute a rule, define all variables indicated in the desired  "
	@echo " module as a required parameter before including the build module  "
	@echo " in your project makefile. Parameters marked as optional will      "
	@echo " assume a default value in the modules if not user-assigned.       "
	@echo "                                                                   "
	@echo " By default the target output shows both a friendly summary, as    "
	@echo " well as the actual invoked command. To suppress the output of the "
	@echo " invoked commands and show only the friendly command output, run   "
	@echo " make with the \"-s\" switch added before the target(s).           "		
	@echo "==================================================================="
	@echo "                                                                   "
	@echo "  Currently used build system modules in this application:         "
	@echo "                                                                   "
	@printf " %b" "$(PRINTABLE_LUFA_BUILD_MODULES:%=   - %\n)"
	@echo "                                                                   "
	@echo "                                                                   "
	@echo "  Currently available build targets in this application:           "
	@echo "                                                                   "
	@printf " %b" "$(PRINTABLE_LUFA_BUILD_TARGETS:%=   - %\n)"
	@echo "                                                                   "
	@echo "                                                                   "
	@echo "  Mandatory variables required by the selected build Modules:      "
	@echo "                                                                   "
	@printf " %b" "$(PRINTABLE_LUFA_MANDATORY_VARS:%=   - %\n)"
	@echo "                                                                   "
	@echo "                                                                   "
	@echo "  Optional variables required by the selected build Modules:       "
	@echo "                                                                   "
	@printf " %b" "$(PRINTABLE_LUFA_OPTIONAL_VARS:%=   - %\n)"
	@echo "                                                                   "
	@echo "                                                                   "
	@echo "  Variables provided by the selected build Modules:                "
	@echo "                                                                   "
	@printf " %b" "$(PRINTABLE_LUFA_PROVIDED_VARS:%=   - %\n)"
	@echo "                                                                   "
	@echo "                                                                   "
	@echo "  Macros provided by the selected build Modules:                   "
	@echo "                                                                   "
	@printf " %b" "$(PRINTABLE_LUFA_PROVIDED_MACROS:%=   - %\n)"
	@echo "                                                                   "
	@echo "==================================================================="
	@echo "        The LUFA BuildSystem 2.0 - Powered By Unicorns (tm)        "
	@echo "==================================================================="
	
list_modules:
	@echo Currently Used Build System Modules: $(PRINTABLE_LUFA_BUILD_MODULES)

list_targets:
	@echo Currently Available Build Targets: $(PRINTABLE_LUFA_BUILD_TARGETS)	

list_mandatory:
	@echo Mandatory Variables for Included Modules: $(PRINTABLE_LUFA_MANDATORY_VARS)
	
list_optional:
	@echo Optional Variables for Included Modules: $(PRINTABLE_LUFA_OPTIONAL_VARS)

list_provided:
	@echo Variables Provided by the Included Modules: $(PRINTABLE_LUFA_PROVIDED_VARS)

list_macros:
	@echo Macros Provided by the Included Modules: $(PRINTABLE_LUFA_PROVIDED_MACROS)

# Disable default in-built make rules (those that are needed are explicitly
# defined, and doing so has performance benefits when recursively building)
.SUFFIXES:

# Phony build targets for this module
.PHONY: help list_modules list_targets list_mandatory list_optional list_provided list_macros
