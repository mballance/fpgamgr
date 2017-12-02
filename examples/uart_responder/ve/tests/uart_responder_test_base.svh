
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
	
endclass

