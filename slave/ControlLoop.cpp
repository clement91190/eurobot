#include "ControlLoop.h"

ControlLoop::ControlLoop():
    pidcap(GAIN_KP_CAP, GAIN_KI_CAP, GAIN_KD_CAP, NEAR_ERROR_CAP, DONE_ERROR_CAP),
    piddep(GAIN_KP_DEP, GAIN_KI_DEP, GAIN_KD_DEP, NEAR_ERROR_DEP, DONE_ERROR_DEP),
    bf_type(STOP),
    asserv_state(DONE),
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
    asserv_state = FAR; 
    pidcap.reinit();
    piddep.reinit();
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


void ControlLoop::next_asserv_state(){
    switch (asserv_state){
        case FAR:
            asserv_state = NEAR ; 
            Serial.println("NEAR");
            break;
        case NEAR:
            asserv_state = DONE ;
            Serial.println("AFINI");
            bf_type = STOP;
            break;
    }
}

void ControlLoop::compute_pids(){
    /* update cmd_dep ;
     * cmd_cap */
    
    Vector to_target;
    //real_coord.write_serial();
    to_target = Vector(real_coord, target_position);
    switch (bf_type){
        case STOP:
            //Serial.println("coucou stop");
            cmd_cap = 0;
            cmd_dep = 0;
            break;
        case BFFW:
            //Serial.println("coucou BFFW");
            //cmd_cap = 0;
            //Serial.println(to_target.scalar(dir));
            cmd_dep = - piddep.compute(to_target.scalar(Vector(real_coord))); // the error is a scalar product
            cmd_cap = 0 // pidcap.compute(real_coord.get_cap());
            if (piddep.check_if_over(asserv_state)) // && pidcap.check_if_over(asserv_state))
            {
               next_asserv_state(); 
            }
            break;
        case BFCAP:
            //Serial.println("coucou BFCAP");
            //cmd_dep = 0;
            //Serial.println(real_coord.get_cap());
            //Serial.print ("  CAP  ");
            cmd_cap = pidcap.compute(real_coord.get_cap());
            //Serial.print ("  DEP  ");
            //cmd_dep = - piddep.compute(to_target.scalar(Vector(real_coord))); // the error is a scalar product
            cmd_dep = 0;

            //Vector(real_coord).write_serial();
            //to_target.write_serial();
            if (pidcap.check_if_over(asserv_state))
            {
               next_asserv_state(); 
            }
        
            //Serial.println(cmd_cap);
            break;
        case BFXYCAP:
            /* later ! */
            break;
    }
    //Serial.println("");
}

void ControlLoop::compute_cmds(){
    /* compute the command G and D of the motors */
    cmd_g = (cmd_dep / 2) - (cmd_cap / 2);
    cmd_d = (cmd_dep / 2) + (cmd_cap / 2);
      
    fw_g = cmd_g <= 0;
    fw_d = cmd_d <= 0;

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
    
       
