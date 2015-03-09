#include <stdio.h>
#include "trap_commands.h"
//#include "io_controller_model.h"
#include "cpu_model.h"
#include "mem_model.h"

void getstringtrap(Io_monitor_p keyboard_io, CPU_p cpu, Memory_p memory){
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
                                        if(currentCharacter = CARRAGE_RETURN){
                                            break;
                                        }
                                        //get the next character in stdin.
                                        currentCharacter = getchar();
                                    }
}
