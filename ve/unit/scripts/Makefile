
FPGAMGR_DIR := $(shell cd $(SIM_DIR)/../.. ; pwd)

MK_INCLUDES += $(FPGAMGR_DIR)/src/fpgamgr.mk
MK_INCLUDES += $(FPGAMGR_DIR)/subprojects/googletest_uvm/src/rules_defs.mk
MK_INCLUDES += $(SIMSCRIPTS_DIR)/mkfiles/common_tool_gcc.mk

SRC_DIRS += $(SIM_DIR)/src

BUILD_LINK_TARGETS += unit_test_main
RUN_TARGETS += run_test

UNIT_TEST_SRC=$(notdir $(wildcard $(SIM_DIR)/src/*.cpp))

GTEST_FILTER:=$(call get_plusargs,GTEST_FILTER,$(PLUSARGS))

ifneq (,$(GTEST_FILTER))
	GOOGLE_TEST_ARGS += --gtest_filter=$(GTEST_FILTER)
endif

unit_test_main : $(UNIT_TEST_SRC:.cpp=.o) \
	libfpgamgr_server.a \
	libfpgamgr_client.a \
	libfpgamgr_common.a \
	googletest.o 
	$(Q)$(CXX) -o $@ $(filter-out build-%,$^) -lpthread
	
run_test :
	$(Q)$(BUILD_DIR_A)/unit_test_main $(GOOGLE_TEST_ARGS)

include $(SIMSCRIPTS_DIR)/mkfiles/common_sim.mk
