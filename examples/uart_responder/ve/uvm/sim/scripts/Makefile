
include $(SIMSCRIPTS_DIR)/mkfiles/plusargs.mk

MK_INCLUDES += $(CHISELSCRIPTS)/mkfiles/chiselscripts.mk
MK_INCLUDES += $(SV_BFMS)/src/sv_bfms.mk
MK_INCLUDES += $(CHISELLIB)/src/chisellib.mk
MK_INCLUDES += $(OC_WB_IP)/src/oc_wb_ip.mk
MK_INCLUDES += $(STD_PROTOCOL_IF)/src/std_protocol_if.mk
MK_INCLUDES += $(FPGAMGR)/examples/uart_responder/src/uart_responder.mk

TB=UartResponderTB
TB_MODULES:=UartResponderTB UartResponderTBHvl uart_responder_tests_pkg

# VOPT_FLAGS += -GCLK_PERIOD=20ns

BUILD_PRECOMPILE_TARGETS += UartResponderTB.v

include $(SIMSCRIPTS_DIR)/mkfiles/common_sim.mk

UartResponderTB.v : $(UART_RESPONDER_JARS)
	$(Q)$(DO_CHISELG) uart_responder.GenUartResponderTB
