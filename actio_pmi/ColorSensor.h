#ifndef COLOR_H
#define COLOR_H

#include "Period.h"
#include "Arduino.h"
//#include <Servo.h>
#include "interrupts.h"
#include "pins.h"

#define SEUIL_ROUGE 3000//sup a
#define SEUIL_JAUNE 1300//inf a


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

