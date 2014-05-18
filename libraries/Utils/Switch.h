#ifndef SWITCH_H
#define SWITCH_H

#include "Arduino.h"
class Switch
{
    // class for switches
    // ie bumper + IR
    private:
        int pin;
        bool reversed;
    public:
        Switch(int pin);
        bool is_on();
        bool is_off();
        void reverse();
};

#endif
