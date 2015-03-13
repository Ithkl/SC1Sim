#include <stdio.h>
#include "trap_commands.h"
#include "cpu_model.h"
#include "mem_model.h"

void getch(Io_monitor_p keyboard_io,  CPU_p cpu) {
    int theCharacter;
    fpurge(stdin);
    theCharacter = getchar();
    setData(theCharacter, keyboard_io);
    setReady(keyboard_io);
    setRegisterValue(&(cpu->rf), 0, getData(keyboard_io));
    setNotReady(keyboard_io);
}

void putch(Io_monitor_p monitor_io,  CPU_p cpu) {
    int theCharacter;
    theCharacter = getRegisterValue(&(cpu->rf), 0);
    setData(theCharacter, monitor_io);
    setReady(monitor_io);
    printf("%c", getData(monitor_io), 0);
    setNotReady(monitor_io);
}

void getst(Io_monitor_p keyboard_io, CPU_p cpu, Memory_p memory){
    int memoryLocation, currentCharacter;
    
    memoryLocation = getRegisterValue(&(cpu->rf), 0);
    //flush stdin
    fpurge(stdin);
    //get the first character
    currentCharacter = getchar();
    while (1){
        //If the character isn't a line return,
        //write it to the data register
        if(currentCharacter != CARRAGE_RETURN) {
            setData(currentCharacter & LOW_ORDER_BYTE_MASK, keyboard_io);
            setReady(keyboard_io);
                          
        }
        else {  //write a null zero to data
            setData('\0', keyboard_io);
            setReady(keyboard_io);
        }
        //If the io is ready to be read.
        if(getStatus(keyboard_io)) {
            //load the data and the address
            cpu->mdr = getData(keyboard_io);
            cpu->mar = memoryLocation;
            //write
            setMemoryValue(memory,cpu->mar,cpu->mdr);
            //set io to not ready.
            setNotReady(keyboard_io);
            //increment memory location
            memoryLocation++;
        }
        //if we just wrote a carrage return,
        //get out of the while loop
        if(currentCharacter == CARRAGE_RETURN){
            break;
        }
                                        //get the next character in stdin.
                                        currentCharacter = getchar();
                                    }
}

void putst(Io_monitor_p monitor_io, CPU_p cpu, Memory_p memory) {
    unsigned short input = getRegisterValue(&(cpu->rf), 0);
    unsigned char memoryValue = getMemoryValue(memory, input);
    while(memoryValue != 0) {
    setData(memoryValue, monitor_io);
    setReady(monitor_io);
    printf("%c", memoryValue);
    setNotReady(monitor_io);
    input++;
    memoryValue = getMemoryValue(memory, input);
   }
}
