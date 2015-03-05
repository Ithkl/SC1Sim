
#include <stdlib.h>

#include "io_controller_model.h"

Io_monitor_p createIoController(){
    Io_monitor_p io_p = (Io_monitor_p) malloc(sizeof Io_monitor);
    return io_p;
}

void setReady(Io_monitor_p the_io_monitor) {
    the_io_monitor->registerArray[STATUS] = 1;
}

void setNotReady(Io_monitor_p the_io_monitor) {
    the_io_monitor->registerArray[STATUS] = 0;
}
int getStatus(Io_monitor_p the_io_monitor){
    int the_status;
    the_status = the_io_monitor->registerArray[STATUS];
    return the_status;
}
void setData(unsigned char the_data,Io_monitor_p the_io_monitor) {
    the_io_monitor->registerArray[DATA] = the_data;
}
unsigned char getData(Io_monitor_p the_io_monitor) {
    unsigned char the_data;
    the_data = the_io_monitor->registerArray[DATA];
    return the_data;
}