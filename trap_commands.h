#include "cpu_model.h"
#include "mem_model.h"
#include "io_controller_model.h"


#ifndef TRAP_COMMANDS_H
#define	TRAP_COMMANDS_H


#define CARRAGE_RETURN '\n'

void getstringtrap(Io_monitor_p keyboard_io, CPU_p cpu, Memory_p memory);
#endif	/* TRAP_COMMANDS_H */

