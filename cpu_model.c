#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "cpu_model.h"

CPU_p createCPU(void) {
	CPU_p cpu = (CPU_p)malloc(sizeof(CPU));
	return cpu;
}

void setSWRegFlags(CPU_p cpu) {
	Register aluFlags = (unsigned)cpu->alu.flags;
	cpu->sw &= 0x0FFF;	//clear the old flag values.
	cpu->sw |= aluFlags << 12;	//Load the new flag values.
}