
+incdir+${FPGAMGR}/examples/uart_responder/ve/uvm/tb
+incdir+${FPGAMGR}/examples/uart_responder/ve/uvm/tests

-f ${SV_BFMS}/uart/sv.f
-f ${SV_BFMS}/uart/uvm/uvm.f

-f ${STD_PROTOCOL_IF}/rtl/wb/wb.f
-f ${OC_WB_IP}/rtl/wb_uart/rtl.f

// +define+CLK_PERIOD=40ns

${FPGAMGR}/examples/uart_responder/ve/uvm/tb/uart_responder_env_pkg.sv
${FPGAMGR}/examples/uart_responder/ve/uvm/tests/uart_responder_tests_pkg.sv
${FPGAMGR}/examples/uart_responder/ve/uvm/tb/UartResponderTBClkGen.sv
${FPGAMGR}/examples/uart_responder/ve/uvm/tb/UartResponderTBHvl.sv
${RUNDIR}/uart_responder/qs/UartResponderTB.v


