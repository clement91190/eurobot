#ifndef IO_H
#define IO_H

#include <Period.h>
#include <Switch.h>
#include "Arduino.h"
#include <Servo.h>
#include "pins.h"

class IO
/** (x,y,cap) objects **/
{
    private:
        Servo servo_benne_c;
        Servo servo_benne_g;
        Servo servo_benne_d;

        Servo servo_rampe_g;
        Servo servo_rampe_d;

        Servo servo_tacle_g;
        Servo servo_tacle_d;

        Servo servo_pile_av_g;
        Servo servo_pile_av_d;
        Servo servo_pile_ar_g;
        Servo servo_pile_ar_d;

    public:
        IO();

};

#endif
