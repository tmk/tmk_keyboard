# copied from: https://github.com/ShawnHymel/c-unit-test

# Name of component under test
COMPONENT_NAME = action_tapping

# Dynamically generate absolute paths
# From https://matheusmbar.com/bugfree-robot/2019/04/05/makefile-first-test.html
MAKEFILE_DIR = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))

TMK_CORE_DIR = $(realpath $(MAKEFILE_DIR)../..)
TMK_CORE_COMMON_DIR = $(TMK_CORE_DIR)/common

# Location of CppUTest (if not already set)
CPPUTEST_HOME ?= $(TMK_CORE_DIR)/tool/cpputest

TEST_DIR = $(TMK_CORE_DIR)/test

# Include and source files for component under test
INCLUDE_DIRS = $(TMK_CORE_COMMON_DIR)
#SRC_DIRS = $(TMK_CORE_COMMON_DIR)
SRC_FILES = $(TMK_CORE_COMMON_DIR)/action_tapping.c \
            $(TMK_CORE_COMMON_DIR)/action.c \
            $(TMK_CORE_COMMON_DIR)/action_layer.c \
            $(TMK_CORE_COMMON_DIR)/action_util.c \
            $(TMK_CORE_COMMON_DIR)/hook.c \
            $(TMK_CORE_COMMON_DIR)/util.c \
            $(TMK_CORE_COMMON_DIR)/debug.c

# Includes CppUTest header files
INCLUDE_DIRS += $(CPPUTEST_HOME)/include

# Directories containing the unit tests
TEST_SRC_DIRS = $(TEST_DIR)/$(COMPONENT_NAME)


#CPPUTEST_ADDITIONAL_CFLAGS += -DNO_DEBUG
CPPUTEST_ADDITIONAL_CFLAGS += -DDEBUG_ACTION
CPPUTEST_ADDITIONAL_CFLAGS += -DNO_ACTION_MACRO -DNO_ACTION_FUNCTION
CPPUTEST_ADDITIONAL_CFLAGS += -DMATRIX_ROWS=1 -DMATRIX_COLS=4
#CPPUTEST_ADDITIONAL_CFLAGS += -include config.h

CPPUTEST_ADDITIONAL_CXXFLAGS := $(CPPUTEST_ADDITIONAL_CFLAGS)

#CPPUTEST_WARNINGFLAGS =  -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wno-long-long
CPPUTEST_WARNINGFLAGS =  -W
CPPUTEST_WARNINGFLAGS += -Wno-format
CPPUTEST_WARNINGFLAGS += -Wno-conversion
CPPUTEST_PEDANTIC_ERRORS = N

# Run MakefileWorker.mk with the variables set in this Makefile
include $(CPPUTEST_HOME)/build/MakefileWorker.mk


# Print out the value of a make variable.
# https://stackoverflow.com/questions/16467718/how-to-print-out-a-variable-in-makefile
print-%:
	@echo $* = $($*)
