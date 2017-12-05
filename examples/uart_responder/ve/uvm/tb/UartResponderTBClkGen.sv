/****************************************************************************
 * UartResponderTBClkGen.sv
 ****************************************************************************/
 
`ifndef CLK_PERIOD
	`define CLK_PERIOD 20ns
`endif

/**
 * Module: UartResponderTBClkGen
 * 
 * TODO: Add module documentation
 */
module UartResponderTBClkGen (output clock, output reset);
	reg clock_r = 0;
	reg reset_r = 1;
	parameter reset_cnt = 100;

	assign clock = clock_r;
	assign reset = reset_r;

	initial begin
		repeat (reset_cnt*2) begin
			#((`CLK_PERIOD)/2);
			clock_r <= ~clock_r;
		end

		reset_r <= 0;

		forever begin
			#((`CLK_PERIOD)/2);
			clock_r <= ~clock_r;
		end
	end
endmodule


