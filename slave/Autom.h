#ifndef AUTOM_H
#define AUTOM_H


#include "Period.h"
#include "Arduino.h"
#include "interrupts.h"
#include "pins.h"

class Coord
/** (x,y,cap) objects **/
{
    private:
    float x;
    float y;
    float cap; /*radians !*/
    public:
    Coord();
    Coord(float x_, float y_, float cap_);
    float get_x();
    float get_y();
    float get_cap();
    void set_x(float x_);
    void set_y(float y_);
    void set_cap(float cap_);
    void set_x_y_cap(float x_, float y_, float cap_);
    void forward_translation(float d);
    void write_serial();
};

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
