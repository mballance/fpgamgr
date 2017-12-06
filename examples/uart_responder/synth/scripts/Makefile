TOP_MODULE=uart_responder_top

FAMILY:="Cyclone V"
DEVICE:=5CSXFC6D6F31C8ES
# DEVICE:=5CSXFC6D6F31C6

VERBOSE:=true

MK_INCLUDES += $(CHISELSCRIPTS)/mkfiles/chiselscripts.mk
MK_INCLUDES += $(SV_BFMS)/src/sv_bfms.mk
MK_INCLUDES += $(CHISELLIB)/src/chisellib.mk
MK_INCLUDES += $(OC_WB_IP)/src/oc_wb_ip.mk
MK_INCLUDES += $(STD_PROTOCOL_IF)/src/std_protocol_if.mk
MK_INCLUDES += $(FPGAMGR)/examples/uart_responder/src/uart_responder.mk
MK_INCLUDES += $(FPGAMGR)/subprojects/cyclonev_hps_shell/cyclonev_hps_shell.mk

IMG_PRECOMPILE_TARGETS += UartResponder.v soc_system.d

include $(SYNTHSCRIPTS_DIR)/mkfiles/common_synth.mk

UartResponder.v : $(UART_RESPONDER_JARS)
	$(Q)$(DO_CHISELG) uart_responder.GenUartResponder


