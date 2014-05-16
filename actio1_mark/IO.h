#ifndef IO_H
#define IO_H

#include <Period.h>
#include <Switch.h>
#include "Arduino.h"
#include <Servo.h>
#include "interrupts.h"
#include "pins.h"
#include "ColorSensor.h"

class IO
/** (x,y,cap) objects **/
{
    private:
        Servo servo_rot_g;
        Servo servo_rot_d;

    public:
        IO();
        void run();
        void trigger(int transition);

};

#endif
