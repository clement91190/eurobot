#ifndef SWITCH_ANALOG_H
#define SWITCH_ANALOG_H

#include "Arduino.h"
class SwitchAnalog
{
    // class for switches
    // ie bumper + IR
    private:
        int pin;
        bool reversed;
        int seuil;
    public:
        SwitchAnalog(int pin, int seuil);
        bool is_on();
        bool is_off();
        void reverse();
};

#endif
