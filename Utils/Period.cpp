#include "Period.h"

Period::Period(int period_):period(period_)
{
    reset();
}


void Period::reset()
{
    time_last_reset = millis();
}

bool Period::is_over(){
    long t = millis();
    if (t - time_last_reset > period){
    return true;
    }
    else
    {
    return false;
    }

}

