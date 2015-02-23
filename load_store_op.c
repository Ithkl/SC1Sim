#include "mem_model.h"
#include "cpu_model.h"
#include "load_store_op.h"
#include "rf_model.h"

void ldb(int rd_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory){

	immediate_5 = immediate5NegativeSet(immediate_5);
	//set up the Memory Address Register
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location) + immediate_5;
	//Use the Memory Address Register to get the byte we want into the MDR
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the MDR into the working register.
	setRegisterValue(&(cpu->rf), rd_location, cpu->mdr);
}

void ldw(int rd_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory){
	Register destRegister;
	ldb(rd_location, ra_location, immediate_5, cpu, memory);
	//Get the value of the Rd so we can work with it.
	destRegister = getRegisterValue(&(cpu->rf), rd_location);
	immediate_5 = immediate5NegativeSet(immediate_5);

	cpu->mar = cpu->mar + 1;
	//Use the Memory Address Register to get the byte we want into the MDR
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the MDR into the HOB of the working register.
	loadHOBIntoRf(rd_location, cpu);
}

void ldbr(int rd_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory){

	//Set the Memory Address Register.
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location) + (signed short)getRegisterValue(&(cpu->rf), rx_location);
	//Use the Memory Address Register to get the byte we want into the MDR.
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the MDR into the working register.
	setRegisterValue(&(cpu->rf), rd_location, cpu->mdr);
}

void ldwr(int rd_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory){
	Register destRegister;
	ldbr(rd_location, ra_location, rx_location, cpu, memory);
	cpu->mar = cpu->mar + 1;
	cpu->mdr = getMemoryValue(memory, cpu->mar);

	loadHOBIntoRf(rd_location, cpu);
}

void ldi(int rd_location, int immediate_8, CPU_p cpu){

	setRegisterValue(&(cpu->rf), rd_location, (unsigned) immediate_8);

}

//Load Register with memory at effective address from the PC plus offset 
void lea(int rd_location, unsigned char immediate_8, CPU_p cpu , Memory_p memory){
	//load mar with pc plus offset
	cpu->mar = cpu->pc + immediate_8;
	//load mdr with the contents of that address in memory
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//save LOB
	Register value = cpu->mdr;

	//get the next byte from memory
	cpu->mar = cpu->mar + 1;
	cpu->mdr = getMemoryValue(memory, cpu->mar);

	//place it as the HOB in the destination register
	value |= (cpu->mdr << 8);
	setRegisterValue(&(cpu->rf), rd_location, value);
}

//Copy the value in one Register to another Register
void mov(int rd_location, int rs_location, CPU_p cpu){

	setRegisterValue(&(cpu->rf), rd_location, getRegisterValue(&(cpu->rf), rs_location));
}

//Store LOB from a register into memory address found by adding contents of another register and offset
void stb(int rs_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory){

	immediate_5 = immediate5NegativeSet(immediate_5);
	//Create an unsigned char that is the low order byte of the contents of the register
	unsigned char low_order_byte = getRegisterValue(&(cpu->rf), rs_location) & LOW_ORDER_BYTE_MASK;
	//Load mdr with the low order byte
	cpu->mdr = low_order_byte;
	//Load mar with the address in the register plus the offset
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location) + immediate_5;
	//Set the memory at the address stored in mar with the byte stored in mdr
	setMemoryValue(memory, cpu->mar, cpu->mdr);
}

//Store entire contents of register in two consecutive memory locations
void stw(int rs_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory){
	//store low order byte
	stb(rs_location, ra_location, immediate_5, cpu, memory);
	//store high order byte in next memory location
	cpu->mdr = getRegisterValue(&(cpu->rf), rs_location) >> 8;
	cpu->mar = cpu->mar + 1;
	setMemoryValue(memory, cpu->mar, cpu->mdr);
}

//Store LOB of register into memory address found by adding contents of two registers
void stbr(int rs_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory){
	//Create an unsigned char that is the low order byte of the contents of the register
	unsigned char low_order_byte = getRegisterValue(&(cpu->rf), rs_location) & LOW_ORDER_BYTE_MASK;
	//Load mdr with the low order byte
	cpu->mdr = low_order_byte;
	//Load mar with the value of ra plus rx
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location) + getRegisterValue(&(cpu->rf), rx_location);
	//Set the memory at the address stored in mar with the byte stored in mdr
	setMemoryValue(memory, cpu->mar, cpu->mdr);
}

void stwr(int rs_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory){
	//Create an unsigned char that is the high order byte of the contents of the register.
	unsigned char high_order_byte = getRegisterValue(&(cpu->rf), rs_location);
	//Load mdr with the high order byte.
	cpu->mdr = high_order_byte;
	//Load mar with the value of ra plus rx.
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location) + getRegisterValue(&(cpu->rf), rx_location);
	//Set the memory at the address stored in mar with the byte stored in mdr.
	setMemoryValue(memory, cpu->mar, cpu->mdr);
}

void br(int pc_location, CPU_p cpu) {
	//Adds the opcode for the branch location to the current pc location.
	cpu->pc += pc_location;
}

void brN(int pc_location, CPU_p cpu) {
	//Gets the negative flag value.
	int negative = cpu->sw & 0x8000;
	//Checks to see if value is 1.
	if (negative) {
		//Adds the opcode for the branch location to the current pc location.
		cpu->pc += pc_location;
	}
}

void brZ(int pc_location, CPU_p cpu) {
	//Gets the zero flag value.
	int zero = cpu->sw & 0x4000;
	//Checks to see if value is 1.
	if (zero) {
		//Adds the opcode for the branch location to the current pc location.
		cpu->pc += pc_location;
	}
}

void brC(int pc_location, CPU_p cpu) {
	//Gets the carry flag value.
	int carry = cpu->sw & 0x2000;
	//Checks to see if value is 1.
	if (carry) {
		//Adds the opcode for the branch location to the current pc location.
		cpu->pc += pc_location;
	}
}

void brO(int pc_location, CPU_p cpu) {
	//Gets the carry flag value.
	int carry = cpu->sw & 0x1000;
	//Checks to see if value is 1.
	if (carry) {
		//Adds the opcode for the branch location to the current pc location.
		cpu->pc += pc_location;
	}
}


fetch(CPU_p cpu, Memory_p memory){

	//clearing the IR
	cpu->ir = 0;

	//Giving the PC to the MAR
	cpu->mar = cpu->pc;

	//Read signal is sent out, MDR gets the contents of memory at MAR.
	cpu->mdr = getMemoryValue(memory, cpu->mar);

	//Load the low order byte into IR
	cpu->ir |= cpu->mdr;

	//Increment PC
	cpu->pc = cpu->pc + 1;

	//Giving the PC to the MAR
	cpu->mar = cpu->pc;
	//Read signal is sent out, MDR gets the contents of memory at MAR.
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the high order byte into IR
	cpu->ir |= (cpu->mdr << 8);

	//Increment PC
	cpu->pc = cpu->pc + 1;

}


//Helper functions

//Because we don't have a datatype that is 5 bits big we need to check the sign bit of any immediate 5's to make sure they're properly
//Negative in their bigger container.
int immediate5NegativeSet(int immediate_5){
	//If the number is negative we need to extend the sign bit.
	if (immediate_5 & IMMED_5_HOB)
	{
		immediate_5 |= INTEGER_IMMED_5_SIGN_EXTEND;	//Extend the sign bit.
	}

	return immediate_5;
}

void loadHOBIntoRf(int register_number, CPU_p cpu){
	Register destRegister;
	destRegister = getRegisterValue(&(cpu->rf), register_number);	//Getting the contents of the Rd so we can bitshift and load the HOB.
	destRegister |= (cpu->mdr << 8);
	setRegisterValue(&(cpu->rf), register_number, destRegister);

}

