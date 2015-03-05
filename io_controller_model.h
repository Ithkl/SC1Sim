
#ifndef IO_CONTROLLER_MODEL_H
#define	IO_CONTROLLER_MODEL_H
#include "rf_model.h"

#define NUMBER_OF_CONTROL_REGISTERS 2
#define STATUS 0
#define DATA 1

typedef struct {
    ByteRegister registerArray [NUMBER_OF_CONTROL_REGISTERS];
}Io_monitor;

typedef Io_monitor * Io_monitor_p;

Io_monitor_p createIoController();

void setReady(Io_monitor_p the_io_monitor);
void setNotReady(Io_monitor_p the_io_monitor);
int getStatus(Io_monitor_p the_io_monitor);
void setData(unsigned char the_data ,Io_monitor_p the_io_monitor);
unsigned char getData(Io_monitor_p the_io_monitor);
#endif	/* IO_CONTROLLER_MODEL_H */
