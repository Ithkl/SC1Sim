#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "alu_model.h"

/*
Jackson Hubert and James Nance
Date:1-30-2015
TCSS 372
Description:
This is a completed ALU using the third example code as a template.  The arithmetic functions have
been added and the set flag functions have been fleshed out.

alu_model.c
Programmer: George Mobus
Date: 1-22-2015
Description:
This program provides example code for creating register objects and a register file. These two objects
can be instantiated either on the stack ("raw") or on the heap (i.e. constructed). Several methods (OO-style
functions) are given to show how to write them in object-oriented style in C. Note that when an object is
instantiated in the heap the first argument passed in a method is always the pointer to the specific object.
That pointer is actually the 'this' reference in Java, where it is implied in Java it is explicit in C.
The main() test driver also shows how data conversions from unsigned to signed work.
*/

// ALU methods

ALU_p createALU(void) {
	ALU_p alu = (ALU_p)malloc(sizeof(ALU));
	clearALU(alu);
	return alu;
}

void clearALU(ALU_p alu) {
	alu->A = 0x0;
	alu->B = 0x0;
	alu->R = 0x0;
	alu->flags = 0x0;
}

void setALU_Registers(ALU_p alu, Register opnd1, Register opnd2) {
	alu->A = opnd1;
	alu->B = opnd2;
	alu->R = 0x0;
}

void performOperation(ALU_p alu, int op) {

	switch (op) {
	case ADD: {add(alu); break; }
	case SUB: {sub(alu); break; }
	case MUL: {mul(alu); break; }
	case DIV: {divide(alu); break; }
	case AND: {and(alu); break; }
	case OR: {or(alu); break; }
	case NOT: {not(alu); break; }
	case XOR: {xor(alu); break; }
	//case SHL: {shl(alu); break; }
	//case SHR: {shr(alu); break; }
	default: return;	// set machine exception
	}

	setALU_Flags(alu, op);

}

/*void setALU_Flags(ALU_p alu, unsigned result) {
int sign_A, sign_B, sign_R;

sign_A = alu->A & SIGN_BIT_TEST;
sign_B = alu->B & SIGN_BIT_TEST;
sign_R = alu->R & SIGN_BIT_TEST;

if (result > 65535) alu->flags |= OVERFLOW_SET;			// set overflow flag
else if (sign_R) alu->flags |= NEGATIVE_SET;		// set negative flag
else if (alu->R == 0) alu->flags |= ZERO_SET;
else if (result > 32767) alu->flags |= CARRY_SET;

}*/

/*void setALU_Flags(ALU_p alu, unsigned result) {

alu->flags = 0x0;

//alu->flags |= OVERFLOW_SET;
//alu->flags |= NEGATIVE_SET;
alu->flags |= ZERO_SET;
alu->flags |= CARRY_SET;

}*/

void setALU_Flags(ALU_p alu, int opcode) {
	int sign_A, sign_B, sign_R;

	sign_A = alu->A & SIGN_BIT_TEST;
	sign_B = alu->B & SIGN_BIT_TEST;
	sign_R = alu->R & SIGN_BIT_TEST;

	switch (opcode) { // Checking ADD, SUB, and bitshifts for overflow and carry where applicable.

	case ADD:
		if ((sign_A == sign_B) && (sign_R != sign_A)) {
			alu->flags |= OVERFLOW_SET;
		}
		else {
			alu->flags &= ~OVERFLOW_SET;
		}
		if ((alu->A & CARRY_TEST) == CARRY_TEST && (alu->B & CARRY_TEST)) { //If both carry bits have a 1 there is a carry.
			alu->flags |= CARRY_SET;
		}
		else {
			alu->flags &= ~CARRY_SET;
		}
		break;
	case SUB:
		if ((sign_A != sign_B) && (sign_R != sign_A)) {
			alu->flags |= OVERFLOW_SET;
		}
		else {
			alu->flags &= ~OVERFLOW_SET;
		}
		if ((alu->A & CARRY_TEST) == 0x0 && (alu->B & CARRY_TEST) == CARRY_TEST) { //If the carry bit is subtracting one from zero, there is a carry.
			alu->flags |= CARRY_SET;
		}
		else {
			alu->flags &= ~CARRY_SET;
		}
		break;
	case SHL:
		if (sign_A) { //if our register A contains a 1 in the high order bit.
			alu->flags |= CARRY_SET;
		}
		else {
			alu->flags &= ~CARRY_SET;
		}
		alu->flags &= ~OVERFLOW_SET;
		break;
	case SHR:
		if (alu->A & SHR_CARRY_MASK) {
			alu->flags |= CARRY_SET;
		}
		else {
			alu->flags &= ~CARRY_SET;
		}
		alu->flags &= ~OVERFLOW_SET;
		break;
	default:
		//Set carry and overflow to zero, because this operation can't set those flags.
		alu->flags &= ~CARRY_SET;
		alu->flags &= ~OVERFLOW_SET;
		break;
	}
	if (sign_R) {
		alu->flags |= NEGATIVE_SET;
	}
	else {
		alu->flags &= ~NEGATIVE_SET;
	}
	if (alu->R == 0) {
		alu->flags |= ZERO_SET;
	}
	else {
		alu->flags &= ~ZERO_SET;
	}

}

void add(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 + opnd2;	// add two integers to check overflow, etc.
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void sub(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 - opnd2;
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void mul(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 * opnd2;
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void divide(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 / opnd2;
	unsigned remainder = opnd1 % opnd2;
	//setALU_Flags(alu, result);
	alu->A = remainder & LOW_ORDER_WORD_MASK;
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void and(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 & opnd2;
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void or(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 | opnd2;
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void not(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned result = ~opnd1;
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void xor(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 ^ opnd2;
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void shl(ALU_p alu, int immediate_3) {
	unsigned opnd1 = (unsigned)alu->A;
	int opnd2 = immediate_3;
	//printf("Enter immediate 3 value: ");
	//scanf("%d", &opnd2);
	//scanTest(&opnd2, 1);
	unsigned result = opnd1 << opnd2;
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void shr(ALU_p alu, int immediate_3) {
	unsigned opnd1 = (unsigned)alu->A;
	int opnd2 = immediate_3;
	//printf("Enter immediate 3 value: ");
	//scanf("%d", &opnd2);
	//scanTest(&opnd2, 1);
	unsigned result = opnd1 >> opnd2;
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void loadAandBReg(ALU_p alu, Register a, Register b) {
	alu->A = a;
	alu->B = b;
}

void scanTest(int *i, int j) {
	printf("%d", j);
	*i = j;
}
