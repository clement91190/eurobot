#ifndef COLOR_H
#define COLOR_H

#include "Period.h"
#include "Arduino.h"
#include "interrupts.h"
#include "pins.h"

class ColorSensor 
{
    private:
        Period period_reset; 
        int last_count;
        long* pulse_color;
        long last_time;
    public:
        ColorSensor(long* pulse_color);
        bool is_red();
        bool is_yellow();
        void run();
        void write_debug();
    
};

#endif

