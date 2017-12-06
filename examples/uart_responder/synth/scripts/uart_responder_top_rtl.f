
-f ${STD_PROTOCOL_IF}/rtl/axi4/axi4.f

-f ${STD_PROTOCOL_IF}/rtl/wb/wb.f
-f ${OC_WB_IP}/rtl/wb_uart/rtl.f

${RUNDIR}/uart_responder_synth/synth/UartResponder.v
${SYNTH_DIR}/uart_responder_top.sv


