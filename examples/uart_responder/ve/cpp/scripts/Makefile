
include $(SIMSCRIPTS_DIR)/mkfiles/plusargs.mk
FPGAMGR_DIR := $(shell cd $(SIM_DIR)/../../../.. ; pwd)

MK_INCLUDES += $(FPGAMGR_DIR)/src/fpgamgr.mk
MK_INCLUDES += $(SIMSCRIPTS_DIR)/mkfiles/common_tool_gcc.mk

BUILD_LINK_TARGETS += fpga_test_main
RUN_TARGETS += run_test

SRC_DIRS := $(SIM_DIR)/src

fpga_test_main : fpga_test_main.o \
	libfpgamgr_client.a \
	libfpgamgr_common.a
	$(Q)$(CXX) -o $@ $(filter-out build-%,$^) 

run_test :
	$(Q)$(BUILD_DIR_A)/fpga_test_main $(PLUSARGS)

include $(SIMSCRIPTS_DIR)/mkfiles/common_sim.mk

UartResponderTB.v : $(UART_RESPONDER_JARS)
	$(Q)$(DO_CHISELG) uart_responder.GenUartResponderTB
