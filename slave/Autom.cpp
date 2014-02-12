#include "Autom.h"
/* Autom implementation*/

Autom::Autom():
    real_coord(),
    period_update_coords(10),
    period_pid_loop(30),
    gain_odo_g(-0.408),
    gain_odo_d(-0.408),
    gain_inter_odos(0.00163038),
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
    float d = (delta_ticG * gain_odo_g + delta_ticD * gain_odo_d) * 0.5;   
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
        real_coord.write_serial();
    }
    if (period_pid_loop.is_over())
    {
        period_pid_loop.reset();
        /* add the pid here */
    }
}

