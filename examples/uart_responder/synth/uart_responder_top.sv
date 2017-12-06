/****************************************************************************
 * cyclonev_soc_fpga.sv
 ****************************************************************************/
 
/**
 * Module: uart_responder_top
 * 
 * TODO: Add module documentation
 */
module uart_responder_top(
		output wire [3:0]  fpga_led_pio,
		input  wire [3:0]  fpga_button_pio,
		// HPS memory controller ports
		output wire [14:0] memory_mem_a,
		output wire [2:0]  memory_mem_ba,
		output wire        memory_mem_ck,
		output wire        memory_mem_ck_n,
		output wire        memory_mem_cke,
		output wire        memory_mem_cs_n,
		output wire        memory_mem_ras_n,
		output wire        memory_mem_cas_n,
		output wire        memory_mem_we_n,
		output wire        memory_mem_reset_n,
		inout  wire [39:0] memory_mem_dq,
		inout  wire [4:0]  memory_mem_dqs,
		inout  wire [4:0]  memory_mem_dqs_n,
		output wire        memory_mem_odt,
		output wire [4:0]  memory_mem_dm,
		input  wire        memory_oct_rzqin,
		// HPS peripherals
		output wire        hps_emac1_TX_CLK,
		output wire        hps_emac1_TXD0,
		output wire        hps_emac1_TXD1,
		output wire        hps_emac1_TXD2,
		output wire        hps_emac1_TXD3,
		input  wire        hps_emac1_RXD0,
		inout  wire        hps_emac1_MDIO,
		output wire        hps_emac1_MDC,
		input  wire        hps_emac1_RX_CTL,
		output wire        hps_emac1_TX_CTL,
		input  wire        hps_emac1_RX_CLK,
		input  wire        hps_emac1_RXD1,
		input  wire        hps_emac1_RXD2,
		input  wire        hps_emac1_RXD3,
		inout  wire        hps_qspi_IO0,
		inout  wire        hps_qspi_IO1,
		inout  wire        hps_qspi_IO2,
		inout  wire        hps_qspi_IO3,
		output wire        hps_qspi_SS0,
		output wire        hps_qspi_CLK,
		inout  wire        hps_sdio_CMD,
		inout  wire        hps_sdio_D0,
		inout  wire        hps_sdio_D1,
		output wire        hps_sdio_CLK,
		inout  wire        hps_sdio_D2,
		inout  wire        hps_sdio_D3,
		inout  wire        hps_usb1_D0,
		inout  wire        hps_usb1_D1,
		inout  wire        hps_usb1_D2,
		inout  wire        hps_usb1_D3,
		inout  wire        hps_usb1_D4,
		inout  wire        hps_usb1_D5,
		inout  wire        hps_usb1_D6,
		inout  wire        hps_usb1_D7,
		input  wire        hps_usb1_CLK,
		output wire        hps_usb1_STP,
		input  wire        hps_usb1_DIR,
		input  wire        hps_usb1_NXT,
		output wire        hps_spim0_CLK,
		output wire        hps_spim0_MOSI,
		input  wire        hps_spim0_MISO,
		output wire        hps_spim0_SS0,
		output wire        hps_spim1_CLK,
		output wire        hps_spim1_MOSI,
		input  wire        hps_spim1_MISO,
		output wire        hps_spim1_SS0,
		input  wire        hps_uart0_RX,
		output wire        hps_uart0_TX,
		inout  wire        hps_i2c1_SDA,
		inout  wire        hps_i2c1_SCL,
		inout  wire        hps_gpio_GPIO00,
		inout  wire        hps_gpio_GPIO09,
		inout  wire        hps_gpio_GPIO35,
		inout  wire        hps_gpio_GPIO48,
		inout  wire        hps_gpio_GPIO53,
		inout  wire        hps_gpio_GPIO54,
		inout  wire        hps_gpio_GPIO55,
		inout  wire        hps_gpio_GPIO56,
		inout  wire        hps_gpio_GPIO61,
		inout  wire        hps_gpio_GPIO62,
		// FPGA clock and reset
		input  wire        clk_50m_fpga
		);
	wire [1:0] fpga_debounced_buttons;
	wire [3:0]  fpga_led_internal;
	wire        hps_fpga_reset_n;
	wire        hps_cold_reset = 0;
	wire        hps_warm_reset = 0;
	wire        hps_debug_reset = 0;
	wire [27:0] stm_hw_events;
	
	wire hps_0_uart1_cts, hps_0_uart1_dsr, hps_0_uart1_dcd;
	wire hps_0_uart1_ri, hps_0_uart1_dtr, hps_0_uart1_rts;
	wire hps_0_uart1_out1_n, hps_0_uart1_out2_n;
	wire hps_0_uart1_rxd, hps_0_uart1_txd;
	
	wire io_u_CTS=1, io_u_DSR=1, io_u_DCD=1, io_u_RI=1;
	
	reg [7:0] rst_cnt = 0;
	reg reset = 1;
	
	wire[3:0] count;
	assign fpga_led_pio[0] = count[0];
	assign fpga_led_pio[1] = count[1];
	assign fpga_led_pio[2] = count[2];
	assign fpga_led_pio[3] = count[3];
	
	assign hps_0_uart1_cts = 1;
	assign hps_0_uart1_dsr = 1;
	assign hps_0_uart1_dcd = 1;
	assign hps_0_uart1_ri  = 1;
	
	always @(posedge clk_50m_fpga) begin
		if (rst_cnt == 100) begin
			reset <= 0;
		end else begin
			rst_cnt <= rst_cnt + 1;
		end
	end
	
	// TODO: instantiate design here
	UartResponder UartResponder (
		.clock     (clk_50m_fpga    ), 
		.reset     (reset  			), 
		.io_count  (count           ),
		.io_u_TxD  (hps_0_uart1_rxd ), 
		.io_u_RxD  (hps_0_uart1_txd ), 
		.io_u_CTS  (io_u_CTS ), 
		.io_u_DSR  (io_u_DSR ), 
		.io_u_DCD  (io_u_DCD ), 
		.io_u_RI   (io_u_RI  ));
	
	// SoC sub-system module
	soc_system soc_inst (
			.memory_mem_a                         (memory_mem_a),
			.memory_mem_ba                        (memory_mem_ba),
			.memory_mem_ck                        (memory_mem_ck),
			.memory_mem_ck_n                      (memory_mem_ck_n),
			.memory_mem_cke                       (memory_mem_cke),
			.memory_mem_cs_n                      (memory_mem_cs_n),
			.memory_mem_ras_n                     (memory_mem_ras_n),
			.memory_mem_cas_n                     (memory_mem_cas_n),
			.memory_mem_we_n                      (memory_mem_we_n),
			.memory_mem_reset_n                   (memory_mem_reset_n),
			.memory_mem_dq                        (memory_mem_dq),
			.memory_mem_dqs                       (memory_mem_dqs),
			.memory_mem_dqs_n                     (memory_mem_dqs_n),
			.memory_mem_odt                       (memory_mem_odt),
			.memory_mem_dm                        (memory_mem_dm),
			.memory_oct_rzqin                     (memory_oct_rzqin),
			.dipsw_pio_external_connection_export (fpga_dipsw_pio),
			/**
			.led_pio_external_connection_in_port  (fpga_led_internal),
			.led_pio_external_connection_out_port (fpga_led_internal),
			 **/
			.button_pio_external_connection_export(fpga_debounced_buttons),
			.hps_0_hps_io_hps_io_emac1_inst_TX_CLK(hps_emac1_TX_CLK),
			.hps_0_hps_io_hps_io_emac1_inst_TXD0  (hps_emac1_TXD0),
			.hps_0_hps_io_hps_io_emac1_inst_TXD1  (hps_emac1_TXD1),
			.hps_0_hps_io_hps_io_emac1_inst_TXD2  (hps_emac1_TXD2),
			.hps_0_hps_io_hps_io_emac1_inst_TXD3  (hps_emac1_TXD3),
			.hps_0_hps_io_hps_io_emac1_inst_RXD0  (hps_emac1_RXD0),
			.hps_0_hps_io_hps_io_emac1_inst_MDIO  (hps_emac1_MDIO),
			.hps_0_hps_io_hps_io_emac1_inst_MDC   (hps_emac1_MDC),
			.hps_0_hps_io_hps_io_emac1_inst_RX_CTL(hps_emac1_RX_CTL),
			.hps_0_hps_io_hps_io_emac1_inst_TX_CTL(hps_emac1_TX_CTL),
			.hps_0_hps_io_hps_io_emac1_inst_RX_CLK(hps_emac1_RX_CLK),
			.hps_0_hps_io_hps_io_emac1_inst_RXD1  (hps_emac1_RXD1),
			.hps_0_hps_io_hps_io_emac1_inst_RXD2  (hps_emac1_RXD2),
			.hps_0_hps_io_hps_io_emac1_inst_RXD3  (hps_emac1_RXD3),
			.hps_0_hps_io_hps_io_qspi_inst_IO0    (hps_qspi_IO0),
			.hps_0_hps_io_hps_io_qspi_inst_IO1    (hps_qspi_IO1),
			.hps_0_hps_io_hps_io_qspi_inst_IO2    (hps_qspi_IO2),
			.hps_0_hps_io_hps_io_qspi_inst_IO3    (hps_qspi_IO3),
			.hps_0_hps_io_hps_io_qspi_inst_SS0    (hps_qspi_SS0),
			.hps_0_hps_io_hps_io_qspi_inst_CLK    (hps_qspi_CLK),
			.hps_0_hps_io_hps_io_sdio_inst_CMD    (hps_sdio_CMD),
			.hps_0_hps_io_hps_io_sdio_inst_D0     (hps_sdio_D0),
			.hps_0_hps_io_hps_io_sdio_inst_D1     (hps_sdio_D1),
			.hps_0_hps_io_hps_io_sdio_inst_CLK    (hps_sdio_CLK),
			.hps_0_hps_io_hps_io_sdio_inst_D2     (hps_sdio_D2),
			.hps_0_hps_io_hps_io_sdio_inst_D3     (hps_sdio_D3),
			.hps_0_hps_io_hps_io_usb1_inst_D0     (hps_usb1_D0),
			.hps_0_hps_io_hps_io_usb1_inst_D1     (hps_usb1_D1),
			.hps_0_hps_io_hps_io_usb1_inst_D2     (hps_usb1_D2),
			.hps_0_hps_io_hps_io_usb1_inst_D3     (hps_usb1_D3),
			.hps_0_hps_io_hps_io_usb1_inst_D4     (hps_usb1_D4),
			.hps_0_hps_io_hps_io_usb1_inst_D5     (hps_usb1_D5),
			.hps_0_hps_io_hps_io_usb1_inst_D6     (hps_usb1_D6),
			.hps_0_hps_io_hps_io_usb1_inst_D7     (hps_usb1_D7),
			.hps_0_hps_io_hps_io_usb1_inst_CLK    (hps_usb1_CLK),
			.hps_0_hps_io_hps_io_usb1_inst_STP    (hps_usb1_STP),
			.hps_0_hps_io_hps_io_usb1_inst_DIR    (hps_usb1_DIR),
			.hps_0_hps_io_hps_io_usb1_inst_NXT    (hps_usb1_NXT),
			.hps_0_hps_io_hps_io_spim0_inst_CLK    (hps_spim0_CLK),
			.hps_0_hps_io_hps_io_spim0_inst_MOSI   (hps_spim0_MOSI),
			.hps_0_hps_io_hps_io_spim0_inst_MISO   (hps_spim0_MISO),
			.hps_0_hps_io_hps_io_spim0_inst_SS0    (hps_spim0_SS0),
			.hps_0_hps_io_hps_io_spim1_inst_CLK    (hps_spim1_CLK),
			.hps_0_hps_io_hps_io_spim1_inst_MOSI   (hps_spim1_MOSI),
			.hps_0_hps_io_hps_io_spim1_inst_MISO   (hps_spim1_MISO),
			.hps_0_hps_io_hps_io_spim1_inst_SS0    (hps_spim1_SS0),
			.hps_0_hps_io_hps_io_uart0_inst_RX    (hps_uart0_RX),
			.hps_0_hps_io_hps_io_uart0_inst_TX    (hps_uart0_TX),
			.hps_0_hps_io_hps_io_i2c1_inst_SDA    (hps_i2c1_SDA),
			.hps_0_hps_io_hps_io_i2c1_inst_SCL    (hps_i2c1_SCL),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO00 (hps_gpio_GPIO00),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO09 (hps_gpio_GPIO09),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO35 (hps_gpio_GPIO35),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO48 (hps_gpio_GPIO48),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO53 (hps_gpio_GPIO53),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO54 (hps_gpio_GPIO54),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO55 (hps_gpio_GPIO55),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO56 (hps_gpio_GPIO56),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO61 (hps_gpio_GPIO61),
			.hps_0_hps_io_hps_io_gpio_inst_GPIO62 (hps_gpio_GPIO62),
			.hps_0_uart1_cts                      (hps_0_uart1_cts),
			.hps_0_uart1_dsr                      (hps_0_uart1_dsr),
			.hps_0_uart1_dcd                      (hps_0_uart1_dcd),
			.hps_0_uart1_ri                       (hps_0_uart1_ri),
			.hps_0_uart1_dtr                      (hps_0_uart1_dtr),
			.hps_0_uart1_rts                      (hps_0_uart1_rts),
			.hps_0_uart1_out1_n                   (hps_0_uart1_out1_n),
			.hps_0_uart1_out2_n                   (hps_0_uart1_out2_n),
			.hps_0_uart1_rxd                      (hps_0_uart1_rxd),
			.hps_0_uart1_txd                      (hps_0_uart1_txd),
			.hps_0_f2h_stm_hw_events_stm_hwevents (stm_hw_events),
			.clk_clk                              (fpga_clk_50),
			.hps_0_h2f_reset_reset_n              (hps_fpga_reset_n),
			.reset_reset_n                        (hps_fpga_reset_n),
			.hps_0_f2h_cold_reset_req_reset_n     (~hps_cold_reset),
			.hps_0_f2h_warm_reset_req_reset_n     (~hps_warm_reset),
			.hps_0_f2h_debug_reset_req_reset_n    (~hps_debug_reset)
		);
	
endmodule


