#include "Autom.h"

float Coord::get_x(){return x;}

float Coord::get_y(){return y;}

float Coord::get_cap(){return cap;}

void Coord::set_x(float x_){x = x_;}

void Coord::set_y(float y_){y = y_;}

void Coord::set_cap(float cap_){cap = cap_;}

void Coord::set_x_y_cap(float x_, float y_, float cap_){
 x = x_;
 y = y_;
 cap = cap_;
}

void Coord::forward_translation(float d){
    x = x + d * cos(cap);
    y = y + d * sin(cap);
}

void Coord::write_serial()
{
    Serial.print("x");
    Serial.print(x);
    Serial.print("x");
    Serial.print(y);
    Serial.print("cap");
    Serial.println(cap);
}

Coord::Coord():x(0.0), y(0.0), cap(0.0){}

Coord::Coord(float x_, float y_, float cap_):x(x_), y(y_), cap(cap_){}

/* Autom implementation*/

Autom::Autom():
    real_coord(),
    period_update_coords(10),
    period_pid_loop(30),
    gain_odo_g(1.0),
    gain_odo_d(1.0),
    gain_inter_odos(1/20.0),
    last_ticG(0),
    last_ticD(0)
   {}

void Autom::update_cap(){
    /* attention ici, faudra tester la precision*/
    float cap = (ticD * gain_odo_d - ticG * gain_odo_d) * gain_inter_odos;       
    real_coord.set_cap(cap);
}

void Autom::update_coords(){
    /* peut etre prendre la moyenne des caps ou autre technique d'integration? */ 
    update_cap();
    int delta_ticG = ticG - last_ticG;
    int delta_ticD = ticD - last_ticD;
    float d = (delta_ticG * gain_odo_d + delta_ticD * gain_odo_d) * 0.5;   
    real_coord.forward_translation(d);
    /* maybe add a delta cond on distance to avoid noise ? */
    last_ticD = ticD;
    last_ticG = ticG;
};

void Autom::reset_tics_odos(){
    ticG = 0;
    ticD = 0;
}

void Autom::run(){
    /* this is where all the magic happends */
    
    if (period_update_coords.is_over())
    {
        period_update_coords.reset();
        update_coords();
    }
    if (period_pid_loop.is_over())
    {
        period_pid_loop.reset();
        /* add the pid here */
    }
}

