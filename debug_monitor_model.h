/* 
 * File:   debug_monitor_h.h
 *
 * Created on February 24, 2015, 5:32 PM
 */

#ifndef _DEBUG_MONITOR_MODEL_H
#define	_DEBUG_MONITOR_MODEL_H

#include "cpu_model.h"
#include "mem_model.h"

#define MENU_STATE 0
#define RUN_STATE 1
#define STEP_STATE 2
#define EXIT_STATE 3
#define FILE_DO_NOT_DISPLAY -1
#define COMMAND_LOAD 1
#define COMMAND_RUN 2
#define COMMAND_STEP 3
#define COMMAND_DUMP 4
#define COMMAND_EXIT 5

void display(CPU_p cpu, Memory_p memory, unsigned short dump_location);
void command_prompt(int * fileState, int * executeState, unsigned short * dump_location, CPU_p cpu, Memory_p memory);
void stdinFlush();
#endif	/* _DEBUG_MONITOR_MODEL_H */
