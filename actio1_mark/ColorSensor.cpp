#include "ColorSensor.h"

ColorSensor::ColorSensor(long* pulse_color):
    period_reset(80), pulse_color(pulse_color)
{
   // periode d'echantillonage de remise a 0 pour la lecture de la couleur
}

bool ColorSensor::is_red()
{
   //TODO write constants for red and yellowlast_count
   return true;
}
bool ColorSensor::is_yellow()
{
    return false;
}

void ColorSensor::run()
{
    if (period_reset.is_over())
    {
        period_reset.reset();
        last_count = (*pulse_color);
        (*pulse_color) = 0;
    }
}
 
