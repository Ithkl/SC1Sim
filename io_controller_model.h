/* 
 * File:   io_controller_model.h
 * Author: James
 *
 * Created on March 4, 2015, 11:43 AM
 */

#ifndef IO_CONTROLLER_MODEL_H
#define	IO_CONTROLLER_MODEL_H
#include "rf_model.h"

#define NUMBER_OF_CONTROL_REGISTERS 2
#define STATUS 0
#define DATA 1

typedef struct {
    ByteRegister registerArray [NUMBER_OF_CONTROL_REGISTERS];
}io_monitor;

typedef io_monitor * io_monitor_p;

void setReady(io_monitor_p);
void setNotReady(io_monitor_p);
int getStatus(io_monitor_p);
void setData(char ,io_monitor_p);
char getData(char ,io_monitor_p);
#endif	/* IO_CONTROLLER_MODEL_H */
