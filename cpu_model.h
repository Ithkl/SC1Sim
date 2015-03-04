#include "alu_model.h"
#include "rf_model.h"

#ifndef _CPU_MODEL_H
#define _CPU_MODEL_H

//The CPU struct, contains the registerfile, the system registers, and the alu.
typedef struct cpu {
	//Register A, B, R;
	//unsigned char flags;	// n, z, c, o - low order nybble
	ALU alu;
	RegisterFile rf;
	Register sw;
	Register ir;
	Register pc;
	ByteRegister mdr;
	Register mar;
} CPU;

//Renaming cpu pointer to something nicer.
typedef CPU * CPU_p;

//CPU functions
CPU_p createCPU(void);
void setSWRegFlags(CPU_p cpu);

#endif
