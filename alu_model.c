#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "alu_model.h"


// ALU methods
//This method creates an ALU object on the heap.
ALU_p createALU(void) {
	ALU_p alu = (ALU_p)malloc(sizeof(ALU));
	clearALU(alu);
	return alu;
}

//This method clears all stored ALU memory.
void clearALU(ALU_p alu) {
	alu->A = 0x0;
	alu->B = 0x0;
	alu->R = 0x0;
	alu->flags = 0x0;
}

//This method sets an ALU register with the given value.
void setALU_Registers(ALU_p alu, Register opnd1, Register opnd2) {
	alu->A = opnd1;
	alu->B = opnd2;
	alu->R = 0x0;
}

//This method determines what operations are performed on the ALU register values.
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

//Sets the ALU's flags with the given values.
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
        //Subtracts two values and checks for flag values (negative, zero).
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
        //Shifts a value to the left and checks for flag values (overflow, negative, zero)
	case SHL:
		if (sign_A) { //if our register A contains a 1 in the high order bit.
			alu->flags |= CARRY_SET;
		}
		else {
			alu->flags &= ~CARRY_SET;
		}
		alu->flags &= ~OVERFLOW_SET;
		break;
        //Shifts a value to the right and checks for flag values (negative, zero).
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

//Adds the ALU A and B registers and sets the total in the R register.
void add(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 + opnd2;	// add two integers to check overflow, etc.
	alu->R = result & LOW_ORDER_WORD_MASK;
}
//Subtracts the ALU A and B registers and sets the sum in the R register.
void sub(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 - opnd2;
	alu->R = result & LOW_ORDER_WORD_MASK;
}
//Multiplies the ALU A and B registers and sets the product in the R register.
void mul(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 * opnd2;
	alu->R = result & LOW_ORDER_WORD_MASK;
}

//Multiplies the ALU A and B registers and sets the quotient in the R register.
//The remainder goes in the A register
void divide(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 / opnd2;
	unsigned remainder = opnd1 % opnd2;
	alu->A = remainder & LOW_ORDER_WORD_MASK;
	alu->R = result & LOW_ORDER_WORD_MASK;
}
//Performs a bitwise and on the ALU A and B registers and sets the result in the R register.
void and(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 & opnd2;
	alu->R = result & LOW_ORDER_WORD_MASK;
}

//Performs a bitwise or on the ALU A and B registers and sets the result in the R register.
void or(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 | opnd2;
	alu->R = result & LOW_ORDER_WORD_MASK;
}

//Performs a bitwise not on the ALU A register and sets the result in the R register.
void not(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned result = ~opnd1;
	alu->R = result & LOW_ORDER_WORD_MASK;
}

//Performs a bitwise exclusive or on the ALU A and B registers and sets the result in the R register.
void xor(ALU_p alu) {
	unsigned opnd1 = (unsigned)alu->A;
	unsigned opnd2 = (unsigned)alu->B;
	unsigned result = opnd1 ^ opnd2;
	alu->R = result & LOW_ORDER_WORD_MASK;
}

//Performs a logical shift left on the A register and sets it in the R register.
void shl(ALU_p alu, int immediate_3) {
	unsigned opnd1 = (unsigned)alu->A;
	int opnd2 = immediate_3;
	unsigned result = opnd1 << opnd2;
	alu->R = result & LOW_ORDER_WORD_MASK;
}

//Performs a logical shift right on the A register and sets it in the R register.
void shr(ALU_p alu, int immediate_3) {
	unsigned opnd1 = (unsigned)alu->A;
	int opnd2 = immediate_3;
	unsigned result = opnd1 >> opnd2;
	//setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

//A helper function that loads the values from the given registers for operations.
void loadAandBReg(ALU_p alu, Register a, Register b) {
	alu->A = a;
	alu->B = b;
}
