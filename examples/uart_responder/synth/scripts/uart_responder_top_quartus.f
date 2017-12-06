// -f ${MEMORY_PRIMITIVES}/rtl/fpga/altsyncram/rtl.f

-f ${SYNTH_DIR}/scripts/uart_responder_top_rtl.f

// Map pins on top-level block
-do ${FPGAMGR}/subprojects/cyclonev_hps_shell/scripts/soc_system_pins.do
${RUNDIR_A}/uart_responder_synth/synth/soc_system/synthesis/soc_system.qip

/**
 * Port mapping
-assign-pin clk PIN_K14
-assign-pin pad0_o PIN_AF10
-assign-pin pad1_o PIN_AD10
-assign-pin pad2_o PIN_AE11
-assign-pin pad3_o PIN_AD7
 */

/**
 * Voltage
-io-standard clk "3.3-V LVTTL"
-io-standard pad0_o "3.3-V LVTTL"
-io-standard pad1_o "3.3-V LVTTL"
-io-standard pad2_o "3.3-V LVTTL"
-io-standard pad3_o "3.3-V LVTTL"
 */


