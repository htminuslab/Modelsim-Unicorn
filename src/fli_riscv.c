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

struct inst_rec_struct inst_rec;

extern uint32_t dbus;
extern uint32_t abus;
extern uint32_t pc;
extern uint32_t xreg[];

extern bool     wrn;
extern bool     rdn;

// Prototypes
static void riscv_process(void *param);  		
mtiUInt32T conv_std_logic_vector(mtiSignalIdT stdvec);
void drive_lv_uint(mtiSignalIdT bus, mtiDriverIdT * drivers, uint32_t newval, mtiDelayT delay);	

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void call_unicorn_cleanup(void *ip)
{
	uc_err err;
	mti_PrintMessage("Cleaning up.....");

	if ((err = uc_close(ip))) {
		mti_PrintFormatted("Failed on uc_close() with error returned: %s\n", uc_strerror(err));
	}
	mti_PrintMessage("done\n");
}

char * get_generic_filename(mtiInterfaceListT * generic_list, int free_it)
{
	static char filename[MAX_WORD]="";
	int i=0;
	
	mtiInterfaceListT * glp;
	mtiInterfaceListT * glp_next;
	for ( glp = generic_list; glp; glp = glp_next ) {
		mti_PrintFormatted("Generic: %s = ", glp->name);
		if  (mti_GetTypeKind(glp->type)==MTI_TYPE_ARRAY){
			mtiInt32T  num_elems = mti_TickLength( glp->type );
			mtiTypeIdT elem_type = mti_GetArrayElementType( glp->type );
			if  (mti_GetTypeKind( elem_type )==MTI_TYPE_ENUM) {
				char *  array_val = glp->u.generic_array_value;
				for (i=0; i < num_elems; i++ ) filename[i]=array_val[i];
				filename[i]='\0';
				mti_PrintFormatted("%s\n",filename);
			}
		} else {
			mti_PrintFormatted("\n*** Invalid generic, expecting filename ***\n");
			mti_FatalError(); 
		}			

		glp_next = glp->nxt;
		if (free_it) {
			mti_Free(glp);
		}
	}
	return filename;
}


void cif_init(mtiRegionIdT region, char *param, mtiInterfaceListT *generics, mtiInterfaceListT *ports)
{
    struct inst_rec_struct *ip;										// Declare ports			
    mtiProcessIdT proc;												// current process id
	uc_err err;
	char regname[8]="";												// reg_x0..reg_x31
	mtiDriverIdT drvid;
	
	char *binfilename=NULL; 										// Binary image loaded into memory before simulation

	binfilename=get_generic_filename(generics,0);					// binary filename specified as a generic

    ip = (struct inst_rec_struct *)mti_Malloc(sizeof(struct inst_rec_struct));	// allocate memory for ports
    mti_AddRestartCB(mti_Free, ip);									// restart entry point
	mti_AddQuitCB(mti_Free, ip);

	ip->clk 	 = mti_FindPort(ports, "clk");  					// Get entity ports
	ip->sreset	 = mti_FindPort(ports, "sreset");   
	
	ip->rdn      = mti_CreateDriver(mti_FindPort(ports, "rdn")); 	
	ip->wrn      = mti_CreateDriver(mti_FindPort(ports, "wrn")); 	// Add drivers
	
	ip->dbus 	 = mti_FindPort(ports, "dbus");	
	drvid 		 = mti_CreateDriver(ip->dbus);						
	ip->dbus_elems = mti_GetDriverSubelements(drvid, 0);
	
	ip->abus 	 = mti_FindPort(ports, "abus");	
	drvid 		 = mti_CreateDriver(ip->abus);						
	ip->abus_elems = mti_GetDriverSubelements(drvid, 0);	
	
	ip->pc 		 = mti_FindPort(ports, "pc");	
	drvid 		 = mti_CreateDriver(ip->pc);						// Add drivers
	ip->pc_elems = mti_GetDriverSubelements(drvid, 0);

										
	for (int i=1;i<32;i++) {										// Add RISC-V Register drivers
		sprintf(regname,"reg_x%d",i);
		ip->xreg[i]       = mti_FindPort(ports, regname);
		drvid 		      = mti_CreateDriver(ip->xreg[i] );
		ip->xreg_elems[i] = mti_GetDriverSubelements(drvid, 0);
	}
	

    proc = mti_CreateProcess("riscv_process", riscv_process, ip);
    mti_Sensitize(proc, ip->clk, MTI_EVENT);	 					// Add sensitivity signals
    mti_Sensitize(proc, ip->sreset, MTI_EVENT);
	mti_SetDriverOwner(drvid,proc);			 						// Set Owner of output drivers TODO check as this is wrong
	mti_SetDriverOwner(ip->rdn,proc);			 
	mti_SetDriverOwner(ip->wrn,proc);	

    if ((err = uc_open(UC_ARCH_RISCV, UC_MODE_RISCV32, &(ip->uc)))) {// Initialize emulator in RISCV32 mode
        mti_PrintFormatted("Failed on uc_open() with error returned: %u (%s)\n", err,uc_strerror(err));
        mti_FatalError(); 
    }
	init_unicorn(ip->uc,binfilename);
		
	mti_AddCommand("riscv_cmd",riscv_cmd);							// Add command to disable regs and mem r/w display

	mti_AddQuitCB(call_unicorn_cleanup, ip->uc);
	mti_AddRestartCB(call_unicorn_cleanup, ip->uc);
	
	mti_PrintMessage("\ncif_init called\n");
}

static void riscv_process(void *param)								
{
	struct inst_rec_struct *ip = (struct inst_rec_struct *)param;
	uc_err err;

	if (mti_GetSignalValue(ip->clk)==STD_LOGIC_1) {
		if (mti_GetSignalValue(ip->sreset)==STD_LOGIC_1) { 			// Check sync reset negated
			pc=START_ADDRESS;
			wrn=true; 												// negate (active low)
			rdn=true;			
		} else {
			
			if ((err=uc_emu_start(ip->uc, pc, pc+4, 0, 1))) {
				mti_PrintFormatted("uc_emu_start() returned %s\n", uc_strerror(err));
				mti_Quit();//FatalError(); 
			} 
			
			uc_reg_read(ip->uc, UC_RISCV_REG_PC,   &pc);			// Update PC
			mti_ScheduleDriver(ip->rdn, rdn ? STD_LOGIC_1:STD_LOGIC_0, 0, MTI_INERTIAL);
			mti_ScheduleDriver(ip->wrn, wrn ? STD_LOGIC_1:STD_LOGIC_0, 0, MTI_INERTIAL);

		}	  		
	}


	drive_lv_uint(ip->dbus, ip->dbus_elems, dbus, 0);
	drive_lv_uint(ip->abus, ip->abus_elems, abus, 0);
	drive_lv_uint(ip->pc, ip->pc_elems, pc, 0);

	for (int i=1;i<32;i++) {
		drive_lv_uint(ip->xreg[i], ip->xreg_elems[i], xreg[i], 0);
	}

	wrn=true; // negate (active low)
	rdn=true;

}


void drive_lv_uint(mtiSignalIdT bus, mtiDriverIdT * drivers, uint32_t newval, mtiDelayT delay)
{
	char 	* sigval;
	mtiInt32T arraylen;
	
	arraylen=mti_TickLength(mti_GetSignalType(bus));
	sigval = (char *)mti_GetArraySignalValue(bus, 0);
	
	if (arraylen>32) {
		mti_PrintFormatted("*** ERROR: Vector length %d > 32bits\n",arraylen);
		mti_FatalError(); 											// Do not continue
	} else {
		for (int i=arraylen-1;i>=0;i--) { 
			sigval[i]= (newval&1) ? STD_LOGIC_1 : STD_LOGIC_0;
		 	mti_ScheduleDriver(drivers[i], (long)sigval[i], delay, MTI_INERTIAL );
			newval>>=1;
		}
	}	
}
