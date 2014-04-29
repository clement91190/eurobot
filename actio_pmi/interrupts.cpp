#include "interrupts.h"

//tics interrupt functions
//
//

//-> ok for PMI
long tic_odo = 0;


void inc_tic_odo(){
    int chb = digitalRead(PIN_ODO_DIR) ;
    if (chb == 1)
    {
    tic_odo = tic_odo - 1;
    }
    else
    {
    tic_odo = tic_odo + 1; 
    }
}


