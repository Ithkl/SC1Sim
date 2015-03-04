#ifndef _MEM_MODEL_H
#define _MEM_MODEL_H

typedef unsigned char MemoryNode;
typedef MemoryNode * MemoryNode_p;

//Prototype for creating a memory node on the heap.
MemoryNode_p createMemoryNode(void);

#define MEMORY_SIZE 65535
//#define MEMORY_SIZE 25

//Definition of the memory structure.
typedef struct memory {
	MemoryNode mn[MEMORY_SIZE];
} Memory;

//Creates a nice pointer name for memory.
typedef Memory * Memory_p;

//Prototype methods
Memory_p createMemory(void);	// constructor
MemoryNode getMemoryValue(Memory_p m, int which);
void setMemoryValue(Memory_p m, int which, MemoryNode v);
void clearMemory(Memory_p m);
void printMemory(Memory_p m);

#endif //_MEM_MODEL_H
