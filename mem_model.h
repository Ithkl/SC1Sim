#ifndef _MEM_MODEL_H
#define _MEM_MODEL_H

typedef unsigned char MemoryNode;
typedef MemoryNode * MemoryNode_p;

MemoryNode_p createMemoryNode(void);
/*void clearMemoryNode(MemoryNode_p rf);
void setMemoryNode(MemoryNode_p rf, Memory value);*/

//#define MEMORY_SIZE 65535
#define MEMORY_SIZE 15

typedef struct memory {
	MemoryNode mn[MEMORY_SIZE];
} Memory;

typedef Memory * Memory_p;

Memory_p createMemory(void);	// constructor
MemoryNode getMemoryValue(Memory_p m, int which);
void setMemoryValue(Memory_p m, int which, MemoryNode v);
void clearMemory(Memory_p m);
void printMemory(Memory_p m);
/*int getContentLOB(Memory_p m, int which);	// returns LOB of register which
void putContentLOB(Memory_p m, int which, int v);	// puts an unsigned value from v into register which*/



#endif
