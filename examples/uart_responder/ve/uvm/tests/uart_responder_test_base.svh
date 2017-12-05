
class uart_responder_test_base extends uvm_test;
	
	`uvm_component_utils(uart_responder_test_base)
	
	uart_responder_env				m_env;
	
	function new(string name, uvm_component parent=null);
		super.new(name,parent);
	endfunction
	
	function void build_phase(uvm_phase phase);
		super.build_phase(phase);
	
		m_env = uart_responder_env::type_id::create("m_env", this);
	endfunction

	task run_phase(uvm_phase phase);
		bit[7:0] data;
		bit valid;
		phase.raise_objection(this, "Main");
	
		// Test sending a bit
		for (int i=1; i<=10; i++) begin
			m_env.m_uart0.putc('h5a);
			m_env.m_uart0.getc(data, valid, 2);
			
			if (valid) begin
				break;
			end
		end
		
		if (!valid) begin
			$display("Error: failed to connect to uart");
		end
		
		for (int i=1; i<=512; i++) begin
			m_env.m_uart0.putc(i);
			m_env.m_uart0.getc(data, valid, 10);
			
			if (!valid) begin
				$display("Error: failed to receive data");
				break;
			end else begin
				$display("Note: receive 'h%02h", data);
			end
			
			if (data != i) begin
				$display("Error: expected 'h%02h received 'h%02h", i, data);
			end
		end
		
		phase.drop_objection(this, "Main");
		
	endtask
	
endclass

