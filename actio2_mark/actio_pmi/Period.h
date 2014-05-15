#ifndef PERIOD_H
#define PERIOD_H

#include "Arduino.h"


class Period
{
    public:
        Period(int period_);
        bool is_over();
        void reset();
    private:
        long period;
        long time_last_reset;

};


#endif
