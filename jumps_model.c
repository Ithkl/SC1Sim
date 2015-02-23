#include "jumps_model.h"
#include "load_store_op.h"

void jmp(int rx_location, CPU_p cpu, Memory_p memory) {
	cpu->pc = getRegisterValue(&(cpu->rf), rx_location);
}
/*void jsr(int rd_location, int rx_location, CPU_p cpu, Memory_p memory) {
	stpc(rd_location, cpu, memory);
	setRegisterValue(&(cpu->rf), rd_location, getRegisterValue(&(cpu->rf), rd_location) - 2);

	jmp(rx_location, cpu, memory);
}
void ret(int rd_location, CPU_p cpu, Memory_p memory) {
	setRegisterValue(&(cpu->rf), rd_location, getRegisterValue(&(cpu->rf), rd_location) + 2);
	ldpc(rd_location, cpu, memory);
}*/
void jsr(int rx_location, CPU_p cpu, Memory_p memory) {
	stpc(STACK_POINTER, cpu, memory);
	setRegisterValue(&(cpu->rf), STACK_POINTER, getRegisterValue(&(cpu->rf), STACK_POINTER) - 2);

	jmp(rx_location, cpu, memory);
}
void ret(CPU_p cpu, Memory_p memory) {
	setRegisterValue(&(cpu->rf), STACK_POINTER, getRegisterValue(&(cpu->rf), STACK_POINTER) + 2);
	ldpc(STACK_POINTER, cpu, memory);
}
void push(int rd_location, int ra_location, CPU_p cpu, Memory_p memory) {
	setRegisterValue(&(cpu->rf), ra_location, getRegisterValue(&(cpu->rf), ra_location) - 1);
	stw(rd_location, ra_location, 0, cpu, memory);
	setRegisterValue(&(cpu->rf), ra_location, getRegisterValue(&(cpu->rf), ra_location) - 1);
}
void pop(int rd_location, int ra_location, CPU_p cpu, Memory_p memory) {
	setRegisterValue(&(cpu->rf), ra_location, getRegisterValue(&(cpu->rf), ra_location) + 1);
	ldw(rd_location, ra_location, 0, cpu, memory);
	setRegisterValue(&(cpu->rf), ra_location, getRegisterValue(&(cpu->rf), ra_location) + 1);
}

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
