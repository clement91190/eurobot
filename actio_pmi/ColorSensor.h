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
        float last_count;
        long last_time;
    public:
        ColorSensor();
        bool is_red();
        bool is_yellow();
        void run();
        void write_debug();
    
};

#endif

