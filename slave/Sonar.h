#ifndef SONAR_H
#define SONAR_H

#include "Arduino.h"
#include "Coord.h"

class Sonar
/** class for using US sensor and detect the position
 * of the adversary**/
{
   private:
       int pin;
       Coord dir; // direction and offset of sensor compared to the center of the robot
       Coord adv;
   public:
        Sonar(int pin_, Coord dir_);
        float get_dist();
        bool adv_detected();
        bool mean_adv(Coord adv);
        Coord get_adv();
        void write_adv_coord();
};


#endif
