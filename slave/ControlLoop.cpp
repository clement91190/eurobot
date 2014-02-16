#include "ControlLoop.h"

ControlLoop::ControlLoop():
    pidcap(GAIN_KP_CAP, GAIN_KI_CAP, GAIN_KD_CAP),
    piddep(GAIN_KP_DEP, GAIN_KI_DEP, GAIN_KD_DEP),
    bf_type(STOP),
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

void ControlLoop::set_BF(int bf_type_, Coord target_position_){

    bf_type = bf_type_;
    float d;
    switch(bf_type){
        case STOP:
            break;
        case BFFW:
            target_position.forward_translation(target_position_.get_x());
            dir = Vector(real_coord, target_position);
            if (target_position_.get_x() < 0){
                dir.neg();
            }
            //d = dir.norm();
            dir.normalize();    
            piddep.setTarget(0.0);
            break;
        case BFCAP:
            target_position.set_cap(target_position_.get_cap());
            pidcap.setTarget(target_position.get_cap());
            //Serial.println(target_position.get_cap());
            break;
        case BFXYCAP:
            break;
    }

}

void ControlLoop::compute_pids(){
    /* update cmd_dep ;
     * cmd_cap */
    
    Vector to_target;
    target_position.write_serial();
    switch (bf_type){
        case STOP:
            cmd_cap = 0;
            cmd_dep = 0;
        case BFFW:
            cmd_cap = 0;
            to_target = Vector(real_coord, target_position);
            //Serial.println(to_target.scalar(dir));
            cmd_dep = -piddep.compute(to_target.scalar(dir)); // the error is a scalar product
            break;
        case BFCAP:
            cmd_dep = 0;
            //Serial.println(real_coord.get_cap());
            cmd_cap = pidcap.compute(real_coord.get_cap());
            //Serial.println(cmd_cap);
            break;
        case BFXYCAP:
            /* later ! */
            break;
    }
}

void ControlLoop::compute_cmds(){
    /* compute the command G and D of the motors */
    cmd_g = (cmd_dep / 2) - (cmd_cap / 2);
    cmd_d = (cmd_dep / 2) + (cmd_cap / 2);
      
    fw_g = cmd_g >= 0;
    fw_d = cmd_d >= 0;

    if (cmd_g <0){ cmd_g = - cmd_g;}
    if (cmd_d <0){ cmd_d = - cmd_d;}

}

void ControlLoop::run(Coord real_coord_){
    real_coord = real_coord_;
    compute_pids();
    compute_cmds();
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
    
       
