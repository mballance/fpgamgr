
+incdir+${FPGAMGR}/examples/uart_responder/ve/tb
+incdir+${FPGAMGR}/examples/uart_responder/ve/tests

-f ${SV_BFMS}/uart/sv.f
-f ${SV_BFMS}/uart/uvm/uvm.f

-f ${STD_PROTOCOL_IF}/rtl/wb/wb.f
-f ${OC_WB_IP}/rtl/wb_uart/rtl.f

${FPGAMGR}/examples/uart_responder/ve/tb/uart_responder_env_pkg.sv
${FPGAMGR}/examples/uart_responder/ve/tests/uart_responder_tests_pkg.sv
${FPGAMGR}/examples/uart_responder/ve/tb/UartResponderTBClkGen.sv
${FPGAMGR}/examples/uart_responder/ve/tb/UartResponderTBHvl.sv
${RUNDIR}/uart_responder/qs/UartResponderTB.v


