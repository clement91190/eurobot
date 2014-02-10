#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "Arduino.h"
#include "pins.h"

//variables globales

extern long ticG;
extern long ticD;

extern void inc_ticG();
extern void inc_ticD();

#endif
