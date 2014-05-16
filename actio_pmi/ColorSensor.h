#ifndef COLOR_H
#define COLOR_H

#include "Period.h"
#include "Arduino.h"
//#include <Servo.h>
#include "interrupts.h"
#include "pins.h"

class ColorSensor 
{
    private:
        Period period_reset; 
        int last_count;
    public:
        ColorSensor();
        bool is_red();
        bool is_yellow();
        void run();
    
};

#endif

