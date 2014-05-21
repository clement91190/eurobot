#include "interrupts.h"

//tics interrupt functions
//
//

//-> ok for PMI
long tic_odo = 0;
long pulse_color = 0;


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

 
 /*
 interruption capteur couleur
 */
void inc_pulse_color() {
  // increase pulse count
    pulse_color = pulse_color + 1;
 } 
 
