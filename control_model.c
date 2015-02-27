
#include "control_model.h"
#include "alu_model.h"
#include "load_store_op.h"

//Gets the opcode out of the instruction.
int getOperation(Register ir){
	return ir >> 11;
}

//Gets the immediate 8 out of an instruction.
int decodeImmed8(Register ir){
	return ir & IMMED8_MASK;
}

//Decode the number of the destination register.
int decodeRd(Register ir){
	int rd;
        //Move the rd value down to the end of the opcode
	rd = ir >> 8;
        //mask it out.
	rd = rd & REGISTER_MASK;
	return rd;
}
//Decode the number of the address register.
int decodeRa(Register ir) {
	int ra;
        //Move the rd value down to the end of the opcode
	ra = ir >> 5;
        //mask it out.
	ra = ra & REGISTER_MASK;
	return ra;
}
//For use with the immediate 3.
int decodeRxAndImmd3(Register ir) {
	int rx;
        //Move the rd value down to the end of the opcode
	rx = (ir >> 2);
        //mask it out.
	rx = (rx & REGISTER_MASK);
	return rx;
}
//For use with the immediate 5.
int decodeImmed5(Register ir){
	return ir & IMMED5_MASK;
}

//For use with SHL and SHR
int decodeArgument1(Register ir){
	return ir & ARGS1_MASK;
}
//For use with TRAP and RET
int decodeArgument2(Register ir){
	return ir & ARGS2_MASK;
}
//Gets out the immediate 10 for trap commands
int decodeImmed10(Register ir){
	return ir & IMMED10_MASK;
}

void fetch(CPU_p cpu, Memory_p memory){

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

void decode(int * opcode_ptr, int * rd_loc_ptr, int * ra_loc_ptr, int * rx_loc_ptr, int * args_ptr, int * immediate_ptr, CPU_p cpu) {
    *opcode_ptr = getOperation(cpu->ir);
    //Decode the opcode so we know what registers to prep.
    switch (*opcode_ptr){
		case ADD:
			//Get the register locations from the Instruction Register.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
			//Load Ra and Rx into ALU registers A and B.
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc_ptr), getRegisterValue(&(cpu->rf), *rx_loc_ptr));
			break;

		case SUB:
                        //Get the register locations.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
                        //Prep the A and B registers with the RA and RX Registers.
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc_ptr), getRegisterValue(&(cpu->rf), *rx_loc_ptr));
			break;

		case MUL:
                        //Get the register locations.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
                        //Prep the A and B registers with the RA and RX Registers.
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc_ptr) & LOW_ORDER_BYTE_MASK, getRegisterValue(&(cpu->rf), *rx_loc_ptr) & LOW_ORDER_BYTE_MASK);
			break;

		case DIV:
                        //Get the register locations.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
                        //Prep the A and B registers with the RA and RX Registers.
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc_ptr), getRegisterValue(&(cpu->rf), *rx_loc_ptr));
			break;

		case AND:
                        //Get the register locations.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
                        //Prep the A and B registers with the RA and RX Registers.
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc_ptr), getRegisterValue(&(cpu->rf), *rx_loc_ptr));
			break;

		case OR:
                        //Get the register locations.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
                        //Prep the A and B registers with the RA and RX Registers.
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc_ptr), getRegisterValue(&(cpu->rf), *rx_loc_ptr));
			break;
		case NOT:
                        //Get the register locations.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
                        //Prep the A Register with Ra.
			cpu->alu.A = getRegisterValue(&(cpu->alu), *ra_loc_ptr);
			break;

		case XOR:
                        //Get the register locations.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
                        //Prep the A and B registers with the RA and RX Registers.
			loadAandBReg(&(cpu->alu), getRegisterValue(&(cpu->rf), *ra_loc_ptr), getRegisterValue(&(cpu->rf), *rx_loc_ptr));
			break;

		case SHL:
                        //Get the register locations and the immediate.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*args_ptr = decodeArgument1(cpu->ir);
			*immediate_ptr = decodeRxAndImmd3(cpu->ir);
                        //Prep the A Register with Ra.
			cpu->alu.A = getRegisterValue(&(cpu->alu), *ra_loc_ptr);
			break;

		case LDB:
                        //Get the register locations and the immediate.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*immediate_ptr = decodeImmed5(cpu->ir);
			break;
		case LDW:
                        //Get the register locations and the immediate.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*immediate_ptr = decodeImmed5(cpu->ir);
			break;
		case LDBR:
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
			break;
		case LDWR:
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
			break;
		case LDI:
                        //Get the register locations and the immediate.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*immediate_ptr = decodeImmed8(cpu->ir);
			break;
		case LEA:
                        //Get the register locations and the immediate.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*immediate_ptr = decodeImmed8(cpu->ir);
			break;
		case MOV:
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			break;
		case STB:
                        //Get the register locations and the immediate.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*immediate_ptr = decodeImmed5(cpu->ir);
			break;
		case STW:
                        //Get the register locations and the immediate.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*immediate_ptr = decodeImmed5(cpu->ir);
			break;
		case STBR:
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
			break;
		case STWR:
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
			break;
		case BR:
			*immediate_ptr = decodeImmed8(cpu->ir);
			break;
		case BRN:
			*immediate_ptr = decodeImmed8(cpu->ir);
			break;
		case BRZ:
			*immediate_ptr = decodeImmed8(cpu->ir);
			break;
		case BRC:
			*immediate_ptr = decodeImmed8(cpu->ir);
			break;
		case BRO:
			*immediate_ptr = decodeImmed8(cpu->ir);
			break;
		case JMP:
			*rx_loc_ptr = decodeRxAndImmd3(cpu->ir);
			break;
		case RET:
                    //Get Argument bits and immediate.
                    *args_ptr = decodeArgument2(cpu->ir);
                    *immediate_ptr = decodeImmed10(cpu->ir);	
			break;
		case PUSH:
                        //Get registers.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			break;
		case POP:
                        //Get registers.
			*rd_loc_ptr = decodeRd(cpu->ir);
			*ra_loc_ptr = decodeRa(cpu->ir);
			break;
		default:
			break;
		}
}

void execute(int opcode,int rd_loc, int ra_loc, int rx_loc, int args, int immediate, int * halt_ptr, CPU_p cpu, Memory_p memory) {
    //Check the opcode so we know what command to execute.
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
                        //Execute
			sub(&(cpu->alu));
                        //write back
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), SUB);
			setSWRegFlags(cpu);
			break;

		case MUL:
                        //Execute
			mul(&(cpu->alu));
                        //Write back
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), MUL);
			setSWRegFlags(cpu);
			break;

		case DIV:
                        //execute
			divide(&(cpu->alu));
                        //writeback
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			//Ra gets remainder
			setRegisterValue(&(cpu->rf), ra_loc, cpu->alu.A);
			setALU_Flags(&(cpu->alu), MUL);
			setSWRegFlags(cpu);
			break;

		case AND:
                        //execute
			and(&(cpu->alu));
                        //Writeback
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), AND);
			setSWRegFlags(cpu);
			break;

		case OR:
                        //execute
			or(&(cpu->alu));
                        //Writeback
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), OR);
			setSWRegFlags(cpu);
			break;
		case NOT:
                        //execute
			not(&(cpu->alu));
                        //writeback
			setRegisterValue(&(cpu->rf), rd_loc, cpu->alu.R);
			setALU_Flags(&(cpu->alu), NOT);
			setSWRegFlags(cpu);
			break;

		case XOR:
                        //execute
			xor(&(cpu->alu));
                        //writeback
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
                                case HALT:
                                    //HALT
                                    *halt_ptr = 1;
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
