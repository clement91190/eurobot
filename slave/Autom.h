#ifndef AUTOM_H
#define AUTOM_H


#include "Period.h"
#include "Arduino.h"
#include "interrupts.h"
#include "pins.h"
#include "Coord.h"
#include "ControlLoop.h"

class Autom
{
    private:
        Coord real_coord;
        ControlLoop control;
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
        void send_cmd(); 
        void write_cmd(int cmd_g, int cmd_d, bool fw_g, bool fw_d);
        void update_cap();
        void update_coords();
        void reset_tics_odos();
        void run();
        ControlLoop* get_control();
        
};

void write_serial_strat();
#endif
