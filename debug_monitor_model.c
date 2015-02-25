
#include <stdlib.h>
#include <stdio.h>
#include "debug_monitor_model.h"
#include "rf_model.h"
#include "mem_model.h"


void display(CPU_p cpu, Memory_p memory, unsigned short dump_location) {
    int i = 0;
    system("cls");
    printf("\nRegisters                                  Memory");
    //Printing out the register file and 14 memory addresses.
    for (i;i < 8; i++)
    {
        printf("\nR%d: [%0.4X]                               0x%0.4X: [%0.2X] [%0.2X]", i,getRegisterValue(&(cpu->rf), i), dump_location + 2*i, getMemoryValue(memory, dump_location + 2*i), getMemoryValue(memory, dump_location + 2*i + 1));
    }
    printf("\nSW: [%0.4X]         IR: [%0.4X]        PC: [%0.4X]", cpu->sw, cpu->ir, cpu->pc);  
}
