#include "mem_model.h"
#include "cpu_model.h"

#ifndef _JUMPS_MODEL_H
#define _JUMPS_MODEL_H

#define STACK_POINTER 7
#define BR 0x0009
#define BRN 0x000A
#define BRZ 0x000B
#define BRC 0x000C
#define BRO 0x000D
#define JMP 0x000E
#define JSR 0x000F
#define RET 0x001B

//Prototypes for the jump model
void jmp(int rx_location, CPU_p cpu, Memory_p memory);
void jsr(int rx_location, CPU_p cpu, Memory_p memory);
void ret(CPU_p cpu, Memory_p memory);
void br(int pc_location, CPU_p cpu);
void brN(int pc_location, CPU_p cpu);
void brZ(int pc_location, CPU_p cpu);
void brC(int pc_location, CPU_p cpu);
void brO(int pc_location, CPU_p cpu);

void trapPush(CPU_p cpu, Memory_p memory);
void trapPop(CPU_p cpu, Memory_p memory);

//Helper prototypes for the jump model.
void stpc(int ra_location, CPU_p cpu, Memory_p memory);
void ldpc(int ra_location, CPU_p cpu, Memory_p memory);


#endif
