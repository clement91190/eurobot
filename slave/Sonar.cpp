#include "Sonar.h"

Sonar::Sonar(int pin_, Coord dir_):pin(pin_), dir(dir_){}

float Sonar::get_dist(){
    int sensorValue = analogRead(pin); 
    return 10 + (sensorValue / 800.0) * 900. ; //divide by max sensor value, multiply by 900 mm

}

bool Sonar::adv_detected(){
    float d = get_dist();
    if (d < 500.0)
    {
        adv = Coord(dir);
        adv.forward_translation(d) ;
        return adv.is_on_map();
    }
    return false;
}

void Sonar::write_adv_coord(){
    adv.write_serial();
}

bool Sonar::mean_adv(Coord adv_){
   adv.barycentre(adv_, 0.5); 
}

Coord Sonar::get_adv(){
    return adv;
}
       
