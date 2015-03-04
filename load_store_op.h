#ifndef _LOAD_STORE_OP_H
#define _LOAD_STORE_OP_H

#include "cpu_model.h"
#include "mem_model.h"
#include "rf_model.h"

#define IMMED_5_HOB 0x00000010
#define INTEGER_IMMED_5_SIGN_EXTEND 0xffffffe0;
#define SHIFT_TO_HOB 8
//Defining opcodes
#define LDB 0x0010
#define LDW 0x0011
#define LDBR 0x0012
#define LDWR 0x0013
#define LDI 0x0014
#define LEA 0x0015
#define MOV 0x0016
#define STB 0x0017
#define STW 0x0018
#define STBR 0x0019
#define STWR 0x001A
#define PUSH 0x001C
#define POP 0x001D


//load and store functions
void ldb(int rd_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory);
void ldw(int rd_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory);
void ldbr(int rd_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory);
void ldwr(int rd_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory);
void ldi(int rd_location, int immediate_8, CPU_p cpu);
void lea(int rd_location, int immediate_8, CPU_p cpu , Memory_p memory);
void mov(int rd_location, int rs_location, CPU_p cpu);
void stb(int rs_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory);
void stw(int rs_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory);
void stbr(int rs_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory);
void push(int rd_location, int ra_location, CPU_p cpu, Memory_p memory);
void pop(int rd_location, int ra_location, CPU_p cpu, Memory_p memory);


//helper functions
int immediate5NegativeSet(int immediate_5);
void loadHOBIntoRf(int register_number, CPU_p cpu);

#endif
