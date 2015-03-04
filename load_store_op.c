/*load_store_op.c
 * Contains the functions that run the load and store instructions
 * These functions load values into registers and store values into memory
 * Also includes Fetch which loads the Instruction Register with the next instruction
 */

#include "mem_model.h"
#include "cpu_model.h"
#include "load_store_op.h"
#include "rf_model.h"

/*Loads one byte from memory into a register
 *Memory address is found by adding an offset to the contents of a register
 *Operands: Destination register, Base register for address, immediate value for address offset
 *Also accepts a pointer to the cpu and a pointer to the memory*/
void ldb(int rd_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory){

	immediate_5 = immediate5NegativeSet(immediate_5);
	//set up the Memory Address Register
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location) + immediate_5;
	//Use the Memory Address Register to get the byte we want into the MDR
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the MDR into the working register.
	setRegisterValue(&(cpu->rf), rd_location, cpu->mdr);
}

/*Loads one word from memory into a register
 *Memory address is found by adding an offset to the contents of a register
 *Operands: Destination register, Base register for address, immediate value for address offset
 *Also accepts a pointer to the cpu and a pointer to the memory*/
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

/*Loads one byte from memory into a register 
 *Memory address is the sum of two registers
 *Operands: Destination register, Base register for address, Extra register for address
 *Also accepts a pointer to the cpu and a pointer to the memory*/
void ldbr(int rd_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory){

	//Set the Memory Address Register.
	cpu->mar = (unsigned short)getRegisterValue(&(cpu->rf), ra_location) + (signed short)getRegisterValue(&(cpu->rf), rx_location);
	//Use the Memory Address Register to get the byte we want into the MDR.
	cpu->mdr = getMemoryValue(memory, cpu->mar);
	//Load the MDR into the working register.
	setRegisterValue(&(cpu->rf), rd_location, cpu->mdr);
}

/*Loads one word from memory into a register 
 *Memory address is the sum of two registers
 *Operands: Destination register, Base register for address, Extra register for address
 *Also accepts a pointer to the cpu and a pointer to the memory*/
void ldwr(int rd_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory){
	Register destRegister;
	ldbr(rd_location, ra_location, rx_location, cpu, memory);
	cpu->mar = cpu->mar + 1;
	cpu->mdr = getMemoryValue(memory, cpu->mar);

	loadHOBIntoRf(rd_location, cpu);
}

/*Loads a register with an immediate value
 *Operands: Destination register, immediate value to be stored
 *Also accepts a pointer to the cpu*/
void ldi(int rd_location, int immediate_8, CPU_p cpu){
	setRegisterValue(&(cpu->rf), rd_location, (unsigned) immediate_8);
}

/*Load Register with one word from memory
 *Memory address is the "effective address" found by adding the PC plus offset 
 *Operands: Destination register, Immediate value for PC offset
 *Also accepts a pointer to the cpu and a pointer to the memory*/
void lea(int rd_location, int immediate_8, CPU_p cpu , Memory_p memory){
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

/*Copy the value in one Register to another Register
 * Operands: Destination register, Source register
 * Also accepts a pointer to the cpu*/
void mov(int rd_location, int rs_location, CPU_p cpu){
	setRegisterValue(&(cpu->rf), rd_location, getRegisterValue(&(cpu->rf), rs_location));
}

/*Store LOB from a register into memory 
 *Memory address found by adding contents of a register and offset
 *Operands: Source register, Base register for address, Immediate value for offset
 *Also accepts a pointer to the cpu and a pointer to the memory */
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

/*Store entire contents of register in two consecutive memory locations
 *Memory address found by adding contents of a register and offset
 *Operands: Source register, Base register for address, Immediate value for offset
 *Also accepts a pointer to the cpu and a pointer to the memory */
void stw(int rs_location, int ra_location, int immediate_5, CPU_p cpu, Memory_p memory){
	//store low order byte
	stb(rs_location, ra_location, immediate_5, cpu, memory);
	//store high order byte in next memory location
	cpu->mdr = getRegisterValue(&(cpu->rf), rs_location) >> 8;
	cpu->mar = cpu->mar + 1;
	setMemoryValue(memory, cpu->mar, cpu->mdr);
}

/*Store LOB of register into memory
 *Memory address found by adding contents of two registers
 *Operands: Source register, Base register for address, extra register for address
 *Also accepts a pointer to the cpu and a pointer to the memory */
void stbr(int rs_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory){
	//Create an unsigned char that is the low order byte of the contents of the register
	unsigned char low_order_byte = getRegisterValue(&(cpu->rf), rs_location) & LOW_ORDER_BYTE_MASK;
	//Load mdr with the low order byte
	cpu->mdr = low_order_byte;
	//Load mar with the value of ra plus rx
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location) + (signed)getRegisterValue(&(cpu->rf), rx_location);
	//Set the memory at the address stored in mar with the byte stored in mdr
	setMemoryValue(memory, cpu->mar, cpu->mdr);
}

/*Store contents of register into two consecutive addresses in memory
 *Memory address found by adding contents of two registers
 *Operands: Source register, Base register for address, extra register for address
 *Also accepts a pointer to the cpu and a pointer to the memory */
void stwr(int rs_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory){
	//Create an unsigned char that is the high order byte of the contents of the register.
	void stbr(int rs_location, int ra_location, int rx_location, CPU_p cpu, Memory_p memory);
	unsigned char high_order_byte = getRegisterValue(&(cpu->rf), rs_location) >> 8;
	//Load mdr with the high order byte.
	cpu->mdr = high_order_byte;
	//Load mar with the value of ra plus rx.
	cpu->mar = getRegisterValue(&(cpu->rf), ra_location) + getRegisterValue(&(cpu->rf), rx_location);
	//Set the memory at the address stored in mar with the byte stored in mdr.
	setMemoryValue(memory, cpu->mar + 1, cpu->mdr);
}

//Stores the value of a register on the stack.
void push(int rd_location, int ra_location, CPU_p cpu, Memory_p memory) {
	setRegisterValue(&(cpu->rf), ra_location, getRegisterValue(&(cpu->rf), ra_location) - 1);
	stw(rd_location, ra_location, 0, cpu, memory);
	setRegisterValue(&(cpu->rf), ra_location, getRegisterValue(&(cpu->rf), ra_location) - 1);
}

//removes the value of a register on the stack and stores it in a register.
void pop(int rd_location, int ra_location, CPU_p cpu, Memory_p memory) {
	setRegisterValue(&(cpu->rf), ra_location, getRegisterValue(&(cpu->rf), ra_location) + 1);
	ldw(rd_location, ra_location, 0, cpu, memory);
	setRegisterValue(&(cpu->rf), ra_location, getRegisterValue(&(cpu->rf), ra_location) + 1);
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

/*Loads the contents of mdr into the High Order Byte of the register 
 *while preserving the Low Order Byte that is already stored in the register*/
void loadHOBIntoRf(int register_number, CPU_p cpu){
	Register destRegister;
        Register hob = (cpu->mdr << SHIFT_TO_HOB);
	destRegister = getRegisterValue(&(cpu->rf), register_number);	//Getting the contents of the Rd so we can bitshift and load the HOB.
	destRegister |= hob;
	setRegisterValue(&(cpu->rf), register_number, destRegister);
}
