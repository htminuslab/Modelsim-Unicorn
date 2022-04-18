//=================================================================================================
// FLI Unicorn Interface
//
// https://github.com/htminuslab  
//
// This library is free software; you can redistribute it and/or modify it under the terms of the GNU 
// Lesser General Public License as published by the Free Software Foundation; either version 2.1 of 
// the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   
// See the GNU Lesser General Public License for more details.   
// See http://www.gnu.org/copyleft/lesser.txt
//
//-------------------------------------------------------------------------------------------------
// Version   Author          Date          Changes
// 0.1       Hans Tiggeler   26 Oct 2019   
// 0.2       Hans Tiggeler   19 Mar 2022   Updated for Modelsim 2022.1
//==================================================================================================
#ifndef HHMAIN_H_
#define HHMAIN_H_ 

#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unicorn/unicorn.h>
#include "riscv-disas.h"

#include <mti.h>			  										// MTI Headers & Prototypes

#define START_ADDRESS 0
#define MAX_WORD      128											// used for filename etc

typedef enum {														// std_logic enumerated type
	STD_LOGIC_U,
	STD_LOGIC_X,
	STD_LOGIC_0,
	STD_LOGIC_1,
	STD_LOGIC_Z,
	STD_LOGIC_W,
	STD_LOGIC_L,
	STD_LOGIC_H,
	STD_LOGIC_D
} StdLogicType;

struct inst_rec_struct {			  								// VHDL RISCV Entity ports 
     mtiSignalIdT clk ;                        
     mtiSignalIdT sreset;  
	 
	 mtiDriverIdT rdn;
	 mtiDriverIdT wrn;
	 
	 mtiSignalIdT dbus;         									// Databus  
	 mtiDriverIdT * dbus_elems;
	 
	 mtiSignalIdT abus;         									// Addressbus  
	 mtiDriverIdT * abus_elems;
	 
	 mtiSignalIdT pc;         										// Program Counter  
	 mtiDriverIdT * pc_elems;

	 mtiSignalIdT xreg[32];    										// RISC-V Registers
	 mtiDriverIdT * xreg_elems[32];	 
	 
	 uc_engine *uc;													// Global unicorn engine pointer
};


int init_unicorn(uc_engine *uc, char * binfilename);
void riscv_cmd(void *param);

#endif 