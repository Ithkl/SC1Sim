#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "cpu_model.h"
#include "mem_model.h"
#include "load_store_op.h"
#include "jumps_model.h"
#include "parser_model.h"
#include "debug_monitor_model.h"

#define PROGRAM_START_LOCATION 0x3000;

//2^16
//unsigned char memory[65536];

int main() {
	int rd_loc, ra_loc, rx_loc, immediate, args, opcode;
	int halt = 0;
	int executeState = MENU_STATE;
	//char fileName[40];
	unsigned short dump_location = PROGRAM_START_LOCATION;
	int fileState = -1;
    //char fileName[] = "C:\\Users\\James\\Desktop\\add.txt";
	//ALUTest();
	//memTest();
	//cpu_mem_pipeline_test();
	//load_store_test();
	//jump_test();    
	CPU_p cpu = createCPU();
	Memory_p memory = createMemory();
        //We'll be loading the file around here.
	cpu->pc = PROGRAM_START_LOCATION;
        //parserTest();
        //parser(&fileName, memory, cpu);
        while(executeState != EXIT_STATE) {
            while (executeState == MENU_STATE){
            display(cpu, memory, dump_location);
            command_prompt(&fileState, &executeState, &dump_location, cpu, memory);
            }
            while (!halt && executeState != EXIT_STATE)
            {
                fetch(cpu, memory);
                decode(&opcode, &rd_loc, &ra_loc, &rx_loc, &args, &immediate, cpu, memory);
                execute(opcode, rd_loc, ra_loc, rx_loc, args, immediate, &halt, cpu, memory);
                if (executeState == STEP_STATE) {
                    display(cpu, memory, cpu->pc);
                    printf("\nPress enter key to continue...");
                    stdinFlush();
                    getchar();
                    
                }
            }
            if (executeState != EXIT_STATE){
                executeState = MENU_STATE;
            }
        }
	/*display(cpu, memory, cpu->pc);
        printf("\n\nPress any key to continue...");
        getchar();*/

	free(memory);
	memory = NULL;
	free(cpu);
	cpu = NULL;
	return EXIT_SUCCESS;
}

int decode(int * opcode, int * rd_loc, int * ra_loc, int * rx_loc, int * args, int * immediate, CPU_p cpu, Memory_p memory) {
    *opcode = getOperation(cpu->ir);
    switch (*opcode){
		case ADD:
			//Get the register locations from the Instruction Register.
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			//Load Ra and Rx into ALU registers A and B.
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc), getRegisterValue(&(cpu->rf), *rx_loc));
			break;

		case SUB:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc), getRegisterValue(&(cpu->rf), *rx_loc));
			break;

		case MUL:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc) & LOW_ORDER_BYTE_MASK, getRegisterValue(&(cpu->rf), *rx_loc) & LOW_ORDER_BYTE_MASK);
			break;

		case DIV:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc), getRegisterValue(&(cpu->rf), *rx_loc));
			break;

		case AND:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc), getRegisterValue(&(cpu->rf), *rx_loc));
			break;

		case OR:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc), getRegisterValue(&(cpu->rf), *rx_loc));
			break;
		case NOT:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			cpu->alu.A = getRegisterValue(&(cpu->alu), *ra_loc);
			break;

		case XOR:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc), getRegisterValue(&(cpu->rf), *rx_loc));
			break;

		case SHL:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*args = decodeArgument1(cpu->ir);
			*immediate = decodeRxAndImmd3(cpu->ir);
			cpu->alu.A = getRegisterValue(&(cpu->alu), *ra_loc);
			break;

		case LDB:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*immediate = decodeImmed5(cpu->ir);
			break;
		case LDW:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*immediate = decodeImmed5(cpu->ir);
			break;
		case LDBR:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			break;
		case LDWR:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			break;
		case LDI:
			*rd_loc = decodeRd(cpu->ir);
			*immediate = decodeImmed8(cpu->ir);
			break;
		case LEA:
			*rd_loc = decodeRd(cpu->ir);
			*immediate = decodeImmed8(cpu->ir);
			break;
		case MOV:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			break;
		case STB:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*immediate = decodeImmed5(cpu->ir);
			break;
		case STW:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*immediate = decodeImmed5(cpu->ir);
			break;
		case STBR:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			break;
		case STWR:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			break;
		case BR:
			*immediate = decodeImmed8(cpu->ir);
			break;
		case BRN:
			*immediate = decodeImmed8(cpu->ir);
			break;
		case BRZ:
			*immediate = decodeImmed8(cpu->ir);
			break;
		case BRC:
			*immediate = decodeImmed8(cpu->ir);
			break;
		case BRO:
			*immediate = decodeImmed8(cpu->ir);
			break;
		case JMP:
			*rx_loc = decodeRxAndImmd3(cpu->ir);
			break;
		case RET:
                    *args = decodeArgument2(cpu->ir);
                    *immediate = decodeImmed10(cpu->ir);	
			break;
		case PUSH:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			break;
		case POP:
			*rd_loc = decodeRd(cpu->ir);
			*ra_loc = decodeRa(cpu->ir);
			break;
		default:
			break;
		}
}

int execute(int opcode,int rd_loc, int ra_loc, int rx_loc, int args, int immediate, int * halt, CPU_p cpu, Memory_p memory){
    
    switch (opcode){
		case ADD:
			//Execute
			add(&(cpu->alu));
			//Set Rd as the value of ALU register R
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			//Set the flags on the ALU and transfer them over.
			setALU_Flags(&(cpu->alu), ADD);
			setSWRegFlags(cpu);
			break;

		case SUB:
			sub(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), SUB);
			setSWRegFlags(cpu);
			break;

		case MUL:
			mul(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), MUL);
			setSWRegFlags(cpu);
			break;

		case DIV:
			divide(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			//Ra gets remainder
			setRegisterValue(&(cpu->rf), ra_loc, cpu->alu.A);
			setALU_Flags(&(cpu->alu), MUL);
			setSWRegFlags(cpu);
			break;

		case AND:
			and(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), AND);
			setSWRegFlags(cpu);
			break;

		case OR:
			or(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), OR);
			setSWRegFlags(cpu);
			break;
		case NOT:
			not(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), NOT);
			setSWRegFlags(cpu);
			break;

		case XOR:
			xor(&(cpu->alu));
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), XOR);
			setSWRegFlags(cpu);
			break;

		case SHL:
			if (args) {
				//SHR
				shr(&(cpu->alu), immediate);
				setALU_Flags(&(cpu->alu), SHR);
			}
			else {
				//SHL
				shl(&(cpu->alu), immediate);
				setALU_Flags(&(cpu->alu), SHL);
			}
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setSWRegFlags(cpu);
			break;

		case LDB:
			ldb(rd_loc, ra_loc, immediate, cpu, memory);
			break;
		case LDW:
			ldw(rd_loc, ra_loc, immediate, cpu, memory);
			break;
		case LDBR:
			ldbr(rd_loc, ra_loc, rx_loc, cpu, memory);
			break;
		case LDWR:
			ldwr(rd_loc, ra_loc, rx_loc, cpu, memory);
			break;
		case LDI:
			ldi(rd_loc, immediate, cpu);
			break;
		case LEA:
			lea(rd_loc, immediate, cpu, memory);
			break;
		case MOV:
			mov(rd_loc, ra_loc, cpu);
			break;
		case STB:
			stb(rd_loc, ra_loc, immediate, cpu, memory);
			break;
		case STW:
			stb(rd_loc, ra_loc, immediate, cpu, memory);
			break;
		case STBR:
			stbr(rd_loc, ra_loc, rx_loc, cpu, memory);
			break;
		case STWR:
			stwr(rd_loc, ra_loc, rx_loc, cpu, memory);
			break;
		case BR:
			br(immediate, cpu);
			break;
		case BRN:
			brN(immediate, cpu);
			break;
		case BRZ:
			brZ(immediate, cpu);
			break;
		case BRC:
			brC(immediate, cpu);
			break;
		case BRO:
			brO(immediate, cpu);
			break;
		case JMP:
			jmp(rx_loc, cpu, memory);
			break;
		case RET:
				if (args) {
					switch (immediate) {
					case 23:
						*halt = 1;
                                                break;
					default:
						break;
					}
				}
				else {
					ret(cpu, memory);
				}
			break;
		case PUSH:
			push(rd_loc, ra_loc, cpu, memory);
			break;
		case POP:
			pop(rd_loc, ra_loc, cpu, memory);
			break;
		default:
			break;
		}
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

int parserTest()
{
    int parserflag;
    CPU_p cpu = createCPU();
    Memory_p memory = createMemory();
    char fileName[] = "C:\\Users\\James\\Desktop\\add.txt";
    
    parserflag = parser(&fileName, memory, cpu);
    printf("%d", parserflag);
    printMemory(memory);
    return 0;
}
