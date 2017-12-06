create_clock -period 20 [get_ports clk]
# create_generated_clock -name clk -source [get_ports {clk_i}] -divide_by 2 [get_registers {clk_r}]

derive_pll_clocks

derive_clock_uncertainty




