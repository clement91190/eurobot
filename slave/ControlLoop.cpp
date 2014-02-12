#include "ControlLoop.h"

ControlLoop::ControlLoop():
    pidcap(),
    piddep(),
    cmd_d(0),
    cmd_g(0),
    fw_d(true),
    fw_g(true){
};

void ControlLoop::set_target(Coord coord){

}
void ControlLoop::update_error(Coord coord){}

void ControlLoop::bf_avance(float d){
    piddep.addToTarget(d);
}

void ControlLoop::run(Coord real_coord){
    int cmd_dep = piddep.compute(real_coord.get_x()); 
    if (cmd_dep >= 0){
        cmd_g = cmd_dep / 2;
        cmd_d = cmd_dep / 2;
        fw_g = true;
        fw_d = true;
    }
    else
    {
        cmd_g = -cmd_dep / 2;
        cmd_d = -cmd_dep / 2;
        fw_g = false;
        fw_d = false;
    }

}

int ControlLoop::get_cmd_g(){
    return cmd_g;
}

bool ControlLoop::get_fw_g(){
    return fw_g;
}

int ControlLoop::get_cmd_d(){
    return cmd_d;
}

bool ControlLoop::get_fw_d(){
    return fw_d;
}
    
       
