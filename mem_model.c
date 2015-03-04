#include <stdio.h>
#include <stdlib.h>
#include "mem_model.h"

//Creates a block of memory on the heap
MemoryNode_p createMemoryNode(void) {
	MemoryNode_p mn = (MemoryNode_p)malloc(sizeof(MemoryNode));
	return mn;
}

// Memory Methods

//Creates a memory node on the heap
Memory_p createMemory(void) {
	Memory_p m = (Memory_p)malloc(sizeof(Memory));
	clearMemory(m);
	return m;		// actually a pointer to a register - the first one
}
//Clears the memory
void clearMemory(Memory_p m) {
	int i;
	for (i = 0; i<MEMORY_SIZE; i++) setMemoryValue(m, i, (MemoryNode)0x0);	// clear registers
}

//Sets a value into memory.
void setMemoryValue(Memory_p m, int which, MemoryNode value) {
	m->mn[which] = value;
}

//Gets a value from memory.
MemoryNode getMemoryValue(Memory_p m, int which) {
        MemoryNode returnNode = m->mn[which];
        return returnNode;
}

//Prints the values in memory onto the console.
void printMemory(Memory_p m) {
	int i;
	for (i = 0; i<MEMORY_SIZE; i++) printf("memory at address %d content: %04X\n", i,
		getMemoryValue(m, i));
}
