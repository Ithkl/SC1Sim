#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "cpu_model.h"
#include "mem_model.h"
#include "load_store_op.h"
#include "jumps_model.h"

#define PROGRAM_START_LOCATION 0x0000;

//2^16
//unsigned char memory[65536];

int main() {
	int rd_loc, ra_loc, rx_loc, immed3, immed5, immed8, arg;
	int halt = 0;
	//ALUTest();
	//memTest();
	//cpu_mem_pipeline_test();
	//load_store_test();
	//jump_test();
	CPU_p cpu = createCPU();
	Memory_p memory = createMemory();
        //We'll be loading the file around here.
	cpu->pc = PROGRAM_START_LOCATION;

	while (!halt)
	{
            fetch(cpu, memory);
		switch (getOperation(cpu->ir)){
		case ADD:
			//Get the register locations from the Instruction Register.
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);
			//Load Ra and Rx into ALU registers A and B.
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), ra_loc), getRegisterValue(&(cpu->rf), rx_loc));
			//Execute
			add(&(cpu->alu));
			//Set Rd as the value of ALU register R
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			//Set the flags on the ALU and transfer them over.
			setALU_Flags(&(cpu->alu), ADD);
			setSWRegFlags(cpu);
			break;

		case SUB:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), ra_loc), getRegisterValue(&(cpu->rf), rx_loc));
			sub(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), SUB);
			setSWRegFlags(cpu);
			break;

		case MUL:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), ra_loc) & LOW_ORDER_BYTE_MASK, getRegisterValue(&(cpu->rf), rx_loc) & LOW_ORDER_BYTE_MASK);
			mul(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), MUL);
			setSWRegFlags(cpu);
			break;

		case DIV:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), ra_loc), getRegisterValue(&(cpu->rf), rx_loc));
			divide(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			//Ra gets remainder
			setRegisterValue(&(cpu->rf), ra_loc, cpu->alu.A);
			setALU_Flags(&(cpu->alu), MUL);
			setSWRegFlags(cpu);
			break;

		case AND:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), ra_loc), getRegisterValue(&(cpu->rf), rx_loc));
			and(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), AND);
			setSWRegFlags(cpu);
			break;

		case OR:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), ra_loc), getRegisterValue(&(cpu->rf), rx_loc));
			or(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), OR);
			setSWRegFlags(cpu);
			break;
		case NOT:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			cpu->alu.A = getRegisterValue(&(cpu->alu), ra_loc);
			not(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), NOT);
			setSWRegFlags(cpu);
			break;

		case XOR:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), ra_loc), getRegisterValue(&(cpu->rf), rx_loc));
			xor(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), XOR);
			setSWRegFlags(cpu);
			break;

		case SHL:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			arg = decodeArgument1(cpu->ir);
			immed3 = decodeRxAndImmd3(cpu->ir);
			cpu->alu.A = getRegisterValue(&(cpu->alu), ra_loc);
			if (arg) {
				//SHR
				shr(&(cpu->alu), immed3);
				setALU_Flags(&(cpu->alu), SHR);
			}
			else {
				//SHL
				shl(&(cpu->alu), immed3);
				setALU_Flags(&(cpu->alu), SHL);
			}
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setSWRegFlags(cpu);
			break;

		case LDB:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			immed5 = decodeImmed5(cpu->ir);

			ldb(rd_loc, ra_loc, immed5, cpu, memory);
			break;
		case LDW:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			immed5 = decodeImmed5(cpu->ir);

			ldw(rd_loc, ra_loc, immed5, cpu, memory);
			break;
		case LDBR:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);

			ldbr(rd_loc, ra_loc, rx_loc, cpu, memory);
			break;
		case LDWR:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);

			ldwr(rd_loc, ra_loc, rx_loc, cpu, memory);
			break;
		case LDI:
			rd_loc = decodeRd(cpu->ir);
			immed8 = decodeImmed8(cpu->ir);

			ldi(rd_loc, immed8, cpu);
			break;
		case LEA:
			rd_loc = decodeRd(cpu->ir);
			immed8 = decodeImmed8(cpu->ir);

			lea(rd_loc, immed8, cpu, memory);
			break;
		case MOV:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);

			mov(rd_loc, ra_loc, cpu);
			break;
		case STB:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			immed5 = decodeImmed5(cpu->ir);

			stb(rd_loc, ra_loc, immed5, cpu, memory);
			break;
		case STW:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			immed5 = decodeImmed5(cpu->ir);

			stb(rd_loc, ra_loc, immed5, cpu, memory);
			break;
		case STBR:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);

			stbr(rd_loc, ra_loc, rx_loc, cpu, memory);
			break;
		case STWR:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			rx_loc = decodeRxAndImmd3(cpu->ir);

			stwr(rd_loc, ra_loc, rx_loc, cpu, memory);
			break;
		case BR:
			immed8 = decodeImmed8(cpu->ir);

			br(immed8, cpu);
			break;
		case BRN:
			immed8 = decodeImmed8(cpu->ir);

			brN(immed8, cpu);
			break;
		case BRZ:
			immed8 = decodeImmed8(cpu->ir);

			brZ(immed8, cpu);
			break;
		case BRC:
			immed8 = decodeImmed8(cpu->ir);

			brC(immed8, cpu);
			break;
		case BRO:
			immed8 = decodeImmed8(cpu->ir);
			brO(immed8, cpu);
			break;
		case JMP:
			rx_loc = decodeRxAndImmd3(cpu->ir);

			void jmp(int rx_location, CPU_p cpu, Memory_p memory);
			break;
		case RET:
				//Get the argument bit
				if (decodeArgument2(cpu->ir)) {
					switch (decodeImmed10(cpu->ir)) {
					case 23:
						halt = 1;
					default:
						break;
					}
				}
				else {
					ret(cpu, memory);
				}
				
				
			break;
		case PUSH:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			push(rd_loc, ra_loc, cpu, memory);
			break;
		case POP:
			rd_loc = decodeRd(cpu->ir);
			ra_loc = decodeRa(cpu->ir);
			pop(rd_loc, ra_loc, cpu, memory);
			break;
		default:
			break;
		}
	}
	printRegisterFile(&(cpu->rf));
	printMemory(memory);
	printf("Press any key to finish");
	getchar();
	getchar();

	free(memory);
	memory = NULL;
	free(cpu);
	cpu = NULL;
	return EXIT_SUCCESS;
}

int memTest() {
	Memory_p m = createMemory();
	MemoryNode node1 = 5;
	MemoryNode node2 = 10;
	clearMemory(m);

	int dest, val;

	printMemory(m);

	printf("\n\nPutting 5 into memory address zero");
	printf("\n\nPutting 10 into memory address 5\n\n");
	setMemoryValue(m, 0, node1);
	setMemoryValue(m, 5, node2);

	printMemory(m);
	
	printf("\n\nMemory value at 0 is %d", (signed char)getMemoryValue(m, 0));
	printf("\n\nMemory value at 0 is %d", (signed char)getMemoryValue(m, 5));

	getchar();
	getchar();
}

int ALUTest() {
	int dest, op1, op2, sel;			// register addresses

	RegisterFile_p rf = createRegisterFile();
	int i;

	clearRegisterFile(rf);


	ALU_p alu_p = createALU();

	int resp;

	printf("Enter destination register number: ");
	//scanf("%d", &dest);
	scanTest(&dest, 2);
	printf("\nEnter operand1 register number: ");
	//scanf("%d", &op1);
	scanTest(&op1, 0);
	printf("\nEnter value for operand 1 [up to 32768]: ");
	//scanf("%d", &resp);
	scanTest(&resp, 65001);
	setRegisterValue(rf, op1, (Register)resp);
	printf("\nEnter operand2 register number: ");
	//scanf("%d", &op2);
	scanTest(&op2, 1);
	printf("\nEnter value for operand 2 [up to 32768]: ");
	//scanf("%d", &resp);
	scanTest(&resp, 0);
	setRegisterValue(rf, op2, (Register)resp);
	printf("\nEnter the number of the operation");
	printf("\n0 ADD, 1 SUB, 2 MUL, 3 DIV, 4 AND, 5 OR, 6 NOT, 7 XOR, 8 SHL, 9 SHR: ");
	//scanf("%d", &sel);
	scanTest(&sel, 9);
	printf("\n");
	alu_p->A = getRegisterValue(rf, op1);
	alu_p->B = getRegisterValue(rf, op2);
	performOperation(alu_p, sel);
	printf("\n");
	printf("Register A = 0x%04X\n", alu_p->A);
	printf("Register B = 0x%04X\n", alu_p->B);
	printf("Register R = 0x%04X\n", alu_p->R);
	printf("Overflow Flag = %d\n", (alu_p->flags & OVERFLOW_SET)>0);
	printf("Negative Flag = %d\n", (alu_p->flags & NEGATIVE_SET)>0);
	printf("Zero Flag = %d\n", (alu_p->flags & ZERO_SET)>0);
	printf("Carry Flag = %d\n", (alu_p->flags & CARRY_SET)>0);
	setRegisterValue(rf, op1, alu_p->A);	//for the divide operation.
	setRegisterValue(rf, dest, alu_p->R);
	printRegisterFile(rf);

	getchar();
	getchar();
	free(alu_p);
	free(rf);
}

int cpu_mem_pipeline_test()
{
	CPU_p cpu = createCPU();
	Memory_p memory = createMemory();

	setMemoryValue(memory, 0, 0xed);
	setMemoryValue(memory, 1, 0xfe);
	
	//These will be cleaned up with function calls later.
	//setting our PC
	cpu->pc = 0;
	//clearing the IR
	cpu->alu.A = cpu->ir;
	cpu->alu.B = cpu->ir;
	xor(&(cpu->alu));
	cpu->ir = cpu->alu.R;
	//Giving the PC to the MAR
	cpu->mar = cpu->pc;
	//Read signal is sent out, MDR gets the contents of memory at MAR.
	cpu->mdr = getMemoryValue(memory, cpu->pc);
	//Load the low order byte into IR
	cpu->ir |= cpu->mdr;
	printf("\n\nthe value of the IR after the first part of fetch is: [%04X]", getSignedValue(&(cpu->ir)));
	//Increment PC
	cpu->alu.A = cpu->pc;
	cpu->alu.B = 1;
	add(&(cpu->alu));
	cpu->pc = cpu->alu.R;

	//Giving the PC to the MAR
	cpu->mar = cpu->pc;
	//Read signal is sent out, MDR gets the contents of memory at MAR.
	cpu->mdr = getMemoryValue(memory, cpu->pc);
	//Load the high order byte into IR
	cpu->ir |= (cpu->mdr<<8);

	//Increment PC
	cpu->alu.A = cpu->pc;
	cpu->alu.B = 1;
	add(&(cpu->alu));
	cpu->pc = cpu->alu.R;

	printf("\n\nthe value of the PC is: %d", getSignedValue(&(cpu->pc)));
	printf("\n\nthe value of the IR after the second part of fetch is: [%04X]\n\n", (unsigned short)getSignedValue(&(cpu->ir)));
	//Going to put the value AB into memory at 5

	//Giving the PC to the MAR
	cpu->mar = 5;
	//Sending the write signal
	setMemoryValue(memory, cpu->mar, 0xab);

	printMemory(memory);
	getchar();

	free(memory);
	memory = NULL;
	free(cpu);
	cpu = NULL;
	return 0;
}

load_store_test()
{
	Memory_p memory = createMemory();
	CPU_p cpu = createCPU();
	//Loading memory
	setMemoryValue(memory, 3, 0xed);
	setMemoryValue(memory, 4, 0xfe);
	setMemoryValue(memory, 12, 0xcd);
	setMemoryValue(memory, 13, 0xab);

	printMemory(memory);

	clearRegisterFile(&cpu->rf);
	//putting memory pointers into memory
	ldi(4,5,cpu);
	ldi(5, 10, cpu);
	ldi(6, 2, cpu);
	//load into register zero the memory at 3
	ldb(0, 4, -2, cpu, memory);
	//load into register one the memory at 3
	ldw(1, 4, -2, cpu, memory);

	ldbr(2, 5, 6, cpu, memory);
	ldwr(3, 5, 6, cpu, memory);

	printRegisterFile(&(cpu->rf));

	getchar();
	getchar();

	free(memory);
	memory = NULL;
	free(cpu);
	cpu = NULL;
}

int jumps_test() {
	Memory_p memory = createMemory();
	CPU_p cpu = createCPU();

	clearRegisterFile(&cpu->rf);

	printf("ldi(0,10,cpu)\n");
	ldi(0, 10, cpu);
	printf("ldi(1,1,cpu)\n");
	ldi(1, 1, cpu);
	printf("push(0,1,cpu,memory)\n");
	push(1, 0, cpu, memory);
	printf("ldi(1,2,cpu)\n");
	ldi(1, 2, cpu);
	printf("push(0,1,cpu,memory)\n");
	push(1, 0, cpu, memory);
	printf("ldi(1,3,cpu)\n");
	ldi(1, 3, cpu);
	printf("push(0,1,cpu,memory)\n");
	push(1, 0, cpu, memory);

	printf("\n");
	printMemory(memory);
	printf("\n");

	setMemoryValue(memory, 6, 4);
	setMemoryValue(memory, 8, 5);
	setMemoryValue(memory, 10, 6);

	printf("\n");
	printMemory(memory);
	printf("\n");

	printf("R0: %04X\n", getRegisterValue(&(cpu->rf), 0));
	printf("R1: %04X\n", getRegisterValue(&(cpu->rf), 1));
	//printRegisterFile(&(cpu->rf));

	printf("ldi(1,0,cpu)\n");
	ldi(1, 0, cpu);
	printf("R0: %04X\n", getRegisterValue(&(cpu->rf), 0));
	printf("R1: %04X\n", getRegisterValue(&(cpu->rf), 1));
	//printRegisterFile(&(cpu->rf));

	printf("pop(1, 0, cpu, memory)\n");
	pop(1, 0, cpu, memory);
	printf("R0: %04X\n", getRegisterValue(&(cpu->rf), 0));
	printf("R1: %04X\n", getRegisterValue(&(cpu->rf), 1));
	//printRegisterFile(&(cpu->rf));

	printf("pop(1, 0, cpu, memory)\n");
	pop(1, 0, cpu, memory);
	printf("R0: %04X\n", getRegisterValue(&(cpu->rf), 0));
	printf("R1: %04X\n", getRegisterValue(&(cpu->rf), 1));
	//printRegisterFile(&(cpu->rf));

	printf("pop(0, 1, cpu, memory)\n");
	pop(1, 0, cpu, memory);
	//printf("ldw(1, 0, cpu, memory)\n");
	//ldw(1, 0, 0, cpu, memory);
	printf("R0: %04X\n", getRegisterValue(&(cpu->rf), 0));
	printf("R1: %04X\n", getRegisterValue(&(cpu->rf), 1));
	//printRegisterFile(&(cpu->rf));

	getchar();
	getchar();

	free(memory);
	memory = NULL;
	free(cpu);
	cpu = NULL;
	return 0;
}
