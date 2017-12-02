/****************************************************************************
 * uart_responder_tb.sv
 ****************************************************************************/

/**
 * Module: uart_responder_tb
 * 
 * TODO: Add module documentation
 */
`include "uvm_macros.svh"
module uart_responder_tb;
	import uvm_pkg::*;
	import uart_responder_tests_pkg::*;
	
	initial begin
		run_test();
	end

endmodule

