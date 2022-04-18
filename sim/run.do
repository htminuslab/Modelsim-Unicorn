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
run 1 us
quit -f
