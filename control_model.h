#ifndef _CONTROL_MODEL_H
#define	_CONTROL_MODEL_H

#include "cpu_model.h"
#include "mem_model.h"

#define HALT 32
#define GETS 22
#define CARRAGE_RETURN '\n'

//Prototypes for control.h.
int getOperation(Register ir);
int decodeImmed8(Register ir);
int decodeRd(Register ir);
int decodeRa(Register ir);
int decodeRxAndImmd3(Register ir);
int decodeImmed5(Register ir);
int decodeArgument1(Register ir);
int decodeArgument2(Register ir);
int decodeImmed10(Register ir);
void fetch(CPU_p cpu, Memory_p);
void decode(int * opcode_ptr, int * rd_loc_ptr, int * ra_loc_ptr, int * rx_loc_ptr, int * args_ptr, int * immediate_ptr, CPU_p cpu);
void execute(int opcode,int rd_loc, int ra_loc, int rx_loc, int args, int immediate, int * halt_ptr, Io_monitor_p keyboard_io, Io_monitor_p monitor_io, CPU_p cpu, Memory_p memory);

//helper meathods
int immed8_sign_extend(int immed8);
#endif	/* _CONTROL_MODEL_H */
