# Modelsim-Unicorn

Demo Modelsim/Questa QEMU Unicorn integration via the VHDL FLI


<img src="unicorn.png" alt="Unicorn logo"/>  <img src="modeltech.PNG" alt="Old Modelsim logo"/>  

 
This repository shows how to integrate the [Unicorn Emulator](https://github.com/unicorn-engine/unicorn) to Modelsim/Questa via the VHDL FLI interface. The demo runs under Windows, an additional disassembler is included which was developed by [Michael Clark](https://github.com/michaeljclark/riscv-disassembler)
To simplify the (initial) test pre-build unicorn and FLI DLL files are supplied but I would strongly recommend not at least from a security point of view that you build your own DLL files. 
 
Unicorn is an excellent multiprocessor CPU emulator based on QEMU, it supports a wide range of processors such as ARM, ARM64, M68K, MIPS, PowerPC, SPARC, S390X, X86 and the for this demo used 32bits RISC-V. The emulator was developed by Nguyen Anh Quynh and Dang Hoang Vu from the university of Illinois. Further information can be found at http://www.unicorn-engine.org
 
## Requirements
1) 64bits Modelsim DE (or Questa), Modelsim PE users need to install 64bits Modelsim DE (works on a PE license)
2) 64bits gcc-7.4.0-mingw64vc15 as supplied with Modelsim 2022.1
3) Optional Risc-V gnu tools (not discussed here as there are lots of website describing the build process. However, an easy ready made solution can be obtained by installing the [Gowin's MCU Designer](https://www.gowinsemi.com/en/support/database/1331/) which contains the GNU tools for the popular opencore PicoRV32.

## Check before building
1) Make sure the Modelsim installation root directory is set via the **MTI_HOME** environmental variable. This is required because the build.bat file uses %MTI_HOME%/include and %MTI_HOME%/win64pe.
```
E:\Modelsim-Unicorn\sim>echo %MTI_HOME%
D:\Products\modelsim_de_2022
```

2) Make sure the Modelsim gcc bin directory (*<modelsim_install_directory>\gcc-7.4.0-mingw64vc15\bin*) is in the search path.
```
E:\Modelsim-Unicorn\sim>gcc --version
gcc (GCC) 7.4.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
 
## How does it work?
The design contains a VHDL riscv entity with the following architecture:

```VHDL
ARCHITECTURE rtl OF riscv IS
	attribute foreign : string;
	attribute foreign of rtl: architecture is "cif_init ./fli_riscv.dll";
BEGIN	
END rtl;
```
During elaboration Modelsim calls cif_init from fli_riscv.dll. The cif_init function effectively builds a VHDL process in memory (see riscv_process in src/riscv.c) with a positive clock edge and a positive asserted synchronous reset. For each clock edge the VHDL process calls the Unicorn **uc_emu_start()** function which executes 1 instruction followed by increasing the program counter.  
The Unicorn emulator supports so called **hooks** which are functions that are called whenever the emulator accesses memory, when interrupts occurs or after executing instructions. The latter one is used to update the local copy of the RISC-V register values. The FLI then drives these register values on to the output ports (each register has it own output port). Similarly, when the Unicorn emulator accesses memory a hook is called which the FLI uses to drive the WR and RD output port signals.
 
```VHDL 
entity riscv is
    generic(bin_filename : string(1 to 8) := "test.bin");
	port( 
		clk     : in    std_logic;
		sreset  : in    std_logic;						-- Sync reset
		rdn     : out   std_logic;						-- Active low Read strobe
		wrn     : out   std_logic;						-- Active low Write strobe
		dbus    : out	std_logic_vector(31 downto 0);
		abus    : out	std_logic_vector(31 downto 0);
      --reg_x0  : out	std_logic_vector(31 downto 0);
        reg_x1  : out	std_logic_vector(31 downto 0);
		...
        reg_x30 : out	std_logic_vector(31 downto 0);
        reg_x31 : out	std_logic_vector(31 downto 0); 	  
        pc	    : out	std_logic_vector(31 downto 0));	-- Program Counter
end riscv; 
``` 
 
For program storage the fli_riscv.c file mallocs 2Mbyte using **mti_Malloc()**, the standard malloc() function can also be used but the user is then responsible for storing and retrieving the memory when a checkpoint and restore is used (Questa only). The allocated memory needs to be released when the simulations ends. The FLI has 2 functions that can be used for this, the first one is **mti_AddQuitCB()** which is called when the user ends Modelsim/Questa and **mti_AddRestartCB()** which is called during a restart. The fli_riscv.c file calls both functions to free the memory, this is reported in the transcript window as "cleaning up....".
 
Note that the fli_riscv.dll file load unicorn.dll so this need to be either in the Modelsim/Questa root directory or in the search path.


## Adding commands to Modelsim
The FLI has a unique capability to allows the user to add commands. This is done via the FLI **mti_AddCommand()** function. We use this option to configure Unicorn during simulation. Two commands are added, one to enable/disable the disassembler output and one to enable/disable the memory access messages. The command added is called **riscv_cmd** and can be called any time after elaboration, example:

```
riscv_cmd disable_reg_display
riscv_cmd enable_reg_display
riscv_cmd disable_rw_display
riscv_cmd enable_rw_display
```
 
## Test Software
The test directory contains a simple hello_world example. The Makefile is executed under Mingw64:

```
$ /opt/riscv-toolchain/bin/riscv32-unknown-elf-gcc --version
riscv32-unknown-elf-gcc.exe (GCC) 11.1.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

```
$ make
/opt/riscv-toolchain/bin/riscv32-unknown-elf-gcc -c -mabi=ilp32 -march=rv32im -IC:/utils/RISC-V_toolchain/riscv-none-embed/include -ffreestanding -nostdlib start.S
/opt/riscv-toolchain/bin/riscv32-unknown-elf-gcc -c -mabi=ilp32 -march=rv32im -IC:/utils/RISC-V_toolchain/riscv-none-embed/include -ffreestanding -nostdlib -Wno-implicit-int -Wno-implicit-function-declaration main.c
/opt/riscv-toolchain/bin/riscv32-unknown-elf-gcc -mabi=ilp32 -march=rv32im -IC:/utils/RISC-V_toolchain/riscv-none-embed/include -ffreestanding -nostdlib -Wl,-T,rv32.lds,-Map,main.map,--strip-debug -o main.elf start.o main.o
/opt/riscv-toolchain/bin/riscv32-unknown-elf-objdump -drwx -S main.elf > main.asm
/opt/riscv-toolchain/bin/riscv32-unknown-elf-objcopy -O binary  main.elf main.bin
/opt/riscv-toolchain/bin/riscv32-unknown-elf-size --format=berkeley main.elf
   text    data     bss     dec     hex filename
    204       0       0     204      cc main.elf
```

The main.bin file is then loaded by the testbench at address 0x0000000

```VHDL
entity tb is
	generic(bin_filename : string(1 to 16) := "..\test\main.bin");
end tb;
```

If the main functions returns it will encounter an **ebreak** instruction which will cause an exception. The ebreak is located in the **start.S** file which will setup the stack and then calls main, if main returns it will hit the ebreak. The design does not handle the exception but will simply reports it.

```
# uc_emu_start() returned Invalid instruction (UC_ERR_INSN_INVALID)
```

## Build FLI DLL
To build the FLI DLL navigate to the sim directory and run the **build.bat** file in a CMD prompt. This should produce **fli_riscv.dll**.


## Simulation

The sim directory contains 2 Modelsim/Questa *.do* files. To run the simulation open a CMD prompt/terminal, navigate to the sim directory and execute 

```
vsim -c -do run.do
```

The testbench will load ..\test\main.bin into local memory and execute it. The transcript window will show the registers and disassembled opcodes after each instruction executed (1 per clock cycle) as shown below. It will also track any memory reads/writes and interrupts(exceptions).
 
The output should look something like (trimmed display):

```
E:\Modelsim-Unicorn\sim>vsim -c -do run.do
Reading pref.tcl

# 2022.1

# do run.do
# vsim -quiet tb
# //  ModelSim DE-64 2022.1 Jan 29 2022
# //
# //  Copyright 1991-2022 Mentor Graphics Corporation
# //  All Rights Reserved.
# //
# //  ModelSim DE-64 and its associated documentation contain trade
# //  secrets and commercial or financial information that are the property of
# //  Mentor Graphics Corporation and are privileged, confidential,
# //  and exempt from disclosure under the Freedom of Information Act,
# //  5 U.S.C. Section 552. Furthermore, this information
# //  is prohibited from disclosure under the Trade Secrets Act,
# //  18 U.S.C. Section 1905.
# //
# Generic: bin_filename = ..\test\main.bin
# Init Unicorn RISCV32 Emulator
# Reading rom file ..\test\main.bin, total 204 bytes loaded
#
# cif_init called
#  x0=00000000  x1=00000000  x2=00000000  x3=00000000  x4=00000000  x5=00000000  x6=00000000  x7=00000000
#  x8=00000000  x9=00000000 x10=00000000 x11=00000000 x12=00000000 x13=00000000 x14=00000000 x15=00000000
# x16=00000000 x17=00000000 x18=00000000 x19=00000000 x20=00000000 x21=00000000 x22=00000000 x23=00000000
# x24=00000000 x25=00000000 x26=00000000 x27=00000000 x28=00000000 x29=00000000 x30=00000000 x31=00000000
# pc=00010000 dbus=0040006f ; 0040006f j 4 # 0x4
#
#  x0=00000000  x1=00000000  x2=00000000  x3=00000000  x4=00000000  x5=00000000  x6=00000000  x7=00000000
#  x8=00000000  x9=00000000 x10=00000000 x11=00000000 x12=00000000 x13=00000000 x14=00000000 x15=00000000
# x16=00000000 x17=00000000 x18=00000000 x19=00000000 x20=00000000 x21=00000000 x22=00000000 x23=00000000
# x24=00000000 x25=00000000 x26=00000000 x27=00000000 x28=00000000 x29=00000000 x30=00000000 x31=00000000
# pc=00010004 dbus=00010137 ; 00010137 lui sp,65536
#
#  x0=00000000  x1=00000000  x2=00010000  x3=00000000  x4=00000000  x5=00000000  x6=00000000  x7=00000000
#  x8=00000000  x9=00000000 x10=00000000 x11=00000000 x12=00000000 x13=00000000 x14=00000000 x15=00000000
# x16=00000000 x17=00000000 x18=00000000 x19=00000000 x20=00000000 x21=00000000 x22=00000000 x23=00000000
# x24=00000000 x25=00000000 x26=00000000 x27=00000000 x28=00000000 x29=00000000 x30=00000000 x31=00000000
# pc=00010008 dbus=00010113 ; 00010113 mv sp,sp
#
#  x0=00000000  x1=00010010  x2=0000ffe0  x3=00000000  x4=00000000  x5=00000000  x6=00000000  x7=00000000
#  x8=00000000  x9=00000000 x10=00000000 x11=00000000 x12=00000000 x13=00000000 x14=00000000 x15=00000000
# x16=00000000 x17=00000000 x18=00000000 x19=00000000 x20=00000000 x21=00000000 x22=00000000 x23=00000000
# x24=00000000 x25=00000000 x26=00000000 x27=00000000 x28=00000000 x29=00000000 x30=00000000 x31=00000000
# pc=0001004c dbus=00112e23 ; 00112e23 sw ra,28(sp)
#
# WRITE: 0xfffc, value = 0x10010
#  x0=00000000  x1=00010010  x2=0000ffe0  x3=00000000  x4=00000000  x5=00000000  x6=00000000  x7=00000000
#  x8=00000000  x9=00000000 x10=00000000 x11=00000000 x12=00000000 x13=00000000 x14=00000000 x15=00000000
# x16=00000000 x17=00000000 x18=00000000 x19=00000000 x20=00000000 x21=00000000 x22=00000000 x23=00000000
# x24=00000000 x25=00000000 x26=00000000 x27=00000000 x28=00000000 x29=00000000 x30=00000000 x31=00000000
# pc=00010050 dbus=00812c23 ; 00812c23 sw s0,24(sp)
#
#
# Cleaning up.....
# done
# End time: 19:38:58 on Jan 18,2022, Elapsed time: 0:00:01
# Errors: 0, Warnings: 0
```
 
A second *.do* file is **run2.do** which runs the same main.bin file but disables the disassembler and memory accesses using the FLI command riscv_cmd:

```
riscv_cmd disable_reg_display
riscv_cmd disable_rw_display
```

The testbench has a simple debug monitor, any memory writes to address 0x00100000 are displayed in ASCII in the transcript window, this will allow simple strings to be displayed.

The main.c file uses this function:
```C
#define DEBUGPORT	0x00100000
void putc_debug(char ch)
{
	*((volatile uint32_t*)DEBUGPORT) = ch;
}
```
which is then picked up by the testbench which displays a LF terminated string 

```VHDL
if (wrn_s='0' AND abus_s=X"00100000") then  		-- Debug Port
	write(L,string'("PORT_MON : "));
	write(L,std_to_char(std_logic_vector(dbus_s(7 downto 0)))); 
```

```
E:\Modelsim-Unicorn\sim>vsim -c -do run2.do
Reading pref.tcl

# 2022.1

# do run2.do
# vsim -quiet tb
# //  ModelSim DE-64 2022.1 Jan 29 2022
# //
# //  Copyright 1991-2022 Mentor Graphics Corporation
# //  All Rights Reserved.
# //
# //  ModelSim DE-64 and its associated documentation contain trade
# //  secrets and commercial or financial information that are the property of
# //  Mentor Graphics Corporation and are privileged, confidential,
# //  and exempt from disclosure under the Freedom of Information Act,
# //  5 U.S.C. Section 552. Furthermore, this information
# //  is prohibited from disclosure under the Trade Secrets Act,
# //  18 U.S.C. Section 1905.
# //
# Generic: bin_filename = ..\test\main.bin
# Init Unicorn RISCV32 Emulator
# Reading rom file ..\test\main.bin, total 204 bytes loaded
#
# cif_init called
# PORT_MON : Hello World
# uc_emu_start() returned Invalid instruction (UC_ERR_INSN_INVALID)
# Cleaning up.....
# done
# End time: 19:43:14 on Jan 18,2022, Elapsed time: 0:00:03
# Errors: 0, Warnings: 0
``` 
 
 
## License

As per the Unicorn project this project is also licenses under GPL - see the LICENSE file for details
