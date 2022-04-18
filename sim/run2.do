# Create work library.
if [file exists work] {
	vdel -all	
}
vlib work    

# Compile Design
vcom -quiet -2008 ../rtl/riscv.vhd

# Compile Testbench
vcom -quiet -2008 ../rtl/tb.vhd

# set StdArithNoWarnings 1
vsim -quiet tb 
nolog -all

# Disable the disassembler and memory outputs
riscv_cmd disable_reg_display
riscv_cmd disable_rw_display

run 2000 us
quit -f
