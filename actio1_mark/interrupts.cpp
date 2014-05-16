#include "interrupts.h"

//tics interrupt functions
//
//

//-> ok for PMI
long pulse_color_g = 0;
long pulse_color_d = 0;

  
 /*
 interruption capteur couleur
 */
void inc_pulse_color_g() {
 
  // increase pulse count
    pulse_color_g += 1 ;
 } 
 
 /*
 interruption capteur couleur
 */
void inc_pulse_color_d() {
 
  // increase pulse count
    pulse_color_d += 1;
 } 
 
