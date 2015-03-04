#ifndef _RF_MODEL_H
#define _RF_MODEL_H
// Register ADT
typedef unsigned short Register;
typedef Register * Register_p;

//defines the byte registers.
typedef unsigned char ByteRegister;
//creates a nice name for the byte register pointer.
typedef ByteRegister * ByteRegister_p;

#define LOW_ORDER_WORD_MASK 0x0000FFFF
#define LOW_ORDER_BYTE_MASK 0x000000FF

Register_p createRegister(void);	// constructor
int getSignedValue(Register_p r);	// returns a signed value of what is in register r
void putUnsignedValue(Register_p r, int v);	// sets the value in register r to the unsigned value v

// Register file ADT
//Defines the size of a register file
#define REGISTER_FILE_SIZE 8
//Defines the names of the registers.
#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7

//The struct of a register file.
typedef struct registerFile {
	Register r0, r1, r2, r3, r4, r5, r6, r7;
} RegisterFile;

//creates a nice name for the register files.
typedef RegisterFile * RegisterFile_p;

//Prototypes
RegisterFile_p createRegisterFile(void);	// constructor
Register getRegisterValue(RegisterFile_p rf, int which);
void setRegisterValue(RegisterFile_p rf, int which, Register v);
void clearRegisterFile(RegisterFile_p);
void printRegisterFile(RegisterFile_p rf);

#endif
