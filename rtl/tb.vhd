---------------------------------------------------------------------------------------------------
--  FLI demo calling Unicorn QEMU from VHDL                                                                           
--                                                                            
--  https://github.com/htminuslab                                                                         
---------------------------------------------------------------------------------------------------
--  License : see github project page                                              
---------------------------------------------------------------------------------------------------
--
--  Testbench
-- 
--  Revision History:                                                        
--                                                                           
--  Date:          Revision         Author         
--  26-Oct-2019    0.1              Hans Tiggeler 
--  18-Apr-2022    0.1a             Hans Tiggeler  Updated/checked with Modelsim 2022.1
---------------------------------------------------------------------------------------------------
entity tb is
	generic(bin_filename : string(1 to 16) := "..\test\main.bin");
end tb;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

library std;
use std.textio.all;


architecture struct of tb is

   signal sreset_s  : std_logic;
   signal clk_s  	: std_logic:='0';
   signal pc_s     	: std_logic_vector(31 downto 0);
   signal rdn_s     : std_logic;
   signal wrn_s     : std_logic;
   signal dbus_s    : std_logic_vector(31 downto 0);
   signal abus_s    : std_logic_vector(31 downto 0);
   
 --signal reg_x0_s  : std_logic_vector(31 downto 0);
   signal reg_x1_s  : std_logic_vector(31 downto 0);
   signal reg_x2_s  : std_logic_vector(31 downto 0);
   signal reg_x3_s  : std_logic_vector(31 downto 0);
   signal reg_x4_s  : std_logic_vector(31 downto 0);
   signal reg_x5_s  : std_logic_vector(31 downto 0);
   signal reg_x6_s  : std_logic_vector(31 downto 0);
   signal reg_x7_s  : std_logic_vector(31 downto 0);
   signal reg_x8_s  : std_logic_vector(31 downto 0);
   signal reg_x9_s  : std_logic_vector(31 downto 0);
   signal reg_x10_s : std_logic_vector(31 downto 0);
   signal reg_x11_s : std_logic_vector(31 downto 0);
   signal reg_x12_s : std_logic_vector(31 downto 0);
   signal reg_x13_s : std_logic_vector(31 downto 0);
   signal reg_x14_s : std_logic_vector(31 downto 0);
   signal reg_x15_s : std_logic_vector(31 downto 0);
   signal reg_x16_s : std_logic_vector(31 downto 0);
   signal reg_x17_s : std_logic_vector(31 downto 0);
   signal reg_x18_s : std_logic_vector(31 downto 0);
   signal reg_x19_s : std_logic_vector(31 downto 0);
   signal reg_x20_s : std_logic_vector(31 downto 0);
   signal reg_x21_s : std_logic_vector(31 downto 0);
   signal reg_x22_s : std_logic_vector(31 downto 0);
   signal reg_x23_s : std_logic_vector(31 downto 0);
   signal reg_x24_s : std_logic_vector(31 downto 0);
   signal reg_x25_s : std_logic_vector(31 downto 0);
   signal reg_x26_s : std_logic_vector(31 downto 0);
   signal reg_x27_s : std_logic_vector(31 downto 0);
   signal reg_x28_s : std_logic_vector(31 downto 0);
   signal reg_x29_s : std_logic_vector(31 downto 0);
   signal reg_x30_s : std_logic_vector(31 downto 0);
   signal reg_x31_s : std_logic_vector(31 downto 0); 	   
   

   signal udbus_s   : std_logic_vector(7 downto 0); -- debug port

   component riscv
   generic(
	  bin_filename : string(1 to 16) := "..\test\main.bin");
   port (
      clk     : in  std_logic;
      sreset  : in  std_logic;
	  rdn     : out std_logic;
	  wrn     : out std_logic;
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
      pc	  : out	std_logic_vector(31 downto 0));
   end component;


BEGIN

	clk_s <= NOT clk_s after 10 ns;

	dut : riscv
	    generic map (
			bin_filename  => bin_filename
		)
		port map (
			clk     => clk_s,
			sreset  => sreset_s,									-- Sync reset
			rdn     => rdn_s,
			wrn     => wrn_s,
			dbus    => dbus_s,
			abus    => abus_s,
			reg_x1 	=> reg_x1_s, 	
			reg_x2 	=> reg_x2_s, 	
			reg_x3 	=> reg_x3_s, 	
			reg_x4 	=> reg_x4_s, 	
			reg_x5 	=> reg_x5_s, 	
			reg_x6 	=> reg_x6_s, 	
			reg_x7 	=> reg_x7_s, 	
			reg_x8 	=> reg_x8_s, 	
			reg_x9 	=> reg_x9_s, 	
			reg_x10	=> reg_x10_s,	
			reg_x11	=> reg_x11_s,	
			reg_x12	=> reg_x12_s,	
			reg_x13	=> reg_x13_s,	
			reg_x14	=> reg_x14_s,	
			reg_x15	=> reg_x15_s,	
			reg_x16	=> reg_x16_s,	
			reg_x17	=> reg_x17_s,	
			reg_x18	=> reg_x18_s,	
			reg_x19	=> reg_x19_s,	
			reg_x20	=> reg_x20_s,	
			reg_x21	=> reg_x21_s,	
			reg_x22	=> reg_x22_s,	
			reg_x23	=> reg_x23_s,	
			reg_x24	=> reg_x24_s,	
			reg_x25	=> reg_x25_s,	
			reg_x26	=> reg_x26_s,	
			reg_x27	=> reg_x27_s,	
			reg_x28	=> reg_x28_s,	
			reg_x29	=> reg_x29_s,	
			reg_x30	=> reg_x30_s,	
			reg_x31 => reg_x31_s,
			pc      => pc_s);
		
	process
		begin
			sreset_s <= '1';
			wait for 100 ns;
			sreset_s <= '0';
			wait;	
	end process;
		
		
	-- Display char received from debug port  
	process (clk_s,sreset_s) 									
		variable L   : line;
		variable i_v : integer;
		constant MAXCHAR_C : integer := 80;
		
		function std_to_char(inp: std_logic_vector) return character is
		   constant ASCII_TABLE : string (1 to 256) :=
			".......... .. .................. !" & '"' &
			"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI"  &
			"JKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnop"  &
			"qrstuvwxyz{|}~........................."  &
			"......................................."  &
			"......................................."  &
			"..........................";

		   variable temp : integer;
		begin  
			temp:=CONV_INTEGER(inp)+1;
			return ASCII_TABLE(temp);
		end std_to_char;
		
		
	begin
		if (falling_edge(clk_s)) then
			if sreset_s='1' then
				i_v := 0;                              				-- clear character counter
				udbus_s <= X"20";									-- Space
			else        
				if (wrn_s='0' AND abus_s=X"00100000") then  		-- #define DEBUGPORT
					if i_v=0 then 
						write(L,string'("PORT_MON : "));
						if (dbus_s(7 downto 0)/=X"0D" and dbus_s(7 downto 0)/=X"0A" and dbus_s(7 downto 0)/=X"00") then 
							write(L,std_to_char(std_logic_vector(dbus_s(7 downto 0)))); 
							udbus_s <= dbus_s(7 downto 0); 
						end if;         
						i_v := i_v+1;
					elsif (i_v=MAXCHAR_C or dbus_s(7 downto 0)=X"0D" or dbus_s(7 downto 0)=X"00") then                
						writeline(output,L);
						i_v:=0;
					else 
						if (dbus_s(7 downto 0)/=X"0D" and dbus_s(7 downto 0)/=X"0A" and dbus_s(7 downto 0)/=X"00") then 
							write(L,std_to_char(std_logic_vector(dbus_s(7 downto 0))));
							udbus_s <= dbus_s(7 downto 0); 
						end if;         
						i_v := i_v+1;
					end if;
				end if;
			end if;
		end if;   
    end process;		
		

end struct;
