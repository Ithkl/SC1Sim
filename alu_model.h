/*
alu_model.h
Programmer: George Mobus

Continued by Jackson Hubert and James Nance
*/

#include "rf_model.h"
#ifndef _ALU_MODEL_H
#define _ALU_MODEL_H
// ALU ADT
// 0 ADD, 1 SUB, 2 MUL, 3 DIV, 4 AND, 5 OR, 6 NOT, 7 XOR, 8 SHL, 9 SHR
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define AND 4
#define OR  5
#define NOT 6
#define XOR 7
#define SHL 8
#define SHR 9

#define OVERFLOW_SET 0x1
#define NEGATIVE_SET 0x8
#define ZERO_SET 0x4
#define CARRY_SET 0x2

#define IMMED8_MASK 0x00FF
#define REGISTER_MASK 0x0007
#define IMMED5_MASK 0x001F
#define ARGS1_MASK 0x0003
#define ARGS2_MASK 0x0400
#define IMMED10_MASK 0x03FF

#define CARRY_TEST 0x4000
#define SIGN_BIT_TEST 0x8000

#define SHR_CARRY_MASK 0x1

typedef struct alu {
	Register A, B, R;
	unsigned char flags;	// n, z, c, o - low order nybble
} ALU;

typedef ALU * ALU_p;

ALU_p createALU(void);
void clearALU(ALU_p);	// clears registers and flags
void performOperation(ALU_p, int);
void setALU_Flags(ALU_p, int);
void add(ALU_p);
void sub(ALU_p);
void mul(ALU_p);
void divide(ALU_p);
void and(ALU_p);
void or(ALU_p);
void not(ALU_p);
void xor(ALU_p);
void shl(ALU_p, int);
void shr(ALU_p, int);

int getOperation(Register ir);
int decodeImmed8(Register ir);
int decodeRd(Register ir);
int decodeRa(Register ir);
int decodeRxAndImmd3(Register ir);
int decodeImmed5(Register ir);
int decodeArgument1(Register ir);
int decodeArgument2(Register ir);
int decodeImmed10(Register ir);

void loadAandBReg(ALU_p alu, Register a, Register b);

void scanTest(int*, int);

#endif
