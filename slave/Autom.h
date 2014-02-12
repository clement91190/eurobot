#ifndef AUTOM_H
#define AUTOM_H


#include "Period.h"
#include "Arduino.h"
#include "interrupts.h"
#include "pins.h"
#include "Coord.h"

class Autom
{
    private:
        Coord real_coord;
        Period period_update_coords; /* loop to update the position of the robot*/
        Period period_pid_loop; /* loop of the control loop */
        float gain_odo_g;
        float gain_odo_d;
        float gain_inter_odos; /* 1 / dist inter odos */
        int last_ticG;
        int last_ticD;

    public:
        Autom();
        Coord get_real_coord();
        void send_cmd(int cmdG, int cmdD); 
        void update_cap();
        void update_coords();
        void reset_tics_odos();
        void run();
        
};
#endif
