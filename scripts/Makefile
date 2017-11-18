
FPGAMGR_SCRIPTS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
FPGAMGR_SCRIPTS_DIR := $(shell cd $(FPGAMGR_SCRIPTS_DIR) ; pwd)
FPGAMGR_DIR := $(shell cd $(FPGAMGR_SCRIPTS_DIR)/.. ; pwd)

MK_INCLUDES += $(FPGAMGR_DIR)/src/fpgamgr.mk

include $(MK_INCLUDES)

vpath %.cpp $(SRC_DIRS)
CXXFLAGS += -fPIC $(foreach d,$(SRC_DIRS),-I$(d))

RULES := 1

all : $(LIB_TARGETS) 

include $(MK_INCLUDES)

