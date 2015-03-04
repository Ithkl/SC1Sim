#include <stdio.h>
#include <stdlib.h>
#include "rf_model.h"

// Register methods
//Creates a register on the heap.
Register_p createRegister(void) {
	Register_p r = (Register_p)malloc(sizeof(Register));
	return r;
}

//Gets the signed value of a register.
int getSignedValue(Register_p r) {
	return (signed short)*r;
}

//Puts and unsigned value into a register.
void putUnsignedValue(Register_p r, int v) {
	*r = (unsigned short)v & LOW_ORDER_BYTE_MASK;
}

// RegisterFile Methods
//Creates a register file on the heap.
RegisterFile_p createRegisterFile(void) {
	RegisterFile_p rf = (RegisterFile_p)malloc(sizeof(RegisterFile));
	return rf;		// actually a pointer to a register - the first one
}

//Sets all the register files to zero.
void clearRegisterFile(RegisterFile_p rf) {
	int i;
	for (i = 0; i<REGISTER_FILE_SIZE; i++) setRegisterValue(rf, i, (Register)0);	// clear registers
}
//Sets a register in the register file.
void setRegisterValue(RegisterFile_p rf, int which, Register value) {
    //Determines which register was chosen.
	switch (which) {
	case R0: {rf->r0 = value; break; }
	case R1: {rf->r1 = value; break; }
	case R2: {rf->r2 = value; break; }
	case R3: {rf->r3 = value; break; }
	case R4: {rf->r4 = value; break; }
	case R5: {rf->r5 = value; break; }
	case R6: {rf->r6 = value; break; }
	case R7: {rf->r7 = value; break; }
	}
}

//Returns the value in the requested register.
Register getRegisterValue(RegisterFile_p rf, int which) {
	Register r;
        //Determines which register was requested.
	switch (which) {
	case R0: {r = rf->r0; break; }
	case R1: {r = rf->r1; break; }
	case R2: {r = rf->r2; break; }
	case R3: {r = rf->r3; break; }
	case R4: {r = rf->r4; break; }
	case R5: {r = rf->r5; break; }
	case R6: {r = rf->r6; break; }
	case R7: {r = rf->r7; break; }
	}
	return r;
}

//Prints what is in the register file.
void printRegisterFile(RegisterFile_p rf) {
	int i;
	for (i = 0; i<REGISTER_FILE_SIZE; i++) printf("register R%d content: %04X\n", i,
		getRegisterValue(rf, i));
}
