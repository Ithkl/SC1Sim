#include "mem_model.h"
#include "cpu_model.h"

#define STACK_POINTER 7

void jmp(int rx_location, CPU_p cpu, Memory_p memory);
void jsr(int rx_location, CPU_p cpu, Memory_p memory);
void ret(CPU_p cpu, Memory_p memory);
void push(int rd_location, int ra_location, CPU_p cpu, Memory_p memory);
void pop(int rd_location, int ra_location, CPU_p cpu, Memory_p memory);

void stpc(int ra_location, CPU_p cpu, Memory_p memory);
void ldpc(int ra_location, CPU_p cpu, Memory_p memory);