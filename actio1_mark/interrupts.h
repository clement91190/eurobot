#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "Arduino.h"
#include "pins.h"

//variables globales

extern long pulse_color_g;
extern long pulse_color_d;

extern void inc_pulse_color_g();
extern void inc_pulse_color_d();

#endif
