#include "alu_model.h"
#include "rf_model.h"

#ifndef _CPU_MODEL_H
#define _CPU_MODEL_H

typedef struct cpu {
	//Register A, B, R;
	//unsigned char flags;	// n, z, c, o - low order nybble
	ALU alu;
	RegisterFile rf;
	unsigned short sw;
	unsigned short ir;
	unsigned short pc;
	ByteRegister mdr;
	Register mar;
} CPU;

typedef CPU * CPU_p;

CPU_p createCPU(void);
void setSWRegFlags(CPU_p cpu);

#endif