#include "Sonar.h"

Sonar::Sonar(int pin_, Coord dir_):pin(pin_), dir(dir_), on(true){}

float Sonar::get_dist(){
    int sensorValue = analogRead(pin); 
    if (sensorValue == 0)
    {
        Serial.println("warning check sonar connection");
    }
    return 10 + (sensorValue / 800.0) * 900. ; //divide by max sensor value, multiply by 900 mm

}

bool Sonar::adv_detected(){
    if (!on) return false;
    float d = get_dist();
    if (10 < d && d < 500.0)
    {
        adv = Coord(dir);
        adv.forward_translation(d) ;
        return adv.is_on_map();
    }
    return false;
}

void Sonar::turn_on(){
    on = true;
}

void Sonar::turn_off(){
    on = true;
}

void Sonar::write_debug(){
    Serial.println(analogRead(pin));
}

void Sonar::write_adv_coord(){
    Serial.print("#ADVD");
    adv.write_serial();
}

bool Sonar::mean_adv(Coord adv_){
   adv.barycentre(adv_, 0.5); 
}

Coord Sonar::get_adv(){
    return adv;
}
       
