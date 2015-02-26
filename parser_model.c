#define _CRT_SECURE_NO_WARNINGS
/*
 * parser_model.c
 *  Created on: Feb 20, 2015
 *      Author: Indy
 *      This is a parser that takes a text file and parses it into memory. This file
 *      was researched and borrowed from cs.dvc.edu.
 */

#include <stdio.h>
#include <stdlib.h>
#include "parser_model.h"

//Returns 0 if successful, returns 1 if failed.
int parser(char * fileName, Memory_p memory, CPU_p cpu) {
	int returnValue = FILE_LOAD_SUCCESS;
	int hexValue;
	int commandWord;
	unsigned char memoryValue;
	FILE * fp;
	fp = fopen(fileName, "r");
	if(fp != NULL) {
		fscanf (fp, "%x", &hexValue);
		cpu->pc = hexValue;
		while(fscanf(fp, "%x", &commandWord) != EOF) {
			memoryValue = commandWord & LOW_ORDER_BYTE_MASK;
			setMemoryValue(memory, hexValue, memoryValue);
			hexValue++;
			memoryValue = commandWord >> 8;
			setMemoryValue(memory, hexValue, memoryValue);
			hexValue++;
		}
		fclose(fp);
	} else {
		returnValue = FILE_LOAD_FAIL;
	}
	return returnValue;
}
