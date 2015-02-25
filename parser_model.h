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

int parser(char * fileName, Memory_p memory, CPU_p cpu);

#define HIGH_ORDER_BYTE_MASK 0x0000FF00

#endif /* PARSER_MODEL_H_ */
