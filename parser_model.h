/*
 * parser_model.h
 *
 *  Created on: Feb 20, 2015
 *      Author: Indy
 */

#ifndef PARSER_MODEL_H_
#define PARSER_MODEL_H_

#include "cpu_model.h"
#include "mem_model.h"
#include "rf_model.h"

//Values given depending on if the file loading was a success or failure.
#define FILE_LOAD_SUCCESS 0
#define FILE_LOAD_FAIL 1

//Parser prototype.
int parser(char * fileName, Memory_p memory, CPU_p cpu);

//A high order byte mask to get the other half of the word for memory saving.
#define HIGH_ORDER_BYTE_MASK 0x0000FF00

#endif /* PARSER_MODEL_H_ */
