#include "interrupts.h"

//tics interrupt functions
//
//

//-> ok for PMI
long ticG = 0;
long ticD = 0;

void inc_ticG(){
    int chb;
    chb = digitalRead(PIN_ODO_DIRG);
    if (chb != 1)
    {
    ticG = ticG + 1;
    }
    else
    {
    ticG = ticG - 1; 
    }
}

void inc_ticD(){
    int chb = digitalRead(PIN_ODO_DIRD) ;
    if (chb != 1)
    {
    ticD = ticD - 1;
    }
    else
    {
    ticD = ticD + 1; 
    }
}


