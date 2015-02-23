#include <stdio.h>
#include <stdlib.h>
#include "mem_model.h"

// Register methods
MemoryNode_p createMemoryNode(void) {
	MemoryNode_p mn = (MemoryNode_p)malloc(sizeof(MemoryNode));
	return mn;
}
/*
int getSignedValue(MemoryNode_p mn) {
	return (signed short)*mn;
}

void putUnsignedValue(MemoryNode_p mn, int v) {
	*mn = (unsigned short)v & 0x00FF;
}
*/
// RegisterFile Methods

Memory_p createMemory(void) {
	Memory_p m = (Memory_p)malloc(sizeof(Memory));
	clearMemory(m);
	return m;		// actually a pointer to a register - the first one
}

void clearMemory(Memory_p m) {
	int i;
	for (i = 0; i<MEMORY_SIZE; i++) setMemoryValue(m, i, (MemoryNode)0x0);	// clear registers
}

void setMemoryValue(Memory_p m, int which, MemoryNode value) {
	m->mn[which] = value;
}

MemoryNode getMemoryValue(Memory_p m, int which) {
        MemoryNode returnNode = m->mn[which];
        return returnNode;
}

void printMemory(Memory_p m) {
	int i;
	for (i = 0; i<MEMORY_SIZE; i++) printf("memory at address %d content: %04X\n", i,
		getMemoryValue(m, i));
}
