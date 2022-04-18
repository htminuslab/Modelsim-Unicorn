---------------------------------------------------------------------------------------------------
--  FLI demo calling Unicorn QEMU from VHDL                                                                           
--                                                                            
--  https://github.com/htminuslab                                                                         
---------------------------------------------------------------------------------------------------
--  License : see github project page                                              
---------------------------------------------------------------------------------------------------
--  
--  Top level RISC-V Instruction Set Simulator
--
--  Revision History:                                                        
--                                                                           
--  Date:          Revision         Author         
--  26-Oct-2019    0.1              Hans Tiggeler 
--  18-Apr-2022    0.1a             Hans Tiggeler  Updated/checked with Modelsim 2022.1
---------------------------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity riscv is
    generic(bin_filename : string(1 to 16) := "..\test\main.bin");
	port( 
		clk     : in    std_logic;
		sreset  : in    std_logic;									-- Sync reset
		rdn     : out   std_logic;      		 					-- Active low Read strobe
		wrn     : out   std_logic;       							-- Active low Write strobe
		dbus    : out	std_logic_vector(31 downto 0);
		abus    : out	std_logic_vector(31 downto 0);
      --reg_x0  : out	std_logic_vector(31 downto 0);
        reg_x1  : out	std_logic_vector(31 downto 0);
        reg_x2  : out	std_logic_vector(31 downto 0);
        reg_x3  : out	std_logic_vector(31 downto 0);
        reg_x4  : out	std_logic_vector(31 downto 0);
        reg_x5  : out	std_logic_vector(31 downto 0);
        reg_x6  : out	std_logic_vector(31 downto 0);
        reg_x7  : out	std_logic_vector(31 downto 0);
        reg_x8  : out	std_logic_vector(31 downto 0);
        reg_x9  : out	std_logic_vector(31 downto 0);
        reg_x10 : out	std_logic_vector(31 downto 0);
        reg_x11 : out	std_logic_vector(31 downto 0);
        reg_x12 : out	std_logic_vector(31 downto 0);
        reg_x13 : out	std_logic_vector(31 downto 0);
        reg_x14 : out	std_logic_vector(31 downto 0);
        reg_x15 : out	std_logic_vector(31 downto 0);
        reg_x16 : out	std_logic_vector(31 downto 0);
        reg_x17 : out	std_logic_vector(31 downto 0);
        reg_x18 : out	std_logic_vector(31 downto 0);
        reg_x19 : out	std_logic_vector(31 downto 0);
        reg_x20 : out	std_logic_vector(31 downto 0);
        reg_x21 : out	std_logic_vector(31 downto 0);
        reg_x22 : out	std_logic_vector(31 downto 0);
        reg_x23 : out	std_logic_vector(31 downto 0);
        reg_x24 : out	std_logic_vector(31 downto 0);
        reg_x25 : out	std_logic_vector(31 downto 0);
        reg_x26 : out	std_logic_vector(31 downto 0);
        reg_x27 : out	std_logic_vector(31 downto 0);
        reg_x28 : out	std_logic_vector(31 downto 0);
        reg_x29 : out	std_logic_vector(31 downto 0);
        reg_x30 : out	std_logic_vector(31 downto 0);
        reg_x31 : out	std_logic_vector(31 downto 0); 	  
        pc	    : out	std_logic_vector(31 downto 0));				-- Program Counter
end riscv;

ARCHITECTURE rtl OF riscv IS

attribute foreign : string;
attribute foreign of rtl: architecture is "cif_init ./fli_riscv.dll";

BEGIN	
 	assert FALSE report "*** FLI Failure ***" severity failure;	-- never called
END rtl;

