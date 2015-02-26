#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include "debug_monitor_model.h"
#include "rf_model.h"
#include "mem_model.h"
#include "parser_model.h"


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

void command_prompt(int * fileState, int * executeState, unsigned short * dump_location, CPU_p cpu, Memory_p memory){
	int command;
	char fileName[40];
	printf("\nCommand: 1) load program, 2) run program, 3) step program, 4) dump memory, 5) exit");
        switch(*fileState){
            case FILE_LOAD_FAIL:
                printf("\nError loading file, please check file location.");
                *fileState = FILE_DO_NOT_DISPLAY;
                break;
            case FILE_LOAD_SUCCESS:
                printf("\nFile loaded successfully.");
                *fileState = FILE_DO_NOT_DISPLAY;
                break;
            default:
                //Don't do anything
                break;
        
        }
        printf("\n>");    
	scanf("%d",&command);
	switch (command)
	{
	case COMMAND_LOAD:
		printf("\nEnter the file name: ");
                stdinFlush();
		scanf("%s",&fileName);
		*fileState = parser(&fileName, memory, cpu);
		break;
	case COMMAND_RUN:
		*executeState = RUN_STATE;
		break;
	case COMMAND_STEP:
		*executeState = STEP_STATE;
		break;
	case COMMAND_DUMP:
		printf("\nEnter memory location: ");
                stdinFlush();
		scanf("%x", dump_location);
 
		break;
	case COMMAND_EXIT:
		*executeState = EXIT_STATE;
	default:
		break;
	}
}

//This method is attempting to clear out stdin, not quite working as intended.
//Blocks program if there is no \n in the buffer.
void stdinFlush() {
    char c;
    do{
        c = fgetc(stdin);
    }while(c != '\n' && c != EOF);
}