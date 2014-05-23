#include "ControlLoop.h"

ControlLoop::ControlLoop():
    pidcap(CAP, GAIN_KP_CAP, GAIN_KI_CAP, GAIN_KD_CAP, NEAR_ERROR_CAP, DONE_ERROR_CAP),
    piddep(DEP, GAIN_KP_DEP, GAIN_KI_DEP, GAIN_KD_DEP, NEAR_ERROR_DEP, DONE_ERROR_DEP),
    bf_type(STOP),
    asserv_state(DONE),
    cmd_d(0),
    cmd_g(0),
    fw_d(true),
    real_coord(),
    target_position(),
    count_not_moving(0),
    late_pos(),
#ifdef PMI
    sonarg(PIN_AN_SONARG, Coord(0., 0., 0.)),
#else
    sonarg(PIN_AN_SONARG, Coord(10., 10., 5.)),
    sonard(PIN_AN_SONARD, Coord(10., -10., -5.)),
#endif

    fw_g(true){
    piddep.setMinMax(100);
    //sonarg.turn_off();
    //
};

void ControlLoop::set_target(Coord coord){

}
void ControlLoop::update_error(Coord coord){}

void ControlLoop::bf_avance(float d){
    piddep.addToTarget(d);
}

void ControlLoop::set_speed(int speed)
{
    switch(speed){
        case SLOW:
            piddep.setMinMax(42);
            pidcap.setMinMax(42);
            break;

        case MEDIUM:
            piddep.setMinMax(80);
            pidcap.setMinMax(80);
            break;

        case FAST:
            piddep.setMinMax(150);
            pidcap.setMinMax(150);
            break;
    }
}


void ControlLoop::recaler(){
    set_speed(SLOW);
    set_BF(BFFW, Coord(-1000, 0, 0));
}


void ControlLoop::set_BF(int bf_type_, Coord target_position_){
    
    count_not_moving = 0;
    bf_type = bf_type_;
    asserv_state = FAR; 
    pidcap.reinit();
    piddep.reinit();
    float d;
    switch(bf_type){
        case STOP:
            Serial.println("STOP");
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
            pidcap.setTarget(target_position.get_cap());
            break;
        case BFCAP:
            target_position.set_cap(target_position_.get_cap());
            pidcap.setTarget(target_position.get_cap());
            //Serial.println(target_position.get_cap());
            break;
        case BFXYCAP:
            target_position = target_position_;
            piddep.setTarget(0.0);
            pidcap.setTarget(target_position.get_cap());
            break;
    }

}


void ControlLoop::next_asserv_state(){
    switch (asserv_state){
        case FAR:
            asserv_state = NEAR ; 
            write_real_coords();
            Serial.println("# NEAR");


            
            break;
        case NEAR:
            asserv_state = DONE ;
            write_real_coords();
            Serial.println("# AFINI");
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
            cmd_dep = piddep.compute( to_target.scalar(Vector(real_coord))); // the error is a scalar product
            if (asserv_state != NEAR)
            {
                if (abs(diff_cap(to_target.get_angle(), target_position.get_cap())) > PI / 2)
                {
                    pidcap.setTarget(to_target.get_angle() + PI);
                }
                else
                {
                    pidcap.setTarget(to_target.get_angle());
                }
            }
            else
            {
                pidcap.setTarget(target_position.get_cap());
            }
            cmd_cap = pidcap.compute(real_coord.get_cap());
            if (piddep.check_if_over(asserv_state)  && pidcap.check_if_over(asserv_state))
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
            float B; // see supaero report 2012
            B = max(min(2.0, abs(to_target.norm()) / 50.0), 0.1); 
            if (count_not_moving > 4)
            {
                B = 0.;
            }


            /*if (asserv_state == NEAR)
            {
                B = 3.0;
            }
            else
            {
                B = 1.0;
            }*/

      //      float input;
            cmd_cap = pidcap.compute(real_coord.get_cap() - B * (diff_cap(to_target.get_angle(), target_position.get_cap())));
        /*    if (to_target.scalar(Vector(real_coord)) > 0)
            {
               input = to_target.norm(); 
            }
            else
            {
                input = -to_target.norm();
            }*/
            cmd_dep = piddep.compute(to_target.scalar(Vector(real_coord))); // the error is a scalar product
            if (piddep.check_if_over(asserv_state)  && pidcap.check_if_over(asserv_state))
            {
               next_asserv_state(); 
            }
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

    if (bf_type == BFFW)
    {
        check_adversary();
    }
    check_blockage();
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
    
void ControlLoop::check_blockage()
{
    /*code to test if moving when commands are sent*/
    late_pos.barycentre(real_coord, 0.3);
   Vector dep = Vector(real_coord, late_pos);
   if (abs(cmd_dep) + abs(cmd_cap) < 40){
    return;}
   if (dep.norm() < 10.0 && abs(real_coord.get_cap() - late_pos.get_cap()) < 0.05)
   {
        //Serial.println(abs(real_coord.get_cap() - late_pos.get_cap()));
        count_not_moving += 1;
        Serial.println("INC BLOC COUNT");
        
   }
   else{
        count_not_moving = 0;
   }
   
   if (count_not_moving > 15)
   {
        write_real_coords();
        Serial.println("# BLOC");

        set_BF(STOP, Coord());
        count_not_moving = 0;
   }


}

void ControlLoop::check_adversary()
{
    Vector to_target;
    to_target = Vector(real_coord, target_position);
   
    if (to_target.scalar(Vector(real_coord)) <0.)
    {
    //Serial.println("going backward...");
        return;
    }

#ifndef PMI

    if (sonarg.adv_detected()){
        if (sonard.adv_detected()){
            sonarg.mean_adv(sonard.get_adv());
        }
        write_real_coords();
        sonarg.write_adv_coord();
        set_BF(STOP, Coord());
    }
    else if (sonard.adv_detected())
        write_real_coords();
        sonard.write_adv_coord();
        set_BF(STOP, Coord());
#else
    if (sonarg.adv_detected()){
        write_real_coords();
        sonarg.write_adv_coord();
        set_BF(STOP, Coord());
    }
#endif 

}


void ControlLoop::setxycap(Coord new_coord)
{
    real_coord = Coord(new_coord);
    target_position = Coord(real_coord);
    write_real_coords();
}

void ControlLoop::write_real_coords()
{
    Serial.print("* COORD ");
    real_coord.write_serial();

    Serial.print("theorique");
    target_position.write_serial();

}

void ControlLoop::write_debug()
{
    Serial.print("SonarG");
    sonarg.write_debug();
#ifndef PMI
    Serial.print("SonarD");
    sonard.write_debug();
#endif
}
