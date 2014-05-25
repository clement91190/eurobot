#include "SwitchAnalog.h"

SwitchAnalog::SwitchAnalog(int pin_, int seuil_):pin(pin_), seuil(seuil_), reversed(false)
{
}

bool SwitchAnalog::is_on()
{
    if (reversed)
    {
        return (analogRead(pin) > seuil);
    }
    else
    {
        return (analogRead(pin) <= seuil);
    }

}

bool SwitchAnalog::is_off()
{
    return (!is_on());
}

void SwitchAnalog::reverse()
{
    reversed = !reversed;
}
