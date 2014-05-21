#include "ColorSensor.h"

ColorSensor::ColorSensor():
    period_reset(100)
{
   // periode d'echantillonage de remise a 0 pour la lecture de la couleur
}

bool ColorSensor::is_red()
{
   //TODO write constants for red and yellowlast_count
   return last_count > 1800;
}
bool ColorSensor::is_yellow()
{
    //return last_count < 1300;
    return (!is_red());

}

void ColorSensor::run()
{
    int dt;
    dt = millis() - last_time;
    if (period_reset.is_over())
    {
        period_reset.reset();
        if (dt > 0.)
        {
            last_count = 100. *  pulse_color / dt ;
            last_time = millis();
            pulse_color = 0;
        }
    }
}

void ColorSensor::write_debug()
{
    Serial.print("capteur couleur : freq ");
    Serial.print(last_count);
    Serial.print(" rouge ? ");
    Serial.println(is_red());
    Serial.print(" jaune ? ");
    Serial.println(is_yellow());
    
}
