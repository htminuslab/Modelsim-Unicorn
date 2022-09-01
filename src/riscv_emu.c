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
#include <fli_riscv.h>


// RISC-V Register file
uint32_t pc=START_ADDRESS;
uint32_t xreg[32];														// 32 RISCV registers
uint32_t dbus=0;
uint32_t abus=0;
bool     wrn=true;
bool     rdn=true;
bool	 enable_display_regs=true;
bool     enable_display_rw=true;
uint32_t break_address=-1;


//-------------------------------------------------------------------------------------------------
// riscv_cmd disable_reg_display/enable_reg_display
// riscv_cmd disable_rw_display/enable_rw_display
// riscv_cmd break_address <address>
//-------------------------------------------------------------------------------------------------
void riscv_cmd(void *param)
{
	char *cp;
	char *command = param;
	
	for (cp = command; (*cp != ' ') && (*cp != '\0'); cp++ ) { ; }
	for (; (*cp == ' ') && (*cp != '\0'); cp++ ) { ; }

	if (strcmpi(cp,"enable_reg_display")==0) {
		enable_display_regs=true;
	} else if (strcmpi(cp,"disable_reg_display")==0) {
		enable_display_regs=false;
	} else if (strcmpi(cp,"enable_rw_display")==0) {
		enable_display_rw=true;
	} else if (strcmpi(cp,"disable_rw_display")==0) {
		enable_display_rw=false;
	} else {
		mti_PrintFormatted("Usage: riscv_cmd disable_reg_display/enable_reg_display/disable_rw_display/enable_rw_display\n");	
	}

}


void display_regs(uint32_t dbus)
{
	char buf[128] = { 0 };
	mti_PrintFormatted(" x0=00000000  x1=%08x  x2=%08x  x3=%08x  x4=%08x  x5=%08x  x6=%08x  x7=%08x\n",xreg[1],xreg[2],xreg[3],xreg[4],xreg[5],xreg[6],xreg[7]);
	mti_PrintFormatted(" x8=%08x  x9=%08x x10=%08x x11=%08x x12=%08x x13=%08x x14=%08x x15=%08x\n",xreg[8],xreg[9],xreg[10],xreg[11],xreg[12],xreg[13],xreg[14],xreg[15]);
	mti_PrintFormatted("x16=%08x x17=%08x x18=%08x x19=%08x x20=%08x x21=%08x x22=%08x x23=%08x\n",xreg[16],xreg[17],xreg[18],xreg[19],xreg[20],xreg[21],xreg[22],xreg[23]);
	mti_PrintFormatted("x24=%08x x25=%08x x26=%08x x27=%08x x28=%08x x29=%08x x30=%08x x31=%08x\n",xreg[24],xreg[25],xreg[26],xreg[27],xreg[28],xreg[29],xreg[30],xreg[31]);	

	mti_PrintFormatted("pc=%08x dbus=%08x",pc,dbus);
	
	disasm_inst(buf, sizeof(buf), rv32, 0, dbus);
    mti_PrintFormatted(" ; %s\n\n", buf);
	
}

static void hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{		
		
  //uc_reg_read(uc, UC_RISCV_REG_ZERO, &xreg[0]);  
	uc_reg_read(uc, UC_RISCV_REG_RA,   &xreg[1]);
	uc_reg_read(uc, UC_RISCV_REG_SP,   &xreg[2]);
	uc_reg_read(uc, UC_RISCV_REG_GP,   &xreg[3]);
	uc_reg_read(uc, UC_RISCV_REG_TP,   &xreg[4]);
	uc_reg_read(uc, UC_RISCV_REG_T0,   &xreg[5]);
	uc_reg_read(uc, UC_RISCV_REG_T1,   &xreg[6]);
	uc_reg_read(uc, UC_RISCV_REG_T2,   &xreg[7]);
	uc_reg_read(uc, UC_RISCV_REG_S0,   &xreg[8]);
	uc_reg_read(uc, UC_RISCV_REG_S1,   &xreg[9]);
	uc_reg_read(uc, UC_RISCV_REG_A0,   &xreg[10]);
	uc_reg_read(uc, UC_RISCV_REG_A1,   &xreg[11]);
	uc_reg_read(uc, UC_RISCV_REG_A2,   &xreg[12]);
	uc_reg_read(uc, UC_RISCV_REG_A3,   &xreg[13]);
	uc_reg_read(uc, UC_RISCV_REG_A4,   &xreg[14]);
	uc_reg_read(uc, UC_RISCV_REG_A5,   &xreg[15]);
	uc_reg_read(uc, UC_RISCV_REG_A6,   &xreg[16]);
	uc_reg_read(uc, UC_RISCV_REG_A7,   &xreg[17]);
	uc_reg_read(uc, UC_RISCV_REG_S2,   &xreg[18]);
	uc_reg_read(uc, UC_RISCV_REG_S3,   &xreg[19]);
	uc_reg_read(uc, UC_RISCV_REG_S4,   &xreg[20]);
	uc_reg_read(uc, UC_RISCV_REG_S5,   &xreg[21]);
	uc_reg_read(uc, UC_RISCV_REG_S6,   &xreg[22]);
	uc_reg_read(uc, UC_RISCV_REG_S7,   &xreg[23]);
	uc_reg_read(uc, UC_RISCV_REG_S8,   &xreg[24]);
	uc_reg_read(uc, UC_RISCV_REG_S9,   &xreg[25]);
	uc_reg_read(uc, UC_RISCV_REG_S10,  &xreg[26]);
	uc_reg_read(uc, UC_RISCV_REG_S11,  &xreg[27]);
	uc_reg_read(uc, UC_RISCV_REG_T3,   &xreg[28]);
	uc_reg_read(uc, UC_RISCV_REG_T4,   &xreg[29]);
	uc_reg_read(uc, UC_RISCV_REG_T5,   &xreg[30]);
	uc_reg_read(uc, UC_RISCV_REG_T6,   &xreg[31]);
				
	uc_reg_read(uc, UC_RISCV_REG_PC,   &pc);
	
	uc_mem_read(uc, address, &dbus, sizeof(dbus));	
	if (enable_display_regs) display_regs(dbus);	
}


static void hook_mem(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user_data)
{	
	switch(type) {
		case UC_MEM_READ:
			uc_mem_read(uc, address, &value, size);
			if (enable_display_rw) mti_PrintFormatted("READ: 0x%"PRIx64 ", size = %u, value = 0x%"PRIx64 "\n",address, size, value);
			rdn=false;
			wrn=true;			
			break;
		case UC_MEM_WRITE:
			if (enable_display_rw) mti_PrintFormatted("WRITE: 0x%"PRIx64 ", value = 0x%"PRIx64 "\n",	address, value);			
			rdn=true;
			wrn=false;
			break;
		default:			
			//mti_PrintFormatted("\n*** Unknown type %x in hook_mem***\n",type);
			break;
	}
	
	dbus=(uint32_t)value&0xFFFFFFFF;
	abus=address;	
}

static void hook_intr(uc_engine *uc, uint32_t intno, void *user_data)
{
	mti_PrintFormatted("\n\n*** hook_intr called intno=%x****\n\n",intno);
	mti_Quit();
}

int init_unicorn(uc_engine *uc, char * binfilename)
{    
	FILE *fp=NULL;         

	uint32_t addr=START_ADDRESS;
	uint8_t byte;
		
    static uc_hook trace2, trace3;	
	unsigned int major,minor;
	
	uc_version(&major,&minor);									// Get Unicorn Version
    mti_PrintFormatted("Init Unicorn RISCV32 Emulator version %d.%d\n",major,minor);

	
	uc_mem_map(uc, 0, 2 * 1024 * 1024, UC_PROT_ALL);			// map 2MB memory for this emulation

	//---------------------------------------------------------------------------------------------
	// Open bin file and load into unicorn memory, also write to MTI memory? 
	//---------------------------------------------------------------------------------------------
    if ((fp=fopen(binfilename,"rt"))!=NULL) {
		mti_PrintFormatted("Reading rom file %s, ",binfilename);// Start IP address
		while (fread(&byte,sizeof(byte),1,fp)) {
			uc_mem_write(uc, addr++, &byte, sizeof(byte));
		}	 	
		mti_PrintFormatted("total %d bytes loaded\n",addr-START_ADDRESS);
		fclose(fp);
	} else {
        mti_PrintFormatted("\nFailed to open rom init file %s\n",binfilename);
        return 1;
    }	

    // handle interrupt ourself
    uc_hook_add(uc, &trace2, UC_HOOK_INTR, hook_intr, NULL, 1, 0);

    // tracing all instruction
    uc_hook_add(uc, &trace2, UC_HOOK_CODE, hook_code, NULL, 1, 0);
	
	// tracing all read/writes
	uc_hook_add(uc, &trace3, UC_HOOK_MEM_WRITE|UC_HOOK_MEM_READ, hook_mem, NULL, 1, 0); 

	return 0;
}

