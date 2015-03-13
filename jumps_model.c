#include "jumps_model.h"
#include "load_store_op.h"

//Jumps to the location given by rx_location.
void jmp(int rx_location, CPU_p cpu, Memory_p memory) {
	cpu->pc = getRegisterValue(&(cpu->rf), rx_location);
}

//Performs the JSR subroutine and saves PC on the stack.
void jsr(int rx_location, CPU_p cpu, Memory_p memory) {
	stpc(STACK_POINTER, cpu, memory);
	setRegisterValue(&(cpu->rf), STACK_POINTER, getRegisterValue(&(cpu->rf), STACK_POINTER) - 2);

	jmp(rx_location, cpu, memory);
}

//Brings the PC back from the subroutine.
void ret(CPU_p cpu, Memory_p memory) {
	setRegisterValue(&(cpu->rf), STACK_POINTER, getRegisterValue(&(cpu->rf), STACK_POINTER) + 2);
	ldpc(STACK_POINTER, cpu, memory);
}

//Branches the PC to the location given.
void br(int pc_location, CPU_p cpu) {
	//Adds the opcode for the branch location to the current pc location.
	cpu->pc += pc_location;
}

//Branches the PC to the location given when the negative flag is set to true.
void brN(int pc_location, CPU_p cpu) {
	//Gets the negative flag value.
	int negative = cpu->sw & 0x8000;
	//Checks to see if value is 1.
	if (negative) {
		//Adds the opcode for the branch location to the current pc location.
		cpu->pc += pc_location;
	}
}

//Branches the PC to the location given when the zero flag is set to true.
void brZ(int pc_location, CPU_p cpu) {
	//Gets the zero flag value.
	int zero = cpu->sw & 0x4000;
	//Checks to see if value is 1.
	if (zero) {
		//Adds the opcode for the branch location to the current pc location.
		cpu->pc += pc_location;
	}
}

//Branches the PC to the location given when the carry flag is set to true.
void brC(int pc_location, CPU_p cpu) {
	//Gets the carry flag value.
	int carry = cpu->sw & 0x2000;
	//Checks to see if value is 1.
	if (carry) {
		//Adds the opcode for the branch location to the current pc location.
		cpu->pc += pc_location;
	}
}

//Branches the pc to the location given when the overflow flag is set to true.
void brO(int pc_location, CPU_p cpu) {
	//Gets the carry flag value.
	int carry = cpu->sw & 0x1000;
	//Checks to see if value is 1.
	if (carry) {
		//Adds the opcode for the branch location to the current pc location.
		cpu->pc += pc_location;
	}
}

//Saves the PC and SW on the stack before doing the Trap.
void trapPush(CPU_p cpu, Memory_p memory) {
	stpc(STACK_POINTER, cpu, memory);
	setRegisterValue(&(cpu->rf), STACK_POINTER, getRegisterValue(&(cpu->rf), STACK_POINTER) - 2);
	stsw(STACK_POINTER, cpu, memory);
	setRegisterValue(&(cpu->rf), STACK_POINTER, getRegisterValue(&(cpu->rf), STACK_POINTER) - 2);
}

//After doing a trap, loads the PC and SW from stack.
void trapPop(CPU_p cpu, Memory_p memory) {
	setRegisterValue(&(cpu->rf), STACK_POINTER, getRegisterValue(&(cpu->rf), STACK_POINTER) + 2);
	ldsw(STACK_POINTER, cpu, memory);
	setRegisterValue(&(cpu->rf), STACK_POINTER, getRegisterValue(&(cpu->rf), STACK_POINTER) + 2);
	ldpc(STACK_POINTER, cpu, memory);
}

//Helper functions
//Stores the PC value into memory.
void stpc(int ra_location, CPU_p cpu, Memory_p memory) {
	//Load mdr with the low order byte
	cpu->mdr = (cpu->pc) & LOW_ORDER_BYTE_MASK;
	//Load mar with the address in the register
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location);
	//Set the memory at the address stored in mar with the byte stored in mdr
	setMemoryValue(memory, cpu->mar, cpu->mdr);

	//store high order byte in next memory location
	cpu->mdr = (cpu->pc) >> 8;
	cpu->mar = cpu->mar + 1;
	setMemoryValue(memory, cpu->mar, cpu->mdr);
}

//Loads the PC value from memory.
void ldpc(int ra_location, CPU_p cpu, Memory_p memory) {
	//Set up the Memory Address Register
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location);
	//Use the Memory Address Register to get the byte we want into the MDR
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the MDR into the LOB of the PC
	cpu->pc = cpu->mdr;

	//Use the Memory Address Register to get the byte we want into the MDR
	cpu->mar = cpu->mar + 1;
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the MDR into the HOB of the PC
	cpu->pc |= (cpu->mdr << 8);
}

//Loads the SW value from memory.
void ldsw(int ra_location, CPU_p cpu, Memory_p memory) {
	//Set up the Memory Address Register
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location);
	//Use the Memory Address Register to get the byte we want into the MDR
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the MDR into the LOB of the PC
	cpu->sw = cpu->mdr;

	//Use the Memory Address Register to get the byte we want into the MDR
	cpu->mar = cpu->mar + 1;
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the MDR into the HOB of the PC
	cpu->sw |= (cpu->mdr << 8);
}

//Stores the SW value into memory.
void stsw(int ra_location, CPU_p cpu, Memory_p memory) {
	//Load mdr with the low order byte
	cpu->mdr = (cpu->sw) & LOW_ORDER_BYTE_MASK;
	//Load mar with the address in the register
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location);
	//Set the memory at the address stored in mar with the byte stored in mdr
	setMemoryValue(memory, cpu->mar, cpu->mdr);

	//store high order byte in next memory location
	cpu->mdr = (cpu->sw) >> 8;
	cpu->mar = cpu->mar + 1;
	setMemoryValue(memory, cpu->mar, cpu->mdr);
}