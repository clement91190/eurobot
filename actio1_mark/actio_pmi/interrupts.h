#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "Arduino.h"
#include "pins.h"

//variables globales

extern long tic_odo;
extern long pulse_color;

extern void inc_tic_odo();
extern void inc_pulse_color();

#endif
