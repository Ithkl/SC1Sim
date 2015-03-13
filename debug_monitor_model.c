#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include "debug_monitor_model.h"
#include "rf_model.h"
#include "mem_model.h"
#include "parser_model.h"

//Display part of the GUI.  Displays SW, IR, PC, and the working registers.
//Also 14 bytes of memory around the dump location.
void display(CPU_p cpu, Memory_p memory, unsigned short dump_location) {
    int i = 0;
    //system("clear");
    printf("\nRegisters                                  Memory");
    //Printing out the register file and 14 memory addresses.
    for (i;i < 8; i++)
    {
        printf("\nR%d: [%0.4X]                               0x%0.4X: [%0.2X] [%0.2X]", i,getRegisterValue(&(cpu->rf), i), dump_location + 2*i, getMemoryValue(memory, dump_location + 2*i), getMemoryValue(memory, dump_location + 2*i + 1));
    }
    printf("\nSW: [%0.4X]         IR: [%0.4X]        PC: [%0.4X]", cpu->sw, cpu->ir, cpu->pc);  
}

//Prompts the user for the command that they want executed.
void command_prompt(int * fileState_ptr, int * executeState_ptr, unsigned short * dump_location_ptr, CPU_p cpu, Memory_p memory){
	int command;
	char fileName[40];
	printf("\nCommand: 1) load program, 2) run program, 3) step program, 4) dump memory\n5) exit");
        switch(*fileState_ptr){
            //Displays an error if the file was invalid.
            case FILE_LOAD_FAIL:
                printf("\nError loading file, please check file location.");
                *fileState_ptr = FILE_DO_NOT_DISPLAY;
                break;
            //Displays a success message if the file was valid.
            case FILE_LOAD_SUCCESS:
                printf("\nFile loaded successfully.");
                *fileState_ptr = FILE_DO_NOT_DISPLAY;
                break;
            default:
                //Don't do anything
                break;
        
        }
        printf("\n>");  
        fpurge(stdin);
	scanf("%d",&command);
        getchar();  //Eat the \n left by the scanf
	switch (command)
	{
        //Loads the file instructions that the user gives.
	case COMMAND_LOAD:
		printf("\nEnter the file name: ");
		scanf("%s",&fileName);
                getchar();  //Eat the \n left by the scanf
		*fileState_ptr = parser(&fileName, memory, cpu);
		break;
        //Runs the file commands that the user input.
	case COMMAND_RUN:
		*executeState_ptr = RUN_STATE;
		break;
        //Steps through the file commands that the user input.
	case COMMAND_STEP:
		*executeState_ptr = STEP_STATE;
		break;
        //Goes to a memory location based on the value the user input.
	case COMMAND_DUMP:
		printf("\nEnter memory location: ");
		scanf("%x", dump_location_ptr);
                getchar();  //Eat the \n left by the scanf
		break;
        //Exits the prompt if the user requests to.
	case COMMAND_EXIT:
		*executeState_ptr = EXIT_STATE;
                break;
	default:
		break;
	}
}
