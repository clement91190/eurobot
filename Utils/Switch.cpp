#include "Switch.h"

Switch::Switch(int pin_):pin(pin_), reversed(false)
{
}
bool Switch::is_on()
{
    if (reversed)
    {
        return (digitalRead(pin) == LOW);
    }
    else
    {
        return (digitalRead(pin) == HIGH);
    }

}

bool Switch::is_off()
{
    if (reversed)
    {
        return (digitalRead(pin) == HIGH);
    }
    else
    {
        return (digitalRead(pin) == LOW);
    }

}

void Switch::reverse()
{
    reversed = !reversed;
}
