
FPGAMGR_SRC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(FPGAMGR_SRC_DIR) 
SRC_DIRS += $(FPGAMGR_SRC_DIR)/client
SRC_DIRS += $(FPGAMGR_SRC_DIR)/common
SRC_DIRS += $(FPGAMGR_SRC_DIR)/server
SRC_DIRS += $(FPGAMGR_SRC_DIR)/server/altera
SRC_DIRS += $(FPGAMGR_SRC_DIR)/server/config
SRC_DIRS += $(FPGAMGR_SRC_DIR)/../subprojects/json/src

FPGAMGR_SERVER_SRC = $(notdir $(wildcard $(FPGAMGR_SRC_DIR)/server/*.cpp))
FPGAMGR_SERVER_ALTERA_SRC = $(notdir $(wildcard $(FPGAMGR_SRC_DIR)/server/altera/*.cpp))
FPGAMGR_CLIENT_SRC = $(notdir $(wildcard $(FPGAMGR_SRC_DIR)/client/*.cpp))
FPGAMGR_COMMON_SRC = $(notdir $(wildcard $(FPGAMGR_SRC_DIR)/common/*.cpp))

LIB_TARGETS += libfpgamgr_server.a libfpgamgr_server_altera.a
LIB_TARGETS += libfpgamgr_client.a libfpgamgr_common.a

else # Rules

libfpgamgr_server.a : $(FPGAMGR_SERVER_SRC:.cpp=.o)
	$(Q)rm -f $@
	$(Q)$(AR) vcq $@ $^
	
libfpgamgr_server_altera.a : $(FPGAMGR_ALTERA_SERVER_SRC:.cpp=.o)
	$(Q)rm -f $@
	$(Q)$(AR) vcq $@ $^
	
libfpgamgr_client.a : $(FPGAMGR_CLIENT_SRC:.cpp=.o)
	$(Q)rm -f $@
	$(Q)$(AR) vcq $@ $^
	
libfpgamgr_common.a : $(FPGAMGR_COMMON_SRC:.cpp=.o)
	$(Q)rm -f $@
	$(Q)$(AR) vcq $@ $^

endif
