#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "cpu_model.h"

//Creates a CPU object.
CPU_p createCPU(void) {
	CPU_p cpu = (CPU_p)malloc(sizeof(CPU));
	return cpu;
}

//Performs the initial setting of the register flags.
void setSWRegFlags(CPU_p cpu) {
	Register aluFlags = (unsigned)cpu->alu.flags;
	cpu->sw &= 0x0FFF;	//clear the old flag values.
	cpu->sw |= aluFlags << 12;	//Load the new flag values.
}
